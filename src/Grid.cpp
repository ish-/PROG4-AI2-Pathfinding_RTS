#include "Grid.h"

ivec2 KERNEL_1[] = {
    ivec2{ 1, 0 },
    ivec2{ 0, 1 },
    ivec2{ -1, 0 },
    ivec2{ 0, -1 },
};

ivec2 DIAGONAL_KERNEL_1[] = {
    ivec2{ 1, 1 },
    ivec2{ -1, 1 },
    ivec2{ -1, -1 },
    ivec2{ 1, -1 },
};

// template <typename TCell>
// Grid<TCell>::Grid (ivec2 _size) {
//     size = _size;

//     int length = size.x * size.y;
//     cells.reserve(length);

//     for (int y = 0; y < size.y; ++y) {
//         for (int x = 0; x < size.x; ++x) {
//             GridCell cell {{x, y}};
//             cells.push_back(cell);
//         }
//     }
// };

// template <typename TCell>
// TCell* Grid<TCell>::at (ivec2 pos) {
//     return at(pos.x, pos.y);
// }

// template <typename TCell>
// TCell* Grid<TCell>::at (int x, int y) {
//     int idx = y * size.x + x;
//     if (x < 0 || y < 0 || x >= size.x || y >= size.y)
//         return nullptr;
//     return &cells.at(idx);
// }