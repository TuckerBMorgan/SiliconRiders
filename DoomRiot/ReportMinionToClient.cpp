#include <iostream>
#include "ReportMinionToClient.h"
#include "GameState.h"

void ReportMinionToClient::execute_rune(std::shared_ptr<GameState> game_state) {
	Minion minion = Minion(this->cost, this->uid, this->id, this->name, this->set, this->base_attack, this->current_attack, this->total_attack, this->base_health, this->current_health, this->total_health, this->controller_uid);
	std::cout << "Executing ReportMinionToClient Rune with parmeters cost: " << this->cost
		<< " uid: " << this->uid
		<< " id: " << this->id
		<< " name: " << this->name
		<< " set:" << this->set
		<< " base attack: " << this->base_attack
		<< " current attack: " << this->current_attack
		<< " total attack: " << this->total_attack
		<< " base health: " << this->base_health
		<< " current health: " << this->current_health
		<< " total_health: " << this->total_health
		<< " controller uid : " << this->controller_uid << std::endl;

	game_state->AddMinion(minion);
}