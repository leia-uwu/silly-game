#include "shape.h"

CollisionFns::CollisionFns()
{
    registerFn(Shape::CIRCLE, Shape::CIRCLE, [](const Shape& shapeA, const Shape& shapeB, auto* res) {
        assert(shapeA.type == Shape::CIRCLE);
        assert(shapeB.type == Shape::CIRCLE);

        const auto& a = static_cast<const Circle&>(shapeA);
        const auto& b = static_cast<const Circle&>(shapeB);
        return Collision::CircleCircle(a.pos, a.rad, b.pos, b.rad, res);
    });
    registerFn(Shape::CIRCLE, Shape::RECT, [](const Shape& shapeA, const Shape& shapeB, auto* res) {
        assert(shapeA.type == Shape::CIRCLE);
        assert(shapeB.type == Shape::RECT);

        const auto& a = static_cast<const Circle&>(shapeA);
        const auto& b = static_cast<const Rect&>(shapeB);
        return Collision::CircleRect(a.pos, a.rad, b.min, b.max, res);
    });
    registerFn(Shape::CIRCLE, Shape::POLYGON, [](const Shape& shapeA, const Shape& shapeB, auto* res) {
        assert(shapeA.type == Shape::CIRCLE);
        assert(shapeB.type == Shape::POLYGON);

        const auto& a = static_cast<const Circle&>(shapeA);
        const auto& b = static_cast<const Polygon&>(shapeB);
        return Collision::CirclePolygon(a.pos, a.rad, b.points, b.normals(), b.center(), res);
    });
    registerFn(Shape::RECT, Shape::RECT, [](const Shape& shapeA, const Shape& shapeB, auto* res) {
        assert(shapeA.type == Shape::RECT);
        assert(shapeB.type == Shape::RECT);

        const auto& a = static_cast<const Rect&>(shapeA);
        const auto& b = static_cast<const Rect&>(shapeB);
        return Collision::RectRect(a.min, a.max, b.min, b.max, res);
    });
    registerFn(Shape::RECT, Shape::POLYGON, [](const Shape& shapeA, const Shape& shapeB, auto* res) {
        assert(shapeA.type == Shape::RECT);
        assert(shapeB.type == Shape::POLYGON);

        const auto& a = static_cast<const Rect&>(shapeA);
        const auto& b = static_cast<const Polygon&>(shapeB);
        return Collision::RectPolygon(a.min, a.max, b.points, b.normals(), b.center(), res);
    });
    registerFn(Shape::POLYGON, Shape::POLYGON, [](const Shape& shapeA, const Shape& shapeB, auto* res) {
        assert(shapeA.type == Shape::POLYGON);
        assert(shapeB.type == Shape::POLYGON);

        const auto& a = static_cast<const Polygon&>(shapeA);
        const auto& b = static_cast<const Polygon&>(shapeB);
        return Collision::PolygonPolygon(a.points, a.normals(), a.center(), b.points, b.normals(), b.center(), res);
    });
}

bool CollisionFns::check(const Shape& shapeA, const Shape& shapeB, Collision::CollRes* res) const
{
    assert(shapeA.type < Shape::COUNT);
    assert(shapeB.type < Shape::COUNT);

    const auto& collisionFn = m_fns[shapeA.type][shapeB.type];

    if (collisionFn.reverse) {
        bool collided = collisionFn.fn(shapeB, shapeA, res);
        res->normal.invert();
        return collided;
    }

    return collisionFn.fn(shapeA, shapeB, res);
}
