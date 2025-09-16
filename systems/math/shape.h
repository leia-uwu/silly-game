#pragma once

#include "collision.h"
#include "vec2.h"

#include <array>
#include <cassert>
#include <cstdint>
#include <format>
#include <functional>
#include <string>
#include <numeric>
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
    virtual Shape& translate(const Vec2& posToAdd) = 0;
    virtual Shape& scale(float scale) = 0;

    virtual ~Shape() = default;

protected:
    Shape(Type type)
        : type(type)
    {
    }
};

class Circle : public Shape
{
public:
    Vec2 pos;
    float rad;

    Circle(Vec2 pos, float rad)
        : Shape(CIRCLE)
        , pos(pos)
        , rad(rad)
    {
        assert(rad >= 0);
    }

    [[nodiscard]] std::string toString() const override
    {
        return std::format("Circle (X: {0:.4f}, Y: {1:.4f}, Rad: {2:.4f})", pos.x, pos.y, rad);
    }

    [[nodiscard]] bool pointInside(const Vec2& point) const override
    {
        return Collision::PointCircle(point, pos, rad);
    }

    Circle& translate(const Vec2& posToAdd) override
    {
        pos += posToAdd;
        return *this;
    };

    Circle& scale(const float scale) override
    {
        rad *= scale;
        return *this;
    }
};

class Rect : public Shape
{
public:
    Vec2 min;
    Vec2 max;

    Rect(Vec2 min, Vec2 max)
        : Shape(RECT)
        , min(min)
        , max(max)
    {
        // min being bigger than max can cause issues with collision functions
        assert(min.x < max.x);
        assert(min.y < max.y);
    }

    [[nodiscard]] float width() const
    {
        return max.x - min.x;
    }

    [[nodiscard]] float height() const
    {
        return max.y - min.y;
    }

    [[nodiscard]] Vec2 center() const
    {
        return min + ((max - min) / 2);
    }

    [[nodiscard]] std::string toString() const override
    {
        return std::format("Rect(Min ({}) Max ({}))", min.toString(), max.toString());
    }

    [[nodiscard]] bool pointInside(const Vec2& point) const override
    {
        return Collision::PointRect(point, min, max);
    }

    Rect& translate(const Vec2& posToAdd) override
    {
        min += posToAdd;
        max += posToAdd;

        return *this;
    };

    Rect& scale(const float scale) override
    {
        Vec2 center = this->center();

        min = (min - center) * scale + center;
        max = (max - center) * scale + center;

        return *this;
    }
};

class Polygon : public Shape
{
public:
    std::vector<Vec2> points;

private:
    std::vector<Vec2> m_normals;
    Vec2 m_center;

public:
    Polygon(const std::vector<Vec2>& points)
        : Shape(POLYGON)
        , points(points)
        , m_normals(points.size())
    {
        assert(points.size() >= 3);

        calculate_normals();
        calculate_center();
    }

    void calculate_normals()
    {
        for (size_t i = 0; i < points.size(); i++) {
            const Vec2& pointA = points[i];
            const Vec2& pointB = points[(i + 1) % points.size()];
            Vec2 edge = pointB - pointA;

            m_normals[i] = edge.perp().normalize();
        }
    }

    void calculate_center()
    {
        m_center = {0, 0};
        for (const auto& point : points) {
            m_center += point;
        }
        m_center /= points.size();
    }

    [[nodiscard]] const Vec2& center() const
    {
        return m_center;
    }

    [[nodiscard]] const std::vector<Vec2>& normals() const
    {
        return m_normals;
    }

    [[nodiscard]] std::string toString() const override
    {
        const std::string formatted = std::accumulate(
            points.cbegin(),
            points.cend(),
            std::string(),
            [](std::string a, const Vec2& b) {
                return std::format("{} ({}),", a, b.toString());
            }
        );
        return std::format("Polygon [{}]", formatted);
    }

    [[nodiscard]] bool pointInside(const Vec2& point) const override
    {
        return Collision::PointPolygon(point, points);
    }

    Polygon& translate(const Vec2& posToAdd) override
    {
        for (auto& point : points) {
            point += posToAdd;
        }
        m_center += posToAdd;

        return *this;
    }

    Polygon& scale(const float scale) override
    {
        for (auto& pt : points) {
            Vec2 toCenter = m_center - pt;
            float length = toCenter.length();
            const Vec2& dir = toCenter.normalize(length);

            pt = m_center - (dir * (length * scale));
        }

        return *this;
    }
};

using CollisionFn = std::function<bool(const Shape&, const Shape&, Collision::CollRes*)>;

struct CollisionFnData
{
    CollisionFn fn;
    bool reverse;
};

class CollisionFns
{
    std::array<std::array<CollisionFnData, Shape::COUNT>, Shape::COUNT> m_fns;

    void registerFn(Shape::Type typeA, Shape::Type typeB, const CollisionFn& fn)
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

const inline CollisionFns COLLISION_FNS;
