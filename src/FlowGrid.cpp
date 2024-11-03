#include <raylib.h>
#include <raymath.h>

#include "FlowGrid.h"
#include "Grid.h"

// FlowGrid<TCell>::FlowGrid (ivec2 _size) : Grid<TCell>(_size) {};

Color CELL_HOVER_COLOR = ORANGE;

void FlowGrid::draw (const Rectangle& rect, FlowCell* hoverCell) {
    Vector2 cellSize = { rect.width / this->size.x, rect.height / this->size.y };

    for (const FlowCell& cell : this->cells) {
        Color color = WHITE;
        if (&cell == hoverCell) {
            color = CELL_HOVER_COLOR;
        } else {
            float lightness = (std::max(0., 20. - cell.pfDist) / 20.) * 255;
            unsigned char uChlightness = (unsigned char)lightness;
            color = { uChlightness, uChlightness, uChlightness, 255 };
        }

        DrawRectangle(cell.pos.x * cellSize.x, cell.pos.y * cellSize.y, cellSize.x, cellSize.y, color);
        DrawRectangleLines(cell.pos.x * cellSize.x, cell.pos.y * cellSize.y, cellSize.x, cellSize.y, BLUE);
    }
};
