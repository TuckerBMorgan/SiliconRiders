#include "SummonMinion.h"
#include "GameState.h"

void SummonMinion::execute_rune(std::shared_ptr<GameState> game_state) {
	std::cout << "Execiting Summon Minion rune with parameters: Controlled UID " << this->controller_uid << " Minion UI: " << this->minion_uid << "Field Index:" << this->field_index << std::endl;
	auto controller = game_state->GetControllerFromUid(this->controller_uid);

	if (controller) {
		controller->AddMinionToBattlefieldAtIndex(this->minion_uid, this->field_index);
	}
}