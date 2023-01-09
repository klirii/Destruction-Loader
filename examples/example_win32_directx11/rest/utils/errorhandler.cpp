#include "errorhandler.hpp"

#include <StringUtils.h>
#include <StringUtils.cpp>

namespace Destruction {
    namespace RestAPI {
        HWND ErrorHandler::window = FindWindowA("Main", "Destruction Loader");

        std::map<std::string, std::string> ErrorHandler::ruErrors{
            {"given username is already in use", "������������ � ����� ������� ��� ���������������!"},
            {"char is prohibited to use in login", "� ������ ����� ������������ ������ ��������� ����� � �����!"},
            {"char is prohibited to use in password", "������ \"^\" ��������� ������������ � ������!"},
            {"char is prohibited to use in mail", "����� ����� �������� ������ �� ��������� ����, ����, � �����"},
            {"username doesn't exist or password is invalid", "�������� ����� ��� ������"},
            {"unexpected exception while generating session", "������ �����������"},
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
