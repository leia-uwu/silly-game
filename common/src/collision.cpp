#include <algorithm>
#include <cmath>
#include <limits>
#include <vector>

#include "collision.h"

static void project_vertices(
    const std::vector<Vec2>& points,
    const Vec2& normal,
    const Vec2& center,

    float* out_min,
    float* out_max
)
{
    float min = std::numeric_limits<float>::max();
    float max = -min;

    for (auto point : points) {
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
    Vec2 sub = posA - posB;

    float distSqrt = sub.length_sqr();
    float rad = radA + radB;

    if (distSqrt > (rad * rad)) {
        return false;
    }

    float dist = std::sqrt(distSqrt);

    if (res != nullptr) {
        res->normal = dist > 0.000001 ? sub / dist : Vec2(1, 0);
        res->depth = rad - dist;
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

    if ((direction * resNormal) > 0) {
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
