#pragma once 
#include <iostream>
#include <vector>
#include <vector3.h>
#include "Components/component.h" 

namespace MirageEngine::Core::Components
{
	enum class InfluenceType
	{
		CHANNEL_DANGER = 0,
		CHANNEL_SAFETY = 1,
		CHANNEL_RESOURCE = 2,
		CHANNEL_COVER = 3,
		COUNT
	};

	struct InfluenceCell
	{
		static constexpr int MAX_CHANNELS = (int)InfluenceType::COUNT;
		float channels[MAX_CHANNELS] = { 0.0f };
	};

	class InfluenceGrid : public Component
	{
	private:
		// Data the user can set
		float m_GridWidth;
		float m_GridHeight;
		float m_CellSize;
		float m_DecayRate = 0.95f;

		// Internal data calculated from the other parameters
		int m_ResolutionX;
		int m_ResolutionY;
		std::vector<InfluenceCell> m_Cells;
		Core::Math::Vector3 m_Origin;

	public:
		InfluenceGrid() = default;
		virtual ~InfluenceGrid() = default;

		void Init() override;
		void Update() override;
		entt::id_type GetType() const override { return entt::type_id<InfluenceGrid>().hash(); }

		void Bake();
		void UpdateDecay(float _deltaTime);

		void WorldToGrid(Core::Math::Vector3 _pos, int& _x, int& _y);
		bool IsInBounds(int _x, int _y) const;
		void AddInfluence(int _x, int _y, InfluenceType _type, float _value);
		void AddInfluenceArea(Core::Math::Vector3 pos, float radius, float value, InfluenceType type);
		int GetIndex(int x, int y) const { return y * m_ResolutionX + x; }
		Core::Math::Vector3 GetCellWorldPosition(int x, int y) const;

		void RenderDebug();

		// Getters
		float GetWidth() const { return m_GridWidth; }
		float GetHeight() const { return m_GridHeight; }
		float GetCellSize() const { return m_CellSize; }
		float GetDecayRate() const { return m_DecayRate; }

		// Setters
		void SetWidth(float _w) { m_GridWidth = _w; }
		void SetHeight(float _h) { m_GridHeight = _h; }
		void SetCellSize(float _s) { m_CellSize = _s; }
		void SetDecayRate(float _d) { m_DecayRate = _d; }
	};
}