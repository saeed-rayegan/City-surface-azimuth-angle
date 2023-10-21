// Point.h// This is to define various instances of a 2D point in every part of the code
#ifndef POINT_H
#define POINT_H

// Define your custom Point type
struct Point {
    double x;
    double y;

    // Define custom comparison operators (if needed)
    bool operator==(const Point& other) const {
        return (x == other.x) && (y == other.y);
    }
};

// Define the hash function specialization for Point
namespace std {
    template <>
    struct hash<Point> {
        size_t operator()(const Point& p) const {
            // Define a hash function for the Point type
            // You can use a combination of the hashes of its components (x and y)
            return std::hash<double>()(p.x) ^ std::hash<double>()(p.y);
        }
    };
}


#endif