#include "SetBaseMana.h"
#include "GameState.h"

void SetBaseMana::execute_rune(std::shared_ptr<GameState> game_State) {
	std::cout << "Executing Set Base Mana rune with parameters, controller_uid: " << this->controller_uid << " base_mana: " << this->base_mana << std::endl;
	auto controller = game_State->GetControllerFromUid(this->controller_uid);
	if (controller != nullptr) {
		controller->SetBaseMana(this->base_mana);
	}
}