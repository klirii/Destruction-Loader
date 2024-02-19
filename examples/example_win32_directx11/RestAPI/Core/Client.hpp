#pragma once
#include <iostream>
#include <Windows.h>
#include <vector>
#include <nlohmann/json.hpp>

namespace RestAPI {
    using json = nlohmann::json;

    struct UserData {
        std::string name;
        std::string password;
        std::string session;

        static bool save(std::string name, std::string password);
        static bool get(std::string& name, std::string& pass);
        static bool del();
    private:
        static std::string directory;
        static std::string path;
    };

    class Client {
    public:
        std::string host;
        UserData user;

        std::string version;
        std::string state;

        Client() {}
        Client(std::string host) {this->host = host;}

        bool GetVersion();
        bool GetState();

        bool Register(const char* username, const char* password, const char* email, std::string unHash);
        bool Login(const char* username, const char* password);
        bool GetFeatures(std::string username, std::string password, std::string session, json& features);
        bool GetSessionHash(std::string hash, std::string username, std::string password, std::string session, std::vector<std::uint8_t>& dll);
    private:
        static const char* prohibitedChars;
        static bool CheckFieldsChars(const char* username, const char* password, const char* email = nullptr);
    };
}
