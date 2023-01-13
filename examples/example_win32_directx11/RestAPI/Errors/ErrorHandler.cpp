#include "ErrorHandler.hpp"

#include <StringUtils.h>
#include <StringUtils.cpp>

namespace RestAPI {
    HWND ErrorHandler::hWindow = FindWindowA("Main", "Destruction Loader");

    std::map<std::string, std::string> ErrorHandler::ruErrors{
        // register
        {"given username is already in use", "������������ � ����� ������� ��� ���������������!"},

        {"char is prohibited to use in login", "� ������ ����� ������������ ������ ��������� ����� � �����!"},
        {"char is prohibited to use in password", "������ \"^\" ��������� ������������ � ������!"},
        {"char is prohibited to use in mail", "����� ����� �������� ������ �� ��������� ����, ����, � �����"},
        {"char is prohibited to use in hash", "������ #8"},
        {"char is prohibited to use in session", "������ #19"},
        {"you can't register account right now", "�� ������� ����� ������������� ��������, ���������� �����"},

        // login
        {"username doesn't exist or password is invalid", "�������� ����� ��� ������"},
        {"unexpected exception while generating session", "������ �����������"},

        // getfeatures
        {"given username are not exist", "������ #0"},
        {"invalid password", "������ #1"},
        {"invalid session", "������ #2"},
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

            MessageBoxA(hWindow, ruError.c_str(), "Destruction Loader", MB_ICONERROR);
            return true;
        }
        return false;
    }
}
