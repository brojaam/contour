#pragma once
#include "MathUtils.h"
#include <memory>

namespace ContourApp
{
    class Segment
    {
    public:
        enum class Type
        {
            Line,
            Arc
        };
        virtual ~Segment() = default;
        [[nodiscard]] virtual Type getType() const = 0;
        [[nodiscard]] virtual Point2D getStart() const = 0;
        [[nodiscard]] virtual Point2D getEnd() const = 0;
        [[nodiscard]] virtual std::unique_ptr<Segment> clone() const = 0;
    };
} // namespace ContourApp