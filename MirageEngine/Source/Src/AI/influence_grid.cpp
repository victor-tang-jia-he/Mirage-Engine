#include <AI/influence_grid.h>
#include <GameObject/game_object.h>
#include <Components/transform.h>
#include <time_manager.h>
#include <DebugDraw/debug_draw.h>
#include <entt/entt.hpp>

using namespace MirageEngine::Core::Math;

namespace MirageEngine::Core::Components
{
    void InfluenceGrid::OnStart() { Bake(); }

    void InfluenceGrid::OnUpdate(float _dt) { UpdateDecay(_dt); }

    void InfluenceGrid::OnDrawLine() { RenderGrid(); }

    void InfluenceGrid::AttachNew(std::shared_ptr<GameObject> _owner)
    {
        std::shared_ptr<InfluenceGrid> compPtr = std::make_shared<InfluenceGrid>(_owner.get());
        _owner->AddComponent<InfluenceGrid>(compPtr);
    }

    entt::id_type InfluenceGrid::GetType() const { return entt::type_id<InfluenceGrid>().hash(); }

    void InfluenceGrid::Bake()
    {
        if (GetOwner())
        {
            m_ResolutionX = static_cast<int>(m_GridWidth / m_CellSize);
            m_ResolutionY = static_cast<int>(m_GridHeight / m_CellSize);

            if (m_ResolutionX <= 0) m_ResolutionX = 1;
            if (m_ResolutionY <= 0) m_ResolutionY = 1;

            Vector3 ownerPos = GetOwner()->GetPosition();
            m_Origin = Vector3(
                ownerPos.X - (m_GridWidth * 0.5f),
                ownerPos.Y,
                ownerPos.Z - (m_GridHeight * 0.5f)
            );

            m_Cells.assign(m_ResolutionX * m_ResolutionY, InfluenceCell());
            for (InfluenceCell& cell : m_Cells)
                cell.Channels.resize(m_ChannelsNames.size(), 0.0f);
        }
    }

    void InfluenceGrid::UpdateDecay(float _deltaTime)
    {
        float frameDecay = powf(m_DecayRate, _deltaTime);

        for (auto& cell : m_Cells)
        {
            for (int i = 0; i < m_ChannelsNames.size(); i++)
            {
                cell.Channels[i] *= frameDecay;
                if (cell.Channels[i] < 0.01f)
                    cell.Channels[i] = 0.0f;
            }
        }
    }

    void InfluenceGrid::WorldToGrid(Vector3 _pos, int& _x, int& _y)
    {
        _x = static_cast<int>((_pos.X - m_Origin.X) / m_CellSize);
        _y = static_cast<int>((_pos.Z - m_Origin.Z) / m_CellSize);
    }

    bool InfluenceGrid::IsInBounds(int _x, int _y) const { return _x >= 0 && _x < m_ResolutionX && _y >= 0 && _y < m_ResolutionY; }

    void InfluenceGrid::AddInfluence(int _x, int _y, int _channelIndex, float _value)
    {
        if (IsInBounds(_x, _y) && _channelIndex >= 0 && _channelIndex < m_ChannelsNames.size())
        {
            int index = GetIndex(_x, _y);
            m_Cells[index].Channels[_channelIndex] += _value;
        }
    }

    void InfluenceGrid::AddInfluenceArea(Vector3 pos, float radius, float value, int _channelIndex)
    {
        int centerX, centerY;
        WorldToGrid(pos, centerX, centerY);
        if (!IsInBounds(centerX, centerY))
            return;

        int radiusInCells = static_cast<int>(radius / m_CellSize);
        for (int y = -radiusInCells; y <= radiusInCells; y++)
        {
            for (int x = -radiusInCells; x <= radiusInCells; x++)
            {
                int cellX = centerX + x;
                int cellY = centerY + y;
                if (IsInBounds(cellX, cellY))
                {
                    float distSq = (x * x + y * y) * (m_CellSize * m_CellSize);
                    float distance = sqrtf(distSq);

                    if (distance <= radius)
                    {
                        float influenceValue = value * (1.0f - (distance / radius));
                        AddInfluence(cellX, cellY, _channelIndex, influenceValue);
                    }
                }
            }
        }
    }

    void InfluenceGrid::AddChannel(std::string _name)
    {
        m_ChannelsNames.push_back(_name);
        for (InfluenceCell& cell : m_Cells)
            cell.Channels.push_back(0.0f);
    }

    void InfluenceGrid::SetChannelsNames(const std::vector<std::string>& _names)
    {
        m_ChannelsNames = _names;
        for (InfluenceCell& cell : m_Cells)
            cell.Channels.resize(m_ChannelsNames.size(), 0.0f);
    }

    Vector3 InfluenceGrid::GetCellWorldPosition(int _x, int _y) const
    {
        return Vector3(m_Origin.X + _x * m_CellSize + m_CellSize * 0.5f, m_Origin.Y, m_Origin.Z + _y * m_CellSize + m_CellSize * 0.5f);
    }

    int InfluenceGrid::GetChannelsIndex(std::string _name) const
    {
        for (size_t i = 0; i < m_ChannelsNames.size(); i++)
        {
            if (m_ChannelsNames[i] == _name)
                return static_cast<int>(i);
        }
        return -1;
    }

    void InfluenceGrid::RenderGrid() const
    {
        int cols = m_ResolutionX;
        int rows = m_ResolutionY;

        for (int x = 0; x <= cols; ++x) {
            Vector3 a = m_Origin + Vector3(x * m_CellSize, 0.1f, 0.0f);
            Vector3 b = m_Origin + Vector3(x * m_CellSize, 0.1f, rows * m_CellSize);
            Debug_Draw.Line(a, b, { 1, 1, 1, 0.5f });
        }
        for (int y = 0; y <= rows; ++y) {
            Vector3 a = m_Origin + Vector3(0.0f, 0.1f, y * m_CellSize);
            Vector3 b = m_Origin + Vector3(cols * m_CellSize, 0.1f, y * m_CellSize);
            Debug_Draw.Line(a, b, { 1, 1, 1, 0.5f });
        }
    }
} // namespace MirageEngine::Core::Components
