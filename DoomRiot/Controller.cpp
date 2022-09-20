#include <string>
#include "Controller.h"
#include "json.hpp"
#include <iostream>
#include <algorithm>

void Controller::AddMinionToControllersDeck(unsigned int minion_id) {
	this->in_deck.push_back(minion_id);
}

void Controller::AddMinionToHand(unsigned int minion_id) {
	std::cout << "Add Minion To hand " << minion_id << std::endl;
	this->in_hand.push_back(minion_id);
}

void Controller::RemoveCardFromHandIfPresent(unsigned int minion_uid) {
	std::cout << "RemoveCardFromHandIfPresent " << minion_uid << std::endl;
	auto index = std::find(this->in_hand.begin(), this->in_hand.end(), minion_uid);
	if (index != this->in_hand.end()) {
		std::cout << "Remove an index" << std::endl;
		this->in_hand.erase(index);
	}
}

void Controller::AddMinionToBattlefieldAtIndex(unsigned int minion_id, int field_index) {
	std::cout << "AddMinionToBattlefieldAtIndex " << minion_id << " " << field_index << std::endl;
	if (this->on_battlefield.size() > field_index) {
		this->on_battlefield.insert(this->on_battlefield.begin() + field_index, minion_id);
	}
	else {
		this->on_battlefield.push_back(minion_id);
	}
}

bool Controller::IsLocal() {
	return this->is_me;
}


std::ostream& operator << (std::ostream& strm, EControllerState tt)
{
	const std::string nameTT[] = { "Waiting", "InMulligan", "InTurn" };
	return strm << nameTT[(int)tt];
}