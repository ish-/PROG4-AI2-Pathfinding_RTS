#pragma once

#include <vector>

#include "common/math.h"
#include "common/log.cpp"

// ivec2 KERNEL_1[4];
// ivec2 DIAGONAL_KERNEL_1[4];

struct GridCell {
    ivec2 pos = {0,0};
    GridCell (int x, int y) : pos({x, y}) {};
    GridCell () = default;
};

template <typename TCell = GridCell>
class Grid {
public:
    ivec2 size;
    std::vector<TCell> cells;

    Grid (ivec2 size);

    TCell* at (ivec2 pos);

    TCell* at (int x, int y);
};

template <typename TCell>
Grid<TCell>::Grid (ivec2 _size) {
    size = _size;

    int length = size.x * size.y;
    cells.reserve(length);

    for (int y = 0; y < size.y; ++y) {
        for (int x = 0; x < size.x; ++x) {
            // TCell cell = TCell(x, y);
            TCell cell {{x, y}};
            LOG("Grid::Grid", cell.pos);
            cells.push_back(cell);
        }
    }
};

template <typename TCell>
TCell* Grid<TCell>::at (ivec2 pos) {
    return at(pos.x, pos.y);
}

template <typename TCell>
TCell* Grid<TCell>::at (int x, int y) {
    int idx = y * size.x + x;
    if (x < 0 || y < 0 || x >= size.x || y >= size.y)
        return nullptr;
    return &cells.at(idx);
}
