#include <Scripting/script_manager.h>

DEFINE_LOG_CATEGORY(LogScriptManager)

namespace MirageEngine::Core::Scripting
{
    void ScriptManager::Init()
    {
#ifdef _WIN32
        if (!SetupVisualStudioEnvironment())
        {
            LOG(logFatal, LogScriptManager, "Failed to setup VS environment");
            return;
        }
#endif
        ReloadScript();
    }

    void ScriptManager::ReloadScript()
    {
        for (auto& [go, callback] : m_PreReloadSubscribers) callback->Trigger();
        UnloadSharedLibrary(hScripts);
        hScripts = nullptr;
        RebuildScriptsDll();

        hScripts = LoadModule("GameScripts.dll");
        if (!hScripts) return;

        GetScriptCountDLL     = (PFN_GetScriptCount)GetSymbol(hScripts, "GetScriptCount");
        GetScriptNameDLL      = (PFN_GetScriptName)GetSymbol(hScripts, "GetScriptName");
        CreateScriptByNameDLL = (PFN_CreateScriptByName)GetSymbol(hScripts, "CreateScriptByName");
        DestroyScriptDLL      = (PFN_DestroyScript)GetSymbol(hScripts, "DestroyScript");

        if (!GetScriptCountDLL || !GetScriptNameDLL || !CreateScriptByNameDLL || !DestroyScriptDLL)
        {
            LOG(logFatal, LogScriptManager, "Failed to load functions");
            UnloadSharedLibrary(hScripts);
            return;
        }

        LOG(logInfo, LogScriptManager, "Script count : ", ((PFN_GetScriptCount)GetScriptCountDLL)());

        // Trigger callback
        for (auto& [gameobject, callback] : m_PostReloadSubscribers) callback->Trigger();
    }

    void ScriptManager::SubscribePreReload(GameObject* _go, std::function<void()> _func)
    {

        std::shared_ptr<EventSystem::Event<>> event = std::make_shared<EventSystem::Event<>>(_func);
        m_PreReloadSubscribers[_go] = event;
    }

    void ScriptManager::SubscribePostReload(GameObject* _go, std::function<void()> _func)
    {

        std::shared_ptr<EventSystem::Event<>> event = std::make_shared<EventSystem::Event<>>(_func);
        m_PostReloadSubscribers[_go] = event;
    }

    void ScriptManager::Unsubscribe(GameObject* _go)
    {
        m_PreReloadSubscribers.erase(_go);
        m_PostReloadSubscribers.erase(_go);
    }

    void ScriptManager::ShutDown() const { UnloadSharedLibrary(hScripts); }

    int ScriptManager::GetScriptsCount() const { return ((PFN_GetScriptCount)GetScriptCountDLL)(); }

    const char* ScriptManager::GetScriptName(int _index) const { return ((PFN_GetScriptName)GetScriptNameDLL)(_index); }

    std::unique_ptr<IScript, ScriptManager::DllDeleter> ScriptManager::CreateScriptByName(const char* name)
    {

        LOG(logInfo, LogScriptManager, "Script ", name, " created.");
        IScript* script = ((PFN_CreateScriptByName)CreateScriptByNameDLL)(name);
        return { script, DllDeleter{ (PFN_DestroyScript)DestroyScriptDLL } };
    }

    void ScriptManager::DestroyScript(IScript* _script) const { ((PFN_DestroyScript)DestroyScriptDLL)(_script); }

    bool ScriptManager::SetupVisualStudioEnvironment()
    {
        // Search vcvarsall.bat
        std::string vcvarsPath;

        // Using vswhere first
        FILE* vswhere = _popen(
            "\"C:/Program Files (x86)/Microsoft Visual Studio/Installer/vswhere.exe\" "
            "-latest -property installationPath",
            "r");

        if (vswhere)
        {
            char path[512];
            if (fgets(path, sizeof(path), vswhere))
            {
                std::string vsPath(path);
                vsPath.erase(vsPath.find_last_not_of("\r\n") + 1);
                vcvarsPath = vsPath + "\\VC\\Auxiliary\\Build\\vcvarsall.bat";
            }
            _pclose(vswhere);
        }

        // Fallback
        if (vcvarsPath.empty() || !fs::exists(vcvarsPath))
        {
            const char* possiblePaths[] = {
                "C:/Program Files/Microsoft Visual Studio/2022/Community/VC/Auxiliary/Build/vcvarsall.bat",
                "C:/Program Files/Microsoft Visual Studio/2022/Professional/VC/Auxiliary/Build/vcvarsall.bat",
                "C:/Program Files/Microsoft Visual Studio/2022/Enterprise/VC/Auxiliary/Build/vcvarsall.bat",
                "C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Auxiliary/Build/vcvarsall.bat",
                "C:/Program Files (x86)/Microsoft Visual Studio/2019/Professional/VC/Auxiliary/Build/vcvarsall.bat",
            };

            for (const char* p : possiblePaths)
            {
                if (fs::exists(p))
                {
                    vcvarsPath = p;
                    break;
                }
            }
        }

        if (vcvarsPath.empty())
        {
            LOG(logFatal, LogScriptManager, "Cannot find vcvarsall.bat");
            return false;
        }

        LOG(logInfo, LogScriptManager, "Found: ", vcvarsPath.c_str());

        std::string cmd = "cmd /c \"\"" + vcvarsPath + "\" x64 >nul && set\"";

        FILE* pipe = _popen(cmd.c_str(), "r");
        if (!pipe)
        {
            LOG(logFatal, LogScriptManager, "Failed to run vcvarsall.bat\n");
            return false;
        }

        char buffer[4096];
        while (fgets(buffer, sizeof(buffer), pipe))
        {
            std::string line(buffer);
            size_t      eq = line.find('=');
            if (eq != std::string::npos)
            {
                std::string name  = line.substr(0, eq);
                std::string value = line.substr(eq + 1);
                value.erase(value.find_last_not_of("\r\n") + 1);
                _putenv((name + "=" + value).c_str());
            }
        }
        _pclose(pipe);

        LOG(logInfo, LogScriptManager, "Visual Studio environment configured");
        return true;
    }

    bool ScriptManager::RebuildScriptsDll()
    {
        std::vector<std::string> sources;
        if (!fs::exists(m_ScriptsSourceDir))
        {
            fs::create_directory(m_ScriptsSourceDir);
        }
        for (const auto& entry : fs::recursive_directory_iterator(m_ScriptsSourceDir))
        {
            if (entry.path().extension() == ".cpp") sources.push_back("\"" + entry.path().string() + "\"");
        }
        if (sources.empty())
        {
            LOG(logWarning, LogScriptManager, "No Script Find");
            return true;
        }

        bool        isDebug = (config == "Debug");
        std::string outDll  = "\"" + (m_OutputDir / m_DLLBuildName).string() + std::string(".dll") + "\"";
        std::string outPdb  = "\"" + (m_OutputDir / m_DLLBuildName).string() + std::string(".pdb") + "\"";
        std::string impLib  = "\"" + (m_OutputDir / m_DLLBuildName).string() + std::string(".lib") + "\"";

        std::string cmd = "cl.exe";
        cmd += " /LD";
        cmd += " /std:c++17";
        cmd += isDebug ? " /MDd /Zi /Od" : " /MD /O2";
        cmd += " /EHsc /nologo";
        cmd += " /D SCRIPTS_DLL_EXPORT";
        cmd += " /D CPP_TRACE_DEBUG";
        cmd += " /I\"" + m_EngineIncludeDir.string() + "\"";
        cmd += " /I\"" + m_Toolbox.string() + "\"";

        for (const auto& src : sources) cmd += " " + src;

        cmd += " /link";
        cmd += " /DLL";
        cmd += " /OUT:" + outDll;
        cmd += " /IMPLIB:" + impLib;
        cmd += isDebug ? " /DEBUG /PDB:" + outPdb : "";
        cmd += " \"" + m_engineLibPath.string() + "\"";

        FILE* pipe = _popen(cmd.c_str(), "r");
        if (!pipe)
        {
            LOG(logFatal, LogScriptManager, "Failed to execute cl.exe");
            return false;
        }
        char buffer[256];
        while (fgets(buffer, sizeof(buffer), pipe)) LOG(logTemp, LogScriptManager, buffer);

        int status = _pclose(pipe);
        LOG(logInfo, LogScriptManager, "Exit code: ", status);
        return (status == 0);
    }

    LibHandle ScriptManager::LoadModule(const char* _name)
    {
        LibHandle h = LoadSharedLibrary(_name);
        if (!h)
        {
#ifdef _WIN32
            LOG(logFatal, LogScriptManager, "Failed to load library ", GetLastError());
#else
            LOG(logFatal, LogScriptManager, "Failed to load library: ", dlerror());
#endif
        }
        return h;
    };
} // namespace MirageEngine::Core::Scripting
