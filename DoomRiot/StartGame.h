#pragma once
#include "Rune.h"
class StartGame :
    public Rune
{
public:
    void execute_rune(std::shared_ptr<class GameState> game_State) override;

};

