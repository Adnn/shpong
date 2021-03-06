#pragma once

#include "BrickDrawer.h"
#include "Input.h"
#include "Intersection.h"

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
    Position2<GLfloat> mHeldPos{-1.f, 0.f};
};

typedef BrickDrawer::Instance Brick;

struct Ball
{
    void update(const std::vector<Rectangle<GLfloat>> & aColliders,
                const std::vector<Segment<GLfloat>> & aBorders,
                const GLfloat aDuration);

    BrickDrawer::Instance makeDrawInstance();

    Rectangle<GLfloat> mRect;
    Vec3<GLubyte> mColor;
    Vec2<GLfloat> mSpeed{0.f, -250.f};

private:
    void handleReflection(Vec2<GLfloat> n, const GLfloat t, const GLfloat aDuration);
};


struct Game
{
    std::vector<Brick> generateBricks();
    void update(const Timer & aTimer, const MouseInput & aMouse);

    Player mP1{ Rectangle<GLfloat>{{200.f, 50.f}, {50.f, 10.f}},
                rgb(0, 255, 120)};
    std::vector<Brick> mBricks = generateBricks();
    Ball mBall{ Rectangle<GLfloat>{{225.f, 300.f}, {10.f, 10.f}},
                rgb(80, 90, 100)};
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
        mDrawer.render();
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
    aScene.mGame.update(aTimer, aMouse);

    aScene.mDrawer.mBricks.clear();
    aScene.mDrawer.mBricks.push_back(aScene.mGame.mP1.mBrick);
    aScene.mDrawer.mBricks.push_back(aScene.mGame.mBall.makeDrawInstance());
    aScene.mDrawer.mBricks.insert(aScene.mDrawer.mBricks.end(),
                                  aScene.mGame.mBricks.begin(),
                                  aScene.mGame.mBricks.end());
}

inline void renderScene(const Scene & aScene, Engine & aEngine)
{
    aEngine.clear();
    aScene.render();
}

} // namespace ad
