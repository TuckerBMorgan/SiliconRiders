#include "OptionRune.h"
#include "GameState.h"

void OptionRune::execute_rune(std::shared_ptr<GameState> game_State) {
	std::cout << "Executing Option Rune with parmeters: options" << std::endl;

	auto controller = game_State->GetLocalPlayer();
	if (controller != nullptr) {
		controller->SetCurrentOptions(std::move(this->options));
	}

}