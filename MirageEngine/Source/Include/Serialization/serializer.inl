#pragma once

#include "serializer.h"

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

    template<typename T>
    inline json MirageEngine::Serializer::SerializeMap(std::unordered_map<std::string, T> _map)
    {
        json jMap;
        for (const auto& pair : _map)
        {
            jMap[pair.first] = pair.second;
        }
		return jMap;
    }
} // namespace MirageEngine