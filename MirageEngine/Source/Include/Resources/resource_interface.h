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
		std::string p_Path;
	public:
		virtual ~IResource() = default;

		std::string GetName() { return p_Name; }
	};
}