// on Windows compile with:
// cl.exe sdl2_pixels.c -Zi -Iinclude -link -incremental:no -subsystem:windows SDL2.lib SDL2main.lib shell32.lib

#include <SDL.h>
#include <math.h>
#include <vector>
#include <queue>
#include <string>
#include <memory>
#include <iostream>
#include<thread>

#include <stdlib.h>
#include <stdio.h>

#include "json.hpp"
#include "GameState.h"
#include "Controller.h"
#include "NetworkingThread.h"
#include "RuneVM.h"
#include <SDL_ttf.h>


using json = nlohmann::json;

Rune* process_message(json message) {
    std::cout << "\033[0m Processing message " << message << std::endl;
    //TODO: We can remove all of these allocations by using some form of preallocated memory for them to use
    if (message["runeType"] == "NewController") {
        auto new_controller_rune = new NewController(message);
        return new_controller_rune;
    }
    else if (message["runeType"] == "ReportMinionToClient") {
        auto report_minion_controller_rune = new ReportMinionToClient(message);
        return report_minion_controller_rune;
    }
    else if (message["runeType"] == "DealCard") {
        auto deal_card = new DealCard(message);
        return deal_card;
    }
    else if (message["runeType"] == "Mulligan") {
        auto mulligan = new Mulligan();
        return mulligan;
    }
    else if (message["runeType"] == "StartGame") {
        auto start_game = new StartGame();
        return start_game;
    }
    else if (message["runeType"] == "SetBaseMana") {
        auto set_base_mana = new SetBaseMana(message);
        return set_base_mana;
    }
    else if (message["runeType"] == "SetMana") {
        auto set_mana = new SetMana(message);
        return set_mana;
    }
    else if (message["runeType"] == "RotateTurn") {
        auto rotate_turn = new RotateTurn();
        return rotate_turn;
    }
    else if (message["runeType"] == "optionRune") {
        auto option_rune = new OptionRune(message);
        return option_rune;
    }
    else {
        std::cout << "No statement for " << message["runeType"] << " written yet" << std::endl;
    }

    return nullptr;
}

struct NumericalGlyph {
    SDL_Texture* texture;
    int width;
    int height;
};


int main(int argc, char* argv[])
{
    int width = 1280;
    int height = 720;

    TTF_Init();
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("SDL pixels", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
    SDL_Surface* screen = SDL_GetWindowSurface(window);
    SDL_Surface* pixels = SDL_CreateRGBSurfaceWithFormat(0, width, height, 32, SDL_PIXELFORMAT_RGBX8888);
    auto test = SDL_LoadBMP("./card_border.bmp");
    auto renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    auto newTexture = SDL_CreateTextureFromSurface(renderer, test);


    auto gFont = TTF_OpenFont("C:/Users/Tucker/source/repos/DoomRiot/DoomRiot/Resources/fonts/BauhausRegular.ttf", 28);

    SDL_FreeSurface(test);
    std::mutex message_channel_lock;
    std::queue<json> message_channel;

    std::mutex to_server_channel_lock;
    std::queue<json> to_server_message_channel;

    auto shrd_ptr_for_channel = std::make_shared<std::queue<json>>(message_channel);
    auto shrd_ptr_for_to_server_channel = std::make_shared<std::queue<json>>(to_server_message_channel);
    ThreadParameters thread_params(message_channel_lock, shrd_ptr_for_channel, to_server_channel_lock, shrd_ptr_for_to_server_channel);

    std::thread thread_object(network_thread, thread_params);


    GameState the_game_state = GameState();

    unsigned int t1 = SDL_GetTicks();
    auto ptr = std::make_shared<GameState>(the_game_state);
    std::queue<json> messages;

    float pos = 0;
    std::queue<json> to_server_messages;

    SDL_Color textColor;
    textColor.r = 0;
    textColor.g = 255;
    textColor.b = 0;
    textColor.a = 255;


    std::vector<NumericalGlyph> numerical_glyphs;
    for (int i = 0; i < 10;i++) {
        SDL_Surface* mana_text = TTF_RenderText_Solid(gFont, std::to_string(i).c_str(), textColor);
        auto mTexture = SDL_CreateTextureFromSurface(renderer, mana_text);
        NumericalGlyph glyph;
        glyph.texture = mTexture;
        glyph.height = mana_text->h;
        glyph.width = mana_text->w;
        numerical_glyphs.push_back(glyph);
        SDL_FreeSurface(mana_text);
    }
    SDL_Rect text_rect;


    for (;;)
    {
        SDL_Event ev;
        while (SDL_PollEvent(&ev))
        {
            if (ev.type == SDL_QUIT)
            {
                return 0;
            }
        }

        {
            std::unique_lock<std::mutex> a_lock(message_channel_lock);
            while (shrd_ptr_for_channel->empty() == false)
            {
                auto new_message = std::move(shrd_ptr_for_channel->front());
                messages.push(new_message);
                shrd_ptr_for_channel->pop();
            }
        }

        while (messages.empty() == false)
        {
            json new_message = messages.front();
            messages.pop();
            Rune* new_rune = process_message(new_message);
            if (new_rune != nullptr) {
                new_rune->execute_rune(ptr);
            }
        }

        ptr->Tick(to_server_messages);

        {
            if (to_server_messages.size() > 0) {
                to_server_channel_lock.lock();
                while (to_server_messages.empty() == false)
                {
                    json to_server_message = to_server_messages.front();
                    to_server_messages.pop();

                    shrd_ptr_for_to_server_channel->push(to_server_message);
                }
                to_server_channel_lock.unlock();
            }
        }



        unsigned int t2 = SDL_GetTicks();
        float delta = (t2 - t1) / 1000.0f;
        
        t1 = t2;

        // clear pixels to black background
        SDL_RenderClear(renderer);

        //SDL_BlitSurface(gHelloWorld, NULL, gScreenSurface, NULL);
        // copy to window
        SDL_Rect src_rect;
        src_rect.x = 0;
        src_rect.y = 0;
        src_rect.w = 347;
        src_rect.h = 627;

        SDL_Rect dest_rect;
        dest_rect.x = 0;
        dest_rect.y = 0;
        dest_rect.w = 347 / 2.0;
        dest_rect.h = 627 / 2.0;


        auto local_player = ptr->GetLocalPlayer();

        if (local_player != nullptr) {


            auto in_hand_ids = local_player->GetHandIds();
            for (int i = 0; i < in_hand_ids.size();i++) {


                auto minion_ptr = ptr->GetMinionFromUID(in_hand_ids[i]);

                auto attack = minion_ptr->GetCurrentAttack();
                auto health = minion_ptr->GetCurrentHealth();


                SDL_RenderCopy(renderer, newTexture, &src_rect, &dest_rect);
                
                SDL_Surface* textSurface = TTF_RenderText_Solid(gFont, minion_ptr->GetName().c_str(), textColor);
                auto mTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

                auto numerical_glyph = numerical_glyphs[attack];
                text_rect.x = dest_rect.x + 10;
                text_rect.y = dest_rect.y;
                text_rect.w = textSurface->w;
                text_rect.h = textSurface->h;
                
                SDL_RenderCopy(renderer, mTexture, NULL, &text_rect);
                text_rect.y = (627 / 2.0) - numerical_glyph.height;
                text_rect.w = numerical_glyph.width;
                text_rect.h = numerical_glyph.height;

                SDL_RenderCopy(renderer, numerical_glyph.texture, NULL, &text_rect);

                numerical_glyph = numerical_glyphs[health];
                SDL_RenderCopy(renderer, mTexture, NULL, &text_rect);
                text_rect.x = dest_rect.x + (347.0 / 2.0) - numerical_glyph.width;
                text_rect.w = numerical_glyph.width;
                text_rect.h = numerical_glyph.height;

                SDL_RenderCopy(renderer, numerical_glyph.texture, NULL, &text_rect);

                dest_rect.x += (347.0 / 2.0);
            }



            int current_mana = local_player->GetCurrentMana();
            int start_end = width - 10;
            int summed_shift = 0;
            do {
                auto digit = numerical_glyphs[current_mana % 10];
                SDL_Rect text_rect;
                text_rect.x = start_end - digit.width;
                text_rect.y = height - digit.height;
                text_rect.w = digit.width;
                text_rect.h = digit.height;

                SDL_RenderCopy(renderer, numerical_glyphs[current_mana % 10].texture, NULL, &text_rect);
                current_mana = current_mana / 10;
            } while (current_mana > 0);
        }
 
        SDL_RenderPresent(renderer);
        //SDL_UpdateWindowSurface(window);
    }

    TTF_Quit();
    thread_object.join();
}
