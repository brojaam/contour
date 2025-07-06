#pragma once
#include <cmath>

namespace ContourApp
{
    /**
     * @brief Represents a 2D point with x and y coordinates.
     */
    struct Point2D
    {
        double x;
        double y;
        Point2D(double x_, double y_) : x(x_), y(y_) {}
        [[nodiscard]] bool operator==(const Point2D &other) const;
        [[nodiscard]] bool operator!=(const Point2D &other) const { return !(*this == other); }
    };

    /**
     * @brief Checks if two doubles are approximately equal within an epsilon.
     */
    [[nodiscard]] bool isApproximatelyEqual(double a, double b, double epsilon = 1e-6);
} // namespace ContourApp