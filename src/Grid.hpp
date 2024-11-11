#pragma once

#include <vector>

#include "common/math.hpp"
#include "common/log.hpp"
#include "raymath.h"

using namespace std;

struct GridCell {
    ivec2 pos = {-1,-1};
    int idx = -1;
    GridCell (ivec2 pos, int idx) : pos(pos), idx(idx) {};
    GridCell () = default;
};

template <typename TCell = GridCell>
class Grid {
public:
    ivec2 toCoord (vec2 cellSize, vec2 pos) {
        return ivec2 {
            std::clamp(int(pos.x / cellSize.x), 0, size.x),
            std::clamp(int(pos.y / cellSize.y), 0, size.y)
        };
    }

    ivec2& size;
    vec2& cellSize;
    std::vector<TCell> cells;

    // Grid () {};
    Grid (ivec2& size, vec2& cellSize);

    ~Grid () {
        LOG("~Grid()");
    }

    void init (const ivec2& size);

    TCell* at (const ivec2& pos);

    TCell* at (const vec2& pos);

    TCell* at (const int& x, const int& y);

    vector<TCell*> atRect(Rectangle& rect);

    vector<TCell*> atRadius (vec2 pos, float radius);
};

// template <typename TCell>
// Grid<TCell>::Grid () {
//     LOG("Grid()");
// }

template <typename TCell>
Grid<TCell>::Grid (ivec2& size, vec2& cellSize)
    : size(size), cellSize(cellSize) {
    init(size);
};

template <typename TCell>
void Grid<TCell>::init (const ivec2& size) {
    LOG("Grid()", size);
    this->size = size;

    int length = size.x * size.y;
    cells.reserve(length);
    for (int y = 0; y < size.y; ++y) {
        for (int x = 0; x < size.x; ++x) {
            int idx = y * size.x + x;
            TCell cell({ivec2{x, y}, idx});
            cells.push_back(cell);
        }
    }
}

template <typename TCell>
TCell* Grid<TCell>::at (const ivec2& pos) {
    return at(pos.x, pos.y);
}

template <typename TCell>
TCell* Grid<TCell>::at (const vec2& pos) {
    return at(toCoord(cellSize, pos));
}

template <typename TCell>
TCell* Grid<TCell>::at (const int& x, const int& y) {
    int idx = y * size.x + x;
    if (x < 0 || y < 0 || x >= size.x || y >= size.y)
        return nullptr;
    return &cells.at(idx);
}

template <typename TCell>
vector<TCell*> Grid<TCell>::atRect(Rectangle& rect) {
    vector<TCell*> _cells;
    int toX = rect.x + rect.width;
    int toY = rect.y + rect.height;
    _cells.reserve((toX - rect.x) * (toY - rect.y));

    int i = 0;
    for (int x = rect.x; x < rect.x + rect.width; x++) {
        for (int y = rect.y; y < rect.y + rect.height; y++) {
            if (TCell* cell = at(x, y))
                _cells.push_back(cell);
        }
    }
    return _cells;
}

template <typename TCell>
vector<TCell*> Grid<TCell>::atRadius (vec2 pos, float radius) {
    vector<TCell*> _cells;
    Rectangle radiusRect = { pos.x - radius, pos.y - radius, radius * 2, radius * 2 };
    vector<TCell*> cellsRect = atRect(radiusRect);
    float radiusSqr = radius * radius;
    for (auto* cell : cellsRect) {
        if (Vector2DistanceSqr(pos, cell->pos) < radiusSqr)
            _cells.push_back(cell);
    }
}

inline const ivec2 KERNEL_1[4] = {
    ivec2{ 1, 0 },
    ivec2{ 0, 1 },
    ivec2{ -1, 0 },
    ivec2{ 0, -1 },
};

inline const ivec2 KERNEL_DIAGONAL_1[4] = {
    ivec2{ 1, 1 },
    ivec2{ -1, 1 },
    ivec2{ -1, -1 },
    ivec2{ 1, -1 },
};

inline const ivec2 KERNEL_ALL_1[8] = {
    ivec2{ 1, 0 },
    ivec2{ 0, 1 },
    ivec2{ -1, 0 },
    ivec2{ 0, -1 },
    ivec2{ 1, 1 },
    ivec2{ -1, 1 },
    ivec2{ -1, -1 },
    ivec2{ 1, -1 },
};

inline const vec2 KERNEL_ALL_UNIT_1[8] = {
    Vector2Normalize(KERNEL_ALL_1[0]),
    Vector2Normalize(KERNEL_ALL_1[1]),
    Vector2Normalize(KERNEL_ALL_1[2]),
    Vector2Normalize(KERNEL_ALL_1[3]),
    Vector2Normalize(KERNEL_ALL_1[4]),
    Vector2Normalize(KERNEL_ALL_1[5]),
    Vector2Normalize(KERNEL_ALL_1[6]),
    Vector2Normalize(KERNEL_ALL_1[7]),
};
