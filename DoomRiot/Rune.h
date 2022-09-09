#pragma once

#include <memory>
class Rune {
public:
	virtual void execute_rune(std::shared_ptr<class GameState> game_state) = 0;
};