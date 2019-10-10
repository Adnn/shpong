#pragma once

#include "GenericDrawer.h"

#include <renderer/commons.h>
#include <renderer/VertexSpecification.h>


namespace ad {

struct Instance
{
    Rectangle<GLfloat> mRect;
    Vec3<GLubyte> mColor;
};

static const std::initializer_list<AttributeDescription> gInstanceDescription{
    {
        2,
        4, offsetof(Instance, mRect), MappedGL<GLfloat>::enumerator
    },
    {
        {3, Attribute::Access::Float, true},
        3, offsetof(Instance, mColor), MappedGL<GLubyte>::enumerator
    },
};

struct BrickDrawer : public GenericDrawer
{
    BrickDrawer();

    VertexBufferObject mVertexBuffer;
    VertexBufferObject mInstanceBuffer;
};


} // namespace ad
