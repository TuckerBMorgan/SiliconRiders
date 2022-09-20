#include "SetMana.h"
#include "GameState.h"
void SetMana::execute_rune(std::shared_ptr<GameState> game_state) {
	std::cout << "Executing Set Mana Rune with parameters controller_uid: "  << this->controller_uid << " Current Mana: " << this->current_mana << std::endl;
	auto controller = game_state->GetControllerFromUid(this->controller_uid);
	if (controller != nullptr) {
		controller->SetCurrentMana(this->current_mana);
	}
}