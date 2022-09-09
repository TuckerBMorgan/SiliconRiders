#include "SetMana.h"
#include "GameState.h"
void SetMana::execute_rune(std::shared_ptr<GameState> game_state) {
	auto controller = game_state->GetControllerFromUid(this->controller_uid);
	if (controller != nullptr) {
		controller->SetCurrentMana(this->current_mana);
	}
}