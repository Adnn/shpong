#include "BrickDrawer.h"

#include "shaders.h"

#include <engine/Vertex.h>


namespace ad {

namespace {

    constexpr size_t gVerticesCount{4};
    Vertex gVerticesQuad[gVerticesCount] = {
        Vertex{
            {0.0f, 0.0f},
            {0, 0},
        },
        Vertex{
            {0.0f,  1.0f},
            {0, 1},
        },
        Vertex{
            { 1.0f, 0.0f},
            {1, 0},
        },
        Vertex{
            { 1.0f,  1.0f},
            {1, 1},
        },
    };

} // anonymous namespace


BrickDrawer::BrickDrawer() :
        GenericDrawer(makeLinkedProgram({
                                            {GL_VERTEX_SHADER, gVertexShader},
                                            {GL_FRAGMENT_SHADER, gFragmentShader},
                                        })),
        mVertexBuffer(addVertexBuffer(gVertexDescription, range(gVerticesQuad))),
        mInstanceBuffer(addInstanceBuffer(gInstanceDescription, sizeof(Instance)))
{}


void BrickDrawer::setBufferResolution(Size2<int> aNewResolution)
{
    GLint location = glGetUniformLocation(mProgram, "in_BufferResolution");
    glProgramUniform2iv(mProgram, location, 1, aNewResolution.data());
}

void BrickDrawer::render() const
{
    respecifyBuffer(mInstanceBuffer, range(mBricks));
    GenericDrawer::render(mBricks.size());
}

} // namespace ad
