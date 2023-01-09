#pragma once
#include <iostream>
#include <nlohmann/json.hpp>

namespace Destruction {
    namespace RestAPI {
        using json = nlohmann::json;

        struct UserData {
            std::string session;
            std::string name;
            std::string password;
        };

        class Client {
        public:
            UserData user;
            Client(std::string host) {this->host = host;}

            bool reg(const char* username, const char* password, const char* email, std::string hash);
            bool login(const char* username, const char* password);

            bool GetFeatures(std::string username, std::string password, std::string session, json& features);
        private:
            std::string host;
            static const char* prohibitedChars;
            static bool checkFieldsChars(const char* username, const char* password, const char* email = nullptr);
        };
    }
}
