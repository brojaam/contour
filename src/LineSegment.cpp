#include "LineSegment.h"

namespace ContourApp
{
    LineSegment::LineSegment(Point2D start, Point2D end) : start_(start), end_(end) {}

    Segment::Type LineSegment::getType() const
    {
        return Type::Line;
    }

    Point2D LineSegment::getStart() const
    {
        return start_;
    }

    Point2D LineSegment::getEnd() const
    {
        return end_;
    }

    std::unique_ptr<Segment> LineSegment::clone() const
    {
        return std::make_unique<LineSegment>(*this);
    }
} // namespace ContourApp