#pragma once
#include <Components/component.h>
#include <Scripting/i_script.h>
#include <AI/ai_action.h>

#ifdef ENGINE_EXPORTS
#    define ENGINE_API __declspec(dllexport)
#else
#    define ENGINE_API __declspec(dllimport)
#endif

//#define REGISTER_SCRIPT(ClassName)                               \
//    inline static bool _reg_##ClassName = []()                   \
//    {                                                            \
//        entt::meta<ClassName>()                                  \
//            .type(entt::type_id<ClassName>().hash(), #ClassName) \
//            .base<MirageEngine::AI::AIAction>()                  \
//            .base<MirageEngine::Core::Scripting::IScript>()      \
//            .base<MirageEngine::Core::Components::Component>()   \
//            .ctor<std::shared_ptr<ClassName>>();                 \
//        return true;                                             \
//    }();

extern "C"
{
}
