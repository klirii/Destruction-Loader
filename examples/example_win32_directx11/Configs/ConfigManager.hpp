#pragma once
#include <iostream>
#include "../Features/Features.hpp"

namespace Configs {
    class ConfigManager {
    public:
        static std::string UnlimitedCPS;
        static std::string Spammer;
        static bool configsIsLoaded;
        static bool WriteFeatureSettings(Features::Feature* feature);
    };

    class Spammer {
    public:
        static uint64_t ParseDelayFromSliders(uint64_t ms, uint64_t s, uint64_t m, uint64_t h);
        static void ParseDelayInSliders(int count, std::string unit, int& ms, int& s, int& m, int& h);

        static std::string serialize_ms(uint64_t ms);
        static bool SaveDelay(Features::Spammer* instance, int ms, int s, int m, int h);

        static bool Parse(bool& antiMute, int& keyCode, int& delayCount, std::string& delayUnit, std::string& message);
    private:
        static std::string GetTimeUnit(std::string delay);
        static int GetTimeCount(std::string delay);
    };
}
