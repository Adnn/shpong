#pragma once

#include "BrickDrawer.h"

#include <engine/commons.h>
#include <engine/Engine.h>
#include <engine/Timer.h>

#include <renderer/Drawing.h>


namespace ad
{

struct Player : public Instance
{};


struct Field
{
    Player mP1{ Rectangle<GLfloat>{{50.f, 50.f}, {50.f, 10.f}},
                {0, 255, 120}};
};


struct Scene {
    void render() const
    {
        mDrawer.render(1);
    }

    Field mField;
    BrickDrawer mDrawer;
};


/// \note Scene is not copiable nor movable, hence return by pointer
inline std::unique_ptr<Scene> setupScene(Engine & aEngine)
{
    return std::unique_ptr<Scene>{new Scene{}};
}

inline void updateScene(Scene & aScene, Engine & aEngine, const Timer & aTimer)
{
}

inline void renderScene(const Scene & aScene, Engine & aEngine)
{
    respecifyBuffer(aScene.mDrawer.mInstanceBuffer, single(aScene.mField.mP1));

    aEngine.clear();
    aScene.render();
}

} // namespace ad
