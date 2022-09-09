#pragma once
#include <string>
#include <vector>
#include <iostream>
#include "Rune.h"
#include "json.hpp"

using json = nlohmann::json;

class Option {
    std::string option_type;
    unsigned int source;
    unsigned int target;
public:
    Option(json blob) : option_type(blob["option_type"]), source(blob["source_uid"]), target(blob["target_uid"]) {};
};

class OptionRune :
    public Rune
{
    std::vector<Option> options;
public:
    OptionRune(json blob) {
        auto& options = blob["options"];
        for (auto& option : options) {
            this->options.push_back(Option(option));
        }
    };
    void execute_rune(std::shared_ptr<class GameState> game_State) override;
};

