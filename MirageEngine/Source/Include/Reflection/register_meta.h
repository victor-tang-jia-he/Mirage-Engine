#pragma once

#include <EnTT/meta/meta.hpp>

namespace MirageEngine
{
    class RegisterMeta
    {
        public:

            RegisterMeta();
            ~RegisterMeta();

            void RegisterAllMeta(entt::locator<entt::meta_ctx>::node_type handle);

        private:

            void RegisterGameObjectMeta();

            void RegisterTransformMeta();

            void RegisterMeshRendererMeta();
            void RegisterCameraMeta();
            void RegisterInputMappingMeta();
            void RegisterInfluenceGridMeta();
            void RegisterInfluenceEmitterMeta();

            void RegisterMeshMeta();
            void RegisterTextureMeta();
            void RegisterMaterialMeta();
            void RegisterShaderMeta();

            template <typename T> void RegisterSharedPtr();
    };
} // namespace MirageEngine

#include "register_meta.inl"
