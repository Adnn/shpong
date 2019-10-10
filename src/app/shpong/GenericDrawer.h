#pragma once

#include <renderer/Shading.h>
#include <renderer/VertexSpecification.h>


namespace ad {

class GenericDrawer
{
public:
    GenericDrawer(Program aProgram);

    VertexBufferObject
    addVertexBuffer(const std::initializer_list<AttributeDescription> & aAttributes,
                    std::size_t aStride);
    VertexBufferObject
    addInstanceBuffer(const std::initializer_list<AttributeDescription> & aAttributes,
                      std::size_t aStride);

    template <class T_iterator, class T_sentinel>
    VertexBufferObject
    addVertexBuffer(const std::initializer_list<AttributeDescription> & aAttributes,
                    const Range<T_iterator, T_sentinel> & aRange);
    template <class T_iterator, class T_sentinel>
    VertexBufferObject
    addInstanceBuffer(const std::initializer_list<AttributeDescription> & aAttributes,
                      const Range<T_iterator, T_sentinel> & aRange);

    void render(GLsizei aInstanceCount) const;

private:
    VertexArrayObject mVertexArray;
    Program mProgram;
};

/// \todo this is not leveraging the actual ability to load the vertex buffer at creation
template <class T_iterator, class T_sentinel>
VertexBufferObject
GenericDrawer::addVertexBuffer(const std::initializer_list<AttributeDescription> & aAttributes,
                               const Range<T_iterator, T_sentinel> & aRange)
{
    //glBindVertexArray(mVertexArray);
    //return makeLoadedVertexBuffer(aAttributes, aRange);

    auto result = addVertexBuffer(aAttributes,
                                  sizeof(typename Range<T_iterator, T_sentinel>::value_type));
    respecifyBuffer(result, aRange);
    return result;
}

template <class T_iterator, class T_sentinel>
VertexBufferObject
GenericDrawer::addInstanceBuffer(const std::initializer_list<AttributeDescription> & aAttributes,
                                 const Range<T_iterator, T_sentinel> & aRange)
{
    auto result = addInstanceBuffer(aAttributes,
                                    sizeof(typename Range<T_iterator, T_sentinel>::value_type));
    respecifyBuffer(result, aRange);
    return result;
}

} // namespace ad
