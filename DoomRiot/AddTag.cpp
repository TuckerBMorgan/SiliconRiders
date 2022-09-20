#include "AddTag.h"
#include "GameState.h"

void AddTag::execute_rune(std::shared_ptr<GameState> game_state) {
	auto minion = game_state->GetMutableMinionFromUID(this->minion_uid);
	std::cout << "executing Add Tag Rune with parameters: Minion UID: " << this->minion_uid << " Tag: " << this->tag << std::endl;
	if (minion) {
		minion->AddTag(this->tag);
	}
}