#include "game.h"

#include "Intersection.h"

namespace ad {

void Ball::update(const Rectangle<GLfloat> &aRect, GLfloat aDuration)
{
    Position2<GLfloat> & pos = mBrick.mRect.mPosition;

    auto optIntersection = intersect(segment(pos, pos+mSpeed*aDuration),
                                     segment(aRect.topLeft(), aRect.topRight()));

    if (optIntersection)
    {
        float t = optIntersection->first.t;
        // Minus epsilon, otherwise we are stuck colliding at zero in the recursions
        pos += mSpeed * aDuration * (t-0.00001f);
        mSpeed *= -1;

        // Recursive call, with the remaining travel time
        update(aRect, aDuration*(1-t));
    }
    else
    {
        pos += mSpeed * aDuration;
    }
}


std::vector<Brick> Game::generateBricks()
{
    std::vector<Brick> result;
    for (int i = 0; i != 10; ++i)
    {
        result.push_back({
            Rectangle<GLfloat>{{i*50.f, 350.f}, {50.f, 10.f}},
            rgb(50, 85, 200)
        });
    }
    return result;
}

} // namespace ad
