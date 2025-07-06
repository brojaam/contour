#include "Contour.h"
#include <future>
#include <vector>
#include <algorithm>
#include <iostream>

namespace ContourApp
{
    std::vector<const Contour *> findValidContours(const std::vector<Contour> &contours)
    {
        std::vector<const Contour *> valid;
        valid.reserve(contours.size());
        for (const auto &contour : contours)
        {
            if (contour.isValid())
            {
                valid.push_back(&contour);
            }
        }
        return valid;
    }

    std::vector<const Contour *> findInvalidContours(const std::vector<Contour> &contours)
    {
        std::vector<const Contour *> invalid;
        invalid.reserve(contours.size());
        for (const auto &contour : contours)
        {
            if (!contour.isValid())
            {
                invalid.push_back(&contour);
            }
        }
        return invalid;
    }
} // namespace ContourApp

int main()
{
    using namespace ContourApp;

    std::vector<Contour> contours;

    // Valid contour: closed square
    {
        Contour c;
        c.addSegment(std::make_unique<LineSegment>(Point2D(0, 0), Point2D(1, 0)));
        c.addSegment(std::make_unique<LineSegment>(Point2D(1, 0), Point2D(1, 1)));
        c.addSegment(std::make_unique<LineSegment>(Point2D(1, 1), Point2D(0, 1)));
        c.addSegment(std::make_unique<LineSegment>(Point2D(0, 1), Point2D(0, 0)));
        contours.push_back(std::move(c));
    }

    // Valid contour: open triangle
    {
        Contour c;
        c.addSegment(std::make_unique<LineSegment>(Point2D(0, 0), Point2D(1, 0)));
        c.addSegment(std::make_unique<LineSegment>(Point2D(1, 0), Point2D(0.5, 1)));
        contours.push_back(std::move(c));
    }

    // Valid contour: open semicircle (ArcSegment)
    {
        Contour c;
        c.addSegment(std::make_unique<ArcSegment>(
            Point2D(0, 1),  // Start at (0,1)
            Point2D(0, -1), // End at (0,-1)
            Point2D(0, 0),  // Center at (0,0)
            1.0,            // Radius
            true            // Clockwise
            ));
        contours.push_back(std::move(c));
    }

    // Invalid contour: disconnected segments
    {
        Contour c;
        c.addSegment(std::make_unique<LineSegment>(Point2D(0, 0), Point2D(1, 0)));
        c.addSegment(std::make_unique<LineSegment>(Point2D(2, 0), Point2D(2, 1)));
        contours.push_back(std::move(c));
    }

    // Invalid contour: gap between segments
    {
        Contour c;
        c.addSegment(std::make_unique<LineSegment>(Point2D(0, 0), Point2D(1, 0)));
        c.addSegment(std::make_unique<LineSegment>(Point2D(1.1, 0), Point2D(1.1, 1)));
        contours.push_back(std::move(c));
    }

    auto validFuture = std::async(std::launch::async, findValidContours, std::cref(contours));
    auto invalidFuture = std::async(std::launch::async, findInvalidContours, std::cref(contours));

    auto validContours = validFuture.get();
    auto invalidContours = invalidFuture.get();

    std::cout << "Valid contours: " << validContours.size() << "\n";
    std::cout << "Invalid contours: " << invalidContours.size() << "\n";

    std::vector<const Contour *> allResults;
    allResults.reserve(validContours.size() + invalidContours.size());
    allResults.insert(allResults.end(), validContours.begin(), validContours.end());
    allResults.insert(allResults.end(), invalidContours.begin(), invalidContours.end());

    std::sort(allResults.begin(), allResults.end());
    auto uniqueEnd = std::unique(allResults.begin(), allResults.end());
    bool isUnique = (uniqueEnd == allResults.end());
    bool isComplete = (allResults.size() == contours.size());

    std::cout << "Results are unique: " << (isUnique ? "Yes" : "No") << "\n";
    std::cout << "All contours accounted for: " << (isComplete ? "Yes" : "No") << "\n";

    return 0;
}