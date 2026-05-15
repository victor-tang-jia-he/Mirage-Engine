#pragma once

#include "deserializer.h"

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

    template<typename T>
    inline std::unordered_map<std::string, T> MirageEngine::Deserializer::DeserializeMap(json _jMap)
    {
        std::unordered_map<std::string, T> mapData;
        for (const json& element : _jMap)
        {
            std::string key = element.type_name();
            T value = element.get<T>();
            mapData[key] = value;
        }
		return mapData;
    }
} // namespace MirageEngine