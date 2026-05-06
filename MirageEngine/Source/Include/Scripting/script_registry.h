#pragma once
#include <Scripting/i_script.h>

namespace MirageEngine::Core::Scripting
{
    class IScript;

    class ScriptRegistry
    {
        public:

            using Factory = std::function<IScript*()>;

            static ScriptRegistry& Instance()
            {
                static ScriptRegistry instance;
                return instance;
            }

            bool RegisterScript(const char* name, Factory factory)
            {
                if (m_Factories[name] != nullptr)
                    return false;
                m_Factories[name] = factory;
                m_Names.push_back(name);
                return true;
            }

            IScript* CreateByName(const std::string& name)
            {
                auto it = m_Factories.find(name);
                if (it == m_Factories.end())
                    return nullptr;
                return it->second();
            }

            const std::vector<std::string>& GetScriptNames() const { return m_Names; }

        private:

            std::unordered_map<std::string, Factory> m_Factories;
            std::vector<std::string> m_Names;
    };

    struct ScriptAutoRegister
    {
            ScriptAutoRegister(const char* name, std::function<IScript*()> factory) { ScriptRegistry::Instance().RegisterScript(name, factory); }
    };
} // namespace MirageEngine::Core::Scripting

#define REGISTER_SCRIPT(ClassName)                                                        \
    static MirageEngine::Core::Scripting::ScriptAutoRegister s_autoRegister_##ClassName(  \
        #ClassName,                                                                       \
        []                                                                                \
        {                                                                                 \
            return static_cast<MirageEngine::Core::Scripting::IScript*>(new ClassName()); \
        });
