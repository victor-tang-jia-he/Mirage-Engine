#pragma once
#include <Components/transform.h>
#include "component.h"
#include <Camera/i_camera.h>

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
			class Camera : public Component, public ICamera , public std::enable_shared_from_this<Camera>
			{
                public:

                    Camera() = default;
                    Camera(unsigned int _width, unsigned int _height, float _fov, float _near, float _far, const MirageEngine::Core::Math::Vector3 _position);
                    ~Camera() = default;

                    void Init() override {};
                    void Update() override {};

                    void AttachMeta(std::shared_ptr<GameObject> _Owner);

                    entt::id_type GetType() const override { return entt::type_id<Camera>().hash(); }
            };
        } // namespace Components
    } // namespace Core
} // namespace MirageEngine
