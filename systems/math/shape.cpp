#include "shape.h"

Circle::Circle(Vec2 pos, float rad) :
    Shape(CIRCLE),
    pos(pos),
    rad(rad)
{
    assert(rad >= 0);
}

std::string Circle::toString() const
{
    return std::format("Circle (X: {0:.4f}, Y: {1:.4f}, Rad: {2:.4f})", pos.x, pos.y, rad);
}

bool Circle::pointInside(const Vec2& point) const
{
    return Collision::PointCircle(point, pos, rad);
}

Vec2 Circle::center() const
{
    return pos;
}

Circle& Circle::translate(const Vec2& posToAdd)
{
    pos += posToAdd;
    return *this;
}

Circle& Circle::scale(const float scale)
{
    rad *= scale;
    return *this;
}

Rect::Rect(Vec2 min, Vec2 max) :
    Shape(RECT),
    min(min),
    max(max)
{
    // min being bigger than max can cause issues with collision functions
    assert(min.x < max.x);
    assert(min.y < max.y);
}

Rect Rect::fromDims(float width, float height, Vec2 center)
{
    Vec2 size{width / 2, height / 2};

    return Rect{center - size, center + size};
}

Rect& Rect::scale(const float scale)
{
    Vec2 center = this->center();

    min = (min - center) * scale + center;
    max = (max - center) * scale + center;

    return *this;
}

Rect& Rect::translate(const Vec2& posToAdd)
{
    min += posToAdd;
    max += posToAdd;

    return *this;
}

bool Rect::pointInside(const Vec2& point) const
{
    return Collision::PointRect(point, min, max);
}

std::string Rect::toString() const
{
    return std::format("Rect(Min ({}) Max ({}))", min.toString(), max.toString());
}

std::vector<Vec2> Rect::getPoints() const
{
    return {
        min,
        {min.x, max.y},
        max,
        {max.x, min.y}
    };
}

Vec2 Rect::center() const
{
    return min + ((max - min) / 2);
}

Polygon::Polygon(const std::vector<Vec2>& points) :
    Shape(POLYGON),
    points(points),
    m_normals(points.size())
{
    assert(points.size() >= 3);

    calculate_normals();
    calculate_center();
}

Polygon& Polygon::scale(const float scale)
{
    for (auto& pt : points) {
        Vec2 toCenter = m_center - pt;
        float length = toCenter.length();
        const Vec2& dir = toCenter.normalize(length);

        pt = m_center - (dir * (length * scale));
    }

    return *this;
}

Polygon& Polygon::translate(const Vec2& posToAdd)
{
    for (auto& point : points) {
        point += posToAdd;
    }
    m_center += posToAdd;

    return *this;
}

bool Polygon::pointInside(const Vec2& point) const
{
    return Collision::PointPolygon(point, points);
}

std::string Polygon::toString() const
{
    std::string out = "Polygon [";

    for (size_t i = 0, size = points.size(); i < size; i++) {
        out += std::format("({})", points[i].toString());
        if (i != size - 1) {
            out += ", ";
        }
    }
    out += "]";

    return out;
}

Vec2 Polygon::center() const
{
    return m_center;
}

void Polygon::calculate_center()
{
    m_center = {0, 0};
    for (const auto& point : points) {
        m_center += point;
    }
    m_center /= points.size();
}

void Polygon::calculate_normals()
{
    for (size_t i = 0; i < points.size(); i++) {
        const Vec2& pointA = points[i];
        const Vec2& pointB = points[(i + 1) % points.size()];
        Vec2 edge = pointB - pointA;

        m_normals[i] = edge.perp().normalize();
    }
}

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

void CollisionFns::registerFn(Shape::Type typeA, Shape::Type typeB, const CollisionFn& fn)
{
    m_fns[typeA][typeB] = {
        .fn = fn,
        .reverse = false
    };

    if (typeA != typeB) {
        m_fns[typeB][typeA] = {
            .fn = fn,
            .reverse = true
        };
    }
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
