#include <Scripting/script_api.h>
#include "ScriptRegistry.h"
#include <functional>

extern "C" {
	SCRIPT_API int GetScriptCount() {
		return (int)MirageEngine::Core::Scripting::ScriptRegistry::Instance().GetScriptNames().size();
	}
	SCRIPT_API const char* GetScriptName(int index) 
	{
		auto& names = MirageEngine::Core::Scripting::ScriptRegistry::Instance().GetScriptNames();
		if (index < 0 || index >= (int)names.size()) return nullptr;
		return names[index].c_str();
	}

	SCRIPT_API MirageEngine::Core::Scripting::IScript* CreateScriptByName(const char* name)
	{
		return MirageEngine::Core::Scripting::ScriptRegistry::Instance().CreateByName(name);
	}

	SCRIPT_API void DestroyScript(MirageEngine::Core::Scripting::IScript* script) {
		delete script;
	}
}