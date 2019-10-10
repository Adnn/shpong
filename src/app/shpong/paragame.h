#pragma once

#include "BrickDrawer.h"
#include "Input.h"

#include <engine/commons.h>
#include <engine/Engine.h>
#include <engine/Timer.h>

#include <renderer/Drawing.h>

#include <math/Range.h>


namespace ad
{

struct Piece
{
    BrickDrawer::Instance mBlock;
};

struct Game
{
    static const Size2<GLfloat> gPieceDimension;
    static const Size2<GLfloat> gPieceOffset;
    static const Color gInactiveColor;
    static const Color gActiveColor;

    Game(int aColumns, int aRows)
    {
        for (int x : ::ad::math::range(aColumns))
        {
            for (int y : ::ad::math::range(aRows))
            {
                Rectangle<GLfloat> square {
                    static_cast<Position2<GLfloat>>(gPieceDimension+gPieceOffset)
                        .cwMul({GLfloat(x), GLfloat(y)}),
                    gPieceDimension
                };
                mPieces.push_back({square, gInactiveColor});
            }
        }
    };

    bool click_impl(Position2<GLfloat> aPosition)
    {
        auto found = find_if(mPieces.begin(), mPieces.end(), [&aPosition](const Piece &aPiece)
                     {
                        return aPiece.mBlock.mRect.contains(aPosition);
                     });

        if (found != mPieces.end())
        {
            auto & brick = found->mBlock;
            brick.mColor = (brick.mColor == gActiveColor) ? gInactiveColor : gActiveColor;
            return true;
        }
        return false;
    }

    void click(Position2<GLfloat> aPosition)
    {
        std::initializer_list<int> spread{-1, 1};
        if(click_impl(aPosition))
        {
            for (int move : spread)
            {
                click_impl(aPosition + static_cast<Vec2<GLfloat>>(gPieceDimension).cwMul({move, 0}));
                click_impl(aPosition + static_cast<Vec2<GLfloat>>(gPieceDimension).cwMul({0, move}));
            }
        }
    }

    std::vector<Piece> mPieces;
};

const Size2<GLfloat>  Game::gPieceDimension{50., 50.};
const Size2<GLfloat>  Game::gPieceOffset{2., 2.};
const Color Game::gInactiveColor{30, 10, 30};
const Color Game::gActiveColor{0, 255, 120};

struct Scene {

    Scene(const Scene &) = delete;
    Scene & operator=(const Scene &) = delete;

    Scene(Engine & aEngine) :
        mGame(3, 3)
    {
        mDrawer.setBufferResolution(aEngine.getWindowSize());
        aEngine.listenResize([this](Size2<int> aNewSize)
        {
            mDrawer.setBufferResolution(aNewSize);
        });
    }

    void render() const
    {
        mDrawer.render(mGame.mPieces.size());
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
                        const Timer & aTimer, const MouseInput &aInput)
{
    if (aInput.mClickEdge)
    {
        aScene.mGame.click(aInput.mPosition);
    }
}

inline void renderScene(const Scene & aScene, Engine & aEngine)
{
    respecifyBuffer(aScene.mDrawer.mInstanceBuffer, range(aScene.mGame.mPieces));

    aEngine.clear();
    aScene.render();
}

} // namespace ad
