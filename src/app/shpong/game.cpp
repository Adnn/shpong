#include "game.h"

#include "Intersection.h"
#include "Rotation.h"

#include <handy/random.h>

#include <limits>


namespace ad {

void Ball::update(const std::vector<Rectangle<GLfloat>> aColliders, GLfloat aDuration)
{
    Position2<GLfloat> & pos = mRect.mPosition;

    auto optIntersection = findFirstIntersection(segment(pos, pos+mSpeed*aDuration),
                                                 begin(aColliders), end(aColliders));

    if (optIntersection)
    {
        float t = static_cast<float>(optIntersection->first.first.t);
        // Minus epsilon, otherwise we are stuck colliding at zero in the recursions
        pos += mSpeed * aDuration * (t-0.001f);


        Vec2<GLfloat> n = optIntersection->first.second.mSegment.normal();
        if (optIntersection->second == begin(aColliders)) // The player
        {
            GLfloat rotationFactor = 2 * (optIntersection->first.second.t - 0.5); // [-1., 1.]
            // Counterclockwise rotation angle
            n = n*makeRotation(rotationFactor * -0.4);
        }
        // Reflection
        // r = d - 2(d . n)n
        mSpeed = mSpeed - 2 * mSpeed.dot(n) * n;

        // Recursive call, with the remaining travel time
        update(aColliders, aDuration*(1-t));
    }
    else
    {
        pos += mSpeed * aDuration;
    }
}

BrickDrawer::Instance Ball::makeDrawInstance()
{
    return {mRect.centered(), mColor};
}

std::vector<Brick> Game::generateBricks()
{
    Randomizer<> randomChannel(0, 255);

    std::vector<Brick> result;
    for (int i = 0; i != 10; ++i)
    {
        result.push_back({
            Rectangle<GLfloat>{{i*50.f, 350.f}, {50.f, 10.f}},
            rgb(randomChannel(), randomChannel(), randomChannel())
        });
    }
    return result;
}

void Game::update(const Timer & aTimer, const MouseInput & aMouse)
{
    mP1.input(aMouse);

    std::vector<Rectangle<GLfloat>> colliders({mP1.mBrick.mRect});
    for (const auto & brick : mBricks)
    {
        colliders.push_back(brick.mRect);
    }
    mBall.update(colliders, static_cast<GLfloat>(aTimer.mDelta));
}

} // namespace ad
