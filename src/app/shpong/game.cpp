#include "game.h"

#include "Rotation.h"

#include <handy/random.h>

#include <limits>


namespace ad {

void Ball::handleReflection(Vec2<GLfloat> n, const GLfloat t, const GLfloat aDuration)
{
    // Minus epsilon, otherwise we are stuck colliding at zero in the recursions
    mRect.mPosition += mSpeed * aDuration * (t-0.001f);

    // Reflection
    // r = d - 2(d . n)n
    mSpeed = mSpeed - 2 * mSpeed.dot(n) * n;
}

void Ball::update(const std::vector<Rectangle<GLfloat>> & aColliders,
                  const std::vector<Segment<GLfloat>> & aBorders,
                  const GLfloat aDuration)
{
    Position2<GLfloat> & pos = mRect.mPosition;
    Segment<GLfloat> segment{pos, pos+mSpeed*aDuration};

    // Note: Impossible to collide with a border first
    if (auto optIntersection = findFirstIntersection(segment, begin(aColliders), end(aColliders)))
    {
        Vec2<GLfloat> n = optIntersection->first.second.mSegment.normal();
        if (optIntersection->second == begin(aColliders)) // The player
        {
            GLfloat rotationFactor = 2 * (optIntersection->first.second.t - 0.5); // [-1., 1.]
            // Counterclockwise rotation angle
            n = n*makeRotation(rotationFactor * -0.4);
        }
        auto t = static_cast<float>(optIntersection->first.first.t);
        handleReflection(n, t, aDuration);

        // Preventing a reflection on the racket that would "enter" the racket
        // (because of the varying normal)
        if (optIntersection->second == begin(aColliders)) // The player
        {
            if (mSpeed.y() < 35.f)
            {
                mSpeed = {
                    (mSpeed.x() < 0. ? -230.f : 230.f),
                    35.f
                };
            }
        }

        // Recursion
        return update(aColliders, aBorders, aDuration*(1-t));
    }
    else if (auto borderIntersection = findFirstIntersection(segment, begin(aBorders), end(aBorders)))
    {
        auto t = static_cast<float>(borderIntersection->first.first.t);
        return handleReflection(borderIntersection->first.second.mSegment.normal(),
                                t,
                                aDuration);
        // Recursion
        return update(aColliders, aBorders, aDuration*(1-t));
    }
    else
    {
        pos += mSpeed * aDuration;
        if (pos.y() < 0.f)
        {
            pos.y() = 300.f;
        }
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

        if(i%2)
        result.push_back({
            Rectangle<GLfloat>{{i*50.f, 200.f}, {50.f, 10.f}},
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

    std::vector<Segment<GLfloat>> borders {
        {{0.f, 0.f}, {0.f, 1000.f}},
        {{500.f, 1000.f}, {500.f, 0.f}},
    };

    mBall.update(colliders, borders, static_cast<GLfloat>(aTimer.mDelta));
}

} // namespace ad
