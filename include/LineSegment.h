#pragma once
#include "Segment.h"

namespace ContourApp
{
    class LineSegment : public Segment
    {
    public:
        LineSegment(Point2D start, Point2D end);
        [[nodiscard]] Type getType() const override;
        [[nodiscard]] Point2D getStart() const override;
        [[nodiscard]] Point2D getEnd() const override;
        [[nodiscard]] std::unique_ptr<Segment> clone() const override;

    private:
        Point2D start_;
        Point2D end_;
    };
} // namespace ContourApp