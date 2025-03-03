#include <pch.hpp>
struct VertexArrayAttrib
{
    GLenum underlying_type = GL_FLOAT;
    uint16_t component_count = 4;
    bool normalized = false;
};

using VertexArrayID = uint64_t;
class VertexArray
{
    static inline std::vector<VertexArrayAttrib> current_attribs;
    static inline VertexArrayID current_VA = -1;

    const VertexArrayID ID = []()
    {static VertexArrayID nextID = 0;
    return nextID++; }();

    GLuint vbo_handle = 0,ebo_handle = 0;
    GLenum ebo_element_type = GL_UNSIGNED_INT;
    std::vector<VertexArrayAttrib> vertex_attributes;
    bool dirty_attribs= true;
    size_t overall_stride = 0;
    void Precompute();
    size_t GetAttribSizeBytes(const VertexArrayAttrib& attrib);
public:
    VertexArray(void *data,size_t byte_size, const std::vector<VertexArrayAttrib> &);
    ~VertexArray();
    VertexArray& SetElementBuffer(void* data, size_t size,GLenum index_type);
    VertexArray& RemoveElementBuffer();
    void Use();
    void Draw(uint64_t primitive_count,GLenum primitive_type = GL_TRIANGLES);
};