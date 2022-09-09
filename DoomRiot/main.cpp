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

int main(int argc, char* argv[])
{
    int width = 1280;
    int height = 720;

    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("SDL pixels", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
    SDL_Surface* screen = SDL_GetWindowSurface(window);
    SDL_Surface* pixels = SDL_CreateRGBSurfaceWithFormat(0, width, height, 32, SDL_PIXELFORMAT_RGBX8888);

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
        SDL_FillRect(pixels, NULL, 0);

        // write the pixels
        SDL_LockSurface(pixels);
        {
            int pitch = pixels->pitch;

            // move 100 pixels/second
            pos += delta * 100.0f;
            pos = fmodf(pos, width);

            // draw red diagonal line
            for (int i = 0; i < height; i++)
            {
                int y = i;
                int x = ((int)pos + i) % width;

                unsigned int* row = (unsigned int*)((char*)pixels->pixels + pitch * y);
                row[x] = 0xff0000ff; // 0xAABBGGRR
            }
        }
        SDL_UnlockSurface(pixels);

        // copy to window
        SDL_BlitSurface(pixels, NULL, screen, NULL);
        SDL_UpdateWindowSurface(window);
    }

    thread_object.join();
}
