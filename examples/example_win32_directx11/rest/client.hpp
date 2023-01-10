#pragma once
#include <iostream>
#include <nlohmann/json.hpp>

namespace Destruction {
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
            static std::string path;
        };

        class Client {
        public:
            UserData user;
            Client(std::string host) {this->host = host;}

            bool Register(const char* username, const char* password, const char* email, std::string unHash);
            bool Login(const char* username, const char* password);
            bool GetFeatures(std::string username, std::string password, std::string session, json& features);
        private:
            std::string host;
            static const char* prohibitedChars;
            static bool CheckFieldsChars(const char* username, const char* password, const char* email = nullptr);
        };
    }
}
