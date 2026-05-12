#pragma once
#include <iostream>
#include <vector>
#include <string>

namespace MirageEngine::AI
{
	class ResponsiveCurve
	{
	public:
		enum class CurveType { Linear, Exponential, Logistic };
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

		// Getters
		float GetSlope() { return m_Slope; }
		float GetExponent() { return m_Exponent; }
		float GetXOffset() { return m_XOffset; }
		float GetYOffset() { return m_YOffset; }
		bool GetInverted() { return m_Inverted; }
		CurveType GetType() { return m_Type; }

		// Setters
		void SetSlope(float _newValue) { m_Slope = _newValue; }
		void SetExponent(float _newValue) { m_Exponent = _newValue; }
		void SetXOffset(float _newValue) { m_XOffset = _newValue; }
		void SetYOffset(float _newValue) { m_YOffset = _newValue; }
		void SetInverted(bool _isInverted) { m_Inverted = _isInverted; }
		void SetType(CurveType _type) { m_Type = _type; }
	};
}