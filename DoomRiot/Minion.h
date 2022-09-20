#pragma once
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>

class Minion {
private:
	unsigned int cost;
	unsigned int uid;
	std::string id;	
	std::string display_name;
	std::string set;

	unsigned int base_attack;
	unsigned int current_attack;
	unsigned int total_attack;

	unsigned int base_health;
	unsigned int current_health;
	unsigned int total_health;

	unsigned int controlled_uid;

	std::vector<std::string> tags;



public:

	Minion(unsigned int cost,
		unsigned int uid,
		std::string id,
		std::string display_name,
		std::string set,

		unsigned int base_attack,
		unsigned int current_attack,
		unsigned int total_attack,

		unsigned int base_health,
		unsigned int current_health,
		unsigned int total_health,

		unsigned int controlled_uid
	) : cost(cost), 
		uid(uid),
		id(id),
		display_name(display_name),
		set(set),
		base_attack(base_attack),
		current_attack(current_attack),
		total_attack(total_attack),
		base_health(base_health),
		current_health(current_health),
		total_health(total_health),
		controlled_uid(controlled_uid)
	{};
	unsigned int UID() const { return this->uid; };

	std::string GetName() const {
		return this->display_name;
	};

	int GetCurrentAttack() const {
		return this->current_attack;
	};

	int GetCurrentHealth() const {
		return this->current_health;
	};

	void AddTag(std::string tag) {
		std::cout << "Adding Tag: " << tag << "to minion " << this->id << std::endl;
		// TODO: this SHOULD be a hashset, a TAG should be an enum that is genned from the code
		this->tags.push_back(tag);
	}

	void RemoveTag(std::string tag) {
		auto index = std::find(this->tags.begin(), this->tags.end(), tag);
		if (index != this->tags.end()) {
			std::cout << "Revmoing Tag: " << tag << "from minion " << this->id << std::endl;
			this->tags.erase(index);
		}
	}
};