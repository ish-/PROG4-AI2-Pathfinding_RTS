#include <raylib.h>
#include <raymath.h>
#include <stdexcept>

#include "common/math.hpp"
#include "common/log.hpp"

#include "FlowGrid.hpp"
#include "Grid.hpp"

// FlowGrid<TCell>::FlowGrid (ivec2 _size) : Grid<TCell>(_size) {};

Color CELL_HOVER_COLOR = ORANGE;

ivec2 FlowGrid::SIZE = ivec2(16, 9);
vec2 FlowGrid::CELL_SIZE = vec2(1280./16., 720./9.);

vector<FlowCell*> FlowGrid::getPath (vec2& start, vec2& dest) {
    return setFlowField(at(start), at(dest));
}

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
                // if (cell.debug) {
                //     fillColor = BLACK;
                // }
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
        static const vec2 PATH_SIZE = cellSize / 3;
        if (cell.pfPath)
            DrawRectangle(pos.x + (cellSize.x-PATH_SIZE.x)/2, pos.y + (cellSize.y-PATH_SIZE.y)/2, PATH_SIZE.x, PATH_SIZE.y, RED);
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

const int FLOW_MAX_ITERS = 10000;
vector<FlowCell*> FlowGrid::setFlowField(FlowCell* startCell, FlowCell* destCell) {
    pfRun++;
    // auto* tmp = startCell;
    // startCell = destCell;
    // destCell = tmp;
    this->startCell = startCell;
    this->destCell = destCell;
    float dist = Vector2Length(startCell->pos - destCell->pos);
    // reset();
    // LOG_TIMER timer("FlowGrid.setFlowField");
    startCell->pfDist = 0;
    queueCells.push(startCell);

    const float MAX_DIST = sqrt(size.x*size.x + size.y*size.y);
    const float MAX_DIST_SQR = size.x*size.x + size.y*size.y;

    // if (repeat) {
    //     LOG("R", startCell->pos, destCell->pos);
    // } else {
    //     LOG("-R", startCell->pos, destCell->pos);
    // }

    int maxIters = FLOW_MAX_ITERS;
    while (!queueCells.empty() && (maxIters--) > 0) {
        FlowCell* curCell = queueCells.top();
        curCell->pfRun = pfRun;
        // FlowCell* curCell = queueCells.front();
        queueCells.pop();
        passedCells.insert(curCell);

        if (curCell->pfPath) {
            LOG("Found PATH", curCell->pos);
            clearQueue();
            return setPath(startCell, destCell, curCell);
        }

        for (int i = 0; i < 8; i++) {
            const ivec2& offset = KERNEL_ALL_1[i];

            FlowCell* cell = at(curCell->pos + offset);
            if (cell == nullptr)
                continue;
            // bool isPassedCell = passedCells.contains(cell);
            // if (isPassedCell)
            //     continue;

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
            if (cell->pfRun == pfRun && cell->pfDist <= nextDist)
                continue;

            // if (cell->pfPath) {
            //     cell->pfToStartCell = curCell;
            //     LOG("Found PATH", cell->pos, curCell->pos);
            //     clearQueue();
            //     setPath(startCell, destCell);
            //     return;
            // }

            cell->pfRun = pfRun;
            cell->pfDist = nextDist;
            // if (cell != startCell)
                cell->pfToStartCell = curCell;
            cell->pfToStart = offset;
            vec2 toDest = destCell->pos - cell->pos;
            float distToDest = Vector2Length(toDest);
            // float toLenSqr = Vector2LengthSqr(toDest);
            cell->pfDirWeight = Vector2DotProduct(
                KERNEL_ALL_UNIT_1[i],
                // Vector2Normalize(toDest)
                toDest / distToDest
            ) + (1. - distToDest / dist) * 2.;

            // if (repeat && isPassedCell) {
            //     setPath(startCell, destCell);
            //     clearQueue();
            //     return;
            // }

            // cell->pfDirWeight = 1. - toLenSqr / MAX_DIST_SQR;

            if (cell == destCell/* && !path.size()*/) {
                LOG("Found DESTination", startCell->pos, destCell->pos, FLOW_MAX_ITERS - maxIters);
                clearQueue();
                return setPath(startCell, destCell);
            } else
                queueCells.push(cell);
        }
    }
    LOG("End of while", startCell->pos, destCell->pos, FLOW_MAX_ITERS - maxIters);
}

vector<FlowCell*> FlowGrid::setPath(FlowCell* startCell, FlowCell* destCell, FlowCell* usedCell) {
    vector<FlowCell*> path; {
        vec2 toDest = startCell->pos - destCell->pos;
        vec2 toDestAbs = abs(toDest);
        path.reserve(Vector2Distance(startCell->pos, destCell->pos) + abs(toDest.x - toDest.y) * 1.5);
    }

    if (usedCell) {
        int i = 0;
        for(FlowCell* cell = usedCell; cell != destCell && i < 100; i++) {
            if (!cell) {
                LOG("setPath() used!", usedCell->pos);
                break;
            }
            cell = cell->pfToDestCell;
            path.push_back(cell);
        }
        destCell = usedCell;
    }

    int i = 0;
    for(FlowCell* cell = destCell; cell != startCell && i < 100; i++) {
        if (cell == nullptr) {
            LOG(CRED("setPath() cell is empty"), i, startCell->pos, destCell->pos);
            break;
        }
        FlowCell* prevCell = cell;
        cell = cell->pfToStartCell;

        if (cell == nullptr) {
            LOG(CRED("pfToStartCell is empty"), i);

            // print bad path
            int i = 0;
            for(FlowCell* cell = destCell; cell != startCell && i < 100; i++) {
                LOG("---", cell->pos);
                cell->debug = true;
                if (!cell || !cell->pfToStartCell)
                    break;
                cell = cell->pfToStartCell;
            }

            LOG(CRED("------"));
            break;
        }
        cell->pfPath = true;
        cell->pfToDestCell = prevCell;
        path.push_back(cell);
    }
    startCell->pfPath = true;
    path.push_back(startCell);
    return path;
}

vec2 FlowGrid::getDir (vec2& pos, bool repeat) {
    // vec2 gridCoord = pos / cellSize;
    // FlowCell* cell = at(gridCoord.x, gridCoord.y);
    if (FlowCell* cell = at(pos)) {
        if (isZero(cell->pfToStart)) {
            // if (repeat)
                return {0,0};
            // setFlowField(startCell, cell, true);
            // return getDir(pos, true);
        }
        return cell->pfToStart;
    }
    else
        return {0, 0};
}


void FlowGrid::reset () {
    // LOG_TIMER timer("FlowGrid::reset");
    // path.erase(path.begin(), path.end());
    for (auto& cell : cells) {
        cell.pfDist = 9999;
        cell.pfToStart = Vector2{0,0};
        cell.pfToStartCell = nullptr;
        cell.pfPath = false;
    }

    passedCells.erase(passedCells.begin(), passedCells.end());
    clearQueue();
}

void FlowGrid::clearQueue () {
    passedCells.erase(passedCells.begin(), passedCells.end());
    while (!queueCells.empty()) queueCells.pop();
}
