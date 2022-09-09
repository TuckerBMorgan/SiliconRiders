#pragma once
#include <memory>
#include "Rune.h"
#include "json.hpp"
using json = nlohmann::json;

class ReportMinionToClient : public Rune {
private:
	unsigned int cost;
	unsigned int uid;
	std::string id;
	std::string name;
	std::string set;

	unsigned int base_attack;
	unsigned int current_attack;
	unsigned int total_attack;
	
	unsigned int base_health;
	unsigned int current_health;
	unsigned int total_health;

	unsigned int controller_uid;
	bool is_deal;


public:
	ReportMinionToClient(json blob) : 
		cost(blob["cost"]),
		id(blob["id"]),
		uid(blob["uid"]),
		name(blob["name"]),
		set(blob["set"]),

		base_attack(blob["base_attack"]),
		current_attack(blob["current_attack"]),
		total_attack(blob["total_attack"]),

		base_health(blob["base_health"]),
		current_health(blob["current_health"]),
		total_health(blob["current_health"]),
		controller_uid(blob["controller_uid"]),
		is_deal(blob["is_deal"])
	{};
	void execute_rune(std::shared_ptr<class GameState> game_state) override;
};