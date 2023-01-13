#pragma once
#include <iostream>
#include <map>
#include <Windows.h>

namespace RestAPI {
    class ErrorHandler {
    public:
        static HWND hWindow;
        static bool handle(const char* status);
    private:
        static std::map<std::string, std::string> ruErrors;
        static const char* GetErrorFromStatus(std::string status);
        static char GetProhibitedChar(std::string charIsProhibitedError);
    };
}
