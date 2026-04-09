#include <time_manager.h>

using namespace MirageEngine;

std::chrono::high_resolution_clock::time_point TimeManager::m_LastFrameTime;
float TimeManager::m_DeltaTime = 0.f;
float TimeManager::m_Time = 0.f;

void TimeManager::Init()
{
	m_LastFrameTime = std::chrono::high_resolution_clock::now();
	m_DeltaTime = 0.f;
	m_Time = 0.f;
}

void TimeManager::Update()
{
	std::chrono::high_resolution_clock::time_point currentTime = std::chrono::high_resolution_clock::now();
	m_DeltaTime = std::chrono::duration<float>(currentTime - m_LastFrameTime).count();
	m_Time += m_DeltaTime;
	m_LastFrameTime = currentTime;
}
