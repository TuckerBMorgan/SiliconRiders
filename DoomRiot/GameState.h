#pragma once
#include <vector>
#include <iostream>
#include <queue>
#include "Controller.h"
#include "Minion.h"
#include <optional>

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
	 unsigned int current_controller;

 public:
	 GameState() {
		 e_game_state = EGameState::EPregame;
		 current_turn = -1;
		 current_controller = -1;
	 }
	 void AddController(Controller controller);
	 void AddMinion(Minion minion);
	 void DealMinion(unsigned int minion_id, unsigned int controller_id);
	 Controller* GetLocalPlayer();
	 Controller* GetControllerFromUid(unsigned int uid);
	 Controller* GetRemotePlayer();
	 void Tick(std::queue<json>& to_server_messages);

	

	 //TODO: move these functions to the cpp file
	 //TODO: add more logging to each of these files
	 void SetState(EGameState new_state) {
		 std::cout << "Changed Game state from " << this->e_game_state << " to " << new_state << std::endl;
		 this->e_game_state = new_state; 
	 };

	 void AdvanceTurn() {
		 this->current_turn += 1;
		 if (this->current_turn == 0) {
			 this->SetState(EGameState::EInGame);
		 }
	 }

	 void SetCurrentController(unsigned int new_current_controller) {
		 if (this->current_controller == -1) {
			 auto new_current_controller_ptr = this->GetControllerFromUid(new_current_controller);
			 new_current_controller_ptr->SetControllerState(EControllerState::InTurn);
			 this->current_controller = new_current_controller;
		 }
		 else {
			 auto current_controller_ptr = this->GetControllerFromUid(this->current_controller);
			 current_controller_ptr->SetControllerState(EControllerState::Waiting);
			 auto new_current_controller_ptr = this->GetControllerFromUid(new_current_controller);
			 new_current_controller_ptr->SetControllerState(EControllerState::InTurn);
			 this->current_controller = new_current_controller;
		 }
	 };

	 void AttemptToPlayCard(unsigned int uid);
	 
	 const Minion* GetMinionFromUID(unsigned int uid) {
		 
		 for (auto& minion : this->minions) {
			 if (minion.UID() == uid) {
				 return &minion;
			 }
		 }

		 return nullptr;
	 }
	 
};