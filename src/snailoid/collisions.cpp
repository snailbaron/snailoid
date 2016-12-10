#include "collisions.hpp"

#include <utility>
#include <cmath>

const CircleCollision CircleCollision::none;

CircleCollision::CircleCollision()
    : happened(false)
    , circleCenter(Point::zero)
    , collisionPoint(Point::zero)
{ }

CircleCollision::CircleCollision(Point circleCenter, Point collisionPoint)
    : happened(true)
    , circleCenter(std::move(circleCenter))
    , collisionPoint(std::move(collisionPoint))
{ }

CircleCollision circleCollidesLine(
    const Circle& circle, 
    const Vector& direction,
    const Line& line)
{
    Scalar d = distance(circle.center(), line);
    Line circleMoveLine = Line::directed(circle.center(), direction);

    Point x = lineCross(circleMoveLine, line);
    Scalar distanceToCross = len(x - circle.center());

    Scalar coeff = len(x - circle.center()) / d;
    Scalar offset = circle.radius() * coeff;

    Point newCenter = circle.center() +
        normalized(direction) * (distanceToCross - offset);
    Point collisionPoint = projection(newCenter, line);

    return CircleCollision(newCenter, collisionPoint);
}

CircleCollision circleCollidesPoint(
    const Circle& circle,
    const Vector& direction,
    const Point& point)
{
    Line movementLine = Line::directed(circle.center(), direction);
    Scalar c = pointCoordOnLine(point, movementLine);
    if (c < 0) {
        return CircleCollision::none;
    }

    Point proj = linePointAtCoord(movementLine, c);
    Scalar dist = len(proj - point);
    if (dist > circle.radius()) {
        return CircleCollision::none;
    }

    Scalar offset = std::sqrt(circle.radius() * circle.radius() - dist * dist);
    Point newCenter = proj - normalized(direction) * offset;

    return CircleCollision(newCenter, point);
}
