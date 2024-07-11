#pragma once
#include "../Features/Features.hpp"
#include <string>
#include <fstream>

namespace Configs {
    class ConfigManager {
    public:
        static std::string UnlimitedCPS;
        static std::string Spammer;
        static bool WriteFeatureSettings(Features::Feature* feature);
    };

    class Spammer {
    public:
        static bool configIsLoaded;

        static uint64_t ParseDelayFromSliders(uint64_t ms, uint64_t s, uint64_t m, uint64_t h);
        static void ParseDelayInSliders(int count, std::string unit, int& ms, int& s, int& m, int& h);

        static std::string serialize_ms(uint64_t ms);
        static bool SaveDelay(Features::Spammer* instance, int ms, int s, int m, int h);

        static bool Parse(bool& antiMute, int& keyCode, int& delayCount, std::string& delayUnit, std::string& message);
    private:
        static std::string GetTimeUnit(std::string delay);
        static int GetTimeCount(std::string delay);
    };

    class UnlimitedCPS {
    public:
        static bool configIsLoaded;
        static bool Parse(bool& isEnabled, int& keyCode);
    };

    namespace Velocity {
        static HWND loader = FindWindowA("Main", "Destruction Loader");

        static std::string path = std::string(getenv("APPDATA")) + "\\.vimeworld\\minigames\\Velocity.ini";
        static bool loaded = false;

        void Rewrite(Features::Velocity* velocity);
        void Load(
            int& horizontal_min,
            int& horizontal_max,
            int& vertical_min,
            int& vertical_max,
            int& keycode,
            bool& only_forward,
            bool& only_moving,
            bool& enabled
        );
        void UpdateState(bool& enabled);
    }

    namespace Visuals {
        extern std::string config_path;
        extern bool config_is_loaded;

        void UpdateSettings(std::string data);
    }
}
