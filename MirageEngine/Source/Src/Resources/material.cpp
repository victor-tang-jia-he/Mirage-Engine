#include <Resources/material.h>
#include <RHI/OpenGL/openGL_pipeline.h>
#include <RHI/OpenGL/openGL_buffer.h>
#include <RHI/i_texture.h>
#include <RHI/i_shader.h>
#include <Resources/mesh.h>
#include <Resources/texture.h>

using namespace MirageEngine::Core::Resources;
using namespace MirageRHI;

void Material::SetShader(MirageRHI::IPipeline* _shader)
{
	this->m_Shader = _shader;
}

Material& Material::SetFloat(const std::string& _name, float _value)
{
	m_Data.Floats[_name] = _value;
	return *this;
}

Material& Material::SetVec3(const std::string& _name, const MirageEngine::Core::Math::Vector3& _value)
{
	m_Data.Vec3s[_name] = _value;
	return *this;
}

Material& Material::SetColor(const std::string& _name, const MirageEngine::Core::Math::Vector3& _color)
{
	return SetVec3(_name, _color);
}

Material& Material::SetTexture(const std::string& _name, Texture* _texture)
{
	m_Data.Textures[_name] = _texture;
	return *this;
}

float Material::GetFloat(const std::string& _name, float _defaultValue) const
{
	auto it = m_Data.Floats.find(_name);
	return (it != m_Data.Floats.end()) ? it->second : _defaultValue;
}

MirageEngine::Core::Math::Vector3 Material::GetVec3(const std::string& _name, const MirageEngine::Core::Math::Vector3& _defaultValue) const
{
	auto it = m_Data.Vec3s.find(_name);
	return (it != m_Data.Vec3s.end()) ? it->second : _defaultValue;
}

Texture* Material::GetTexture(const std::string& _name) const
{
	auto it = m_Data.Textures.find(_name);
	return (it != m_Data.Textures.end()) ? it->second : nullptr;
}

void Material::Bind()
{
	if (!m_Shader) {
		DEBUG_THROW("Material has no shader assigned");
	}

	// Bind textures
	int textureUnit = 0;
	for (const auto& [_name, _texture] : m_Data.Textures) 
	{
		if (_texture) 
		{
			// Activate the correct texture unit, then bind the texture object.
			glActiveTexture(GL_TEXTURE0 + textureUnit);
			_texture->GetTextureBuffer()->Bind();
			m_Shader->SetInt(_name, textureUnit);
			textureUnit++;
		}
	}
	// Bind Floats
	for (const auto& [_name, _value] : m_Data.Floats) {
		m_Shader->SetFloat(_name, _value);
	}
	// Bind vec3s
	for (const auto& [_name, _value] : m_Data.Vec3s) {
		m_Shader->SetVec3(_name, _value);
	}
}