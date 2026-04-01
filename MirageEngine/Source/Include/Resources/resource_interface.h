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
				std::string m_Name;

			public:
				virtual ~IResource() = default;

				std::string GetName() { return m_Name; }
			};
		}
	}
}