#pragma once
#include "../../RestAPI/Core/Client.hpp"
#include "../../RestAPI/Errors/ErrorHandler.hpp"

#include "../Features.hpp"
#include <nlohmann/json.hpp>

namespace Features {
    using json = nlohmann::json;
    class License {
    public:
        static RestAPI::Client* client;
        static std::map<std::string, Feature*> features;

        static bool isSetting;
        static bool updaterIsInitialized;

        static void UpdateLicenseTimes();
        static std::string GetLicenseTime(int* tabs);
        static std::string GetLicenseTime(int seconds);

        static void SetLicenseTimes(int* tabs);
        static void SetLicenseTime(std::string& feature);
        static __forceinline bool ToggleTabIfLicenseExists(const char* feature, std::uint8_t tab, int* tabs) {
            json features;
            if (client->GetFeatures(client->user.name, client->user.password, client->user.session, features)) {
                if (features.contains(feature)) {
                    if (features[feature].get<int>()) {
                        *tabs = tab;
                        return true;
                    }
                }

                MessageBoxA(RestAPI::ErrorHandler::hWindow, "У Вас нет лицензии :(\nПриобрести её можно здесь -> https://vk.com/destructiqn", "Destruction Loader", MB_ICONERROR);
                return false;
            }
            return false;
        }
    };
}
