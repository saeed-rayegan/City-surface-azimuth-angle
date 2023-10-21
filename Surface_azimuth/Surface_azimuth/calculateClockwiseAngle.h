#pragma once

#include <cmath>

double calculateClockwiseAngle(double x, double y) {
    // Calculate the angle in radians between the given vector and the reference vector (0, -1)
    double angleRadians = atan2(x, -y);

    // Convert the angle to degrees
    double angleDegrees = angleRadians * (180.0 / PI);

    // Ensure the result is positive and in the range [0, 360] degrees
    if (angleDegrees < 0) {
        angleDegrees += 360.0;
    }

    return angleDegrees;
}