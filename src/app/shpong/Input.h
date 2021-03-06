#pragma once

#include <renderer/commons.h>

namespace ad {

struct MouseInput
{
    Position2<GLfloat> mPosition{0.f, 0.f};
    bool mClick{false};
    bool mClickEdge{false};
    bool mReleaseEdge{false};
};

} // namespace ad
