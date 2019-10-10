#include "GenericDrawer.h"

namespace ad {

GenericDrawer::GenericDrawer(Program aProgram) :
        mVertexArray(),
        mProgram(std::move(aProgram))
{}

VertexBufferObject
GenericDrawer::addVertexBuffer(const std::initializer_list<AttributeDescription> & aAttributes,
                               std::size_t aStride)
{
    glBindVertexArray(mVertexArray);
    return makeLoadedVertexBuffer(aAttributes, aStride, 0, nullptr);
}

VertexBufferObject
GenericDrawer::addInstanceBuffer(const std::initializer_list<AttributeDescription> & aAttributes,
                                 std::size_t aStride)
{
    auto result = addVertexBuffer(aAttributes, aStride);
    for (const auto & attribute : aAttributes)
    {
        glVertexAttribDivisor(attribute.mIndex, 1);
    }
    return result;
}

void GenericDrawer::render(GLsizei aInstanceCount) const
{
    glBindVertexArray(mVertexArray);
    glUseProgram(mProgram);

    glDrawArraysInstanced(GL_TRIANGLE_STRIP,
                          0,
                          4,
                          aInstanceCount);
}

} // namespace ad
