#pragma once
#include <Components/transform.h>
#include "component.h"
#include <Camera/i_camera.h>
#include <Scripting/engine_script_api.h>

namespace MirageEngine
{
    namespace Core
    {
        namespace Input
        {
            struct InputMapping;
            struct CallbackInputInfo;
        } // namespace Input

		namespace Components
		{
			class ENGINE_API Camera : public Component, public ICamera , public std::enable_shared_from_this<Camera>
			{
                private:

                    // Clipping Planes
                    float m_Near = 0.f;
                    float m_Far = 0.f;
                    unsigned int m_Width = 0;
                    unsigned int m_Height = 0;
                    float m_Fov = 90.f;
                    float m_CameraVelocity = 1.f;
                    float m_CameraYaw;
                    float m_CameraPitch;

                    MirageEngine::Core::Components::Transform m_transform;

                public:

                    Camera() = default;
                    Camera(GameObject* _owner);
                    Camera(unsigned int _width, unsigned int _height, float _fov, float _near, float _far, const MirageEngine::Core::Math::Vector3 _position);
                    ~Camera() = default;

                    void AttachMeta(std::shared_ptr<GameObject> _Owner);
                    static void AttachNew(std::shared_ptr<GameObject> _owner);

                    entt::id_type GetType() const override;
            };
        } // namespace Components
    } // namespace Core
} // namespace MirageEngine
