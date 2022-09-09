
#include <iostream>
#include <mutex>


#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>

// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

#include "NetworkingThread.h"
#include "json.hpp"


constexpr auto DEFAULT_BUFLEN = 2048;
constexpr auto DEFAULT_PORT = "1337";
constexpr auto MESSAGE_BREAK_CHARACTER = '@';

using json = nlohmann::json;

std::vector<std::string> find_network_messages(const char* buffer, size_t buffer_length) {
    bool found_first_symbol = false;
    std::vector<std::string> return_buffer;
    size_t message_start = 0;
    for (size_t i = 0; i < buffer_length; i++) {
        if (buffer[i] == MESSAGE_BREAK_CHARACTER) {
            if (found_first_symbol == false) {
                found_first_symbol = true;
            }
            else {
                //Copy into new string
                std::string message(&buffer[message_start], i - 1 - message_start);
                return_buffer.push_back(message);
                found_first_symbol = false;
                message_start = i + 1;
            }
        }
    }

    return return_buffer;
}

void network_thread(ThreadParameters thead_parameters) {
    WSADATA wsaData;
    SOCKET ConnectSocket = INVALID_SOCKET;
    struct addrinfo* result = NULL,
        * ptr = NULL,
        hints;
    const char* sendbuf = "this is a test";
    char recvbuf[DEFAULT_BUFLEN];
    char send_buffer[DEFAULT_BUFLEN];
    int iResult;
    int recvbuflen = DEFAULT_BUFLEN;

    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed with error: %d\n", iResult);
        //return 1;TODO: Figure out a way to handle this error here
    }


    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    iResult = getaddrinfo("127.0.0.1", DEFAULT_PORT, &hints, &result);
    if (iResult != 0) {
        printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
       // return 1;TODO: Figure out a way to handle this error here
    }

    // Attempt to connect to an address until one succeeds
    for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {

        // Create a SOCKET for connecting to server
        ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype,
            ptr->ai_protocol);
        
        if (ConnectSocket == INVALID_SOCKET) {
            printf("socket failed with error: %ld\n", WSAGetLastError());
            WSACleanup();
            //return 1;TODO: Figure out a way to handle this error here
        }


        // Connect to server.
        iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
        if (iResult == SOCKET_ERROR) {
            closesocket(ConnectSocket);
            ConnectSocket = INVALID_SOCKET;
            continue;
        }
        break;
    }
    unsigned long ul = 1;
    auto test = ioctlsocket(ConnectSocket, FIONBIO, &ul);
    do {

        {
            thead_parameters.to_server_message_channel_lock.lock();
            while (thead_parameters.to_server_message_channel->empty() == false) {
                auto new_message = thead_parameters.to_server_message_channel->front();
                thead_parameters.to_server_message_channel->pop();
                auto jump = new_message.dump();

                for (int i = 0; i < jump.size();i++) {
                    send_buffer[i] = jump.c_str()[i];                        
                }
                auto result = send(ConnectSocket, send_buffer, jump.size(), 0);
            }
            thead_parameters.to_server_message_channel_lock.unlock();
        }
        
        iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
        if (iResult > 0) {
            auto test = find_network_messages(recvbuf, iResult);
            thead_parameters.message_channel_lock.lock();
            for (auto message : test) {
                auto js_msg = json::parse(message);
                thead_parameters.message_channel.get()->push(std::move(js_msg));
            }
            thead_parameters.message_channel_lock.unlock();
        }

        else if (iResult == 0)
            printf("Connection closed\n");
        else if (WSAGetLastError() == 10035) {
            iResult = 1;
            continue;
        }
        else
            printf("recv failed with error: %d\n", WSAGetLastError());

    } while (iResult > 0);
    printf("recv failed with error: %d\n", WSAGetLastError());
    std::cout << "It fucking exited" << std::endl;
}