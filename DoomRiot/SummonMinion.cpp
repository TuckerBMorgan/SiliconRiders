#include "SummonMinion.h"
#include "GameState.h"

void SummonMinion::execute_rune(std::shared_ptr<GameState> game_state) {
	auto controller = game_state->GetControllerFromUid(this->controller_uid);

	if (controller) {
		controller->AddMinionToBattlefieldAtIndex(this->minion_uid, this->field_index);
	}
}