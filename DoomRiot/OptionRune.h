#pragma once
#include <string>
#include <vector>
#include <iostream>
#include "Rune.h"
#include "json.hpp"

using json = nlohmann::json;

class Option {
public:
    std::string option_type;
    unsigned int source;
    unsigned int target;
    int option_index;

    Option(json blob, int option_index) : option_type(blob["option_type"]), source(blob["source_uid"]), target(blob["target_uid"]), option_index(option_index) {};
};

class OptionRune :
    public Rune
{
    std::vector<Option> options;
public:
    OptionRune(json blob) {
        auto& options = blob["options"];
        int i = 0;
        for (auto& option : options) {
            this->options.push_back(Option(option, i)); 
            i++;
        }
    };
    void execute_rune(std::shared_ptr<class GameState> game_State) override;
};

