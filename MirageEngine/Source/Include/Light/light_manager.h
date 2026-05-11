#pragma once

#include <vector3.h>

namespace MirageEngine
{
    struct Light;
    struct SpotLight;
    struct DirectionalLight;
    struct PointLight;

    class LightManager
    {
            using Vector3 = MirageEngine::Core::Math::Vector3;

        public:

            static LightManager& Instance()
            {
                static LightManager instance;
                return instance;
            }

            void ShutDown() {};

            void AddDirectionalLight(std::shared_ptr<DirectionalLight> _light);
            void AddPointLight(std::shared_ptr<PointLight> _light);
            void AddSpotLight(std::shared_ptr<SpotLight> _light);

            const std::vector<std::weak_ptr<DirectionalLight>>& GetDirectionalLights() const;
            const std::vector<std::weak_ptr<PointLight>>& GetPointLights() const;
            const std::vector<std::weak_ptr<SpotLight>>& GetSpotLights() const;

        private:

            std::vector<std::weak_ptr<DirectionalLight>> m_DirectionalLights;
            std::vector<std::weak_ptr<PointLight>> m_PointLights;
            std::vector<std::weak_ptr<SpotLight>> m_SpotLights;
    };
} // namespace MirageEngine

#define Light_Manager MirageEngine::LightManager::Instance()
