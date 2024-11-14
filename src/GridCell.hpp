#pragma once
#include "common/math.hpp"

struct GridCell {
    ivec2 pos = {-1,-1};
    int idx = -1;
    GridCell (ivec2 pos, int idx) : pos(pos), idx(idx) {};
    GridCell () = default;
};
