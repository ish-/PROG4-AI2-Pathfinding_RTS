#pragma once

#include <vector>

#include "common/math.hpp"
#include "common/log.hpp"

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

    ivec2 size;
    std::vector<TCell> cells;

    Grid ();
    Grid (const ivec2& size);

    ~Grid () {
        LOG("~Grid()");
    }

    void init (const ivec2& size);

    TCell* at (const ivec2& pos);

    TCell* at (const int& x, const int& y);
};

template <typename TCell>
Grid<TCell>::Grid () {
    LOG("Grid()");
}

template <typename TCell>
Grid<TCell>::Grid (const ivec2& size) {
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
TCell* Grid<TCell>::at (const int& x, const int& y) {
    int idx = y * size.x + x;
    if (x < 0 || y < 0 || x >= size.x || y >= size.y)
        return nullptr;
    return &cells.at(idx);
}

inline ivec2 KERNEL_1[4] = {
    ivec2{ 1, 0 },
    ivec2{ 0, 1 },
    ivec2{ -1, 0 },
    ivec2{ 0, -1 },
};

inline ivec2 KERNEL_DIAGONAL_1[4] = {
    ivec2{ 1, 1 },
    ivec2{ -1, 1 },
    ivec2{ -1, -1 },
    ivec2{ 1, -1 },
};

inline ivec2 KERNEL_ALL_1[8] = {
    ivec2{ 1, 0 },
    ivec2{ 0, 1 },
    ivec2{ -1, 0 },
    ivec2{ 0, -1 },
    ivec2{ 1, 1 },
    ivec2{ -1, 1 },
    ivec2{ -1, -1 },
    ivec2{ 1, -1 },
};
