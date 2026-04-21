#pragma once

#include <EnTT/meta/meta.hpp>

using namespace entt::literals;

namespace MirageEngine
{
    template<typename T>
    void RegisterMeta::RegisterSharedPtr()
    {
        entt::meta_factory<std::shared_ptr<T>> sharePTR{};
    }
}