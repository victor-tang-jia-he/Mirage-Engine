#pragma once
#include <Scripting/i_script.h>
#include <Scripting/script_registry.h>
#include <cstdio>
#include <GameObject/game_object.h>

class AIScript : public MirageEngine::Core::Scripting::IScript {
public:
    void OnStart() override 
    {
    }

    void OnUpdate(float dt) override {
        if (m_Owner != nullptr)
        {
            m_Owner->SetPosition(m_Owner->GetPosition() + MirageEngine::Core::Math::Vector3::Vector3(1 * dt, 0, 0));
        }
    }

    void OnFixedUpdate(float dt) override {};

    void OnDestroy() override {
    }

    const char* GetName() const override {
        return "AIScript";
    }
};
