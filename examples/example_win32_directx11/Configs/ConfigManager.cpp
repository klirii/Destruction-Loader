#include "ConfigManager.hpp"
#include "../Features/Settings/Keycodes.hpp"

#include <string>
#include <fstream>
#include <StringUtils.h>

namespace Configs {
    std::string ConfigManager::UnlimitedCPS = std::string(getenv("appdata")) + "\\.vimeworld\\minigames\\UnlimitedCPS.ini";
    std::string ConfigManager::Spammer = std::string(getenv("appdata")) + "\\.vimeworld\\minigames\\Spammer.ini";
    bool ConfigManager::configsIsLoaded = false;

    uint64_t Spammer::ParseDelayFromSliders(uint64_t ms, uint64_t s, uint64_t m, uint64_t h) {
        return ms + (s * 1000) + (m * 60000) + (h * 3600000);
    }

    void Spammer::ParseDelayInSliders(int count, std::string unit, int& ms, int& s, int& m, int& h) {
        if (unit == "ms") ms = count;
        else if (unit == "s") s = count;
        else if (unit == "m") m = count;
        else if (unit == "h") h = count;
    }

    std::string Spammer::serialize_ms(uint64_t ms) {
        if (ms < 0) return "";
        if (ms % 3600000 == 0) return std::to_string(ms / 3600000) + "h";
        if (ms % 60000 == 0) return std::to_string(ms / 60000) + "m";
        if (ms % 1000 == 0) return std::to_string(ms / 1000) + "s";
        return std::to_string(ms) + "ms";
    }

    bool Spammer::SaveDelay(Features::Spammer* instance, int ms, int s, int m, int h) {
        instance->delay = Configs::Spammer::ParseDelayFromSliders(ms, s, m, h);
        if (Configs::ConfigManager::WriteFeatureSettings(instance)) return true;
        return false;
    }

    std::string Spammer::GetTimeUnit(std::string delay) {
        const char* c_str = delay.c_str();
        for (uint8_t i = 0; c_str[i]; i++)
            if ('a' <= c_str[i] && c_str[i] <= 'z')
                return std::string(c_str + i);
        return "";
    }

    int Spammer::GetTimeCount(std::string delay) {
        char* c_str = const_cast<char*>(delay.c_str());
        for (uint8_t i = 0; c_str[i]; i++) {
            if ('a' <= c_str[i] && c_str[i] <= 'z') {
                c_str[i] = '\0';
                return std::atoi(c_str);
            }
        }
        return 0;
    }

    bool Spammer::Parse(bool& antiMute, int& keyCode, int& delayCount, std::string& delayUnit, std::string& message) {
        char line[108];
        std::ifstream config(ConfigManager::Spammer);

        while (config.getline(line, 108)) {
            char* lineParts[2];
            StringUtils::split(line, '=', lineParts);

            if (StringUtils::contains(line, "keybind")) {
                if (!InverseKeycodes.count(lineParts[1])) return false;
                keyCode = InverseKeycodes[lineParts[1]];
            }
            else if (StringUtils::contains(line, "delay")) {
                delayUnit = GetTimeUnit(lineParts[1]);
                delayCount = GetTimeCount(lineParts[1]);
                if (delayUnit.empty() || delayCount == 0) return false;
            }
            else if (StringUtils::contains(line, "antimute")) {
                antiMute = strcmp(lineParts[1], "true") == 0 ? true : false;
            }
            else if (StringUtils::contains(line, "message")) {
                message = lineParts[1];
                if (message.empty() && message.length() > 100) return false;
            }
        }

        return true;
    }

    bool ConfigManager::WriteFeatureSettings(Features::Feature* feature) {
        if (feature->name == "spammer") {
            Features::Spammer* spammer = reinterpret_cast<Features::Spammer*>(feature);
            std::ofstream config(ConfigManager::Spammer, std::ios::trunc);

            config << "keybind=" + (spammer->keyCode == 0 ? "F10" : Keycodes[spammer->keyCode]) << std::endl;
            config << "delay=" + Spammer::serialize_ms(spammer->delay) << std::endl;
            config << std::string("antimute=") + (spammer->antiMute ? "true" : "false") << std::endl;
            config << "message=" + spammer->message;

            config.close();
            return true;
        }
        else if (feature->name == "unlimitedcps") {

            return true;
        }
        return false;
    }
}
