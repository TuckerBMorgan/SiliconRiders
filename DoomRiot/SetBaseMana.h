#pragma once
#include "Rune.h"
#include "json.hpp"
using json = nlohmann::json;
class SetBaseMana :
    public Rune
{

private:
    unsigned int controller_uid;
    short base_mana;

public:
    SetBaseMana(json blob) : controller_uid(blob["controller_uid"]), base_mana(blob["base_mana"]) {};
    void execute_rune(std::shared_ptr<class GameState> game_State) override;
};

