#include <assert.h>
#include <iostream>
#include <queue>

#include "GameState.h"
#include "Controller.h"

void GameState::AddController(Controller controller) {
	this->controllers.push_back(controller);
}	

void GameState::AddMinion(Minion minion) {
	this->minions.push_back(minion);
}

void GameState::DealMinion(unsigned int minion_id, unsigned int controller_id) {
	for (auto controller : this->controllers) {
		if (controller.ControllerId() == controller_id) {
			controller.MoveMinionToHandFromDeck(minion_id);
		}
	}
}

Controller* GameState::GetLocalPlayer() {
	
	for (int i = 0; i < this->controllers.size(); i++) {
		if (this->controllers[i].IsLocal()) {
			return &this->controllers[i];
		}
	}	

	return nullptr;
}

void GameState::Tick(std::queue<json>& to_server_message) {
	auto local_player = this->GetLocalPlayer();
	if (local_player == nullptr) {
		return;
	}
	switch (local_player->GetControllerState())
	{
	case EControllerState::InMulligan:
	{

		json mulligan_message;
		mulligan_message["message_type"] = "mulligan";
		mulligan_message["index"] = json::array({});
		to_server_message.push(mulligan_message);
		local_player->SetControllerState(EControllerState::Waiting);
	}
		break;
	default:
		break;
	}
}
Controller* GameState::GetControllerFromUid(unsigned int uid) {
	for (int i = 0; i < this->controllers.size();i++) {
		if (this->controllers[i].ControllerId() == uid) {
			return &this->controllers[i];
		}
	}
	return nullptr;
}

std::ostream& operator << (std::ostream& strm, EGameState tt)
{
	const std::string nameTT[] = { "EPregame", "EInGame", "EEndGame" };
	return strm << nameTT[(int)tt];
}

