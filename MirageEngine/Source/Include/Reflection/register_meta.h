#pragma once

namespace MirageEngine
{
	class GameObject;
}

class RegisterMeta
{
// Variables
public:
protected:
private:

// Methods
public:
	RegisterMeta();
	~RegisterMeta();

	void RegisterAllMeta();

protected:
private:
	void RegisterGameObjectMeta();
	void RegisterTransformMeta();
};