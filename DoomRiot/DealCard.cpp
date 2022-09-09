#include <iostream>
#include "DealCard.h"
#include "GameState.h"

void DealCard::execute_rune(std::shared_ptr<GameState> game_state) {
	std::cout << "Executing DealCard rune with parameters card_uid: " << this->card_uid << " controller_uid: " << this->controller_uid << std::endl;
	game_state->DealMinion(this->card_uid, this->controller_uid);
}