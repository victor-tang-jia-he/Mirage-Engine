#pragma once
#include <Components/component.h>
#include <Scripting/i_script.h>
#include <AI/ai_action.h>
#ifdef _WIN32
#    ifdef ENGINE_EXPORTS
#        define ENGINE_API __declspec(dllexport)
#    else
#        define ENGINE_API __declspec(dllimport)
#    endif
#else
#    define ENGINE_API
#endif

extern "C"
{
}
