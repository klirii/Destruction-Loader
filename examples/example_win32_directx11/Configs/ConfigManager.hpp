#pragma once
#include <iostream>
#include "../Features/Features.hpp"

namespace Configs {
    class ConfigManager {
    public:
        static std::string UnlimitedCPS;
        static std::string Spammer;

        static bool WriteFeatureSettings(Features::Feature* feature);
    };

    class ConfigManagerUtils {
    public:
        static uint64_t parseTime(uint64_t ms, uint64_t s, uint64_t m, uint64_t h);
        static std::string ms2string(uint64_t ms);

        static bool parseTimeFromConfig(int& count, std::string& unit);
        static bool parseMessageFromConfig(std::string& message);
    private:
        static std::string getUnit(std::string delay);
        static int getCount(std::string delay);
    };
}
