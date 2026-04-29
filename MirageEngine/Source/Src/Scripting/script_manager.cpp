#include <Scripting/script_manager.h>


DEFINE_LOG_CATEGORY(LogScriptManager)

namespace MirageEngine::Core::Scripting
{
	void ScriptManager::Init()
	{
#ifdef _WIN32
		if (!SetupVisualStudioEnvironment()) {
			LOG(logFatal,LogScriptManager,"Failed to setup VS environment");
			return;
		}
#endif
	}

	void ScriptManager::ReloadScript()
	{
		UnloadSharedLibrary(hScripts);
		RebuildScriptsDll(buildDir, config);

		auto LoadModule = [](const char* name) -> LibHandle {
			LibHandle h = LoadSharedLibrary(name);
			if (!h) {
#ifdef _WIN32
				LOG(logError, LogScriptManager, "Failed to load library ", GetLastError());
#else
				LOG(logError, LogScriptManager, "Failed to load library: ", dlerror());
#endif
			}
			return h;
			};

		hScripts = LoadModule(kScriptLibName);
		if (!hScripts) return;

		GetScriptCountDLL = (PFN_GetScriptCount)GetSymbol(hScripts, "GetScriptCount");
		GetScriptNameDLL = (PFN_GetScriptName)GetSymbol(hScripts, "GetScriptName");
		CreateScriptByNameDLL = (PFN_CreateScriptByName)GetSymbol(hScripts, "CreateScriptByName");
		DestroyScriptDLL = (PFN_DestroyScript)GetSymbol(hScripts, "DestroyScript");

		if (!GetScriptCountDLL || !GetScriptNameDLL || !CreateScriptByNameDLL || !DestroyScriptDLL) {
			LOG(logError, LogScriptManager, "Failed to load functions");
			UnloadSharedLibrary(hScripts);
			return;
		}
	}
	void ScriptManager::ShutDown() const
	{
		UnloadSharedLibrary(hScripts);
	}
	int ScriptManager::GetScriptsCount() const { return ((PFN_GetScriptCount)GetScriptCountDLL)(); }
	const char* ScriptManager::GetScriptName(int index) const { return ((PFN_GetScriptName)GetScriptNameDLL)(index); }
	IScript* ScriptManager::CreateScriptByName(const char* name) {

		IScript* script = ((PFN_CreateScriptByName)CreateScriptByNameDLL)(name);
		// registe script in cache
		if (script)
			scripts.emplace_back(script, DllDeleter{ (PFN_DestroyScript)DestroyScriptDLL });
		return script;
	}
	void ScriptManager::DestroyScript(IScript* script) const { ((PFN_DestroyScript)DestroyScriptDLL)(script); }
	bool ScriptManager::SetupVisualStudioEnvironment() {
		// Search vcvarsall.bat
		std::string vcvarsPath;

		// Using vswhere first
		FILE* vswhere = _popen(
			"\"C:/Program Files (x86)/Microsoft Visual Studio/Installer/vswhere.exe\" "
			"-latest -property installationPath", "r");

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
		if (vcvarsPath.empty() || !fs::exists(vcvarsPath)) {
			const char* possiblePaths[] = {
				"C:/Program Files/Microsoft Visual Studio/2022/Community/VC/Auxiliary/Build/vcvarsall.bat",
				"C:/Program Files/Microsoft Visual Studio/2022/Professional/VC/Auxiliary/Build/vcvarsall.bat",
				"C:/Program Files/Microsoft Visual Studio/2022/Enterprise/VC/Auxiliary/Build/vcvarsall.bat",
				"C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Auxiliary/Build/vcvarsall.bat",
				"C:/Program Files (x86)/Microsoft Visual Studio/2019/Professional/VC/Auxiliary/Build/vcvarsall.bat",
			};

			for (const char* p : possiblePaths) {
				if (fs::exists(p)) {
					vcvarsPath = p;
					break;
				}
			}
		}

		if (vcvarsPath.empty()) {
			LOG(logError, LogScriptManager, "Cannot find vcvarsall.bat");
			return false;
		}

		LOG(logInfo, LogScriptManager, "Found: ", vcvarsPath.c_str());

		std::string cmd = "cmd /c \"\"" + vcvarsPath + "\" x64 >nul && set\"";

		FILE* pipe = _popen(cmd.c_str(), "r");
		if (!pipe) {
			LOG(logError, LogScriptManager, "Failed to run vcvarsall.bat\n");
			return false;
		}

		char buffer[4096];
		while (fgets(buffer, sizeof(buffer), pipe)) {
			std::string line(buffer);
			size_t eq = line.find('=');
			if (eq != std::string::npos) {
				std::string name = line.substr(0, eq);
				std::string value = line.substr(eq + 1);
				value.erase(value.find_last_not_of("\r\n") + 1);
				_putenv((name + "=" + value).c_str());
			}
		}
		_pclose(pipe);

		LOG(logInfo, LogScriptManager, "Visual Studio environment configured");
		return true;
	}
	bool ScriptManager::RebuildScriptsDll(const fs::path& buildDir, const std::string& config)
	{
		if (!fs::exists(buildDir / "CMakeCache.txt")) {
			LOG(logError, LogScriptManager, "Build directory not valid: ", buildDir.string().c_str());
			return false;
		}

		std::string cmd = "cmake --build \"" + fs::absolute(buildDir).string() +
			"\" --target GameScripts --config " + config;
		LOG(logInfo, LogScriptManager, ">>> ", cmd.c_str());

		FILE* pipe = _popen(cmd.c_str(), "r");
		if (!pipe) {
			LOG(logError, LogScriptManager," Failed to execute cmake");
			return false;
		}

		char buffer[256];
		while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
			LOG(logTemp, LogScriptManager, buffer);
		}

		int status = _pclose(pipe);
		LOG(logInfo, LogScriptManager, "Exit code: ", status);
		return (status == 0);
	}
}