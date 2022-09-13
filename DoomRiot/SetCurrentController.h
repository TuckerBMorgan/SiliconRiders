#pragma once
#include "Rune.h"
#include "json.hpp"
using json = nlohmann::json;
class SetCurrentController :
    public Rune
{

private:
    unsigned int controller_uid;

        
public:
    SetCurrentController(json blob) : controller_uid(blob["controller_uid"]) {};
    void execute_rune(std::shared_ptr<class GameState> game_State) override;
};

