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
    private:
        MirageEngine::Renderer m_Renderer;
        MirageEngine::SceneGraph m_SceneGraph;

        uint32_t m_EditorFBO;

        // TODO : Edit in map
        std::shared_ptr<MirageEngine::Core::Input::InputMapping> m_EditorMapping = nullptr;
        std::shared_ptr<MirageEngine::Core::Input::InputMapping> m_PlayMapping = nullptr;

        std::shared_ptr<MirageEngine::Core::Components::Camera> m_EngineCamera = nullptr;
        std::vector<std::shared_ptr<MirageEngine::Core::Components::MeshRenderer>> m_meshRenderer;

        static Application* s_Instance;

        // Fix update 
        float m_AccumulatedTime = 0.f;
		const float m_FixedDeltaTime = 0.02f;

    public:
        std::vector<std::shared_ptr<MirageEngine::Core::Components::Camera>> m_GameCameras;

        void InitApp();
		void RuntimeUpdate(float _dt);
		void EditorUpdate();
		void BeginRender(std::shared_ptr<MirageEngine::Core::ICamera> camera);
		void EndRender();
        void ShutDown();

        MirageEngine::SceneGraph* GetSceneGraph() { return &m_SceneGraph; }

        static Application* Instance() { return s_Instance; }

		bool IsRunning() const { return m_Renderer.IsRunning(); }
		void* GetWindowHandle() const { return m_Renderer.GetWindowHandle(); }
		MirageEngine::Renderer* GetRenderer() { return &m_Renderer; }
		uint32_t GetFramebufferTextureID() const { return m_Renderer.GetFramebufferTextureID(); }
		uint32_t GetEditorFBO() const { return m_EditorFBO; }

    private:

        void InitScene();
        void InitInput();
        void InitGameObject();
};
