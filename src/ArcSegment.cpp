#include "ArcSegment.h"
#include <stdexcept>

namespace ContourApp
{
    ArcSegment::ArcSegment(Point2D start, Point2D end, Point2D center, double radius, bool clockwise)
        : start_(start), end_(end), center_(center), radius_(radius), clockwise_(clockwise)
    {
        if (radius <= 0)
        {
            throw std::invalid_argument("Radius must be positive");
        }
        double distStart = std::hypot(start.x - center.x, start.y - center.y);
        double distEnd = std::hypot(end.x - center.x, end.y - center.y);
        if (!isApproximatelyEqual(distStart, radius) || !isApproximatelyEqual(distEnd, radius))
        {
            throw std::invalid_argument("Start and end points must lie on the circle");
        }
    }

    Segment::Type ArcSegment::getType() const
    {
        return Type::Arc;
    }

    Point2D ArcSegment::getStart() const
    {
        return start_;
    }

    Point2D ArcSegment::getEnd() const
    {
        return end_;
    }

    std::unique_ptr<Segment> ArcSegment::clone() const
    {
        return std::make_unique<ArcSegment>(*this);
    }
} // namespace ContourApp