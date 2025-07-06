#pragma once
#include "Segment.h"

namespace ContourApp
{
    class ArcSegment : public Segment
    {
    public:
        ArcSegment(Point2D start, Point2D end, Point2D center, double radius, bool clockwise);
        [[nodiscard]] Type getType() const override;
        [[nodiscard]] Point2D getStart() const override;
        [[nodiscard]] Point2D getEnd() const override;
        [[nodiscard]] std::unique_ptr<Segment> clone() const override;

    private:
        Point2D start_;
        Point2D end_;
        Point2D center_;
        double radius_;
        bool clockwise_;
    };
} // namespace ContourApp