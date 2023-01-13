#include "ConfigManager.hpp"
#include "../Features/Settings/Keycodes.hpp"

#include <string>
#include <fstream>
#include <StringUtils.h>

namespace Configs {
    std::string ConfigManager::UnlimitedCPS = std::string(getenv("appdata")) + "\\.vimeworld\\minigames\\UnlimitedCPS.ini";
    std::string ConfigManager::Spammer = std::string(getenv("appdata")) + "\\.vimeworld\\minigames\\Spammer.ini";

    uint64_t ConfigManagerUtils::parseTime(uint64_t ms, uint64_t s, uint64_t m, uint64_t h) {
        return ms + (s * 1000) + (m * 60000) + (h * 3600000);
    }

    std::string ConfigManagerUtils::ms2string(uint64_t ms) {
        if (ms < 0) return "";
        if (ms % 3600000 == 0) return std::to_string(ms / 3600000) + "h";
        if (ms % 60000 == 0) return std::to_string(ms / 60000) + "m";
        if (ms % 1000 == 0) return std::to_string(ms / 1000) + "s";
        return std::to_string(ms) + "ms";
    }

    std::string ConfigManagerUtils::getUnit(std::string delay) {
        const char* c_str = delay.c_str();
        for (uint8_t i = 0; c_str[i]; i++)
            if ('a' <= c_str[i] && c_str[i] <= 'z')
                return std::string(c_str + i);
        return "";
    }

    int ConfigManagerUtils::getCount(std::string delay) {
        char* c_str = const_cast<char*>(delay.c_str());
        for (uint8_t i = 0; c_str[i]; i++) {
            if ('a' <= c_str[i] && c_str[i] <= 'z') {
                c_str[i] = '\0';
                return std::atoi(c_str);
            }
        }
        return 0;
    }

    bool ConfigManagerUtils::parseTimeFromConfig(int& count, std::string& unit) {
        char line[108];
        std::ifstream config(ConfigManager::Spammer);
        while (config.getline(line, 108)) {
            if (StringUtils::contains(line, "delay")) {
                char* delayLine[2];
                StringUtils::split(line, '=', delayLine);

                unit = getUnit(delayLine[1]);
                count = getCount(delayLine[1]);

                if (unit.empty() || count == 0) return false;
                return true;
            }
        }
        return false;
    }

    bool ConfigManagerUtils::parseMessageFromConfig(std::string& message) {
        char line[108];
        std::ifstream config(ConfigManager::Spammer);
        while (config.getline(line, 108)) {
            if (StringUtils::contains(line, "message")) {
                char* delayLine[2];
                StringUtils::split(line, '=', delayLine);

                message = delayLine[1];
                if (message.empty() && message.length() > 100) return false;
                return true;
            }
        }
        return false;
    }

    bool ConfigManager::WriteFeatureSettings(Features::Feature* feature) {
        if (feature->name == "Spammer") {
            Features::Spammer* spammer = reinterpret_cast<Features::Spammer*>(feature);
            std::ofstream config(ConfigManager::Spammer, std::ios::trunc);

            config << "keybind=" + (spammer->keyCode == 0 ? "F10" : Keycodes[spammer->keyCode]) << std::endl;
            config << "delay=" + ConfigManagerUtils::ms2string(spammer->delay) << std::endl;
            config << std::string("antimute=") + (spammer->antiMute ? "true" : "false") << std::endl;
            config << "message=" + spammer->message;

            config.close();
            return true;
        }
        else if (feature->name == "UnlimitedCPS") {

            return true;
        }
        return false;
    }
}
