#include "Contour.h"
#include <algorithm>
#include <utility>

namespace ContourApp
{
    Contour::Contour() : segments_(), validityCache_(std::nullopt) {}

    Contour::Contour(const Contour &other)
    {
        segments_.reserve(other.segments_.size());
        for (const auto &seg : other.segments_)
        {
            segments_.push_back(seg->clone());
        }
    }

    Contour &Contour::operator=(Contour other)
    {
        swap(other);
        return *this;
    }

    void Contour::swap(Contour &other) noexcept
    {
        segments_.swap(other.segments_);
        validityCache_.swap(other.validityCache_);
    }

    void Contour::addSegment(SegmentPtr segment)
    {
        if (!segment)
        {
            throw std::invalid_argument("Segment cannot be null");
        }
        segments_.push_back(std::move(segment));
        invalidateCache();
    }

    void Contour::insertSegment(size_t index, SegmentPtr segment)
    {
        if (!segment)
        {
            throw std::invalid_argument("Segment cannot be null");
        }
        if (index > segments_.size())
        {
            throw std::out_of_range("Index out of range");
        }
        segments_.insert(segments_.begin() + index, std::move(segment));
        invalidateCache();
    }

    void Contour::removeSegment(size_t index)
    {
        if (index >= segments_.size())
        {
            throw std::out_of_range("Index out of range");
        }
        segments_.erase(segments_.begin() + index);
        invalidateCache();
    }

    Contour::Iterator Contour::begin()
    {
        return segments_.begin();
    }

    Contour::Iterator Contour::end()
    {
        return segments_.end();
    }

    Contour::ConstIterator Contour::begin() const
    {
        return segments_.begin();
    }

    Contour::ConstIterator Contour::end() const
    {
        return segments_.end();
    }

    size_t Contour::size() const
    {
        return segments_.size();
    }

    bool Contour::empty() const
    {
        return segments_.empty();
    }

    bool Contour::isValid() const
    {
        if (validityCache_.has_value())
        {
            return validityCache_.value();
        }
        validityCache_ = checkValidity();
        return validityCache_.value();
    }

    bool Contour::checkValidity() const
    {
        if (segments_.empty())
        {
            return true;
        }
        if (segments_.size() == 1)
        {
            return true;
        }
        for (size_t i = 0; i < segments_.size() - 1; ++i)
        {
            const auto &current = segments_[i];
            const auto &next = segments_[i + 1];
            if (!(current->getEnd() == next->getStart()))
            {
                return false;
            }
        }
        return true;
    }

    Contour Contour::createFromPolyline(const std::vector<Point2D> &points)
    {
        if (points.size() < 2)
        {
            throw std::invalid_argument("Polyline requires at least 2 points");
        }
        Contour contour;
        for (size_t i = 0; i < points.size() - 1; ++i)
        {
            contour.addSegment(std::make_unique<LineSegment>(points[i], points[i + 1]));
        }
        return contour;
    }

    void Contour::invalidateCache() const
    {
        validityCache_.reset();
    }
} // namespace ContourApp