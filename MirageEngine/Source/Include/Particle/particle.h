#pragma once

#include <vector4.h>

// Resource (CTRL + Click) :
// Learn OpenGL : https://learnopengl.com/In-Practice/2D-Game/Particles
// BillBoard Particle Method : https://www.opengl-tutorial.org/fr/intermediate-tutorials/billboards-particles/particles-instancing/

namespace MirageEngine
{
    struct Particle
    {
        Core::Math::Vector2 Position;
		Core::Math::Vector2 Velocity;
		Core::Math::Vector4 Color;
        float Life;

        Particle();
        Particle(const Core::Math::Vector2& _position, const Core::Math::Vector2& _velocity, const Core::Math::Vector4& _color, float _life);
    };

	class ParticleSystem
	{
    private:
        const int MaxParticles = 100000;
        int LastUsedParticle = 0;

        Particle ParticlesContainer[];

	public:
		ParticleSystem() = default;
		~ParticleSystem() = default;

    private:
        int FindUnusedParticle();
	};
}