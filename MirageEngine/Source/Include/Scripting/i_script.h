#pragma once

namespace MirageEngine::Core::Scripting {
    class IScript 
    {
    public:
        virtual ~IScript() = default;

        virtual void        onStart() = 0;
        virtual void        onUpdate(float dt) = 0;
        virtual void        onDestroy() = 0;
        virtual const char* getName()   const = 0;
    };
}