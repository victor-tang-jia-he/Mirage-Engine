#include <iostream>
#include <vector>
#include <string>

namespace MirageEngine::AI
{
	class ResponsiveCurve
	{
		enum class CurveType { Linear, Exponential, Logistic};

	private:
		CurveType m_Type = CurveType::Linear;
		float m_Slope = 1.0f;
		// For Exponential and logistical curves
		float m_Exponent = 2.0f;
		float m_XOffset = 0.0f;
		float m_YOffset = 0.0f;
		bool m_Inverted = false;

	public:
		float Evaluate(float _x);
	};
}