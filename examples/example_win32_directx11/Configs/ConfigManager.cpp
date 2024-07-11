#include "ConfigManager.hpp"

#include "../Features/Settings/Keycodes.hpp"
#include "../conversion.hpp"

#include <string>
#include <fstream>
#include <codecvt>
#include <StringUtils.h>

namespace Configs {
    std::string ConfigManager::UnlimitedCPS = std::string(getenv("appdata")) + "\\.vimeworld\\minigames\\UnlimitedCPS.ini";
    std::string ConfigManager::Spammer = std::string(getenv("appdata")) + "\\.vimeworld\\minigames\\Spammer.ini";

    bool Spammer::configIsLoaded = false;
    bool UnlimitedCPS::configIsLoaded = false;

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
        std::string line;
        std::wstring wline;

        std::wifstream config(ConfigManager::Spammer);
        config.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t>));

        while (std::getline(config, wline)) {
            line = unicode2ansi(wline);
            char* lineParts[2];
            StringUtils::split(line.c_str(), '=', lineParts);

            if (StringUtils::contains(line.c_str(), "keybind")) {
                if (!InverseKeycodes.count(lineParts[1])) return false;
                keyCode = InverseKeycodes[lineParts[1]];
            }
            else if (StringUtils::contains(line.c_str(), "delay")) {
                delayUnit = GetTimeUnit(lineParts[1]);
                delayCount = GetTimeCount(lineParts[1]);
                if (delayUnit.empty() || delayCount == 0) return false;
            }
            else if (StringUtils::contains(line.c_str(), "antimute")) {
                antiMute = strcmp(lineParts[1], "true") == 0 ? true : false;
            }
            else if (StringUtils::contains(line.c_str(), "message")) {
                message = lineParts[1];
                if (message.empty() || message.length() > 100) return false;
            }
        }

        config.close();
        return true;
    }

    bool UnlimitedCPS::Parse(bool& isEnabled, int& keyCode) {
        std::string line;
        std::wstring wline;

        std::wifstream config(ConfigManager::UnlimitedCPS);
        config.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t>));

        while (std::getline(config, wline)) {
            line = unicode2ansi(wline);
            char* lineParts[2];
            StringUtils::split(line.c_str(), '=', lineParts);

            if (StringUtils::contains(line.c_str(), "keybind"))
                keyCode = InverseKeycodes[lineParts[1]];
            else if (StringUtils::contains(line.c_str(), "enabled")) {
                isEnabled = strcmp(lineParts[1], "true") == 0 ? true : false;
            }
        }

        config.close();
        return true;
    }

    bool ConfigManager::WriteFeatureSettings(Features::Feature* feature) {
        if (feature->name == "spammer") {
            Features::Spammer* spammer = reinterpret_cast<Features::Spammer*>(feature);

            std::wofstream config(ConfigManager::Spammer, std::ios::trunc);
            config.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t>));

            config << L"keybind=" + (spammer->keyCode == NULL ? L"F10" : ansi2unicode(Keycodes[spammer->keyCode])) << std::endl;
            config << L"delay=" + ansi2unicode(Spammer::serialize_ms(spammer->delay)) << std::endl;
            config << std::wstring(L"antimute=") + (spammer->antiMute ? L"true" : L"false") << std::endl;
            config << L"message=" + ansi2unicode(spammer->message);

            config.close();
            return true;
        }
        else if (feature->name == "unlimitedcps") {
            Features::UnlimitedCPS* unlimited = reinterpret_cast<Features::UnlimitedCPS*>(feature);

            std::wofstream config(ConfigManager::UnlimitedCPS, std::ios::trunc);
            config.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t>));

            config << L"keybind=" << (unlimited->keyCode == NULL ? L"F8" : ansi2unicode(Keycodes[unlimited->keyCode])) << std::endl;
            config << std::wstring(L"enabled=") + (unlimited->isEnabled ? L"true" : L"false");

            config.close();
            return true;
        }

        return false;
    }

    namespace Velocity {

#define GET_INT_VAL(stream, line, param) {	\
	if (line == #param) {					\
		std::getline(stream, line);			\
		param = atoi(line.c_str());			\
	}										\
}											\

#define GET_BOOL_VAL(stream, line, param) {		\
	if (line == #param) {						\
		std::getline(stream, line);				\
		param = line == "true" ? true : false;	\
	}											\
}                                               \

#define GET_STRING_VAL(stream, line, param) {	\
	if (line == #param) {						\
		std::getline(stream, line);				\
		param = line;                           \
	}											\
}                                               \

        void Rewrite(Features::Velocity* velocity) {
            std::ofstream cfg(path, std::ios::trunc);
            if (cfg.is_open()) {
                cfg << "horizontal_min=" << velocity->horizontal_min << std::endl;
                cfg << "horizontal_max=" << velocity->horizontal_max << std::endl;
                cfg << "vertical_min=" << velocity->vertical_min << std::endl;
                cfg << "vertical_max=" << velocity->vertical_max << std::endl;
                cfg << "only_forward=" << (velocity->only_forward ? "true" : "false") << std::endl;
                cfg << "only_moving=" << (velocity->only_moving ? "true" : "false") << std::endl;
                cfg << "enabled=" << (velocity->isEnabled ? "true" : "false") << std::endl;
                cfg << "keybind=" << Keycodes[velocity->keyCode];
            }
            else {
                MessageBoxA(loader, "Ошибка записи файла!", "Destruction Loader", MB_ICONERROR);
            }
        }

        void Load(
            int& horizontal_min,
            int& horizontal_max,
            int& vertical_min,
            int& vertical_max,
            int& keycode,
            bool& only_forward,
            bool& only_moving,
            bool& enabled
        ) {
            std::ifstream cfg(path);
            if (cfg.is_open()) {
                for (std::string line; std::getline(cfg, line, '=');) {
                    GET_INT_VAL(cfg, line, horizontal_min);
                    GET_INT_VAL(cfg, line, horizontal_max);
                    GET_INT_VAL(cfg, line, vertical_min);
                    GET_INT_VAL(cfg, line, vertical_max);

                    GET_BOOL_VAL(cfg, line, only_forward);
                    GET_BOOL_VAL(cfg, line, only_moving);
                    GET_BOOL_VAL(cfg, line, enabled);

                    std::string keybind;
                    GET_STRING_VAL(cfg, line, keybind);
                    keycode = InverseKeycodes[keybind];
                }
            }
            else {
                MessageBoxA(loader, "Ошибка чтения файла!", "Destruction Loader", MB_ICONERROR);
            }
        }

        void UpdateState(bool& enabled) {
            std::ifstream cfg(path);

            if (cfg.is_open()) {
                for (std::string param, value; std::getline(cfg, param, '=') && std::getline(cfg, value); ) {
                    if (param == "enabled")
                        enabled = (value == "true" ? true : false);
                }
            }
            else {
                MessageBoxA(loader, "Ошибка чтения файла!", "Destruction Loader", MB_ICONERROR);
            }
        }
    }

    namespace Visuals {
        std::string config_path = std::getenv("appdata") + std::string("\\.vimeworld\\minigames\\Visuals.toml");
        bool config_is_loaded = false;

        void UpdateSettings(std::string data) {
            std::ofstream os(config_path);

            if (os.is_open())
                os.write(data.c_str(), data.size());
        }
    }
}
