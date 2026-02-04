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
struct Response
{
    /**
     * The direction to move the shapes so they separate
     */
    Vec2F normal;
    /**
     * How much the shapes are colliding
     */
    float depth;
};

bool CircleCircle(
    Vec2F posA,
    float radA,

    Vec2F posB,
    float radB,

    Collision::Response* res
);

bool CircleRect(
    Vec2F circlePos,
    float circleRad,

    Vec2F rectMin,
    Vec2F rectMax,

    Collision::Response* res
);

bool CirclePolygon(
    Vec2F circlePos,
    float circleRad,

    const std::vector<Vec2F>& polyPoints,
    const std::vector<Vec2F>& polyNormals,
    Vec2F polyCenter,

    Collision::Response* res
);

bool RectRect(
    Vec2F rectAMin,
    Vec2F rectAMax,

    Vec2F rectBMin,
    Vec2F rectBMax,

    Collision::Response* res
);

bool RectPolygon(
    Vec2F rectMin,
    Vec2F rectMax,

    const std::vector<Vec2F>& polyPoints,
    const std::vector<Vec2F>& polyNormals,
    Vec2F polyCenter,

    Collision::Response* res
);

bool PolygonPolygon(
    const std::vector<Vec2F>& pointsA,
    const std::vector<Vec2F>& normalsA,
    Vec2F centerA,

    const std::vector<Vec2F>& pointsB,
    const std::vector<Vec2F>& normalsB,
    Vec2F centerB,

    Collision::Response* res
);

bool PointCircle(Vec2F point, Vec2F circlePos, float circleRad);

bool PointRect(Vec2F point, Vec2F rectMin, Vec2F rectMax);

bool PointPolygon(Vec2F point, const std::vector<Vec2F>& points);
};
