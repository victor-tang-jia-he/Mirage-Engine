#include "deserializer.h"
#pragma once

namespace MirageEngine
{
    template <typename T>
    std::vector<T> Deserializer::DeserializeVector(json _jVector)
    {
        std::vector<T> vectorData;
        for (const json& element : _jVector)
        {
            vectorData.push_back(element.get<T>());
        }
        return vectorData;
	}

    template<typename TKey, typename TValue>
    inline std::unordered_map<TKey, TValue> MirageEngine::Deserializer::DeserializeMap(json _jMap)
    {
        std::unordered_map<TKey, TValue> mapData;
        for (json::iterator it = _jMap.begin(); it != _jMap.end(); ++it)
        {
            TKey key = static_cast<TKey>(std::stoll(it.key()));
            TValue value = it.value().get<TValue>();
            mapData[key] = value;
        }
		return mapData;
    }
} // namespace MirageEngine