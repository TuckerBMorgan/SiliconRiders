#pragma once
#include "Rune.h"
class RotateTurn :
    public Rune
{
    void execute_rune(std::shared_ptr<class GameState> game_State) override;
};

