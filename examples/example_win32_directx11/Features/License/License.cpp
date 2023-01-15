#include "License.hpp"

#include <Windows.h>
#include <StringUtils.h>

namespace Features {
    RestAPI::Client* License::client;
    std::map<std::string, Feature*> License::features;

    bool License::isSetting = false;
    bool License::updaterIsInitialized = false;

    void License::UpdateLicenseTimes() {
        while (true) {
            for (const auto& kv : features) kv.second->license = "License: ...";
            Sleep(30 * 1000);
        }
    }

    std::string License::GetLicenseTime(int* tabs) {
        if (*tabs == 2 || *tabs == 5) return features["unlimitedcps"]->license;
        else if (*tabs == 3 || *tabs == 4) return features["spammer"]->license;
        return "License: no available";
    }

    std::string License::GetLicenseTime(int seconds) {
        std::string license = "License: ";

        int minutes = round(seconds / 60);
        int hours = round(minutes / 60);
        int days = round(hours / 24);
        int months = round(days / 30);

        if (seconds < 60) license += std::to_string(seconds) + " second(s)";
        else if (minutes < 60) license += std::to_string(minutes) + " minute(s)";
        else if (hours < 24) license += std::to_string(hours) + " hour(s)";
        else if (days < 30) license += std::to_string(days) + " day(s)";
        else if (months <= 12) license += std::to_string(months) + " month(s)";
        else if (months > 12) license += "lifetime";

        return license;
    }

    void License::SetLicenseTimes(int* tabs) {
        for (const auto& kv : features)
            if (kv.second->license == "License: ..." && (*tabs == kv.second->tabs[0] || *tabs == kv.second->tabs[1]) && !isSetting)
                CreateThread(nullptr, NULL, reinterpret_cast<LPTHREAD_START_ROUTINE>(License::SetLicenseTime), &kv.second->name, NULL, nullptr);
    }

    void License::SetLicenseTime(std::string& feature) {
        isSetting = true;
        json features;

        if (client->GetFeatures(client->user.name, client->user.password, client->user.session, features)) {
            if (features.contains(feature)) {
                int seconds = features[feature].get<int>();
                if (seconds) {
                    if (License::features.count(feature))
                        License::features[feature]->license = GetLicenseTime(seconds);
                }
            }
        }

        for (const auto& kv : License::features) {
            if (kv.second->license == "License: ..." && kv.second->name == feature)
                kv.second->license = "License: none";
        }

        isSetting = false;
    }
}
