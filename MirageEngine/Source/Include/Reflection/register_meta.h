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

            void RegisterGameObjectMeta();
            void RegisterMeshRendererMeta();
            void RegisterCameraMeta();
            void RegisterInputMappingMeta();

            void RegisterMeshMeta();
            void RegisterTextureMeta();
            void RegisterMaterialMeta();
            void RegisterShaderMeta();

            void RegisterInfluenceGridMeta();
            void RegisterInfluenceEmitterMeta();
            void RegisterResponsiveCurveMeta();
            void RegisterNavAgentMeta();
            void RegisterUtilityAgentMeta();

            template <typename T> void RegisterSharedPtr();
    };
} // namespace MirageEngine

#include "register_meta.inl"
