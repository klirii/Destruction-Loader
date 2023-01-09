#pragma once
#include <Windows.h>
#include <iostream>

namespace Destruction {
    namespace RestAPI {
        class Utils {
        public:
            static bool containsIllegalChars(std::string field, bool email = false);
        };
        
        class CURLUtils {
        public:
            static size_t response2string(void* data, size_t size, size_t nmemb, void* userp);
        };
    }
}
