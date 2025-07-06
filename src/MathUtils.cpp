#include "MathUtils.h"

namespace ContourApp {
bool isApproximatelyEqual(double a, double b, double epsilon) {
    return std::abs(a - b) < epsilon;
}

bool Point2D::operator==(const Point2D& other) const {
    return isApproximatelyEqual(x, other.x) && isApproximatelyEqual(y, other.y);
}
} // namespace ContourApp