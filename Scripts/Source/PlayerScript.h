#pragma once
#include <Scripting/script_registry.h>
#include <Scripting/i_script.h>
#include <GameObject/game_object.h>
#include <vector3.h>
#include <cstdio>

class PlayerScript : public MirageEngine::Core::Scripting::IScript {
public:
    void OnStart() override {
        printf("[PlayerScript] onStart\n");
    }

    void OnUpdate(float dt) override {
        if (m_Owner != nullptr)
        {
            m_Owner->SetPosition(m_Owner->GetPosition() + MirageEngine::Core::Math::Vector3::Vector3(1, 0, 0));
        }
    }

    void OnFixedUpdate(float dt) override {};

    void OnDestroy() override {
        printf("[PlayerScript] onDestroy\n");
    }

    const char* GetName() const override {
        return "PlayerScript";
    }
};