#include "PlayCard.h"
#include "GameState.h"

void PlayCard::execute_rune(std::shared_ptr<GameState> game_state) {

	std::cout << "Executing Play Card Rune with parameters  card_uid: " << this->card_uid << " controlled_uid: " << this->controller_uid << " field_index: " << this->field_index << "target index: " << this->target_uid << std::endl;

	auto controller = game_state->GetControllerFromUid(this->controller_uid);
	if (controller) {
		controller->RemoveCardFromHandIfPresent(this->card_uid);
	}
}