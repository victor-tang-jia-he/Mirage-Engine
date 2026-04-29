#pragma once
#include <Scripting/dynlib.h>
#include <iostream>
#include <filesystem>
#include <thread>

#include <Scripting/i_script.h>
#include <Logs/log_macro.h>

DECLARE_LOG_CATEGORY_EXTERN(LogScriptManager)

namespace MirageEngine::Core::Scripting
{
	namespace fs = std::filesystem;

#ifdef _WIN32
	constexpr const char* kScriptLibName = "GameScripts_Debug.dll";
#elif __APPLE__
	constexpr const char* kScriptLibName = "LibGameScripts.dylib";
#else
	constexpr const char* kScriptLibName = "LibGameScripts.so";
#endif

	class ScriptManager
	{
		using IScript = MirageEngine::Core::Scripting::IScript;
		fs::path buildDir = BUILD_DIRECTORY;
		std::string config = "Debug";


		// Internal forward definition
		typedef int       (*PFN_GetScriptCount)();
		typedef const char* (*PFN_GetScriptName)(int);
		typedef IScript* (*PFN_CreateScriptByName)(const char*);
		typedef void      (*PFN_DestroyScript)(IScript*);

		void* GetScriptCountDLL;
		void* GetScriptNameDLL;
		void* CreateScriptByNameDLL;
		void* DestroyScriptDLL;

		// handle
		LibHandle hScripts;

		// Dll instance cleaner
		struct DllDeleter {
			PFN_DestroyScript destroyFunc;
			void operator()(IScript* s) const {
				if (s) {
					s->OnDestroy();
					destroyFunc(s);
				}
			}
		};

		// Create and bind ours cleaner 
		using ScriptPtr = std::unique_ptr<IScript, DllDeleter>;

		// Cache for delete later
		std::vector<ScriptPtr> scripts;
	public:
		void Init();
		void ReloadScript();

		void ShutDown() const;

		int GetScriptsCount() const;
		const char* GetScriptName(int index) const;
		IScript* CreateScriptByName(const char* name);
		void DestroyScript(IScript* script) const ;
	private:
		bool SetupVisualStudioEnvironment();

		bool RebuildScriptsDll(const fs::path& buildDir, const std::string& config);
	};
}