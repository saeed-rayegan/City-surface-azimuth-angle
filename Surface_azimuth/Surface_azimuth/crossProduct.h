#pragma once

#include "Point.h"
// Function to calculate the cross product of two vectors
double crossProduct(const Point& A, const Point& B, const Point& C) {
    return (B.x - A.x) * (C.y - A.y) - (B.y - A.y) * (C.x - A.x);
}
