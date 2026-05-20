#pragma once

#include <entt/meta/meta.hpp>

DECLARE_LOG_CATEGORY_EXTERN(LogRegisterMeta);

namespace MirageEngine
{
    class RegisterMeta
    {
    public:
        RegisterMeta();
        ~RegisterMeta();

        void RegisterAllMeta(entt::locator<entt::meta_ctx>::node_type handle);

        void RegisterTransformMeta();

		// Components
        void RegisterGameObjectMeta();
        void RegisterMeshRendererMeta();
        void RegisterCameraMeta();
        void RegisterInputMappingMeta();

		// Lights
        void RegisterDirectionalLightMeta();
        void RegisterPointLightMeta();

		// Physics
        void RegisterBoxColliderMeta();
        void RegisterSphereColliderMeta();
        void RegisterCapsuleColliderMeta();
        void RegisterRigidbodyMeta();

		// AI
        void RegisterInfluenceGridMeta();
        void RegisterInfluenceEmitterMeta();
        void RegisterResponsiveCurveMeta();
        void RegisterNavAgentMeta();
        void RegisterUtilityAgentMeta();
        void RegisterAIActionMeta();

		// Resources
        void RegisterMeshMeta();
        void RegisterTextureMeta();
        void RegisterMaterialMeta();
        void RegisterMaterialInstanceMeta();
        void RegisterShaderMeta();
    };
} // namespace MirageEngine
