#pragma once

#include "vec2.h"
#include <vector>

namespace Collision
{
struct CollisionResponse
{
    Vec2 normal;
    float depth;
};

bool CircleCircle(
    const Vec2& posA,
    float radA,
    const Vec2& posB,
    float radB,
    CollisionResponse* res
);

bool CirclePolygon(
    const Vec2& circlePos,
    float circleRad,
    const std::vector<Vec2>& polyPoints,
    const std::vector<Vec2>& polyNormals,
    const Vec2& polyCenter,

    CollisionResponse* res
);

bool PolygonPolygon(
    const std::vector<Vec2>& pointsA,
    const std::vector<Vec2>& normalsA,
    const Vec2& centerA,

    const std::vector<Vec2>& pointsB,
    const std::vector<Vec2>& normalsB,
    const Vec2& centerB,

    CollisionResponse* res
);
};
