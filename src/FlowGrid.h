#pragma once

#include <raylib.h>
#include <raymath.h>

#include "Grid.h"
#include "common/math.h"

// using vec2 = Vector2;

struct FlowCell : GridCell {
    float pfDist = 0.f;
    Vector2 pfToStart = Vector2{0,0};
};

class FlowGrid : public Grid<FlowCell> {
public:
    FlowGrid (ivec2 size) : Grid(size) {};

    void draw (const Rectangle& rect,  FlowCell* hoverCell);
};

// void Grid::forKernel(Vector2& coord, const std::function<void(Cell* cell, const Vector2& offset)>& func) const {
//     // std::array<Cell*, 4> arr;
//     // LOG("forKernel: ", coord);
//     for (const Vector2& offset : KERNEL) {
//         Cell* cell = at(Vector2Add(coord, offset));
//         // if (cell)
//         //     LOG(":: Kernel Cell: ", cell->coord);
//         if (cell == nullptr) {
//             // LOG("forKernel", "cell == nullptr");
//             continue;
//         }
//         func(cell, offset);
//         // arr[i] = cell;
//     }
//     // return arr;
// }



// std::unordered_set<Cell*> passedCells;
// std::queue<Cell*> queueCells;
// void Grid::setFlowField(Cell* fromCell) const {
//     passedCells.erase(passedCells.begin(), passedCells.end());
//     while (!queueCells.empty()) queueCells.pop();
//     fromCell->pfDist = 0;
//     queueCells.push(fromCell);

//     int i = 0;
//     while (!queueCells.empty()/* && (i++) < 1000*/) {
//         Cell* curCell = queueCells.front();
//         queueCells.pop();
//         passedCells.insert(curCell);
        
//         // LOG("curCell:", curCell->coord);

//         forKernel(curCell->coord, [&curCell](Cell* cell, const Vector2& offset) {
//             // if (!cell || passedCells.find(cell) != passedCells.end())
//             if (!cell || passedCells.contains(cell))
//                 return;

//             // LOG("kernelCell:", cell->coord);

//             cell->pfDist = curCell->pfDist + 1;
//             cell->pfToStart = Vector2Multiply(offset, {-1.f, -1.f});
//             queueCells.push(cell);
//         });
//     }
// }
