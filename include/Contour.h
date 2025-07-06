#pragma once
#include "LineSegment.h"
#include "ArcSegment.h"
#include <vector>
#include <optional>
#include <stdexcept>

namespace ContourApp
{
    /**
     * @brief Represents a 2D contour composed of segments.
     */
    class Contour
    {
    public:
        using SegmentPtr = std::unique_ptr<Segment>;
        Contour();
        Contour(const Contour &other);
        Contour(Contour &&other) noexcept = default;
        Contour &operator=(Contour other);
        ~Contour() = default;
        void swap(Contour &other) noexcept;
        void addSegment(SegmentPtr segment);
        void insertSegment(size_t index, SegmentPtr segment);
        void removeSegment(size_t index);
        using Iterator = std::vector<SegmentPtr>::iterator;
        using ConstIterator = std::vector<SegmentPtr>::const_iterator;
        [[nodiscard]] Iterator begin();
        [[nodiscard]] Iterator end();
        [[nodiscard]] ConstIterator begin() const;
        [[nodiscard]] ConstIterator end() const;
        [[nodiscard]] size_t size() const;
        [[nodiscard]] bool empty() const;
        [[nodiscard]] bool isValid() const;
        [[nodiscard]] static Contour createFromPolyline(const std::vector<Point2D> &points);

    private:
        std::vector<SegmentPtr> segments_;
        mutable std::optional<bool> validityCache_;
        void invalidateCache() const;
        [[nodiscard]] bool checkValidity() const;
    };
} // namespace ContourApp