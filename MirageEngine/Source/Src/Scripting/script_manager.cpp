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
		RebuildScriptsDll(BUILD_DIRECTORY, config);
		
		//LOG(logFatal,LogScriptManager,"END TEST");
	}

	void ScriptManager::ReloadScript()
	{
		UnloadSharedLibrary(hScripts);
		hScripts = nullptr;
		RebuildScriptsDll(buildDir, config);

		auto LoadModule = [](const char* name) -> LibHandle {
			LibHandle h = LoadSharedLibrary(name);
			if (!h) {
#ifdef _WIN32
				LOG(logFatal, LogScriptManager, "Failed to load library ", GetLastError());
#else
				LOG(logFatal, LogScriptManager, "Failed to load library: ", dlerror());
#endif
			}
			return h;
			};

		hScripts = LoadModule("GameScripts.dll");
		if (!hScripts) return;

		GetScriptCountDLL = (PFN_GetScriptCount)GetSymbol(hScripts, "GetScriptCount");
		GetScriptNameDLL = (PFN_GetScriptName)GetSymbol(hScripts, "GetScriptName");
		CreateScriptByNameDLL = (PFN_CreateScriptByName)GetSymbol(hScripts, "CreateScriptByName");
		DestroyScriptDLL = (PFN_DestroyScript)GetSymbol(hScripts, "DestroyScript");

		if (!GetScriptCountDLL || !GetScriptNameDLL || !CreateScriptByNameDLL || !DestroyScriptDLL) {
			LOG(logFatal, LogScriptManager, "Failed to load functions");
			UnloadSharedLibrary(hScripts);
			return;
		}

		LOG(logInfo, LogScriptManager, "Script count : ", ((PFN_GetScriptCount)GetScriptCountDLL)());

		// Trigger callback
		for (auto& [gameobject, callback] : m_Subscribers)
			callback->Trigger();
	}
	void ScriptManager::ShutDown() const
	{
		UnloadSharedLibrary(hScripts);
	}
	int ScriptManager::GetScriptsCount() const { return ((PFN_GetScriptCount)GetScriptCountDLL)(); }
	const char* ScriptManager::GetScriptName(int index) const { return ((PFN_GetScriptName)GetScriptNameDLL)(index); }
	std::unique_ptr<IScript, ScriptManager::DllDeleter>  ScriptManager::CreateScriptByName(const char* name) {

		IScript* script = ((PFN_CreateScriptByName)CreateScriptByNameDLL)(name);
		return { script, DllDeleter{ (PFN_DestroyScript)DestroyScriptDLL } };
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
			LOG(logFatal, LogScriptManager, "Cannot find vcvarsall.bat");
			return false;
		}

		LOG(logInfo, LogScriptManager, "Found: ", vcvarsPath.c_str());

		std::string cmd = "cmd /c \"\"" + vcvarsPath + "\" x64 >nul && set\"";

		FILE* pipe = _popen(cmd.c_str(), "r");
		if (!pipe) {
			LOG(logFatal, LogScriptManager, "Failed to run vcvarsall.bat\n");
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
		fs::path m_scriptsSourceDir = "P:/School/2025_year/C++/2025_gp_2029_gp_2029_customengine-mirageengine/Scripts";
		fs::path m_outputDir = "P:/School/2025_year/C++/2025_gp_2029_gp_2029_customengine-mirageengine/Build/MSVC_x64-debug/Bin/MSVC_Debug/DLL";
		fs::path m_engineIncludeDir = "P:/School/2025_year/C++/2025_gp_2029_gp_2029_customengine-mirageengine/MirageEngine/Source/Include";
		fs::path m_toolbox = "P:/School/2025_year/C++/2025_gp_2029_gp_2029_customengine-mirageengine/MirageEngine/Tools/ToolBox/Source/Include";
		fs::path m_engineLibPath = "P:/School/2025_year/C++/2025_gp_2029_gp_2029_customengine-mirageengine/Build/MSVC_x64-debug/Bin/MSVC_Debug/Libs/MirageEngine_Debug.lib";

		std::vector<std::string> sources;
		for (const auto& entry : fs::recursive_directory_iterator(m_scriptsSourceDir)) {
			if (entry.path().extension() == ".cpp")
				sources.push_back("\"" + entry.path().string() + "\"");
		}
		if (sources.empty()) {
			LOG(logWarning, LogScriptManager, "Aucun script trouve");
			return true;
		}

		bool isDebug = (config == "Debug");
		std::string outDll = "\"" + (m_outputDir / "GameScripts.dll").string() + "\"";
		std::string outPdb = "\"" + (m_outputDir / "GameScripts.pdb").string() + "\"";
		std::string impLib = "\"" + (m_outputDir / "GameScripts.lib").string() + "\"";

		std::string cmd = "cl.exe";
		cmd += " /LD";                          
		cmd += " /std:c++17";
		cmd += isDebug ? " /MDd /Zi /Od" : " /MD /O2";
		cmd += " /EHsc /nologo";
		cmd += " /D SCRIPTS_DLL_EXPORT";
		cmd += " /D CPP_TRACE_DEBUG";
		cmd += " /I\"" + m_engineIncludeDir.string() + "\"";
		cmd += " /I\"" + m_toolbox.string() + "" + "\"";

		for (const auto& src : sources)
			cmd += " " + src;

		cmd += " /link";
		cmd += " /DLL";
		cmd += " /OUT:" + outDll;
		cmd += " /IMPLIB:" + impLib;
		cmd += isDebug ? " /DEBUG /PDB:" + outPdb : "";
		cmd += " \"" + m_engineLibPath.string() + "\"";  

		LOG(logInfo, LogScriptManager, ">>> ", cmd.c_str());

		FILE* pipe = _popen(cmd.c_str(), "r");
		if (!pipe) {
			LOG(logFatal, LogScriptManager, "Failed to execute cl.exe");
			return false;
		}
		char buffer[256];
		while (fgets(buffer, sizeof(buffer), pipe))
			LOG(logTemp, LogScriptManager, buffer);

		int status = _pclose(pipe);
		//LOG(logInfo, LogScriptManager, "Exit code: ", status);
		return 0;
	}

	std::string ScriptManager::FindMSVCCompiler()
	{
		const char* vswhere =
			"\"C:\\Program Files (x86)\\Microsoft Visual Studio\\Installer\\vswhere.exe\"";

		std::string cmd = "\"" + std::string(vswhere) +
			"\" -latest -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64"
			" -find VC\\Tools\\MSVC\\**\\bin\\Hostx64\\x64\\cl.exe 2>nul";

		std::string fullCmd = "cmd /C \"" + cmd + "\"";

		FILE* pipe = _popen(fullCmd.c_str(), "r");
		if (!pipe) return "";

		char buf[512] = {};
		fgets(buf, sizeof(buf), pipe);
		_pclose(pipe);

		std::string path(buf);
		if (!path.empty() && path.back() == '\n') path.pop_back();
		if (!path.empty() && path.back() == '\r') path.pop_back();
		return path;
	}
}