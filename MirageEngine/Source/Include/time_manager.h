#pragma once

#include <chrono>

namespace MirageEngine
{
	class TimeManager
	{
	public:
		static void Init();
		static void Update();
		static float GetDeltaTime() { return m_DeltaTime; }
		static float GetTime() { return m_Time; }
	private:
		static std::chrono::high_resolution_clock::time_point m_LastFrameTime;
		static float m_DeltaTime;
		static float m_Time;
	};
}