#pragma once
#include "i_script.h"
#include <cstdio>

class AIScript : public MirageEngine::Core::Scripting::IScript {
public:
    void onStart() override {
        printf("[AIScript] onStart\n");
    }

    void onUpdate(float dt) override {
        printf("[AIScript] onUpdate dt=%.3f\n", dt);
    }

    void onDestroy() override {
        printf("[AIScript] onDestroy\n");
    }

    const char* getName() const override {
        return "AIScript";
    }
};
