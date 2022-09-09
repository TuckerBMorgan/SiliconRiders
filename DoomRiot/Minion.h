#pragma once
#include<string>

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
	unsigned int UID() { return this->uid; };
};