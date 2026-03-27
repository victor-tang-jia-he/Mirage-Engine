#pragma once
#include <string>

namespace MirageEngine
{
	namespace Core
	{
		namespace Resources
		{
			// Interface
			class IResource
			{
				friend class ResourceManager;
			protected:
				std::string m_name;
			public:
				std::string GetName() { return m_name; }
			};
		}
	}
}