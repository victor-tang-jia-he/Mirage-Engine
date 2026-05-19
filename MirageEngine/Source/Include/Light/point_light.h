#pragma once
#include <Scripting/engine_script_api.h>
#include <Light/i_light.h>

DECLARE_LOG_CATEGORY_EXTERN(LogPointLight)

namespace MirageEngine
{
	class ENGINE_API PointLight : public Light
	{
	private:
		Core::Math::Vector3 m_Position = { 0, 0, 0 };

	public:
		std::function<Core::Math::Vector3()> GetParentWorldPosition;

		PointLight();
		~PointLight() = default;

		Core::Math::Vector3 GetPosition() const { return m_Position; }

		void SetPosition(const Core::Math::Vector3& _position) { m_Position = _position; }
	};
}