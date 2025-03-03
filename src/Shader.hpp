#include <pch.hpp>
#include "OpenGL.hpp"
class Shader
{

    GLuint program_handle;
    const static inline std::string pre_path = "./shaders/";
    bool static CompileShader(GLuint handle, const std::string &source, std::string &log);

    bool static LinkProgram(GLuint handle, std::string &log);
    std::string static ProcessIncludes(const std::string &input, std::unordered_set<std::string> &processedFiles);

public:
    Shader(const std::string &vert_source, const std::string &frag_source);
    ~Shader();
    static std::shared_ptr<Shader> FromSources(const std::string &vert_source, const std::string &frag_source);
    static std::shared_ptr<Shader> FromFiles(const std::string &vert_file, const std::string &frag_file);
    void Use(){glUseProgram(program_handle);}
    void BindAttribLocation(const std::string& name, GLuint location);
};
