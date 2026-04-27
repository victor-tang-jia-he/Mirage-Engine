#pragma once
#include "i_script.h"

#ifdef SCRIPTS_DLL_EXPORT
#define SCRIPT_API __declspec(dllexport)  //  DLL
#else
#define SCRIPT_API __declspec(dllimport)  //  engine
#endif

extern "C" {
	SCRIPT_API int      GetScriptCount();
	SCRIPT_API const char* GetScriptName(int index);
	SCRIPT_API MirageEngine::Core::Scripting::IScript* CreateScriptByName(const char* name);
	SCRIPT_API void     DestroyScript(MirageEngine::Core::Scripting::IScript* _script);
}