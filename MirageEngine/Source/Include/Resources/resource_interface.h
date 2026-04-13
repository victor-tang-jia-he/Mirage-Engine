#pragma once
#include <string>

namespace MirageEngine::Core::Resources
{
	// Interface
	class IResource
	{
		friend class ResourceManager;
	protected:
		std::string p_Name;

	public:
		virtual ~IResource() = default;

		std::string GetName() { return p_Name; }
	};
}