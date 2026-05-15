#include "serializer.h"
#pragma once

namespace MirageEngine
{
    template <typename T>
    json Serializer::SerializeVector(std::vector<T> _vector)
    {
        json jArray = json::array();
        for (const T& element : _vector)
        {
            jArray.emplace_back(element);
        }
        return jArray;
    }

    template<typename TKey, typename TValue>
    inline json MirageEngine::Serializer::SerializeMap(std::unordered_map<TKey, TValue> _map)
    {
        json jMap;
        for (const auto& pair : _map)
        {
            jMap[std::to_string(pair.first)] = pair.second;
        }
		return jMap;
    }
} // namespace MirageEngine