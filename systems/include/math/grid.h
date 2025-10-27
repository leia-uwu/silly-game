#include "math/vec2.h"

#include <algorithm>
#include <cassert>
#include <cfloat>
#include <cmath>
#include <cstdint>
#include <vector>

template<typename GridSize_T, typename EntityID_T>
class Grid
{
public:
    Grid(GridSize_T worldSize, GridSize_T cellSize, EntityID_T maxEntityID) :
        m_worldSize(worldSize),
        m_cellSize(cellSize),
        m_gridSize(worldSize / cellSize),
        m_cellCount(m_gridSize * m_gridSize),
        m_maxEntityID(maxEntityID)
    {
        m_cells = new Cell[m_cellCount];
        m_entityCache = new EntityGridData[maxEntityID];

        m_resultCache.reserve(256);
    }

    Grid(const Grid&) = delete;
    Grid(Grid&&) = delete;
    Grid& operator=(const Grid&) = delete;
    Grid& operator=(Grid&&) = delete;

    ~Grid()
    {
        delete[] m_cells;
        delete[] m_entityCache;
    }

    [[nodiscard]] float worldSize() const
    {
        return m_worldSize;
    }

    [[nodiscard]] GridSize_T cellSize() const
    {
        return m_cellSize;
    }

    [[nodiscard]] GridSize_T gridSize() const
    {
        return m_gridSize;
    }

    [[nodiscard]] EntityID_T maxEntityID() const
    {
        return m_maxEntityID;
    }

    void insertEntity(EntityID_T entityID, Vec2 min, Vec2 max)
    {
        EntityGridData& entity = getEntityData(entityID);

        GridAABB localBounds = {
            .min = roundToGrid(min),
            .max = roundToGrid(max),
        };

        // entity already exists
        if (entity.valid) {
            GridAABB& bounds = entity.bounds;

            // if the bounds didn't change from the existing ones return earlier
            if (
                localBounds.min == bounds.min
                && localBounds.max == bounds.max
            ) {
                return;
            }

            if (bounds.min == bounds.max) {
                Cell& cell = cellAt(bounds.min.x, bounds.min.y);
                auto it = std::find(cell.items.begin(), cell.items.end(), entityID);
                if (it != cell.items.end()) {
                    cell.items.erase(it);
                }
            } else {
                for (GridSize_T y = bounds.min.y; y <= bounds.max.y; y++) {
                    for (GridSize_T x = bounds.min.x; x <= bounds.max.x; x++) {
                        Cell& cell = cellAt(x, y);
                        auto it = std::find(cell.items.begin(), cell.items.end(), entityID);
                        if (it != cell.items.end()) {
                            cell.items.erase(it);
                        }
                    }
                }
            }
        }

        entity.valid = true;
        entity.bounds = localBounds;

        for (GridSize_T y = localBounds.min.y; y <= localBounds.max.y; y++) {
            for (GridSize_T x = localBounds.min.x; x <= localBounds.max.x; x++) {
                Cell& cell = cellAt(x, y);
                cell.items.push_back(entityID);
            }
        }
    }

    void removeEntity(EntityID_T entityID)
    {
        EntityGridData& entity = getEntityData(entityID);

        if (!entity.valid)
            return;

        GridAABB& bounds = entity.bounds;

        // erase entity from current grid cells its occupying
        if (bounds.min == bounds.max) {
            Cell& cell = cellAt(bounds.min.x, bounds.min.y);
            auto it = std::find(cell.items.begin(), cell.items.end(), entityID);
            if (it != cell.items.end()) {
                cell.items.erase(it);
            }
        } else {
            for (GridSize_T y = bounds.min.y; y <= bounds.max.y; y++) {
                for (GridSize_T x = bounds.min.x; x <= bounds.max.x; x++) {
                    Cell& cell = cellAt(x, y);
                    auto it = std::find(cell.items.begin(), cell.items.end(), entityID);
                    if (it != cell.items.end()) {
                        cell.items.erase(it);
                    }
                }
            }
        }

        entity.valid = false;
        entity.queryID = 0;
        entity.bounds = {{0, 0}, {0, 0}};
    }

    const std::vector<EntityID_T>& queryAABB(Vec2 min, Vec2 max)
    {
        GridAABB bounds = {
            .min = roundToGrid(min),
            .max = roundToGrid(max),
        };

        return queryGridAABB(bounds);
    }

    const std::vector<EntityID_T>& queryPosition(Vec2 pos)
    {
        GridPos gridPos = roundToGrid(pos);
        return cellAt(gridPos.x, gridPos.y).items;
    }

    const std::vector<EntityID_T>& queryEntity(EntityID_T entityID)
    {
        EntityGridData& entity = m_entityCache[entityID];
        assert(entity.valid);
        return queryGridAABB(entity.bounds);
    }

    const std::vector<EntityID_T>& queryLine(Vec2 lineStart, Vec2 lineEnd)
    {
        Vec2 diff = lineEnd - lineStart;

        int gridDirX = lineEnd.x >= lineStart.x ? 1 : -1;
        int gridDirY = lineEnd.y >= lineStart.y ? 1 : -1;

        float dirX =
            std::abs(diff.x) > 0.00001
            ? (gridDirX * (int)m_cellSize) / diff.x
            : DBL_MAX;

        float dirY =
            std::abs(diff.y) > 0.00001
            ? (gridDirY * (int)m_cellSize) / diff.y
            : DBL_MAX;

        // cell relative
        float relativeX = std::fmod(lineStart.x / m_cellSize, 1.F);
        float relativeY = std::fmod(lineStart.y / m_cellSize, 1.F);

        float x = dirX * (gridDirX > 0 ? 1.F - relativeX : relativeX);
        float y = dirY * (gridDirY > 0 ? 1.F - relativeY : relativeY);

        GridPos start = roundToGrid(lineStart);
        GridPos endCell = roundToGrid(lineEnd);

        int cellX = start.x;
        int cellY = start.y;

        uint32_t query = getNextQueryID();
        m_resultCache.clear();

        while (true) {
            Cell& cell = cellAt(cellX, cellY);

            for (EntityID_T entityId : cell.items) {
                EntityGridData& entity = getEntityData(entityId);
                if (entity.queryID != query) {
                    entity.queryID = query;
                    m_resultCache.push_back(entityId);
                }
            }

            if (cellX == endCell.x && cellY == endCell.y) {
                break;
            }

            if (x < y) {
                x += dirX;
                cellX += gridDirX;
                if (cellX < 0 || cellX >= m_gridSize) {
                    break;
                }
            } else {
                y += dirY;
                cellY += gridDirY;
                if (cellY < 0 || cellY >= m_gridSize) {
                    break;
                }
            }
        }

        return m_resultCache;
    }

private:
    struct Cell
    {
        std::vector<EntityID_T> items;
    };

    struct GridPos
    {
        GridSize_T x;
        GridSize_T y;

        bool operator==(const GridPos& other) const
        {
            return other.x == x && other.y == y;
        }
    };

    template<typename T>
    struct AABB
    {
        T min;
        T max;
    };

    using GridAABB = AABB<GridPos>;
    using WorldAABB = AABB<Vec2>;

    struct EntityGridData
    {
        bool valid = false;
        uint32_t queryID;
        GridAABB bounds;
    };

    GridPos roundToGrid(Vec2 pos)
    {
        return {
            .x = (GridSize_T)std::clamp((int)(pos.x / m_cellSize), 0, (int)m_gridSize - 1),
            .y = (GridSize_T)std::clamp((int)(pos.y / m_cellSize), 0, (int)m_gridSize - 1),
        };
    }

    const std::vector<EntityID_T>& queryGridAABB(GridAABB bounds)
    {
        m_resultCache.clear();

        uint32_t query = getNextQueryID();

        for (GridSize_T y = bounds.min.y; y <= bounds.max.y; y++) {
            for (GridSize_T x = bounds.min.x; x <= bounds.max.x; x++) {
                Cell& cell = cellAt(x, y);

                for (EntityID_T entityId : cell.items) {
                    EntityGridData& entity = getEntityData(entityId);

                    if (entity.queryID != query) {
                        entity.queryID = query;
                        m_resultCache.push_back(entityId);
                    }
                }
            }
        }

        return m_resultCache;
    }
    /**
     * Width and height of the entire grid, in world units
     */
    float m_worldSize;

    /**
     * Width and height of each grid cell
     */
    GridSize_T m_cellSize;

    /**
     * Width and height of the entire grid, in grid units (world / cell size)
     */
    GridSize_T m_gridSize;

    /**
     * The size of m_cells
     */
    size_t m_cellCount;
    Cell* m_cells;

    Cell& cellAt(GridSize_T x, GridSize_T y)
    {
        size_t idx = (y * m_gridSize) + x;
        assert(idx <= m_cellCount);
        return m_cells[idx];
    }

    /**
     * The biggest possible entity ID and size of m_entityCache
     */
    EntityID_T m_maxEntityID;
    EntityGridData* m_entityCache;

    EntityGridData& getEntityData(EntityID_T ID)
    {
        assert(ID <= m_maxEntityID);
        return m_entityCache[ID];
    };

    /**
     * Cached vector for query results to avoid allocations
     */
    std::vector<EntityID_T> m_resultCache;
    uint32_t m_nextQueryID = 1;
    uint32_t getNextQueryID()
    {
        return m_nextQueryID++;
    }
};
