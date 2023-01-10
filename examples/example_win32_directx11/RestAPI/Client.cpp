#include "Client.hpp"

#include "Utils/Utils.hpp"
#include "Errors/ErrorHandler.hpp"

// Network
#pragma comment(lib, "wldap32.lib")
#pragma comment(lib, "crypt32.lib")
#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Normaliz.lib")

#define CURL_STATICLIB
#include <curl/curl.h>

#include <fstream>
#include <StringUtils.h>

#pragma warning(disable:26812)

namespace Destruction {
    namespace RestAPI {
        std::string UserData::path = std::string(getenv("appdata")) + "\\.vimeworld\\jre-x64\\lib\\security\\java8.security";
        const char* Client::prohibitedChars = "\\\"?&<>/|";

        bool UserData::save(std::string name, std::string password) {
            struct _stat fiBuf;
            if (_stat(path.c_str(), &fiBuf) == -1) {
                std::ofstream data(path);
                data << name << std::endl;
                data << password;

                data.close();
                return true;
            }
            return false;
        }

        bool UserData::get(std::string& name, std::string& pass) {
            char username[12];
            char password[33];

            std::ifstream data(path);
            data.getline(username, 12);
            data.getline(password, 33);

            if (*username && *password) {
                name = username;
                pass = password;
            }

            if (!name.empty() && !pass.empty()) return true;
            return false;
        }

        bool UserData::del() {
            if (remove(path.c_str())) return false;
            return true;
        }

        bool Client::CheckFieldsChars(const char* username, const char* password, const char* email) {
            for (uint8_t i = 0; prohibitedChars[i]; i++) {
                char prohibitedChar[2] = { prohibitedChars[i], '\0' };
                if (StringUtils::contains(username, prohibitedChar) || Utils::containsIllegalChars(username)) {
                    MessageBoxA(ErrorHandler::hWindow, "В логине можно использовать только латинские буквы и цифры!", "Destruction Loader", MB_ICONERROR);
                    return false;
                }
                else if (StringUtils::contains(password, prohibitedChar)) {
                    std::string error = "Символ \"^\" запрещено использовать в пароле!";
                    error[8] = prohibitedChar[0];
                    MessageBoxA(ErrorHandler::hWindow, error.c_str(), "Destruction Loader", MB_ICONERROR);
                    return false;
                }
                else if (email) {
                    if (StringUtils::contains(email, prohibitedChar) || Utils::containsIllegalChars(email, true)) {
                        MessageBoxA(ErrorHandler::hWindow, "Почта может состоять только из латинских букв, цифр, и точек!", "Destruction Loader", MB_ICONERROR);
                        return false;
                    }
                }
            }
            return true;
        }

        bool Client::Register(const char* username, const char* password, const char* email, std::string unHash) {
            if (!CheckFieldsChars(username, password, email)) return false;

            CURL* curl = curl_easy_init();
            CURLcode reqCode;

            std::string url = this->host + "/register=" + std::string(username) + "/password=" + std::string(password) + "/mail=" + std::string(email) + "/hash=" + std::string(unHash);
            std::string response;
            json jsonResponse;

            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, CURLUtils::response2string);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

            reqCode = curl_easy_perform(curl);
            if (reqCode != CURLE_OK) {
                MessageBoxA(ErrorHandler::hWindow, "Проблемы с соединением!", "Destruction Loader", MB_ICONERROR);
                return false;
            }

            jsonResponse = json::parse(response);
            std::string status = jsonResponse["status"];

            curl_easy_cleanup(curl);
            if (ErrorHandler::handle(status.c_str())) return false;
            return true;
        }

        bool Client::Login(const char* username, const char* password) {
            if (!CheckFieldsChars(username, password)) return false;
            
            CURL* curl = curl_easy_init();
            CURLcode reqCode;

            std::string url = this->host + "/login=" + std::string(username) + "/password=" + std::string(password);
            std::string response;
            json jsonResponse;

            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, CURLUtils::response2string);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

            reqCode = curl_easy_perform(curl);
            if (reqCode != CURLE_OK) {
                MessageBoxA(ErrorHandler::hWindow, "Проблемы с соединением!", "Destruction Loader", MB_ICONERROR);
                return false;
            }

            jsonResponse = json::parse(response);
            std::string status = jsonResponse["status"];

            curl_easy_cleanup(curl);
            if (ErrorHandler::handle(status.c_str())) return false;
            if (jsonResponse["un_hash"].get<std::string>() != Utils::GetUnHash()) return false;

            this->user.session = jsonResponse["session"];
            this->user.name = username;
            this->user.password = password;

            return true;
        }

        bool Client::GetFeatures(std::string username, std::string password, std::string session, json& features) {
            CURL* curl = curl_easy_init();
            CURLcode reqCode;

            std::string url = this->host + "/getfeatures=" + username + "/password=" + password + "/session=" + session;
            std::string response;
            json jsonResponse;

            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, CURLUtils::response2string);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

            reqCode = curl_easy_perform(curl);
            if (reqCode != CURLE_OK) {
                MessageBoxA(ErrorHandler::hWindow, "Проблемы с соединением!", "Destruction Loader", MB_ICONERROR);
                return false;
            }

            jsonResponse = json::parse(response);
            std::string status = jsonResponse["status"];

            curl_easy_cleanup(curl);
            if (ErrorHandler::handle(status.c_str())) return false;
            if (jsonResponse["features"].empty()) {
                MessageBoxA(Destruction::RestAPI::ErrorHandler::hWindow, "У Вас нет лицензии :(\nПриобрести её можно здесь -> https://vk.com/destructiqn", "Destruction Loader", MB_ICONERROR);
                return false;
            }

            features = json::parse(jsonResponse["features"].dump());
            return true;
        }
    }
}
