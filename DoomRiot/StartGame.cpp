#include "StartGame.h"
#include "GameState.h"

void StartGame::execute_rune(std::shared_ptr<GameState> game_State) {
	std::cout << "Executing a Start Game Rune" << std::endl;
	game_State->SetState(EGameState::EInGame);
}