#pragma once
#include "i_script.h"

#ifdef SCRIPTS_DLL_EXPORT
#define SCRIPT_API __declspec(dllexport)  //  DLL
#else
#define SCRIPT_API __declspec(dllimport)  //  engine
#endif

extern "C" {
    SCRIPT_API int      getScriptCount();
    SCRIPT_API MirageEngine::Core::Scripting::IScript* createScript(int index);
    SCRIPT_API void     destroyScript(MirageEngine::Core::Scripting::IScript* script);
}