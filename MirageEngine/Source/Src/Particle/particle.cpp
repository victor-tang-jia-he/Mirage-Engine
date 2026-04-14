#include <Particle/particle.h>

using namespace MirageEngine;
using namespace MirageEngine::Core::Math;

Particle::Particle()
	: Position(0.f, 0.f),
	Velocity(0.f, 0.f),
	Color(1.f, 1.f, 1.f, 1.f),
	Life(0.f)
{
}

Particle::Particle(const Vector2& _position, const Vector2& _velocity, const Vector4& _color, float _life)
	: Position(_position),
	Velocity(_velocity),
	Color(_color),
	Life(_life)
{
}

int ParticleSystem::FindUnusedParticle()
{
	return 0;
}
