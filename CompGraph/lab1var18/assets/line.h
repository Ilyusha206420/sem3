#pragma once
#include "point.h"

struct Line {
    Point start, end;
    int r, g, b;
    
    Line() : start(0,0), end(0,0), r(0), g(0), b(0) {}
    
    Line(Point s, Point e, int r, int g, int b) : start(s), end(e), r(r), g(g), b(b) {}
};