#pragma once
#include <i_script.h>
#include <cstdio>

class PlayerScript : public MirageEngine::Core::Scripting::IScript {
public:
    void onStart() override {
        printf("[PlayerScript] onStart\n");
    }

    void onUpdate(float dt) override {
        printf("[PlayerScript] onUpdate dt=%.3f\n", dt);
    }

    void onDestroy() override {
        printf("[PlayerScript] onDestroy\n");
    }

    const char* getName() const override {
        return "PlayerScript";
    }
};
