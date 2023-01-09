#include "errorhandler.hpp"

#include <StringUtils.h>
#include <StringUtils.cpp>

namespace Destruction {
    namespace RestAPI {
        HWND ErrorHandler::window = FindWindowA("Main", "Destruction Loader");

        std::map<std::string, std::string> ErrorHandler::ruErrors{
            {"given username is already in use", "Пользователь с таким логином уже зарегистрирован!"},
            {"char is prohibited to use in login", "В логине можно использовать только латинские буквы и цифры!"},
            {"char is prohibited to use in password", "Символ \"^\" запрещено использовать в пароле!"},
            {"char is prohibited to use in mail", "Почта может состоять только из латинских букв, цифр, и точек"},
            {"username doesn't exist or password is invalid", "Неверный логин или пароль"},
            {"unexpected exception while generating session", "Ошибка авторизации"},
        };

        const char* ErrorHandler::GetErrorFromStatus(std::string status) {
            char* strParts[2];
            StringUtils::split(status.c_str(), ':', strParts);
            return strParts[1] + 1;
        }

        char ErrorHandler::GetProhibitedChar(std::string charIsProhibitedError) {
            return charIsProhibitedError[1];
        }

        bool ErrorHandler::handle(const char* status) {
            if (StringUtils::contains(status, "Error")) {
                const char* error = GetErrorFromStatus(status);
                std::string ruError;

                if (StringUtils::contains(error, "char is prohibited")) {
                    ruError = ruErrors[error + 4];
                    if (StringUtils::contains(error, "password")) {
                        char prohibitedChar = GetProhibitedChar(error);
                        ruError[8] = prohibitedChar;
                    }
                }
                else ruError = ruErrors[error];

                MessageBoxA(window, ruError.c_str(), "Destruction Loader", MB_ICONERROR);
                return true;
            }
            return false;
        }
    }
}
