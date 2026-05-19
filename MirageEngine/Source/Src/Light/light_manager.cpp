#include <Light/light_manager.h>

#include <Light/spot_light.h>
#include <Light/directional_light.h>
#include <Light/point_light.h>

#include <Logs/log.h>

namespace MirageEngine
{
    void LightManager::ShutDown()
    {
        if (m_DirectionalLights.size() > 0 || m_PointLights.size() > 0 || m_SpotLights.size() > 0)
        {
            LOG(logWarning, LogCat::System, "Shutting down Light Manager with still registered lights. This may cause memory leaks.");
            for (int i = 0; i < m_DirectionalLights.size(); i++)
            {
                LOG(logTemp, LogCat::System, " - Unregistering directional light ", i + 1);
                m_DirectionalLights[i].reset();
			}
            for (int i = 0; i < m_PointLights.size(); i++)
            {
                LOG(logTemp, LogCat::System, " - Unregistering point light ", i + 1);
                m_PointLights[i].reset();
            }
            for (int i = 0; i < m_SpotLights.size(); i++)
            {
                LOG(logTemp, LogCat::System, " - Unregistering spot light ", i + 1);
                m_PointLights[i].reset();
            }
        }
        m_DirectionalLights.clear();
        m_PointLights.clear();
        m_SpotLights.clear();
    };

    void LightManager::AddDirectionalLight(std::shared_ptr<DirectionalLight> light) { m_DirectionalLights.push_back(light); }

    void LightManager::AddPointLight(std::shared_ptr<PointLight> light) { m_PointLights.push_back(light); }

    void LightManager::AddSpotLight(std::shared_ptr<SpotLight> light) { m_SpotLights.push_back(light); }

    const std::vector<std::weak_ptr<DirectionalLight>>& LightManager::GetDirectionalLights() const { return m_DirectionalLights; }

    const std::vector<std::weak_ptr<PointLight>>& LightManager::GetPointLights() const { return m_PointLights; }

    const std::vector<std::weak_ptr<SpotLight>>& LightManager::GetSpotLights() const { return m_SpotLights; }
} // namespace MirageEngine
