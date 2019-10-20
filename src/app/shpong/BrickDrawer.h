#pragma once

#include "GenericDrawer.h"

#include <renderer/commons.h>
#include <renderer/VertexSpecification.h>


namespace ad {


struct BrickDrawer : public GenericDrawer
{
    struct Instance
    {
        Rectangle<GLfloat> mRect;
        Vec3<GLubyte> mColor;
    };
    static const std::initializer_list<AttributeDescription> gInstanceDescription;

    BrickDrawer();

    void setBufferResolution(Size2<int> aNewResolution);

    void render(GLsizei aInstanceCount) const;

    VertexBufferObject mVertexBuffer;
    VertexBufferObject mInstanceBuffer;

    std::vector<Instance> mBricks;
};


inline const std::initializer_list<AttributeDescription> BrickDrawer::gInstanceDescription = {
    {
        2,
        4, offsetof(Instance, mRect), MappedGL<GLfloat>::enumerator
    },
    {
        {3, Attribute::Access::Float, true},
        3, offsetof(Instance, mColor), MappedGL<GLubyte>::enumerator
    },
};


} // namespace ad
