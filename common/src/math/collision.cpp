#include <algorithm>
#include <array>
#include <cassert>
#include <cmath>
#include <limits>
#include <vector>

#include "gmath.h"

#include "collision.h"

template<typename T>
static void project_vertices(
    const T& points,
    const Vec2& normal,
    const Vec2& center,

    float* out_min,
    float* out_max
)
{
    float min = std::numeric_limits<float>::max();
    float max = -min;

    for (const Vec2& point : points) {
        float proj = normal * (center - point);

        min = std::min(proj, min);
        max = std::max(proj, max);
    }

    *out_min = min;
    *out_max = max;
}

static void project_circle(
    const Vec2& center,
    const float radius,
    const Vec2& normal,

    float* out_min,
    float* out_max
)
{
    Vec2 scaled = normal * radius;

    Vec2 p1 = center + scaled;
    Vec2 p2 = center - scaled;

    float min = p1 * normal;
    float max = p2 * normal;

    if (min > max) {
        // swap the min and max values.
        float t = min;
        min = max;
        max = t;
    }

    *out_min = min;
    *out_max = max;
}

bool Collision::CircleCircle(
    const Vec2& posA,
    const float radA,

    const Vec2& posB,
    const float radB,

    CollisionResponse* res
)
{
    Vec2 sub = posB - posA;

    float distSqrt = sub.length_sqr();
    float rad = radA + radB;

    if (distSqrt > (rad * rad)) {
        return false;
    }

    if (res != nullptr) {
        float dist = std::sqrt(distSqrt);
        res->normal = dist > 0.000001 ? sub / dist : Vec2(1, 0);
        res->depth = rad - dist;
    }
    return true;
}

bool Collision::CircleRect(
    const Vec2& circlePos,
    const float circleRad,

    const Vec2& rectMin,
    const Vec2& rectMax,

    CollisionResponse* res
)
{
    if (
        rectMin.x <= circlePos.x && circlePos.x <= rectMax.x
        && rectMin.y <= circlePos.y && circlePos.y <= rectMax.y
    ) {
        // circle inside rect
        if (res != nullptr) {
            Vec2 halfDimension = (rectMax - rectMin) * 0.5;
            Vec2 rectToCircle = (rectMin + halfDimension) - circlePos;
            float xDepth = std::abs(rectToCircle.x) - halfDimension.x - circleRad;
            float yDepth = std::abs(rectToCircle.y) - halfDimension.y - circleRad;

            // make the normal relative to the deepest axis
            if (xDepth > yDepth) {
                res->normal = Vec2(
                    rectToCircle.x > 0 ? 1 : -1,
                    0
                );
                res->depth = -xDepth;
            } else {
                res->normal = Vec2(
                    0,
                    rectToCircle.y > 0 ? 1 : -1
                );
                res->depth = -yDepth;
            }
        }

        return true;
    }

    Vec2 dir{
        std::clamp(circlePos.x, rectMin.x, rectMax.x) - circlePos.x,
        std::clamp(circlePos.y, rectMin.y, rectMax.y) - circlePos.y
    };

    float dstSqr = dir.length_sqr();

    if (dstSqr < circleRad * circleRad) {
        if (res != nullptr) {
            float dst = std::sqrt(dstSqr);

            res->normal = dir.normalize(dst);
            res->depth = circleRad - dst;
        }

        return true;
    }

    return false;
}

bool Collision::RectRect(
    const Vec2& rectAMin,
    const Vec2& rectAMax,

    const Vec2& rectBMin,
    const Vec2& rectBMax,

    CollisionResponse* res
)
{
    // if the caller doesn't want the intersection data
    // do a simpler check
    if (res == nullptr) {
        return rectBMin.x < rectBMax.x && rectBMin.y < rectBMax.y && rectAMin.x < rectAMax.x && rectAMin.y < rectAMax.y;
    }

    Vec2 halfDimA = (rectAMax - rectAMin) * 0.5;
    Vec2 halfDimB = (rectBMax - rectBMin) * 0.5;

    Vec2 bToA = ((rectBMin + halfDimB) - (rectAMin + halfDimA));

    float xDepth = halfDimA.x + halfDimB.x - std::abs(bToA.x);

    if (xDepth <= 0)
        return false;

    float yDepth = halfDimA.y + halfDimB.y - std::abs(bToA.y);

    if (yDepth <= 0)
        return false;

    if (xDepth < yDepth) {
        res->normal = Vec2(
            bToA.x < 0 ? -1 : 1,
            0
        );
        res->depth = xDepth;
    } else {
        res->normal = Vec2(
            0,
            bToA.y < 0 ? -1 : 1
        );
        res->depth = yDepth;
    }

    return true;
}

bool Collision::RectPolygon(
    const Vec2& rectMin,
    const Vec2& rectMax,

    const std::vector<Vec2>& polyPoints,
    const std::vector<Vec2>& polyNormals,
    const Vec2& polyCenter,

    CollisionResponse* res
)
{
    assert(polyPoints.size() == polyNormals.size());

    const std::array<Vec2, 4> rectPoints = {
        rectMin,
        {rectMin.x, rectMax.y},
        rectMax,
        {rectMax.x, rectMin.y}
    };

    static const std::array<Vec2, 4> rectNormals{
        Vec2{0, 1},
        Vec2{-1, 0},
        Vec2{0, -1},
        Vec2{1, 0}
    };

    Vec2 rectCenter = rectMin + ((rectMax - rectMin) / 2);

    Vec2 resNormal;
    float resDepth = std::numeric_limits<float>::max();

    for (auto vertNormal : polyNormals) {
        float minA, maxA, minB, maxB;
        project_vertices(polyPoints, vertNormal, Vec2(), &minA, &maxA);
        project_vertices(rectPoints, vertNormal, Vec2(), &minB, &maxB);

        if (minA >= maxB || minB >= maxA) {
            return false;
        }

        float axisDepth = std::min(maxB - minA, maxA - minB);

        if (axisDepth < resDepth) {
            resDepth = axisDepth;
            resNormal = vertNormal;
        }
    }

    for (auto vertNormal : rectNormals) {
        float minA, maxA, minB, maxB;
        project_vertices(polyPoints, vertNormal, Vec2(), &minA, &maxA);
        project_vertices(rectPoints, vertNormal, Vec2(), &minB, &maxB);

        if (minA >= maxB || minB >= maxA) {
            return false;
        }

        float axisDepth = std::min(maxB - minA, maxA - minB);

        if (axisDepth < resDepth) {
            resDepth = axisDepth;
            resNormal = vertNormal;
        }
    }

    Vec2 direction = rectCenter - polyCenter;

    if (direction * resNormal > 0) {
        resNormal.invert();
    }

    if (res != nullptr) {
        res->normal = resNormal;
        res->depth = resDepth;
    }

    return true;
}

bool Collision::CirclePolygon(
    const Vec2& circlePos,
    float circleRad,

    const std::vector<Vec2>& polyPoints,
    const std::vector<Vec2>& polyNormals,
    const Vec2& polyCenter,

    CollisionResponse* res
)
{
    assert(polyPoints.size() == polyNormals.size());

    Vec2 resNormal{};
    float resDepth = std::numeric_limits<float>::max();

    Vec2 circToPoly = polyCenter - circlePos;

    Vec2 closestPoint = {};
    float minDist = std::numeric_limits<float>::max();

    for (size_t i = 0; i < polyPoints.size(); i++) {
        Vec2 normal = polyNormals[i];
        Vec2 point = polyPoints[i];

        float minA, maxA, minB, maxB;
        project_vertices(polyPoints, normal, polyCenter, &minA, &maxA);
        project_circle(circToPoly, circleRad, normal, &minB, &maxB);

        if (minA >= maxB || minB >= maxA) {
            return false;
        }

        float depth = std::min(maxB - minA, maxA - minB);

        if (depth < resDepth) {
            resDepth = depth;
            resNormal = normal;
        }

        float dist = point.distance_to(circlePos);
        if (dist < minDist) {
            minDist = dist;
            closestPoint = point;
        }
    }

    Vec2 normal = (closestPoint - circlePos).normalize();

    float minA, maxA, minB, maxB;
    project_vertices(polyPoints, normal, polyCenter, &minA, &maxA);
    project_circle(circToPoly, circleRad, normal, &minB, &maxB);

    if (minA >= maxB || minB >= maxA) {
        return false;
    }

    float depth = std::min(maxB - minA, maxA - minB);

    if (depth < resDepth) {
        resDepth = depth;
        resNormal = normal;
    }

    Vec2 direction = polyCenter - circlePos;

    if ((direction * resNormal) < 0) {
        resNormal.invert();
    }

    if (res != nullptr) {
        res->normal = resNormal;
        res->depth = resDepth;
    }

    return true;
}

bool Collision::PolygonPolygon(
    const std::vector<Vec2>& pointsA,
    const std::vector<Vec2>& normalsA,
    const Vec2& centerA,

    const std::vector<Vec2>& pointsB,
    const std::vector<Vec2>& normalsB,
    const Vec2& centerB,

    CollisionResponse* res
)
{
    assert(pointsA.size() == normalsA.size());
    assert(pointsB.size() == normalsB.size());

    Vec2 resNormal = {};
    float resDepth = std::numeric_limits<float>::max();

    for (auto vertNormal : normalsA) {
        float minA, maxA, minB, maxB;
        project_vertices(pointsA, vertNormal, Vec2(), &minA, &maxA);
        project_vertices(pointsB, vertNormal, Vec2(), &minB, &maxB);

        if (minA >= maxB || minB >= maxA) {
            return false;
        }

        float axisDepth = std::min(maxB - minA, maxA - minB);

        if (axisDepth < resDepth) {
            resDepth = axisDepth;
            resNormal = vertNormal;
        }
    }

    for (auto vertNormal : normalsB) {
        float minA, maxA, minB, maxB;
        project_vertices(pointsA, vertNormal, Vec2(), &minA, &maxA);
        project_vertices(pointsB, vertNormal, Vec2(), &minB, &maxB);

        if (minA >= maxB || minB >= maxA) {
            return false;
        }

        float axisDepth = std::min(maxB - minA, maxA - minB);

        if (axisDepth < resDepth) {
            resDepth = axisDepth;
            resNormal = vertNormal;
        }
    }

    Vec2 direction = centerB - centerA;

    if (direction * resNormal < 0) {
        resNormal.invert();
    }

    if (res != nullptr) {
        res->normal = resNormal;
        res->depth = resDepth;
    }

    return true;
}

bool Collision::PointCircle(const Vec2& point, const Vec2& circlePos, float circleRad)
{
    return point.distance_to(circlePos) <= circleRad;
}

bool Collision::PointRect(const Vec2& point, const Vec2& rectMin, const Vec2& rectMax)
{
    return point.x > rectMin.x && point.y > rectMin.y && point.x < rectMax.x && point.y < rectMax.y;
}

bool Collision::PointPolygon(const Vec2& point, const std::vector<Vec2>& points)
{
    // https://wrfranklin.org/Research/Short_Notes/pnpoly.html
    size_t count = points.size();
    bool inside = false;

    for (size_t i = 0, j = count - 1; i < count; j = i++) {
        if (
            ((points[i].y >= point.y) != (points[j].y >= point.y))
            && (point.x <= (points[j].x - points[i].x) * (point.y - points[i].y) / (points[j].y - points[i].y) + points[i].x)
        ) {
            inside = !inside;
        }
    }

    return inside;
}
