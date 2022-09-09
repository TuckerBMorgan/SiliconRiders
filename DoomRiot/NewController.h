#pragma once
#include <string>
#include <memory>
#include "Rune.h"
#include "json.hpp"
using json = nlohmann::json;

class NewController : public Rune {
private:
	unsigned int uid;
	std::string hero;
	unsigned int client_id;
	std::string deck;
	bool is_me;
	
public:
	NewController(json blob);
	void execute_rune(std::shared_ptr<class GameState> game_State) override;
};