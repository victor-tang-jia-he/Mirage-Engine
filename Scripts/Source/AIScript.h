#pragma once
#include <Scripting/i_script.h>
#include <Scripting/script_registry.h>
#include <cstdio>
#include <GameObject/game_object.h>

class AIScript : public MirageEngine::Core::Scripting::IScript {
public:
    void OnStart() override 
    {
        m_ComponentName = "caca";
        printf("[AIScript] onStart\n");
    }

    void OnUpdate(float dt) override {
        printf("[AIScript] onUpdate dt=%.3f\n", dt);
        printf("[AIScript] name : %s\n", m_ComponentName.c_str());
    }

    void OnFixedUpdate(float dt) override {};

    void OnDestroy() override {
        printf("[AIScript] onDestroy\n");
    }

    const char* GetName() const override {
        return "AIScript";
    }
};
