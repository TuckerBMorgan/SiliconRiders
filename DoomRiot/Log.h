#pragma once

#include <string>
#include <vector>


struct LogMessage {
	std::string message;
	int duration;
};

class Log
{
private: 
	std::vector<LogMessage> messages;
public:
	void PrintLogMessage(std::string message, int duration) {
		LogMessage log_message;
		log_message.message = message;
		log_message.duration = duration;
		this->messages.push_back(log_message);
	}

	void Render(class RenderCarePackage*);
};




