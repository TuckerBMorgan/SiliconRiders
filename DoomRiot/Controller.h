#pragma once
#include <iostream>
#include <string>
#include "OptionRune.h"

#include "json.hpp"

using json = nlohmann::json;

enum class EControllerState
{
	Waiting = 0,
	InMulligan = 1,
	InTurn = 2
};
std::ostream& operator << (std::ostream& strm, EControllerState tt);


class Controller {
private:
	unsigned int client_id;
	unsigned int uid;
	
	// Attributes
	unsigned int base_mana;
	unsigned int current_mana;

	bool is_me;
	std::string hero;
	std::vector<unsigned int> in_deck;
	std::vector<unsigned int> in_hand;
	std::vector<unsigned> on_battlefield;
	EControllerState controller_state;
	std::vector<Option> current_options;

public:
	Controller(unsigned int client_id, unsigned int uid, bool is_me, std::string hero) : client_id(client_id), uid(uid), is_me(is_me), hero(hero), base_mana(0), current_mana(0) { controller_state = EControllerState::Waiting;  };
	
	void AddMinionToControllersDeck(unsigned int minion_id);
	void AddMinionToHand(unsigned int minion_id);
	void RemoveCardFromHandIfPresent(unsigned int minion_id);
	void AddMinionToBattlefieldAtIndex(unsigned int minion_id, int field_index);
	

	unsigned int ControllerId() { return this->uid; };
	bool IsLocal();
	void SetControllerState(EControllerState state) {
		std::cout << "Changed controller state from " << this->controller_state << " to " << state << std::endl;
		this->controller_state = state; 
	};
	
	void SetBaseMana(unsigned int new_base_mana) { this->base_mana = new_base_mana; };
	void SetCurrentMana(unsigned int new_current_mana) { this->current_mana = new_current_mana; };
	EControllerState GetControllerState() { return this->controller_state; };

	void SetCurrentOptions(std::vector<Option> options) {
		this->current_options = options;
	}
	bool HasOptions() {
		return this->current_options.size() > 0;
	}

	std::vector<unsigned int> GetHandIds() {
		std::vector<unsigned int> new_vector;
		for (auto id : this->in_hand) {
			new_vector.push_back(id);
		}
		return new_vector;
	}

	std::vector<unsigned int> GetBattlefieldIds() {
		std::vector<unsigned int> new_vector;
		for (auto id : this->on_battlefield) {
			new_vector.push_back(id);
		}
		return new_vector;
	}


	int GetCurrentMana() {
		return this->current_mana;
	}

	std::vector<Option> OptionsOfType(std::string option_type) {
		std::vector<Option> return_options;
		for (auto& ptr : this->current_options) {
			if (ptr.option_type == option_type) {
				return_options.push_back(ptr);
			}
		}
		return return_options;
	};
};