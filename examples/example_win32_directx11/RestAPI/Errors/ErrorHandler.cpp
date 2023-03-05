#include "ErrorHandler.hpp"

#include <StringUtils.h>
#include <StringUtils.cpp>

namespace RestAPI {
    HWND ErrorHandler::hWindow = FindWindowA("Main", "Destruction Loader");

    std::map<std::string, std::string> ErrorHandler::ruErrors{
        // register
        {"required in query params", "Ошибка #0"},
        {"char is prohibited to use in params", "Ошибка #1"},
        {"account with given username already exists", "Кто-то уже использует такой логин :("},
        {"failed to register account", "Не получилось зарегистрировать аккаунт, обратитесь в сообщество"},
        {"unresolvable exception occured", "Возникла неопознанная ошибка, обратитесь в сообщество"},

        // login
        {"attempt to login non-existent account", "Аккаунта с таким логином не существует!"},
        {"invalid password", "Неверный пароль!"},

        // getfeatures
        {"invalid login, password or session", "Ошибка #2"},

        // getsessionhash
        {"invalid hash", "Ошибка #3"}
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
            else if (StringUtils::contains(error, "required in query params")) {
                ruError = ruErrors[error + 4];
            }
            else ruError = ruErrors[error];

            MessageBoxA(hWindow, ruError.c_str(), "Destruction Loader", MB_ICONERROR);
            return true;
        }
        return false;
    }
}
