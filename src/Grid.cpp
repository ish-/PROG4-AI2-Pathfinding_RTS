#include <stdexcept>
#include <iostream>
#include <array>
#include <queue>
#include <unordered_set>
#include <sstream>

#include "raylib.h"
#include "raymath.h"

#include "common/log.cpp"
#include "common/math.h"

// #include "Cell.h"
#include "Grid.h"

Grid::Grid (Rectangle _rect, Vector2 _size) {
    rect = _rect; size = _size;
    float dx = rect.width / size.x;
    float dy = rect.height / size.y;
    
    cells.reserve(size.x * size.y);
    for (int y = 0; y < size.y; ++y) {
        for (int x = 0; x < size.x; ++x) {
            Vector2 cellPos {rect.x + (dx * x), rect.y + (dy * y)};
            Vector2 cellSize { dx, dy };
            Cell* cell = new Cell({(float)x, (float)y}, cellPos, cellSize);
            cells.push_back(cell);
        }
    }
};

Cell* Grid::at (Vector2 coord) const {
    return at((int)coord.x, (int)coord.y);
}

Cell* Grid::at (int x, int y) const {
    int idx = y * size.x + x;
    // idx = Clamp((float)idx, 0.f, size.x * size.y - 1.f);
    // if (idx < 0 || idx >= cells.size())
    if (x < 0 || y < 0 || x >= size.x || y >= size.y)
        return nullptr;

    // cout << "x: " << x << "; y: " << y << "; idx=" << idx << "\n";
    return cells.at(idx);
};

void Grid::draw() const {
    for (const Cell* cell : cells) {
        cell->draw();
    }
};

Grid::~Grid() {
    cout << "Grid destroyed\n";
    for (const Cell* cell : cells) {
        delete cell;
    }
    cells.clear();
}

std::array<Vector2, 4> KERNEL = {
    Vector2{ 1, 0 },
    Vector2{ 0, 1 },
    Vector2{ -1, 0 },
    Vector2{ 0, -1 },
};

std::array<Vector2, 4> KERNEL_DIAGONAL = {
    Vector2{ 1, 1 },
    Vector2{ -1, 1 },
    Vector2{ -1, -1 },
    Vector2{ 1, -1 }
};

void Grid::forKernel(Vector2& coord, const std::function<void(Cell* cell, const Vector2& offset)>& func) const {
    // std::array<Cell*, 4> arr;
    // LOG("forKernel: ", coord);
    for (const Vector2& offset : KERNEL) {
        Cell* cell = at(Vector2Add(coord, offset));
        // if (cell)
        //     LOG(":: Kernel Cell: ", cell->coord);
        if (cell == nullptr) {
            // LOG("forKernel", "cell == nullptr");
            continue;
        }
        func(cell, offset);
        // arr[i] = cell;
    }
    // return arr;
}



std::unordered_set<Cell*> passedCells;
std::queue<Cell*> queueCells;
void Grid::setFlowField(Cell* fromCell) const {
    passedCells.erase(passedCells.begin(), passedCells.end());
    while (!queueCells.empty()) queueCells.pop();
    fromCell->pfDist = 0;
    queueCells.push(fromCell);

    int i = 0;
    while (!queueCells.empty() && (i++) < 1000) {
        Cell* curCell = queueCells.front();
        queueCells.pop();
        passedCells.insert(curCell);
        
        // LOG("curCell:", curCell->coord);

        forKernel(curCell->coord, [&curCell](Cell* cell, const Vector2& offset) {
            // if (!cell || passedCells.find(cell) != passedCells.end())
            if (!cell || passedCells.contains(cell))
                return;

            // LOG("kernelCell:", cell->coord);

            cell->pfDist = curCell->pfDist + 1;
            cell->pfToStart = Vector2Multiply(offset, {-1.f, -1.f});
            queueCells.push(cell);
        });
    }
}
