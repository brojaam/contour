#include <gtest/gtest.h>
#include "Contour.h"
#include <future>
#include <vector>

namespace ContourApp {
TEST(ContourTest, EmptyContourIsValid) {
    Contour c;
    EXPECT_TRUE(c.isValid());
}

TEST(ContourTest, ValidClosedSquareContour) {
    Contour c;
    c.addSegment(std::make_unique<LineSegment>(Point2D(0, 0), Point2D(1, 0)));
    c.addSegment(std::make_unique<LineSegment>(Point2D(1, 0), Point2D(1, 1)));
    c.addSegment(std::make_unique<LineSegment>(Point2D(1, 1), Point2D(0, 1)));
    c.addSegment(std::make_unique<LineSegment>(Point2D(0, 1), Point2D(0, 0)));
    EXPECT_TRUE(c.isValid());
}

TEST(ContourTest, ValidOpenSemicircleContour) {
    Contour c;
    c.addSegment(std::make_unique<ArcSegment>(
        Point2D(0, 1),           // Start at (0,1)
        Point2D(0, -1),          // End at (0,-1)
        Point2D(0, 0),           // Center at (0,0)
        1.0,                     // Radius
        true                     // Clockwise
    ));
    EXPECT_TRUE(c.isValid());
}

TEST(ContourTest, InvalidDisconnectedContour) {
    Contour c;
    c.addSegment(std::make_unique<LineSegment>(Point2D(0, 0), Point2D(1, 0)));
    c.addSegment(std::make_unique<LineSegment>(Point2D(2, 0), Point2D(2, 1)));
    EXPECT_FALSE(c.isValid());
}

TEST(ContourTest, PolylineCreation) {
    std::vector<Point2D> points = { Point2D(0, 0), Point2D(1, 0), Point2D(1, 1) };
    auto c = Contour::createFromPolyline(points);
    EXPECT_EQ(c.size(), 2);
    EXPECT_TRUE(c.isValid());
}

TEST(ContourTest, InvalidPolylineCreation) {
    Contour c;
    c.addSegment(std::make_unique<LineSegment>(Point2D(0, 0), Point2D(1, 0)));
    c.addSegment(std::make_unique<LineSegment>(Point2D(2, 1), Point2D(2, 2)));
    EXPECT_EQ(c.size(), 2);
    EXPECT_FALSE(c.isValid()); // Disconnected at (1,0) to (2,1)
}

TEST(ContourTest, PolylineCreationInvalid) {
    std::vector<Point2D> points = { Point2D(0, 0) };
    EXPECT_THROW(Contour::createFromPolyline(points), std::invalid_argument);
}

TEST(ContourTest, CopyAndMove) {
    Contour c1;
    c1.addSegment(std::make_unique<LineSegment>(Point2D(0, 0), Point2D(1, 0)));
    Contour c2 = c1;
    EXPECT_EQ(c2.size(), 1);
    Contour c3 = std::move(c1);
    EXPECT_EQ(c3.size(), 1);
    EXPECT_EQ(c1.size(), 0);
}

TEST(ContourTest, SegmentModification) {
    Contour c;
    c.addSegment(std::make_unique<LineSegment>(Point2D(0, 0), Point2D(1, 0)));
    c.insertSegment(0, std::make_unique<LineSegment>(Point2D(-1, 0), Point2D(0, 0)));
    EXPECT_EQ(c.size(), 2);
    c.removeSegment(0);
    EXPECT_EQ(c.size(), 1);
}

TEST(ContourTest, SegmentModificationInvalid) {
    Contour c;
    EXPECT_THROW(c.insertSegment(1, std::make_unique<LineSegment>(Point2D(0, 0), Point2D(1, 0))), std::out_of_range);
    EXPECT_THROW(c.removeSegment(0), std::out_of_range);
    EXPECT_THROW(c.addSegment(nullptr), std::invalid_argument);
}

TEST(ContourTest, Iterator) {
    Contour c;
    c.addSegment(std::make_unique<LineSegment>(Point2D(0, 0), Point2D(1, 0)));
    size_t count = 0;
    for (const auto& seg : c) {
        EXPECT_EQ(seg->getType(), Segment::Type::Line);
        ++count;
    }
    EXPECT_EQ(count, 1);
}

TEST(ContourTest, ValidityCache) {
    Contour c;
    c.addSegment(std::make_unique<LineSegment>(Point2D(0, 0), Point2D(1, 0)));
    c.addSegment(std::make_unique<LineSegment>(Point2D(1, 0), Point2D(0, 0)));
    bool first = c.isValid();
    bool second = c.isValid();
    EXPECT_EQ(first, second);
}

TEST(ContourTest, AsyncValidityTest) {
    std::vector<Contour> contours;
    
    // Valid contour: closed square (LineSegment)
    {
        Contour c;
        c.addSegment(std::make_unique<LineSegment>(Point2D(0, 0), Point2D(1, 0)));
        c.addSegment(std::make_unique<LineSegment>(Point2D(1, 0), Point2D(1, 1)));
        c.addSegment(std::make_unique<LineSegment>(Point2D(1, 1), Point2D(0, 1)));
        c.addSegment(std::make_unique<LineSegment>(Point2D(0, 1), Point2D(0, 0)));
        contours.push_back(std::move(c));
    }
    
    // Valid contour: open semicircle (ArcSegment)
    {
        Contour c;
        c.addSegment(std::make_unique<ArcSegment>(
            Point2D(0, 1),           // Start at (0,1)
            Point2D(0, -1),          // End at (0,-1)
            Point2D(0, 0),           // Center at (0,0)
            1.0,                     // Radius
            true                     // Clockwise
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
    
    std::vector<std::future<bool>> futures;
    futures.reserve(contours.size());
    for (const auto& contour : contours) {
        futures.push_back(std::async(std::launch::async, &Contour::isValid, &contour));
    }
    
    std::vector<bool> results;
    results.reserve(futures.size());
    for (auto& future : futures) {
        results.push_back(future.get());
    }
    
    EXPECT_TRUE(results[0]);  // Closed square is valid
    EXPECT_TRUE(results[1]);  // Open semicircle is valid
    EXPECT_FALSE(results[2]); // Disconnected contour is invalid
}
} // namespace ContourApp

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}