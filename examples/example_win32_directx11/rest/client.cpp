#include "client.hpp"

#include "utils/utils.hpp"
#include "utils/errorhandler.hpp"

// Network
#pragma comment(lib, "wldap32.lib")
#pragma comment(lib, "crypt32.lib")
#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Normaliz.lib")

#define CURL_STATICLIB
#include <curl/curl.h>

#include <StringUtils.h>

#pragma warning(disable:26812)

namespace Destruction {
    namespace RestAPI {
        const char* Client::prohibitedChars = "\\\"?&<>/|";

        bool Client::checkFieldsChars(const char* username, const char* password, const char* email) {
            for (uint8_t i = 0; prohibitedChars[i]; i++) {
                char prohibitedChar[2] = { prohibitedChars[i], '\0' };
                if (StringUtils::contains(username, prohibitedChar) || Utils::containsIllegalChars(username)) {
                    MessageBoxA(ErrorHandler::window, "В логине можно использовать только латинские буквы и цифры!", "Destruction Loader", MB_ICONERROR);
                    return false;
                }
                else if (StringUtils::contains(password, prohibitedChar)) {
                    std::string error = "Символ \"^\" запрещено использовать в пароле!";
                    error[8] = prohibitedChar[0];
                    MessageBoxA(ErrorHandler::window, error.c_str(), "Destruction Loader", MB_ICONERROR);
                    return false;
                }
                else if (email) {
                    if (StringUtils::contains(email, prohibitedChar) || Utils::containsIllegalChars(email, true)) {
                        MessageBoxA(ErrorHandler::window, "Почта может состоять только из латинских букв, цифр, и точек!", "Destruction Loader", MB_ICONERROR);
                        return false;
                    }
                }
            }
            return true;
        }

        bool Client::reg(const char* username, const char* password, const char* email, std::string hash) {
            if (!checkFieldsChars(username, password, email)) return false;

            CURL* curl = curl_easy_init();
            CURLcode reqCode;

            std::string url = this->host + "/register=" + std::string(username) + "/password=" + std::string(password) + "/mail=" + std::string(email) + "/hash=" + std::string(hash);
            std::string response;
            json jsonResponse;

            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, CURLUtils::response2string);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

            reqCode = curl_easy_perform(curl);
            if (reqCode != CURLE_OK) {
                MessageBoxA(ErrorHandler::window, "Проблемы с соединением!", "Destruction Loader", MB_ICONERROR);
                return false;
            }

            jsonResponse = json::parse(response);
            std::string status = jsonResponse["status"];

            curl_easy_cleanup(curl);
            if (ErrorHandler::handle(status.c_str())) return false;
            return true;
        }

        bool Client::login(const char* username, const char* password) {
            if (!checkFieldsChars(username, password)) return false;

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
                MessageBoxA(ErrorHandler::window, "Проблемы с соединением!", "Destruction Loader", MB_ICONERROR);
                return false;
            }

            jsonResponse = json::parse(response);
            std::string status = jsonResponse["status"];

            curl_easy_cleanup(curl);
            if (ErrorHandler::handle(status.c_str())) return false;
            if (jsonResponse["un_hash"] != "CAFEBABE") return false;

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
                MessageBoxA(ErrorHandler::window, "Проблемы с соединением!", "Destruction Loader", MB_ICONERROR);
                return false;
            }

            jsonResponse = json::parse(response);
            std::string status = jsonResponse["status"];

            curl_easy_cleanup(curl);
            if (ErrorHandler::handle(status.c_str())) return false;
            if (jsonResponse["features"].empty()) return false;

            features = json::parse(jsonResponse["features"]);
            return true;
        }
    }
}
