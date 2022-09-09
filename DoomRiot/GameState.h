#pragma once
#include <vector>
#include <iostream>
#include <queue>
#include "Controller.h"
#include "Minion.h"


enum class EGameState {
	EPregame = 0,
	EInGame = 1,
	EEndGame = 2
};
std::ostream& operator << (std::ostream& strm, EGameState tt);


 class GameState {
	 //Convert this into an array, no more allocations WHAT WHAT
	 std::vector<Controller> controllers;
	 std::vector<Minion> minions;
	 EGameState e_game_state;
	 int current_turn;

 public:
	 GameState() {
		 e_game_state = EGameState::EPregame;
		 current_turn = -1;
	 }
	 void AddController(Controller controller);
	 void AddMinion(Minion minion);
	 void DealMinion(unsigned int minion_id, unsigned int controller_id);
	 Controller* GetLocalPlayer();
	 Controller* GetControllerFromUid(unsigned int uid);
	 void Tick(std::queue<json>& to_server_messages);
	 void SetState(EGameState new_state) {
		 std::cout << "Changed Game state from " << this->e_game_state << " to " << new_state << std::endl;
		 this->e_game_state = new_state; 
	 };
	 void AdvanceTurn() {
		 this->current_turn += 1;
	 }
};