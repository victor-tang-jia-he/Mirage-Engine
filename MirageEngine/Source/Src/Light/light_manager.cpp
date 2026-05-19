#include <Light/light_manager.h>

#include <Light/directional_light.h>
#include <Light/point_light.h>

#include <Logs/log.h>
DEFINE_LOG_CATEGORY(LogLightManager)

namespace MirageEngine
{
	void LightManager::ShutDown()
	{
		if (m_DirectionalLights.size() > 0 || m_PointLights.size() > 0)
		{
			LOG(logWarning, LogLightManager, "Shutting down Light Manager with still registered lights. This may cause memory leaks.");
			LOG(logTemp, LogLightManager, " - Unregistering directional light ", m_DirectionalLights.size());
			LOG(logTemp, LogLightManager, " - Unregistering point light ", m_PointLights.size());
		}
		m_DirectionalLights.clear();
		m_PointLights.clear();
	}
	std::shared_ptr<DirectionalLight> LightManager::CreateDirectionalLight()
	{
		std::shared_ptr<DirectionalLight> light = std::make_shared<DirectionalLight>();
		m_DirectionalLights.push_back(light);
		return light;
	}

	std::shared_ptr<PointLight> LightManager::CreatePointLight()
	{
		std::shared_ptr<PointLight> light = std::make_shared<PointLight>();
		m_PointLights.push_back(light);
		return light;
	}

	void LightManager::AddDirectionalLight(std::shared_ptr<DirectionalLight> light) { m_DirectionalLights.push_back(light); }

	void LightManager::AddPointLight(std::shared_ptr<PointLight> light) { m_PointLights.push_back(light); }

	const std::vector<std::weak_ptr<DirectionalLight>>& LightManager::GetDirectionalLights() const { return m_DirectionalLights; }

	const std::vector<std::weak_ptr<PointLight>>& LightManager::GetPointLights() const { return m_PointLights; }
} // namespace MirageEngine
