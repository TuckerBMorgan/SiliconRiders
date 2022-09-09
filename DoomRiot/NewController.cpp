#include <iostream>


#include "Rune.h"
#include "GameState.h"
#include "NewController.h"
#include "Controller.h"

NewController::NewController(json blob) {
	this->uid = blob["uid"];
	this->hero = blob["hero"];
	this->client_id = blob["client_id"];
	this->deck = blob["deck"];
	this->is_me = blob["is_me"];
}

void NewController::execute_rune(std::shared_ptr<GameState> game_state) {
	std::cout << "Executing NewController Rune with parmeters" << " uid: " << this->uid << " client_id: " << this->client_id << " is_me: " << this->is_me << std::endl;
	Controller controller(this->client_id, this->uid, this->is_me, this->hero);
	//This is a copy, how to avoid that???
	game_state->AddController(controller);
}