#include <script_api.h>
#include "PlayerScript.h"
#include "AIScript.h"

#include <vector>
#include <functional>

// Liste interne ¡ª l'engine ne la voit jamais
static std::vector<std::function<MirageEngine::Core::Scripting::IScript*()>> s_factories = {
    [] { return new PlayerScript(); },
    [] { return new AIScript();    },
};

extern "C" {

    SCRIPT_API int getScriptCount() {
        return static_cast<int>(s_factories.size());
    }

    SCRIPT_API MirageEngine::Core::Scripting::IScript* createScript(int index) {
        if (index < 0 || index >= getScriptCount())
            return nullptr;
        return s_factories[index]();
    }

    SCRIPT_API void destroyScript(MirageEngine::Core::Scripting::IScript* script) {
        delete script;
    }
}
