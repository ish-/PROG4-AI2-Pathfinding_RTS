class Selection {
public:
    bool active;
    vec2 from;
    vec2 to;

    void start (vec2& _from) {
        reset();
        active = true;
        from = _from;
    }
    void update (vec2& _to) { to = _to; }
    void stop (vec2& _to) {
        to = _to;
        active = false;
    }
    void reset () {
        active = false;
        from = {0,0};
        to = {0,0};
    };

    void draw () {
        if (active) {
            float fromX = std::min(from.x, to.x);
            float fromY = std::min(from.y, to.y);
            float toX = std::max(from.x, to.x);
            float toY = std::max(from.y, to.y);
            DrawRectangleLinesEx({ fromX, fromY, toX - fromX, toY - fromY }, 2, WHITE);
            DrawRectangleRec({ fromX, fromY, toX - fromX, toY - fromY }, {255,255,255,150});
        }
    }
};

class BoidSelection: public Selection {
};
