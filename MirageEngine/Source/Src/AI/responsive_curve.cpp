#include "AI/responsive_curve.h"
#include <algorithm>
#include <cmath>

namespace MirageEngine::AI
{
	float ResponsiveCurve::Evaluate(float _x)
	{
		_x = std::clamp(_x, 0.0f, 1.0f);
		float val = std::fmaxf(0.0f, _x - m_XOffset);
		float result = 0.0f;

		switch (m_Type)
		{
		case CurveType::Linear:
			result = m_Slope * val;
			break;
		case CurveType::Exponential:
			result = powf(val, m_Exponent) * m_Slope;
			break;
		case CurveType::Logistic:
			result = 1.0f / (1.0f + expf(-m_Slope * (val - 0.5f)));
			break;
		}
		result += m_YOffset;

		if (m_Inverted)
		{
			result = 1.0f - result;
		}
		
		return std::clamp(result, 0.0f, 1.0f);
	}
}