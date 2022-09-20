#pragma once
#include "Rune.h"
#include "json.hpp"

using json = nlohmann::json;

class PlayCard : public Rune
{
private:
	unsigned int card_uid;
	unsigned int controller_uid;
	int field_index;
	int target_uid;


public:
	PlayCard(json blob) : card_uid(blob["card_uid"]), controller_uid(blob["controller_uid"]), field_index(blob["field_index"]), target_uid(blob["target_uid"]) {};
	void execute_rune(std::shared_ptr<class GameState> game_state);
};

