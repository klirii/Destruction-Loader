#pragma once
#include <iostream>
#include <map>
#include <Windows.h>

namespace Destruction {
    namespace RestAPI {
        class ErrorHandler {
        public:
            static HWND window;
            static std::map<std::string, std::string> ruErrors;

            static bool handle(const char* status);
        private:
            static const char* GetErrorFromStatus(std::string status);
            static char GetProhibitedChar(std::string charIsProhibitedError);
        };
    }
}
