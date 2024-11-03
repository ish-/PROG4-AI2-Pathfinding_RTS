#include <raylib.h>
#include <raymath.h>

#include "common/math.hpp"

#include "FlowGrid.h"
#include "Grid.h"

// FlowGrid<TCell>::FlowGrid (ivec2 _size) : Grid<TCell>(_size) {};

Color CELL_HOVER_COLOR = ORANGE;

void FlowGrid::draw (const Rectangle& rect, FlowCell* hoverCell) {
    Vector2 cellSize = { rect.width / this->size.x, rect.height / this->size.y };

    for (const FlowCell& cell : this->cells) {
        Color fillColor = WHITE;
        Color borderColor = BLUE;
        if (&cell == hoverCell) {
            fillColor = CELL_HOVER_COLOR;
        } else if (cell.obstacle) {
            fillColor = BLACK;
        } else {
            // float lightness = (1. - std::max(0., 20. - cell.pfDist) / 20.) * 255;
            // unsigned char uChLightness = (unsigned char)lightness;
            // fillColor = { uChLightness, uChLightness, uChLightness, 255 };
            
            if (cell.pfDist < 1000) {
                float angle = atan2(cell.pfToStart.y, cell.pfToStart.x) * (180.0f / PI);
                fillColor = ColorFromHSV(angle, .5, 1);
                
            //     float r = mapRange(cell.pfToStart.x, -1, 1, 0, 255);
            //     float g = mapRange(cell.pfToStart.y, -1, 1, 0, 255);
            //     fillColor = Color({(unsigned char)r, (unsigned char)g, 0, 255});
            }
        }

        vec2 pos {cell.pos.x * cellSize.x, cell.pos.y * cellSize.y};
        DrawRectangle(pos.x, pos.y, cellSize.x, cellSize.y, fillColor);
        // DrawRectangleLines(pos.x, pos.y, cellSize.x, cellSize.y, borderColor);
        // DrawText(TextFormat("%.1f,%.1f", cell.pfToStart.x, cell.pfToStart.y), pos.x + 2, pos.y + 2, 11, BLACK);
        if (cell.pfPath)
            DrawRectangle(pos.x + cellSize.x/2, pos.y + cellSize.y/2, cellSize.x/2, cellSize.y/2, RED);
        if (cell.corner)
            DrawRectangle(pos.x, pos.y + cellSize.y/2, cellSize.x/2, cellSize.y/2, GREEN);
    }
};

void FlowGrid::setFlowField(FlowCell* fromCell, FlowCell* toCell) {
    reset();
    fromCell->pfDist = 0;
    queueCells.push(fromCell);

    while (!queueCells.empty() /* && (i++) < 1000 */) {
        FlowCell* curCell = queueCells.front();
        queueCells.pop();
        passedCells.insert(curCell);
        
        // LOG("curCell:", curCell->coord);
        float nextDist = curCell->pfDist + 1;

        int i = -1;
        // LOG("curCell->pos:", curCell->pos);
        for (const ivec2& offset : KERNEL_ALL_1) {
            i++;
            FlowCell* cell = at(curCell->pos + offset);
            if (cell == nullptr || passedCells.contains(cell))
                continue;
            if (cell->obstacle)
                continue;
            if (i > 3) { // diagonal 
                // LOG("i:", i, curCell->pos);
                if (at(curCell->pos + ivec2(offset.x, 0))->obstacle && at(curCell->pos + ivec2(0, offset.y))->obstacle) {
                    curCell->corner = true;
                    cell->corner = true;
                    continue;
                }
            } 
            if (nextDist >= cell->pfDist)
                continue;

            cell->pfDist = nextDist;
            cell->pfFromCell = curCell;
            cell->pfToStart = offset * 1.;
            if (cell == toCell) {
                setPath(fromCell, toCell);
                // return;
            } else
                queueCells.push(cell);
        }
    }
}

void FlowGrid::setPath(FlowCell* fromCell, FlowCell* toCell) {
    for(FlowCell* cell = toCell; cell != fromCell; cell = cell->pfFromCell) {
        cell->pfPath = true;
        path.push_back(cell);
    }
    fromCell->pfPath = true;
    path.push_back(fromCell);
}

void FlowGrid::reset () {
    path.erase(path.begin(), path.end());
    for (auto& cell : cells) {
        cell.pfDist = 9999;
        cell.pfToStart = Vector2{0,0};
        cell.pfFromCell = nullptr;
        cell.pfPath = false;
    }

    passedCells.erase(passedCells.begin(), passedCells.end());
    while (!queueCells.empty()) queueCells.pop();
}