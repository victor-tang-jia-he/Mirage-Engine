#pragma once
#include <Scripting/dynlib.h>
#include <iostream>
#include <filesystem>
#include <thread>

#include <Scripting/i_script.h>

#include <Events/event.h>
#include <Scripting/engine_script_api.h>

DECLARE_LOG_CATEGORY_EXTERN(LogScriptManager)

namespace MirageEngine::Core::Scripting
{
    namespace fs = std::filesystem;

    class ENGINE_API ScriptManager
    {
            using IScript = MirageEngine::Core::Scripting::IScript;
            fs::path m_ScriptsSourceDir = "./Assets/Scripts";
            fs::path m_OutputDir = ".";
            fs::path m_EngineIncludeDir = "./Include/Engine";
            fs::path m_Toolbox = "./Include/Toolbox";
            fs::path m_EngineLibPath = "./Libs/MirageEngine.lib";

            std::string m_DLLBuildName = "GameScripts";
            bool m_BIsDebug = false;

            // Internal forward definition
            typedef int (*PFN_GetScriptCount)();
            typedef const char* (*PFN_GetScriptName)(int);
            typedef IScript* (*PFN_CreateScriptByName)(const char*);
            typedef void (*PFN_DestroyScript)(IScript*);

            PFN_GetScriptCount       m_GetScriptCountDLL = nullptr;
            PFN_GetScriptName        m_GetScriptNameDLL = nullptr;
            PFN_CreateScriptByName   m_CreateScriptByNameDLL = nullptr;
            PFN_DestroyScript        m_DestroyScriptDLL = nullptr;

            // handle
            LibHandle hScripts = nullptr;

            // Callback
            std::unordered_map<GameObject*, std::shared_ptr<EventSystem::Event<>>> m_PreReloadSubscribers;
            std::unordered_map<GameObject*, std::shared_ptr<EventSystem::Event<>>> m_PostReloadSubscribers;

            bool m_ScriptDLLoaded = false;

        public:

            // Dll instance cleaner
            struct DllDeleter
            {
                    PFN_DestroyScript DestroyFunc;

                    void operator()(IScript* _s) const
                    {
                        if (_s)
                        {
                            _s->OnDestroy();
                            DestroyFunc(_s);
                        }
                    }
            };

            // Create and bind ours cleaner
            using ScriptPtr = std::unique_ptr<IScript, DllDeleter>;

            static ScriptManager& Instance()
            {
                static ScriptManager instance;
                return instance;
            }

            // For GameObject
            void SubscribePreReload(GameObject* _go, std::function<void()> _func);
            void SubscribePostReload(GameObject* _go, std::function<void()> _func);
            void Unsubscribe(GameObject* _go);

            // Initialization
            void Init();
            void ReloadScript();

            void ShutDown() const;

            // Getter
            int GetScriptsCount() const;
            const char* GetScriptName(int _index) const;

            // Public function
            std::unique_ptr<IScript, DllDeleter> CreateScriptByName(const char* _name);
            void CreateNewScript(const std::string& _fileName) const;
            void DestroyScript(IScript* script) const;

        private:

            // Initialize MSVC
            bool SetupVisualStudioEnvironment();
            // Main Function for reload
            bool RebuildScriptsDll();
            LibHandle LoadModule(const char* name);
            void GenerateScriptBindings() const;
    };
} // namespace MirageEngine::Core::Scripting
