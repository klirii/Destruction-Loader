#include "utils.hpp"

namespace Destruction {
    namespace RestAPI {
        bool Utils::containsIllegalChars(std::string field, bool email) {
            const char* c_str = field.c_str();
            for (; *c_str; c_str++) {
                if (('a' <= *c_str && *c_str <= 'z') || ('A' <= *c_str && *c_str <= 'Z') || ('0' <= *c_str && *c_str <= '9')) {}
                else if (email ? *c_str == '.' || *c_str == '@' : email) {}
                else return true;
            }
            return false;
        }

        size_t CURLUtils::response2string(void* data, size_t size, size_t nmemb, void* userp) {
            ((std::string*)userp)->append((char*)data, size * nmemb);
            return size * nmemb;
        }
    }
}
