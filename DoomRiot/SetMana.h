#pragma once
#include "Rune.h"
#include "json.hpp"
using json = nlohmann::json;

class SetMana :
    public Rune
{
    unsigned int controller_uid;
    short current_mana;
public:
    SetMana(json blob) : controller_uid(blob["controller_uid"]), current_mana(blob["mana"]) {}
    void execute_rune(std::shared_ptr<class GameState> game_state) override;
};

