#pragma once

#include <vector3.h>
#include <Scripting/engine_script_api.h>

#define LIGHT_MANAGER MirageEngine::LightManager::Instance()

namespace MirageEngine
{
    class Light;
    class SpotLight;
    class DirectionalLight;
    class PointLight;

    class ENGINE_API LightManager
    {
        using Vector3 = Core::Math::Vector3;

    private:
        std::vector<std::weak_ptr<DirectionalLight>> m_DirectionalLights;
        std::vector<std::weak_ptr<PointLight>> m_PointLights;
        std::vector<std::weak_ptr<SpotLight>> m_SpotLights;

    public:
        static LightManager& Instance()
        {
            static LightManager instance;
            return instance;
        }

        void ShutDown();

        void AddDirectionalLight(std::shared_ptr<DirectionalLight> _light);
        void AddPointLight(std::shared_ptr<PointLight> _light);
        void AddSpotLight(std::shared_ptr<SpotLight> _light);

        const std::vector<std::weak_ptr<DirectionalLight>>& GetDirectionalLights() const;
        const std::vector<std::weak_ptr<PointLight>>& GetPointLights() const;
        const std::vector<std::weak_ptr<SpotLight>>& GetSpotLights() const;

    };
} // namespace MirageEngine
