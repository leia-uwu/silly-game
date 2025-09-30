#pragma once

#include "math/vec2.h"

#include <vector>

namespace Collision
{
/**
 *  Collision response between 2 shapes.
 *
 *  To separate them you can move them by the normal multiplied by the depth
 */
struct CollRes
{
    /**
     * The direction to move the shapes so they separate
     */
    Vec2 normal;
    /**
     * How much the shapes are colliding
     */
    float depth;
};

bool CircleCircle(
    const Vec2& posA,
    float radA,

    const Vec2& posB,
    float radB,

    CollRes* res
);

bool CircleRect(
    const Vec2& circlePos,
    float circleRad,

    const Vec2& rectMin,
    const Vec2& rectMax,

    CollRes* res
);

bool CirclePolygon(
    const Vec2& circlePos,
    float circleRad,

    const std::vector<Vec2>& polyPoints,
    const std::vector<Vec2>& polyNormals,
    const Vec2& polyCenter,

    CollRes* res
);

bool RectRect(
    const Vec2& rectAMin,
    const Vec2& rectAMax,

    const Vec2& rectBMin,
    const Vec2& rectBMax,

    CollRes* res
);

bool RectPolygon(
    const Vec2& rectMin,
    const Vec2& rectMax,

    const std::vector<Vec2>& polyPoints,
    const std::vector<Vec2>& polyNormals,
    const Vec2& polyCenter,

    CollRes* res
);

bool PolygonPolygon(
    const std::vector<Vec2>& pointsA,
    const std::vector<Vec2>& normalsA,
    const Vec2& centerA,

    const std::vector<Vec2>& pointsB,
    const std::vector<Vec2>& normalsB,
    const Vec2& centerB,

    CollRes* res
);

bool PointCircle(const Vec2& point, const Vec2& circlePos, float circleRad);

bool PointRect(const Vec2& point, const Vec2& rectMin, const Vec2& rectMax);

bool PointPolygon(const Vec2& point, const std::vector<Vec2>& points);

};
