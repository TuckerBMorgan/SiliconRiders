// on Windows compile with:
// cl.exe sdl2_pixels.c -Zi -Iinclude -link -incremental:no -subsystem:windows SDL2.lib SDL2main.lib shell32.lib


#include <stdio.h>
#include <stdlib.h>

#include <SDL.h>
#include <math.h>
#include <vector>
#include <queue>
#include <string>
#include <memory>
#include <iostream>
#include <SDL_ttf.h>
#include<thread>


#include "json.hpp"
#include "GameState.h"
#include "Controller.h"
#include "NetworkingThread.h"
#include "RuneVM.h"
#include "RenderCarePackage.h"
#include "CardDisplay.h"
#include "Log.h"


using json = nlohmann::json;

Rune* process_message(json message) {
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
    else if (message["runeType"] == "SetCurrentController") {
        auto set_current_controller = new SetCurrentController(message);
        return set_current_controller;
    }
    else if (message["runeType"] == "PlayCard") {
        auto play_card = new PlayCard(message);
        return play_card;
    }
    else if (message["runeType"] == "SummonMinion") {
        auto summon_minion = new SummonMinion(message);
        return summon_minion;
    }
    else if (message["runeType"] == "PlayMinion") {
        auto play_minion = new PlayMinion(message);
        return play_minion;
    }
    else {
        std::cout << "No statement for " << message["runeType"] << " written yet" << std::endl;
    }

    return nullptr;
}


void RenderHiddenCard(RenderCarePackage* render_care_package, std::shared_ptr<GameState> game_state, SDL_Rect* destination_rect) {
    SDL_Rect src_rect;
    src_rect.x = 0;
    src_rect.y = 0;
    src_rect.w = 347;
    src_rect.h = 627;
    SDL_RenderCopy(render_care_package->GetRenderer(), render_care_package->GetCardBackTexture(), &src_rect, destination_rect);
}


void RenderCard(unsigned int minion_uid, RenderCarePackage* render_care_package, std::shared_ptr<GameState> game_state, SDL_Rect* destination_rect) {
    SDL_Rect text_rect;
    SDL_Rect src_rect;
    src_rect.x = 0;
    src_rect.y = 0;
    src_rect.w = 347;
    src_rect.h = 627;

    auto minion_ptr = game_state->GetMinionFromUID(minion_uid);

    auto attack = minion_ptr->GetCurrentAttack();
    auto health = minion_ptr->GetCurrentHealth();

    //Render background
    SDL_RenderCopy(render_care_package->GetRenderer(), render_care_package->GetCardbackgroundTexture(), &src_rect, destination_rect);
    auto name_texture = render_care_package->GetFontTexture(minion_ptr->GetName());


    //TODO: offset this by the destination rect
    text_rect.x = destination_rect->x + 10;
    text_rect.y = destination_rect->y;
    text_rect.w = destination_rect->w * 0.95;
    text_rect.h = name_texture->height;

     //Name
    SDL_RenderCopy(render_care_package->GetRenderer(), name_texture->texture, NULL, &text_rect);

    {
        auto numerical_glyph = render_care_package->GetDigitTextures()[attack];
        text_rect.y = destination_rect->y + (627 / 2) - numerical_glyph.height;
        text_rect.w = numerical_glyph.width;
        text_rect.h = numerical_glyph.height;

        //Attack
        SDL_RenderCopy(render_care_package->GetRenderer(), numerical_glyph.texture, NULL, &text_rect);
    }

    {
        auto numerical_glyph = render_care_package->GetDigitTextures()[health]; 
        text_rect.x = destination_rect->x + (347 / 2) - numerical_glyph.width;
        text_rect.w = numerical_glyph.width;
        text_rect.h = numerical_glyph.height;

        //Health
        SDL_RenderCopy(render_care_package->GetRenderer(), numerical_glyph.texture, NULL, &text_rect);
    }
}

static void debug_log(const char* line, void* argp) {
    fprintf(stderr, "%s\n", line);
}

int main(int argc, char* argv[])
{


    int width = 1280 * 2;
    int height = 720 * 2;

    TTF_Init();
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("SDL pixels", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
    SDL_Surface* screen = SDL_GetWindowSurface(window);
    SDL_Surface* pixels = SDL_CreateRGBSurfaceWithFormat(0, width, height, 32, SDL_PIXELFORMAT_RGBX8888);
    auto test = SDL_LoadBMP("./card_border.bmp");
    Log log;

    log.PrintLogMessage("Hello WOrld", 100);

    auto renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    auto newTexture = SDL_CreateTextureFromSurface(renderer, test);


    auto foo_surface = SDL_LoadBMP("./cardback.bmp");
    auto cardback_texture = SDL_CreateTextureFromSurface(renderer, foo_surface);
    SDL_FreeSurface(foo_surface);

    auto border_bmp = SDL_LoadBMP("./total_border.bmp");
    auto border_Texture = SDL_CreateTextureFromSurface(renderer, border_bmp);

    auto battlefield_break = SDL_LoadBMP("./line_split.bmp");
    auto battlefield_break_texture = SDL_CreateTextureFromSurface(renderer, battlefield_break);
    SDL_FreeSurface(battlefield_break);
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

    RenderCarePackage render_care_package = RenderCarePackage(renderer, newTexture, cardback_texture, numerical_glyphs, gFont);



    std::vector<CardDisplay> card_displays;
    for (int i = 0; i < 5;i++) {
        SDL_Rect dest_rect;
        dest_rect.x = (347 / 2) * i + 200;
        dest_rect.y = height - 200;
        dest_rect.w = 347 / 2;
        dest_rect.h = 627 / 2;
        card_displays.push_back(CardDisplay(dest_rect, 1.0));
    }

    std::vector<CardDisplay> local_player_in_battlefield;
    for (int i = 0; i < 5; i++) {
        SDL_Rect dest_rect;
        dest_rect.x = (347 / 2) * i + 200;
        dest_rect.y = height / 2 + 50;
        dest_rect.w = 347 / 2;
        dest_rect.h = 627 / 2;
        local_player_in_battlefield.push_back(CardDisplay(dest_rect, 1.0));
    }
    
    std::vector<CardDisplay> remote_player_card_displays;
    for (int i = 0; i < 5; i++) {
        SDL_Rect dest_rect;
        dest_rect.x = (347 / 2) * i + 200;
        dest_rect.y = -100;
        dest_rect.w = 347 / 2;
        dest_rect.h = 627 / 2;
        remote_player_card_displays.push_back(CardDisplay(dest_rect, 1.0));
    }
    

    bool mouse_down = false;
    int tracking_card = -1;
    int previous_x = 0;
    int previous_y = 0;
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
            free(new_rune);
        }

        ptr->Tick(to_server_messages);



        int x, y;
        Uint32 buttons;
        SDL_PumpEvents();
        buttons = SDL_GetMouseState(&x, &y);
        SDL_Point point;
        point.x = x;
        point.y = y;

        if ((buttons & SDL_BUTTON_LMASK) != 0) {
            if (mouse_down == false) {
                mouse_down = true;
                previous_x = point.x;
                previous_y = point.y;
                for (int i = 0; i < card_displays.size();i++) {
                    //Maybe turn CurrentScreenRect into a pointer, and cache it
                    auto bark = card_displays[i].CalculateCurrentScreenRectangle();
                    if (SDL_PointInRect(&point, &bark)) {
                        card_displays[i].SetShouldBeOffSetting(true);
                        tracking_card = i;
                        card_displays[i].SetScale(1.2f);
                        break;
                    }
                }
            }
        }

        if (mouse_down && (buttons & SDL_BUTTON_LMASK) == 0 ) {
            mouse_down = false;
            if (tracking_card != -1) {
                card_displays[tracking_card].SetShouldBeOffSetting(false);
                card_displays[tracking_card].SetScale(1.0);
                //Get Local Player
                //If they are in turn
                //and the mouse down point was in the players battlefield
                //return an option

                auto local_player = ptr->GetLocalPlayer();
                switch (local_player->GetControllerState())
                {
                case EControllerState::InTurn:
                    SDL_Rect battle_ground_rect;
                    battle_ground_rect.x = 0;
                    battle_ground_rect.y = height / 2;
                    battle_ground_rect.w = width;
                    battle_ground_rect.h = height / 2 - 206;

                    if (SDL_PointInRect(&point, &battle_ground_rect)) {
                        auto current_options = local_player->OptionsOfType("EPlayCard");
                        for (auto& option : current_options) {
                            //TODO: this is a "happens to corolate"
                            // we need to assign a card display to an id when it gets dealed

                            auto id = local_player->GetHandIds()[tracking_card];
                            if (option.source == id) {
                                json play_card_message;
                                play_card_message["message_type"] = "option";
                                play_card_message["index"] = option.option_index;
                                play_card_message["board_index"] = 0;
                                to_server_messages.push(play_card_message);
                                local_player->SetControllerState(EControllerState::Waiting);
                            }
                        }
                    }
                    break;
                default:
                    break;
                }
            }
            tracking_card = -1;
        }


        if (tracking_card != -1) {
            float x_dif = point.x - previous_x;
            float y_dif = point.y - previous_y;
            SDL_FPoint offset_point;
            offset_point.x = x_dif;
            offset_point.y = y_dif;
            card_displays[tracking_card].AddMoreToOffset(offset_point);
            previous_x = point.x;
            previous_y = point.y;
        }


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
        float delta = ((float)t2 - t1) / 1000.0f;

        
        t1 = t2;
        SDL_RenderClear(renderer);

        for (int i = 0; i < card_displays.size();i++) {
            card_displays[i].Tick(delta);
        }


        
        auto local_player = ptr->GetLocalPlayer();

        if (local_player != nullptr) {
            auto in_hand_ids = local_player->GetHandIds();
            for (int i = 0; i < in_hand_ids.size();i++) {
                auto bite = card_displays[i].CalculateCurrentScreenRectangle();
                RenderCard(in_hand_ids[i], &render_care_package, ptr, &bite);
            }
            
            auto in_battle_ids = local_player->GetBattlefieldIds();
            for (int i = 0; i < in_battle_ids.size(); i++) {
                auto bite = local_player_in_battlefield[i].CalculateCurrentScreenRectangle();
                RenderCard(in_battle_ids[i], &render_care_package, ptr, &bite);
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
        
        auto remote_player = ptr->GetRemotePlayer();

        if (remote_player != nullptr) {
            auto in_hand_ids = remote_player->GetHandIds();
            for (int i = 0; i < in_hand_ids.size();i++) {
                auto bite = remote_player_card_displays[i].CalculateCurrentScreenRectangle();
                RenderHiddenCard(&render_care_package, ptr, &bite);
            }
        }
        

        SDL_Rect border_corner;
        border_corner.x = 0;
        border_corner.y = 0;
        border_corner.h = 6;
        border_corner.w = 6;

        SDL_Rect border_corner_dest;
        border_corner_dest.x = 0;
        border_corner_dest.y = 0;
        border_corner_dest.h = 24;
        border_corner_dest.w = 24;


        SDL_RenderCopy(render_care_package.GetRenderer(), border_Texture, &border_corner, &border_corner_dest);


        border_corner.x = 14;
        border_corner_dest.x = width - 24;
        SDL_RenderCopy(render_care_package.GetRenderer(), border_Texture, &border_corner, &border_corner_dest);

        border_corner.y = 14;
        border_corner_dest.y = height - 24;
        SDL_RenderCopy(render_care_package.GetRenderer(), border_Texture, &border_corner, &border_corner_dest);

        border_corner.x = 0;
        border_corner_dest.x = 0;
        SDL_RenderCopy(render_care_package.GetRenderer(), border_Texture, &border_corner, &border_corner_dest);

        border_corner.x = 6;
        border_corner.h = 6;
        border_corner.w = 1;
        border_corner_dest.x = 24;
        border_corner_dest.w = width - 48;
        SDL_RenderCopy(render_care_package.GetRenderer(), border_Texture, &border_corner, &border_corner_dest);

        border_corner_dest.y = 0;
        border_corner.y = 0;
        SDL_RenderCopy(render_care_package.GetRenderer(), border_Texture, &border_corner, &border_corner_dest);
        
        border_corner.x = 0;
        border_corner.y = 6;
        border_corner.h = 1;
        border_corner.w = 6;
        border_corner_dest.x = 0;
        border_corner_dest.y = 24;
        border_corner_dest.w = 24;
        border_corner_dest.h = height - 48;
        SDL_RenderCopy(render_care_package.GetRenderer(), border_Texture, &border_corner, &border_corner_dest);

        border_corner.x = 14;
        border_corner_dest.x = width - 24;
        SDL_RenderCopy(render_care_package.GetRenderer(), border_Texture, &border_corner, &border_corner_dest);

        border_corner.x = 0;
        border_corner.y = 0;
        border_corner.w = 1;
        border_corner.h = 1;

        border_corner_dest.h = 6;
        border_corner_dest.w = width;
        border_corner_dest.x = 0;
        border_corner_dest.y = height / 2;
        SDL_RenderCopy(render_care_package.GetRenderer(), battlefield_break_texture, &border_corner, &border_corner_dest);
        
        border_corner_dest.y = height - 206;
        SDL_RenderCopy(render_care_package.GetRenderer(), battlefield_break_texture, &border_corner, &border_corner_dest);

        border_corner_dest.y = 212;
        SDL_RenderCopy(render_care_package.GetRenderer(), battlefield_break_texture, &border_corner, &border_corner_dest);
        log.Render(&render_care_package);
        SDL_RenderPresent(renderer);
    }

    TTF_Quit();
    thread_object.join();
}
