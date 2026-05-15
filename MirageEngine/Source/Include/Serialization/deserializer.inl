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
} // namespace MirageEngine