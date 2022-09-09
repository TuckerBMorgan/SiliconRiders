#pragma once
#include <queue>
#include <string>
#include <mutex>

#include "json.hpp"
using json = nlohmann::json;

struct ThreadParameters {
    std::mutex& message_channel_lock;
    std::shared_ptr<std::queue<json>> message_channel;
    std::mutex& to_server_message_channel_lock;
    std::shared_ptr<std::queue<json>> to_server_message_channel;
    ThreadParameters(std::mutex& message_channel_lock, std::shared_ptr<std::queue<json>> message_channel, std::mutex& to_server_message_channel_lock, std::shared_ptr<std::queue<json>> to_server_message_channel)
            : message_channel_lock(message_channel_lock), message_channel(message_channel), to_server_message_channel_lock(to_server_message_channel_lock), to_server_message_channel(to_server_message_channel){}
};

void network_thread(ThreadParameters thead_parameters);