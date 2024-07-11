#pragma once
#include <iostream>
#include <Windows.h>
#include <toml.hpp>

namespace Features {
    class Feature {
    public:
        std::string name;
        std::string license;
        uint8_t tabs[2];

        bool isEnabled;
        int keyCode;
    };

    class Spammer : public Feature {
    public:
        Spammer() {
            this->name = "spammer";
            this->license = "License: ...";

            this->tabs[0] = 3;
            this->tabs[1] = 4;

            this->keyCode = VK_F10;

            this->antiMute = true;
            this->delay = 20000;
            this->message = "Destruction Spammer Ч лучший спамер дл€ VimeWorld!";
        }
        ~Spammer() {delete this;}

        bool antiMute;
        uint64_t delay;
        std::string message;
    };

    class UnlimitedCPS : public Feature {
    public:
        UnlimitedCPS() {
            this->name = "unlimitedcps";
            this->license = "License: ...";

            this->tabs[0] = 2;
            this->tabs[1] = 5;

            this->isEnabled = true;
            this->keyCode = VK_F8;
        }
        ~UnlimitedCPS() {delete this;}
    };

    //class Kogtevran : public Feature {
    //public:
    //    Kogtevran() {
    //        this->name = "kogtevran";
    //        this->license = "License: ...";

    //        this->tabs[0] = 6;

    //        this->isEnabled = false;
    //        this->keyCode = NULL;
    //    }
    //    ~Kogtevran() {delete this;}
    //};

    class NoHurtCam : public Feature {
    public:
        NoHurtCam() {
            this->name = "nohurtcam";
            this->license = "License: ...";

            this->tabs[0] = 7;

            this->isEnabled = false;
            this->keyCode = NULL;
        }
        ~NoHurtCam() { delete this; }
    };

    class Velocity : public Feature {
    public:
        int horizontal_min;
        int horizontal_max;
        int vertical_min;
        int vertical_max;

        bool only_forward;
        bool only_moving;

        Velocity() {
            name = "velocity";
            license = "License: ...";

            tabs[0] = 6;
            tabs[1] = 8;

            isEnabled = true;
            keyCode = NULL;

            horizontal_min = 100;
            horizontal_max = 100;
            vertical_min = 100;
            vertical_max = 100;

            only_forward = true;
            only_moving = false;
        }

        ~Velocity() { delete this; }
    };

    class Visuals : public Feature {
    public:
        class OnEnableModule {
        public:
            std::string module_name = "";
            bool enabled = false;

            OnEnableModule(std::string module_name) : module_name(module_name) {}
            void update_enabled() const;
        };

        class Color {
        public:
            toml::value::array_type color;

            Color(std::initializer_list<__int64> list) {
                for (const __int64& value : list)
                    color.push_back(toml::value(value));
            }

            float h() { return (float)color[0].as_integer() / 255.0F; }
            float s() { return (float)color[1].as_integer() / 255.0F; }
            float v() { return (float)color[2].as_integer() / 255.0F; }
            float a() { return color.size() > 3 ? ((float)color[3].as_integer() / 255.0F) : 0.0F; }
        };

        class ESP : public OnEnableModule {
        public:
            std::string mode = "2D";
            Color box_color = { NULL };

            bool outline = true;
            Color outline_color = { 0, 0, 0 };

            bool filling = false;
            Color filling_color = { 0, 0, 0, 40 };

            ESP(std::string module_name, Color box_color) : OnEnableModule(module_name), box_color(box_color) {}

            void update_mode() const;
            void update_box_color() const;

            void update_outline() const;
            void update_outline_color() const;

            void update_filling() const;
            void update_filling_color() const;
        };

        class NameTags : public OnEnableModule {
        public:
            NameTags(std::string module_name) : OnEnableModule(module_name) {}
        };

        class Chams {
        public:
            bool players = false;
            bool chests = false;

            void update_players();
            void update_chests();
        };

        static const toml::result<toml::value, std::vector<toml::error_info>> config;

        ESP player_esp = ESP("PlayerESP", {220, 20, 60});
        ESP chest_esp  = ESP("ChestESP", {255, 255, 255});
        NameTags name_tags = NameTags("NameTags");
        Chams chams;

        Visuals() {
            if (!config.is_ok())
                MessageBoxA(NULL, "ќшибка парсинга конфига!", "Destruction Loader", MB_ICONERROR);

            this->name = "visuals";
            this->license = "License: ...";

            tabs[0] = 9;
            tabs[1] = 10;

            isEnabled = true;
            keyCode = NULL;

            player_esp.enabled = true;
            chest_esp.enabled = true;
        }

        void LoadSettings();

        ~Visuals() { delete this; }
    };
}
