#pragma once 
#include <iostream>
#include <vector>
#include <vector3.h>
#include "Components/component.h" 

namespace MirageEngine::Core::Components
{
	struct InfluenceCell
	{
		std::vector<float>channels;
	};

	class InfluenceGrid : public Component
	{
	private:
		// Data the user can set
		float m_GridWidth;
		float m_GridHeight;
		float m_CellSize;
		float m_DecayRate = 0.95f;
		std::vector<std::string> m_ChannelsNames;

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
		void AddInfluence(int _x, int _y, int _channelIndex, float _value);
		void AddInfluenceArea(Core::Math::Vector3 pos, float radius, float value, int _channelIndex);
		void AddChannel(std::string _name);
		int GetIndex(int x, int y) const { return y * m_ResolutionX + x; }
		Core::Math::Vector3 GetCellWorldPosition(int x, int y) const;

		void RenderGrid();

		// Getters
		float GetWidth() const { return m_GridWidth; }
		float GetHeight() const { return m_GridHeight; }
		float GetCellSize() const { return m_CellSize; }
		float GetDecayRate() const { return m_DecayRate; }
		const std::vector<InfluenceCell>& GetCells() const{ return m_Cells; }
		int GetChannelsIndex(std::string _name) const;
		std::vector<std::string> GetChannelsNames() const { return m_ChannelsNames; }
		int GetResX() const { return m_ResolutionX; }
		int GetResY() const { return m_ResolutionY; }

		// Setters
		void SetWidth(float _w) { m_GridWidth = _w; }
		void SetHeight(float _h) { m_GridHeight = _h; }
		void SetCellSize(float _s) { m_CellSize = _s; }
		void SetDecayRate(float _d) { m_DecayRate = _d; }
	};
}