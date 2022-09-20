#pragma once
#include "Rune.h"

#include "json.hpp"
using json = nlohmann::json;


class PlayMinion :
    public Rune
{

	unsigned int minion_uid;
	unsigned int controller_uid;
	unsigned int field_index;
	unsigned int target_uid;



public:
	PlayMinion(json blob) : minion_uid(blob["minion_uid"]), controller_uid(blob["controller_uid"]), field_index(blob["field_index"]), target_uid(blob["target_uid"]) {};
    void execute_rune(std::shared_ptr<class GameState> game_state);
};

