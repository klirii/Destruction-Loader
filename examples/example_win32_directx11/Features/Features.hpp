#pragma once
#include <iostream>
#include <Windows.h>

namespace Features {
    class Feature {
    public:
        std::string name;
        std::string license;
        uint8_t tabs[2];

        int isEnabled;
        int keyCode;
    };

    class Spammer : public Feature {
    public:
        Spammer() {
            this->name = "Spammer";
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
            this->name = "UnlimitedCPS";
            this->license = "License: ...";

            this->tabs[0] = 2;
            this->tabs[1] = 5;
        }
        ~UnlimitedCPS() {delete this;}
    };
}
