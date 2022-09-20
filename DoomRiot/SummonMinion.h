#pragma once
#include "Rune.h"

#include "json.hpp"

using json = nlohmann::json; 

class SummonMinion :
    public Rune
{
    unsigned int minion_uid;
    unsigned int controller_uid;
    int field_index;
public:
    SummonMinion(json blob) : minion_uid(blob["minion_uid"]), controller_uid(blob["controller_uid"]), field_index(blob["field_index"]) {};
    void execute_rune(std::shared_ptr<class GameState> game_state);
};

