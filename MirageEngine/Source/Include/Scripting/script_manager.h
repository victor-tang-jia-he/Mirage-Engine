#pragma once
#include <Scripting/dynlib.h>
#include <iostream>
#include <filesystem>
#include <thread>

#include <Scripting/i_script.h>
#include <Logs/log_macro.h>
#include <Events/event.h>
#include <Scripting/engine_script_api.h>

DECLARE_LOG_CATEGORY_EXTERN(LogScriptManager)

namespace MirageEngine::Core::Scripting
{
    namespace fs = std::filesystem;

    class ENGINE_API ScriptManager
    {
            using IScript               = MirageEngine::Core::Scripting::IScript;
            fs::path buildDir           = BUILD_DIRECTORY;
            fs::path m_ScriptsSourceDir = SCRIPT_SOURCE_FILE;
            fs::path m_OutputDir        = BUILD_DIRECTORY;
            fs::path m_engineIncludeDir = ENGINE_INCLUDE_DIR;
            fs::path m_toolbox          = TOOLBOX_INCLUDE_DIR;
            fs::path m_engineLibPath    = BUILD_LIBS_DIRECTORY + std::string("/MirageEngine_Debug.lib");

            std::string config         = "Debug";
            std::string m_DLLBuildName = "GameScripts";

            // Internal forward definition
            typedef int (*PFN_GetScriptCount)();
            typedef const char* (*PFN_GetScriptName)(int);
            typedef IScript* (*PFN_CreateScriptByName)(const char*);
            typedef void (*PFN_DestroyScript)(IScript*);

            void* GetScriptCountDLL     = nullptr;
            void* GetScriptNameDLL      = nullptr;
            void* CreateScriptByNameDLL = nullptr;
            void* DestroyScriptDLL      = nullptr;

            // handle
            LibHandle hScripts;

            // Callback
            std::unordered_map<GameObject*, std::shared_ptr<EventSystem::Event<>>> m_PreReloadSubscribers;
            std::unordered_map<GameObject*, std::shared_ptr<EventSystem::Event<>>> m_PostReloadSubscribers;

        public:

            // Dll instance cleaner
            struct DllDeleter
            {
                    PFN_DestroyScript destroyFunc;

                    void operator()(IScript* s) const
                    {
                        if (s)
                        {
                            s->OnDestroy();
                            destroyFunc(s);
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
            void SubscribePreReload(GameObject* go, std::function<void()> _func)
            {

                std::shared_ptr<EventSystem::Event<>> event = std::make_shared<EventSystem::Event<>>(_func);
                m_PreReloadSubscribers[go]                  = event;
            }

            void SubscribePostReload(GameObject* go, std::function<void()> _func)
            {

                std::shared_ptr<EventSystem::Event<>> event = std::make_shared<EventSystem::Event<>>(_func);
                m_PostReloadSubscribers[go]                 = event;
            }

            void Unsubscribe(GameObject* go)
            {
                m_PreReloadSubscribers.erase(go);
                m_PostReloadSubscribers.erase(go);
            }

            // Initialization
            void Init();
            void ReloadScript();

            void ShutDown() const;

            // Getter
            int         GetScriptsCount() const;
            const char* GetScriptName(int index) const;

            // Public function
            std::unique_ptr<IScript, DllDeleter> CreateScriptByName(const char* name);

            void DestroyScript(IScript* script) const;

        private:

            // Initialize MSVC
            bool SetupVisualStudioEnvironment();

            // Main Function for reload
            bool RebuildScriptsDll();

            LibHandle LoadModule(const char* name);
    };
} // namespace MirageEngine::Core::Scripting
