#include <iostream>
#include "Mulligan.h"
#include "GameState.h"

void Mulligan::execute_rune(std::shared_ptr<GameState> game_state) {
	std::cout << "Mulligan rune" << std::endl;
	Controller* local_player = game_state->GetLocalPlayer();
	local_player->SetControllerState(EControllerState::InMulligan);
	int i = 0;
}