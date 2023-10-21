#pragma once

#include <cmath>
#include <tuple>

std::tuple<double, double, double, double> calculatePerpendicularPoints(double x1, double y1, double x2, double y2, double distance) {
    // Calculate the midpoint of the line segment
    double midpoint_x = (x1 + x2) / 2;
    double midpoint_y = (y1 + y2) / 2;

    // Calculate the direction vector of the line segment
    double dx = x2 - x1;
    double dy = y2 - y1;

    // Calculate the length of the line segment
    double length = sqrt(dx * dx + dy * dy);

    // Calculate the unit direction vector
    double unit_dx = dx / length;
    double unit_dy = dy / length;

    // Calculate the coordinates of the two points
    double xLeft = midpoint_x - distance * unit_dy;
    double yLeft = midpoint_y + distance * unit_dx;
    double xRight = midpoint_x + distance * unit_dy;
    double yRight = midpoint_y - distance * unit_dx;

    return std::make_tuple(xLeft, yLeft, xRight, yRight);
}