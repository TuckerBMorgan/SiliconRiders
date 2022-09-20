#pragma once
#include <string>

#include "Rune.h"
#include "json.hpp"

using json = nlohmann::json;

class AddTag :
    public Rune
{
private:
    unsigned int minion_uid;
    std::string tag;

public:
    AddTag(json blob) : minion_uid(blob["minion_uid"]), tag(blob["tag"]) {}
    void execute_rune(std::shared_ptr<class GameState> game_state);
};

