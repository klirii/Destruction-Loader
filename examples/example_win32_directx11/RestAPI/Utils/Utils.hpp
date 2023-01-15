#pragma once
#include <Windows.h>
#include <iostream>
#include <vector>
#include <md5.h>

namespace RestAPI {
    class Utils {
    public:
        static __forceinline std::string GetUnHash() {
            return md5(md5(getenv("USERNAME")));
        }

        static std::vector<std::uint8_t> rolling_xor(std::vector<std::uint8_t> bytes, bool decode = false);
        static std::vector<std::uint8_t> reverse_bytes(std::vector<std::uint8_t> bytes);

        static bool containsIllegalChars(std::string field, bool email = false);
        static std::vector<std::uint8_t> hex2bytes(const std::string& hex);
    };

    class CURLUtils {
    public:
        static size_t response2string(void* data, size_t size, size_t nmemb, void* userp);
    };
}
