#pragma once
#include "i_script.h"

#ifdef SCRIPTS_DLL_EXPORT
#define SCRIPT_API __declspec(dllexport)
#elif defined(SCRIPTS_LOAD_DYNAMIC)
#define SCRIPT_API    // nothing ¨C but we also omit the function declarations
#else
#define SCRIPT_API __declspec(dllimport)
#endif

// Function pointer types ¨C always available
typedef int      (*PFN_GetScriptCount)();
typedef const char* (*PFN_GetScriptName)(int);
typedef MirageEngine::Core::Scripting::IScript* (*PFN_CreateScriptByName)(const char*);
typedef void     (*PFN_DestroyScript)(MirageEngine::Core::Scripting::IScript*);

// Only declare the actual functions if we are NOT loading dynamically
#ifndef SCRIPTS_LOAD_DYNAMIC
extern "C" {
    SCRIPT_API int      GetScriptCount();
    SCRIPT_API const char* GetScriptName(int index);
    SCRIPT_API MirageEngine::Core::Scripting::IScript* CreateScriptByName(const char* name);
    SCRIPT_API void     DestroyScript(MirageEngine::Core::Scripting::IScript* _script);
}
#endif