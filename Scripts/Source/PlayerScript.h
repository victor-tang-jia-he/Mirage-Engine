#pragma once
#include <Scripting/script_registry.h>
#include <Scripting/i_script.h>
#include <cstdio>

class PlayerScript : public MirageEngine::Core::Scripting::IScript {
public:
    void OnStart() override {
        printf("[PlayerScript] onStart\n");
    }

    void OnUpdate(float dt) override {
        printf("[PlayerScript] onUpdate dt=%.3f\n", dt);
    }

    void OnFixedUpdate(float dt) override {};

    void OnDestroy() override {
        printf("[PlayerScript] onDestroy\n");
    }

    const char* GetName() const override {
        return "PlayerScript";
    }
};