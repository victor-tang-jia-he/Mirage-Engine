#pragma once
#include <Scripting/engine_script_api.h>
#include <Light/i_light.h>

DECLARE_LOG_CATEGORY_EXTERN(LogPointLight)

namespace MirageEngine
{
	class ENGINE_API PointLight : public Light, public Core::Components::Component, public std::enable_shared_from_this<PointLight>
	{
	private:
		Core::Math::Vector3 m_Position = { 0, 0, 0 };

	public:
		PointLight();
		PointLight(GameObject* _owner);
		~PointLight() = default;

		static void AttachNew(std::shared_ptr<GameObject> _owner);

		void OnStart() override;

		entt::id_type GetType() const override;
		Core::Math::Vector3 GetPosition() const { return m_Position; }

		void SetPosition(const Core::Math::Vector3& _position) { m_Position = _position; }
	};
}