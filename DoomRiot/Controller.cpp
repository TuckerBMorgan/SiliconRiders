#include <string>
#include "Controller.h"
#include "json.hpp"
#include <algorithm>

void Controller::AddMinionToControllersDeck(unsigned int minion_id) {
	this->in_deck.push_back(minion_id);
}

void Controller::MoveMinionToHandFromDeck(unsigned int minion_id) {
	auto pos = std::find(this->in_deck.begin(), this->in_deck.end(), minion_id);
	//this->in_deck.erase(pos);
	this->in_hand.push_back(minion_id);
}

bool Controller::IsLocal() {
	return this->is_me;
}


std::ostream& operator << (std::ostream& strm, EControllerState tt)
{
	const std::string nameTT[] = { "Waiting", "InMulligan", "InTurn" };
	return strm << nameTT[(int)tt];
}