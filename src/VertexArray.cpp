#include "VertexArray.hpp"

void VertexArray::Precompute()
{
    if (!dirty_attribs)
        return;

    for (const auto &attrib : vertex_attributes)
    {
        overall_stride += GetAttribSizeBytes(attrib);
    }

    dirty_attribs = false;
}

size_t VertexArray::GetAttribSizeBytes(const VertexArrayAttrib &attrib)
{
    size_t type_size;
        switch (attrib.underlying_type)
        {
        case GL_FLOAT:
        case GL_INT:
        case GL_UNSIGNED_INT:
            return 4 * attrib.component_count;
        default:
            assert(false && "Non defined attrib type");
            return 0;
        }
}

VertexArray::VertexArray( void *data,size_t byte_size, const std::vector<VertexArrayAttrib> &_attribs)
{
    vertex_attributes = _attribs;
    glGenBuffers(1, &vbo_handle);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_handle);
    glBufferData(GL_ARRAY_BUFFER, byte_size, data, GL_STATIC_DRAW);
}

VertexArray::~VertexArray()
{
    if (vbo_handle) glDeleteBuffers(1,&vbo_handle);
    if (ebo_handle) glDeleteBuffers(1,&ebo_handle);
}

VertexArray &VertexArray::SetElementBuffer(void *data, size_t size,GLenum index_type)
{
    if (ebo_handle == 0) glGenBuffers(1,&ebo_handle);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,ebo_handle);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,size,data,GL_STATIC_DRAW);
   ebo_element_type = index_type;
   return *this;
}

VertexArray &VertexArray::RemoveElementBuffer()
{
    if (ebo_handle != 0) glDeleteBuffers(1,&ebo_handle);
    ebo_handle = 0;
    return *this;
}

void VertexArray::Use()
{
    Precompute();
    if (current_VA == ID)
        return;
    // disable non used attribs
    for (int i = vertex_attributes.size(); i < current_attribs.size(); i++)
    {
        glDisableVertexAttribArray(i);
    }
    size_t offset = 0;
    for (int i = 0; i < vertex_attributes.size(); i++)
    {
        const VertexArrayAttrib &attrib = vertex_attributes[i];
        glEnableVertexAttribArray(i);
        glVertexAttribPointer(i, attrib.component_count, attrib.underlying_type, attrib.normalized,overall_stride, (void*)offset);
        offset += GetAttribSizeBytes(attrib);
    }
    if (ebo_handle != 0) glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,ebo_handle);
    current_attribs = vertex_attributes;
    current_VA = ID;
}

void VertexArray::Draw(uint64_t primitive_count,GLenum primitive_type)
{
    Use();

    if (ebo_handle == 0) {
        glDrawArrays(primitive_type,0,primitive_count);
    }
    else {
        glDrawElements(primitive_type,primitive_count,ebo_element_type,0);
    }
}
