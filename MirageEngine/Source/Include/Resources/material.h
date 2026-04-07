#pragma once
#include <vector3.h>
#include <unordered_map>
#include <Resources/resource_interface.h>
#include <Logs/log_macro.h>

DECLARE_LOG_CATEGORY_EXTERN(LogMaterial)

namespace MirageRHI
{
    class IPipeline;
}

namespace MirageEngine
{
	namespace Core
	{
		namespace Resources
		{
            class Texture;
            class IShader;
            class Mesh;

            class Material : public IResource
            {
            public:
				virtual ~Material() = default;
				void SetShader(MirageRHI::IPipeline* _shader);

                Material& SetFloat(const std::string& _name, float _value);
                Material& SetVec3(const std::string& _name, const MirageEngine::Core::Math::Vector3& _value);
                Material& SetColor(const std::string& _name, const MirageEngine::Core::Math::Vector3& _color);
                Material& SetTexture(const std::string& _name, Texture* _texture);

                float GetFloat(const std::string& _name, float _defaultValue = 0.0f) const;
                MirageEngine::Core::Math::Vector3 GetVec3(const std::string& _name, const MirageEngine::Core::Math::Vector3& _defaultValue = MirageEngine::Core::Math::Vector3(0.f)) const;
                Texture* GetTexture(const std::string& _name) const;

                void Bind();

            private:
                struct MaterialData 
                {
                    std::unordered_map<std::string, float> Floats;
                    std::unordered_map<std::string, MirageEngine::Core::Math::Vector3> Vec3s;
                    std::unordered_map<std::string, Texture*> Textures;
                } m_Data;

                MirageRHI::IPipeline* m_Shader;
            };
		}
	}
}