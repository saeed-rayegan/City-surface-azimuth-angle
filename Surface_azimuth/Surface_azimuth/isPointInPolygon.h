#pragma once

#include "Point.h"
#include "crossProduct.h"
#include <set>
#include <unordered_set>

bool isPointInPolygon(const Point& point, const std::vector<Point>& polygon) {
    int n = polygon.size();
    
    /*for (int i = 0; i <= n - 1; i++) {
        cout<< polygon[i].x<<"  " << polygon[i].y<<endl;
    }*/

    if (n < 3) return false;

    bool isInside = false;

    bool isCounterclockwise = true;  // Assume counterclockwise by default

    // Check the orientation of the polygon
    for (int i = 0; i < n; i++) {
        int j = (i + 1) % n;
        double cross = crossProduct(polygon[i], polygon[j], point);
        if (cross < 0) {
            isCounterclockwise = false;  // The polygon is clockwise
            break;
        }
        else if (cross > 0) {
            isCounterclockwise = true;  // The polygon is counterclockwise
            break;
        }
    }

    for (int i = 0, j = n - 1; i < n; j = i++) {
        double cross = crossProduct(polygon[i], polygon[j], point);
        if (isCounterclockwise) {
            if ((polygon[i].y > point.y) != (polygon[j].y > point.y) &&
                (point.x < (polygon[j].x - polygon[i].x) * (point.y - polygon[i].y) / (polygon[j].y - polygon[i].y) + polygon[i].x)) {
                isInside = !isInside;
            }
        }
        else {
            if ((polygon[i].y < point.y) != (polygon[j].y < point.y) &&
                (point.x < (polygon[j].x - polygon[i].x) * (point.y - polygon[i].y) / (polygon[j].y - polygon[i].y) + polygon[i].x)) {
                isInside = !isInside;
            }
        }
    }

    return isInside;
}