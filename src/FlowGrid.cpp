#include <raylib.h>
#include <raymath.h>

#include "common/math.hpp"
#include "common/log.hpp"

#include "FlowGrid.hpp"
#include "Grid.hpp"

// FlowGrid<TCell>::FlowGrid (ivec2 _size) : Grid<TCell>(_size) {};

Color CELL_HOVER_COLOR = ORANGE;

ivec2 FlowGrid::SIZE = ivec2(16, 9);
vec2 FlowGrid::CELL_SIZE = vec2(1280./16., 720./9.);

void FlowGrid::draw (const Rectangle& rect) {
    Vector2 cellSize = { rect.width / this->size.x, rect.height / this->size.y };

    for (const FlowCell& cell : this->cells) {
        Color fillColor = WHITE;
        Color borderColor = BLUE;
        /*if (&cell == hoverCell) {
            fillColor = CELL_HOVER_COLOR;
        } else*/ if (cell.obstacle) {
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
        fillColor.a = 100;
        DrawRectangle(pos.x, pos.y, cellSize.x, cellSize.y, fillColor);
        // DrawRectangleLines(pos.x, pos.y, cellSize.x, cellSize.y, borderColor);
        // DrawText(TextFormat("%.1f,%.1f", cell.pfToStart.x, cell.pfToStart.y), pos.x + 2, pos.y + 2, 11, BLACK);
        if (cell.pfPath)
            DrawRectangle(pos.x + cellSize.x/2, pos.y + cellSize.y/2, cellSize.x/2, cellSize.y/2, RED);
        if (cell.corner)
            DrawRectangle(pos.x, pos.y + cellSize.y/2, cellSize.x/2, cellSize.y/2, GREEN);
    }
};

// Set the weights of the compass cells by dot product to the `destCell` direction
// void FlowGrid::setCompassWeights (FlowCell* startCell, FlowCell* destCell) {
//     vec2 dir = destCell->pos - startCell->pos;
//     for (CompassCell* compassCell : compass) {
//         compassCell->dirWeight = Vector2DotProduct(compassCell->pos, dir);
//     }
// }

void FlowGrid::setFlowField(FlowCell* startCell, bool repeat) {
    // reset();
    // LOG_TIMER timer("FlowGrid.setFlowField");
    startCell->pfDist = 0;
    queueCells.push(startCell);

    const float MAX_DIST_SQR = size.x*size.x + size.y*size.y;

    int maxIters = 100000;
    while (!queueCells.empty() && (maxIters--) > 0) {
        FlowCell* curCell = queueCells.top();
        // FlowCell* curCell = queueCells.front();
        queueCells.pop();
        passedCells.insert(curCell);

        // LOG("curCell:", curCell->coord);

        // LOG("curCell->pos:", curCell->pos);
        for (int i = 0; i < 8; i++) {
            const ivec2& offset = KERNEL_ALL_1[i];

            FlowCell* cell = at(curCell->pos + offset);
            if (cell == nullptr)
                continue;
            if (passedCells.contains(cell)) {
                if (repeat) {
                    return;
                } else {
                    continue;
                }
            }
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
            float nextDist = curCell->pfDist + (i > 3 ? 1.4142135623730951 : 1);
            if (nextDist >= cell->pfDist)
                continue;

            cell->pfDist = nextDist;
            cell->pfFromCell = curCell;
            cell->pfToStart = offset;
            vec2 toDest = destCell->pos - cell->pos;
            // float toLenSqr = Vector2LengthSqr(toDest);
            cell->pfDirWeight = Vector2DotProduct(
                Vector2Normalize(KERNEL_ALL_UNIT_1[i]),
                Vector2Normalize(toDest)
            );
            // cell->pfDirWeight = 1. - toLenSqr / MAX_DIST_SQR;

            if (cell == destCell && !path.size()) {
                setPath(startCell);
                return;
            } else
                queueCells.push(cell);
        }
    }
}

void FlowGrid::setPath(FlowCell* startCell) {
    for(FlowCell* cell = destCell; cell != startCell; cell = cell->pfFromCell) {
        cell->pfPath = true;
        path.push_back(cell);
    }
    startCell->pfPath = true;
    path.push_back(startCell);
}

void FlowGrid::reset () {
    // LOG_TIMER timer("FlowGrid::reset");
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

vec2 FlowGrid::getDir (vec2& pos, bool repeat) {
    vec2 gridCoord = pos / cellSize;
    FlowCell* cell = at(gridCoord.x, gridCoord.y);
    if (!cell)
        return {0,0};
    if (isZero(cell->pfToStart)) {
        // if (repeat)
            return {0,0};
        // setFlowField(at(pos));
        // return getDir(pos, true);
    }
    return cell->pfToStart;
}

// void FlowGrid::setObstacles (vector<Obstacle*>& obstacles) {
//     for (Obstacle* obstacle : obstacles) {
//         Rectangle gridObstacleRect = obstacle->rect / cellSize;
//         for (Cell* cell : atRect(gridObstacleRect)) {
//             cell->obstacle = true;
//         }
//     }
// }