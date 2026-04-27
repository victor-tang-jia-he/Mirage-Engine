#pragma once
#include "i_script.h"
#include "ScriptRegistry.h"
#include <cstdio>

class AIScript : public MirageEngine::Core::Scripting::IScript {
public:
    void OnStart() override {
        printf("[AIScript] onStart\n");
    }

    void OnUpdate(float dt) override {
        printf("[AIScript] onUpdate dt=%.3f\n", dt);
    }

    void OnFixedUpdate(float dt) override {};

    void OnDestroy() override {
        printf("[AIScript] onDestroy\n");
    }

    const char* GetName() const override {
        return "AIScript";
    }
};
