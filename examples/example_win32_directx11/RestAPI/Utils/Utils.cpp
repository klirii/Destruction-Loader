#include "Utils.hpp"
#include <fstream>

namespace RestAPI {
    std::vector<std::uint8_t> Utils::rolling_xor(std::vector<std::uint8_t> bytes, bool decode) {
        if (decode) {
            reverse(bytes.begin(), bytes.end());
            std::vector<std::uint8_t> output;

            for (int i = 0; i < bytes.size(); i++) {
                if (i == (bytes.size() - 1)) output.push_back(bytes[i]);
                else output.push_back(bytes[i] ^ bytes[i + 1]);
            }

            reverse(output.begin(), output.end());
            return output;
        }
        else {
            std::vector<std::uint8_t> output(1);
            output[0] = bytes[0];

            for (int i = 0; i < (bytes.size() - 1); i++) output.push_back(bytes[i + 1] ^ output[i]);
            return output;
        }
    }

    std::vector<std::uint8_t> Utils::reverse_bytes(std::vector<std::uint8_t> bytes) {
        std::vector<std::uint8_t> reversed;
        for (int i = (bytes.size() - 1); i >= 0; i--) reversed.push_back(bytes[i]);
        return reversed;
    }

    bool Utils::containsIllegalChars(std::string field, bool email) {
        const char* c_str = field.c_str();
        for (; *c_str; c_str++) {
            if (('a' <= *c_str && *c_str <= 'z') || ('A' <= *c_str && *c_str <= 'Z') || ('0' <= *c_str && *c_str <= '9')) {}
            else if (email ? *c_str == '.' || *c_str == '@' : email) {}
            else return true;
        }
        return false;
    }

    std::vector<std::uint8_t> Utils::hex2bytes(const std::string& hex) {
        std::vector<std::uint8_t> bytes;
        for (unsigned int i = 0; i < hex.length(); i += 2) {
            std::string byteString = hex.substr(i, 2);
            char bytee = (char)strtol(byteString.c_str(), nullptr, 16);
            bytes.push_back(bytee);
        }
        return bytes;
    }

    size_t CURLUtils::response2string(void* data, size_t size, size_t nmemb, void* userp) {
        ((std::string*)userp)->append((char*)data, size * nmemb);
        return size * nmemb;
    }
}
