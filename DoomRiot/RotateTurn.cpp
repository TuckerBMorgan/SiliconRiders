#include <iostream>

#include "RotateTurn.h"
#include "GameState.h"

void RotateTurn::execute_rune(std::shared_ptr<GameState> game_State) {
	std::cout << "Executing Rotate Turn rune with no parameters\n";
	game_State->AdvanceTurn();
};