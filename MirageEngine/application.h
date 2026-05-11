#pragma once

#include <renderer.h>
#include <SceneGraph/scene_graph.h>

// Input
#include <Input/input_system.h>

#include <Scripting/engine_script_api.h>

namespace MirageEngine
{
    namespace Core
    {
        namespace Components
        {
            class Camera;
            class MeshRenderer;
        } // namespace Components
    } // namespace Core
} // namespace MirageEngine

class ENGINE_API Application
{
    public:

        MirageEngine::Renderer m_Renderer;
        MirageEngine::SceneGraph m_SceneGraph;

    private:

        // TODO : Edit in map
        std::shared_ptr<MirageEngine::Core::Input::InputMapping> m_EditorMapping = nullptr;
        std::shared_ptr<MirageEngine::Core::Input::InputMapping> m_PlayMapping = nullptr;

        std::shared_ptr<MirageEngine::Core::Components::Camera> m_EngineCamera = nullptr;
        std::vector<std::shared_ptr<MirageEngine::Core::Components::MeshRenderer>> m_meshRenderer;

        static Application* s_Instance;

    public:

        void InitApp();
        void ShutDown();

        MirageEngine::SceneGraph* GetSceneGraph() { return &m_SceneGraph; }

        static Application* Instance() { return s_Instance; }

    private:

        void InitScene();
        void InitInput();
        void InitGameObject();
};
