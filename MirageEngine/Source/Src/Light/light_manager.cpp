#include <Light/light_manager.h>

#include <Light/directional_light.h>
#include <Light/point_light.h>

#include <Logs/log.h>
DEFINE_LOG_CATEGORY(LogLightManager)

namespace MirageEngine
{
	void LightManager::ShutDown()
	{
		size_t aliveDirectional = 0;
		size_t alivePoint = 0;

		for (const auto& light : m_DirectionalLights)
		{
			if (!light.expired())
				++aliveDirectional;
		}

		for (const auto& light : m_PointLights)
		{
			if (!light.expired())
				++alivePoint;
		}

		if (aliveDirectional > 0 || alivePoint > 0)
		{
			LOG(logWarning, LogLightManager, "Shutting down Light Manager with still alive lights.");
			LOG(logTemp, LogLightManager, " - Alive directional lights: ", aliveDirectional);
			LOG(logTemp, LogLightManager, " - Alive point lights: ", alivePoint);
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
