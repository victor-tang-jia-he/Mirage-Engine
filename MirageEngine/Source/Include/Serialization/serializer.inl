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
} // namespace MirageEngine