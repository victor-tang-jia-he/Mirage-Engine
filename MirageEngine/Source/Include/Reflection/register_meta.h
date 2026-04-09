#pragma once

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
	void RegisterMeshRendererMeta();
	void RegisterCameraMeta();
	void RegisterInputMappingMeta();

protected:
private:
	void RegisterGameObjectMeta();
	void RegisterTransformMeta();
};