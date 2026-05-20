#pragma once

#include <vector3.h>
#include <Light/i_light.h>
#include <Scripting/engine_script_api.h>

#define LIGHT_MANAGER MirageEngine::LightManager::Instance()

DECLARE_LOG_CATEGORY_EXTERN(LogLightManager)

namespace MirageEngine
{
    class Light;
    class DirectionalLight;
    class PointLight;

    class ENGINE_API LightManager
    {
        using Vector3 = Core::Math::Vector3;

    private:
        std::vector<std::weak_ptr<DirectionalLight>> m_DirectionalLights;
        std::vector<std::weak_ptr<PointLight>> m_PointLights;

    public:
        static LightManager& Instance()
        {
            static LightManager instance;
            return instance;
        }

        void ShutDown();

        std::shared_ptr<DirectionalLight> CreateDirectionalLight();
        std::shared_ptr<PointLight> CreatePointLight();

        void AddDirectionalLight(std::shared_ptr<DirectionalLight> _light);
        void AddPointLight(std::shared_ptr<PointLight> _light);

        const std::vector<std::weak_ptr<DirectionalLight>>& GetDirectionalLights() const;
        const std::vector<std::weak_ptr<PointLight>>& GetPointLights() const;

    };
} // namespace MirageEngine
