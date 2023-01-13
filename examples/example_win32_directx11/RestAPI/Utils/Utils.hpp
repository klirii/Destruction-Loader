#pragma once
#include <Windows.h>
#include <iostream>
#include <md5.h>

namespace RestAPI {
    class Utils {
    public:
        static __forceinline std::string GetUnHash() {
            return md5(md5(getenv("USERNAME")));
        }
        static bool containsIllegalChars(std::string field, bool email = false);
    };

    class CURLUtils {
    public:
        static size_t response2string(void* data, size_t size, size_t nmemb, void* userp);
    };
}
