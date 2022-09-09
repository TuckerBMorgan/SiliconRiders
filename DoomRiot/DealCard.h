#pragma once
#include "Rune.h"
#include "GameState.h"
#include "json.hpp"
using json = nlohmann::json;

class DealCard :
    public Rune
{
private:
    unsigned int card_uid;
    unsigned int controller_uid;

public:
    DealCard(json blob) : card_uid(blob["card_uid"]), controller_uid(blob["controller_uid"]) {};
    void execute_rune(std::shared_ptr<class GameState> game_state);
};

