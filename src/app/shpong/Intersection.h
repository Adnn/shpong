#pragma once

#include <renderer/commons.h>

#include <optional>

namespace ad {


template <class T>
struct Segment
{
    Vec2<T> displacement() const
    {
        return b-a;
    }

    Vec2<T> normal() const
    {
        return {-displacement().y(), displacement.x()};
    }

    Position2<T> a;
    Position2<T> b;
};

template <class T>
Segment<T> segment(Position2<T> a, Position2<T> b)
{
    return Segment<T>{a, b};
}

template <class T>
struct Intersection
{
    double  t;
    Segment<T> mSegment;
};

template <class T_derived, class T_value>
T_value cross2d(const math::Vector<T_derived, 2, T_value> & aLhs,
                const math::Vector<T_derived, 2, T_value> & aRhs)
{
    return aLhs.at(0)*aRhs.at(1) - aLhs.at(1)*aRhs.at(0);
}

template <class T>
std::optional<std::pair<Intersection<T>, Intersection<T>>>
intersect(const Segment<T> & p, const Segment<T> & q)
{
    auto r = p.displacement();
    auto s = q.displacement();

    // if not parallel
    if (cross2d(r, s) != 0)
    {
        double t0 = cross2d(q.a-p.a, s) / cross2d(r, s);
        double t1 = cross2d(q.a-p.a, r) / cross2d(r, s);

        if (   t0 >=0 && t0 <=1
            && t1 >=0 && t1 <=1)
        {
            return std::make_pair<Intersection<T>, Intersection<T>>({t0, p}, {t1, q});
        }
    }

    return {};
}

template <class T_left, class T_iterator>
auto findFirstIntersection(const T_left & aLeft,
                           T_iterator aRightFirst, const T_iterator &aRightLast)
{
    decltype(intersect(aLeft, *aRightFirst)) result;
    for (; aRightFirst != aRightLast; ++aRightFirst)
    {
        if (auto optIntersection = intersect(aLeft, *aRightFirst))
        {
            if (!result || (optIntersection->first.t < result->first.t))
            {
                result = optIntersection;
            }
        }
    }

    return result;
}

template <class T>
std::optional<std::pair<Intersection<T>, Intersection<T>>>
intersect(const Segment<T> & aSegment, const Rectangle<T> & aRect)
{
    auto edges = {
        segment(aRect.bottomLeft(), aRect.topLeft()),
        segment(aRect.topLeft(),    aRect.topRight()),
        segment(aRect.topRight(),   aRect.bottomRight()),
        segment(aRect.bottomRight(), aRect.bottomLeft()),
    };

    return findFirstIntersection(aSegment, edges.begin(), edges.end());
}


} // namespace ad
