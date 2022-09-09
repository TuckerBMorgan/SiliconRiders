#pragma once
#include "Rune.h"
class Mulligan :
    public Rune
{
public:
    void execute_rune(std::shared_ptr<class GameState> game_state) override;
};