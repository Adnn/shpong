#pragma once

#include "BrickDrawer.h"
#include "Input.h"

#include <engine/commons.h>
#include <engine/Engine.h>
#include <engine/Timer.h>

#include <renderer/Drawing.h>


namespace ad
{

struct Player
{
    void input(const MouseInput &aMouse)
    {
        if (aMouse.mClickEdge && mBrick.mRect.contains(aMouse.mPosition))
        {
            mHeldPos = aMouse.mPosition;
            return;
        }
        if (aMouse.mReleaseEdge)
        {
            mHeldPos.x() = -1.;
            return;
        }

        if(aMouse.mClick && mHeldPos.x() >= 0.)
        {
            mBrick.mRect.x() += aMouse.mPosition.x() - mHeldPos.x();
            mHeldPos = aMouse.mPosition;
        }

    }

    BrickDrawer::Instance mBrick;
    Position2<GLfloat> mHeldPos{-1., 0.};
};

struct Ball
{
    void update(const Rectangle<GLfloat> &aRacket, GLfloat aDuration)
    {
        Position2<GLfloat> & pos = mBrick.mRect.mPosition;
        pos += mSpeed * aDuration;

        if (aRacket.contains(pos))
        {
            mSpeed *= -1;
        }
    }

    BrickDrawer::Instance mBrick;
    Vec2<GLfloat> mSpeed{10., -100.};
};


struct Game
{
    Player mP1{ Rectangle<GLfloat>{{50.f, 50.f}, {50.f, 10.f}},
                {0, 255, 120}};
    Ball mBall{ Rectangle<GLfloat>{{300.f, 300.f}, {10.f, 10.f}},
                {80, 90, 100}};
};


struct Scene {
    Scene(Engine & aEngine)
    {
        mDrawer.setBufferResolution(aEngine.getWindowSize());
        aEngine.listenResize([this](Size2<int> aNewSize)
        {
            mDrawer.setBufferResolution(aNewSize);
        });
    }

    void render() const
    {
        mDrawer.render(2);
    }

    Game mGame;
    BrickDrawer mDrawer;
};


/// \note Scene is not copiable nor movable, hence return by pointer
inline std::unique_ptr<Scene> setupScene(Engine & aEngine)
{
    return std::unique_ptr<Scene>{new Scene{aEngine}};
}

inline void updateScene(Scene & aScene, Engine & aEngine,
                        const Timer & aTimer, const MouseInput &aMouse)
{
    aScene.mGame.mP1.input(aMouse);
    aScene.mGame.mBall.update(aScene.mGame.mP1.mBrick.mRect, aTimer.mDelta);

    aScene.mDrawer.mBricks.clear();
    aScene.mDrawer.mBricks.push_back(aScene.mGame.mP1.mBrick);
    aScene.mDrawer.mBricks.push_back(aScene.mGame.mBall.mBrick);
}

inline void renderScene(const Scene & aScene, Engine & aEngine)
{
    aEngine.clear();
    aScene.render();
}

} // namespace ad
