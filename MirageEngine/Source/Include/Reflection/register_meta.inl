#pragma once

#include <EnTT/meta/meta.hpp>

namespace MirageEngine
{
    template<typename T>
    void RegisterSharedPtr()
    {
        entt::meta_factory<std::shared_ptr<T>> sharePTR{};
            sharePTR.func<&std::shared_ptr<T>::get>("get"_hs);
    }
}