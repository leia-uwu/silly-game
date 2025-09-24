#pragma once

#include "collision.h"
#include "vec2.h"

#include <array>
#include <cassert>
#include <cstdint>
#include <functional>
#include <string>
#include <vector>

class Shape
{
public:
    enum Type : uint8_t {
        CIRCLE,
        RECT,
        POLYGON,
        COUNT
    };

    const Type type;

    [[nodiscard]] virtual std::string toString() const = 0;
    [[nodiscard]] virtual bool pointInside(const Vec2& point) const = 0;

    [[nodiscard]] virtual Vec2 center() const = 0;

    virtual Shape& translate(const Vec2& posToAdd) = 0;
    virtual Shape& scale(float scale) = 0;

    virtual ~Shape() = default;

    [[nodiscard]] bool getCollision(const Shape& other, Collision::CollRes* res) const;

protected:
    Shape(Type type) :
        type(type)
    {
    }
};

class Circle : public Shape
{
public:
    Vec2 pos;
    float rad;

    Circle(Vec2 pos, float rad);

    [[nodiscard]] std::string toString() const override;

    [[nodiscard]] bool pointInside(const Vec2& point) const override;

    [[nodiscard]] Vec2 center() const override;

    Circle& translate(const Vec2& posToAdd) override;

    Circle& scale(float scale) override;
};

class Rect : public Shape
{
public:
    Vec2 min;
    Vec2 max;

    Rect(Vec2 min, Vec2 max);

    static Rect fromDims(float width, float height, Vec2 center = {0, 0});

    [[nodiscard]] float width() const
    {
        return max.x - min.x;
    }

    [[nodiscard]] float height() const
    {
        return max.y - min.y;
    }

    [[nodiscard]] Vec2 center() const override;

    [[nodiscard]] std::vector<Vec2> getPoints() const;

    [[nodiscard]] std::string toString() const override;

    [[nodiscard]] bool pointInside(const Vec2& point) const override;

    Rect& translate(const Vec2& posToAdd) override;

    Rect& scale(float scale) override;
};

class Polygon : public Shape
{
public:
    std::vector<Vec2> points;

private:
    std::vector<Vec2> m_normals;
    Vec2 m_center;

public:
    Polygon(const std::vector<Vec2>& points);

    void calculate_normals();

    void calculate_center();

    [[nodiscard]] Vec2 center() const override;

    [[nodiscard]] const std::vector<Vec2>& normals() const
    {
        return m_normals;
    };

    [[nodiscard]] std::string toString() const override;

    [[nodiscard]] bool pointInside(const Vec2& point) const override;

    Polygon& translate(const Vec2& posToAdd) override;

    Polygon& scale(float scale) override;
};

using CollisionFn = std::function<bool(const Shape&, const Shape&, Collision::CollRes*)>;

struct CollisionFnData
{
    CollisionFn fn;
    bool reverse;
};

class CollisionFns
{
private:
    std::array<std::array<CollisionFnData, Shape::COUNT>, Shape::COUNT> m_fns;

    void registerFn(Shape::Type typeA, Shape::Type typeB, const CollisionFn& fn);

public:
    CollisionFns();

    /**
     * Check collision between 2 shapes.
     *
     * @return true if both shapes collide.
     *
     * @param shapeA The first shape.
     * @param shapeB The second shape.
     * @param res A collision response pointer, optional / can be nullptr.
     *
     * @note The collision response will only be valid if this function returned true.
     * @note The collision response normal will always be relative to the first parameter.
     *
     * @example
     * ```
     *  Rect rect({10, 10}, {20, 20});
     *  Circle circle({10, 15}, 1);
     *  Collision::CollRes res;
     *  if (COLLISION_FNS.check(rect, circle, &res)) {
     *      // this will separate the circle from the rectangle
     *      circle.translate(res.normal * res.depth);
     *  }
     * ```
     */
    bool check(const Shape& shapeA, const Shape& shapeB, Collision::CollRes* res) const;
};
