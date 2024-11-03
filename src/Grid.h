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
    ivec2 size;
    std::vector<TCell> cells;

    Grid (const ivec2& size);

    void init ();

    TCell* at (const ivec2& pos);

    TCell* at (const int& x, const int& y);
};

template <typename TCell>
Grid<TCell>::Grid (const ivec2& size): size(size) {
    int length = size.x * size.y;
    cells.reserve(length);
    for (int y = 0; y < size.y; ++y) {
        for (int x = 0; x < size.x; ++x) {
            int idx = y * size.x + x;
            TCell cell({ivec2{x, y}, idx});
            cells.push_back(cell);
        }
    }
};

template <typename TCell>
void Grid<TCell>::init () {
    // cells.erase(cells.begin(), cells.end());

    
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
