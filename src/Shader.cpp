#include "Shader.hpp"

Shader::Shader(const std::string& vert_source,const std::string& frag_source) {

    std::unordered_set<std::string> vert_includes,frag_includes;
    std::string vert_source_processed = ProcessIncludes(vert_source,vert_includes),
    frag_source_process = ProcessIncludes(frag_source,frag_includes);
    program_handle = glCreateProgram();
    GLuint vert_handle = glCreateShader(GL_VERTEX_SHADER);
    GLuint frag_handle = glCreateShader(GL_FRAGMENT_SHADER);
    std::string vert_log = "",frag_log = "",link_log = "";
    const bool vert_success = CompileShader(vert_handle,vert_source_processed,vert_log);
    const bool frag_success = CompileShader(frag_handle,frag_source_process,frag_log);
    glAttachShader(program_handle,vert_handle);
    glAttachShader(program_handle,frag_handle);
    const bool link_success = LinkProgram(program_handle,link_log);
    glDetachShader(program_handle,vert_handle);
    glDetachShader(program_handle,frag_handle);
    glDeleteShader(vert_handle);
    glDeleteShader(frag_handle);
    if (vert_success && frag_success && link_success)
    std::cout << "Shader compiled\n";
    else {
        std::cerr << "Shader failed to compile\n";
        std::cerr << "========== VERTEX ==========\n";
        std::cerr << "=== includes: === \n";
        for (const auto& include : vert_includes)
        std::cerr << include << "\n";
        std::cerr <<"=== Error log ===\n";
        std::cerr << vert_log << std::endl;


        std::cerr << "========== FRAG ==========\n";
        std::cerr << "=== includes: === \n";
        for (const auto& include : frag_includes)
        std::cerr << include << "\n";
        std::cerr <<"=== Error log ===\n";
        std::cerr << frag_log << std::endl;


        std::cerr << "========== LINK ==========\n";
        std::cerr <<"=== Error log ===\n";
        std::cerr << link_log << std::endl;

        assert(false && "Shader failed to compile");
    }
    
}

Shader::~Shader()
{
    if (program_handle)glDeleteProgram(program_handle);
}

std::shared_ptr<Shader> Shader::FromSources(const std::string &vert_source, const std::string &frag_source)
{
    return std::make_shared<Shader>(vert_source,frag_source);
}

std::shared_ptr<Shader> Shader::FromFiles(const std::string &vert_file_path, const std::string &frag_file_path)
{

    std::ifstream vert_file(pre_path+vert_file_path),frag_file(pre_path+frag_file_path);
    assert(vert_file.is_open()&&"Invalid Vertex File");
    assert(frag_file.is_open()&&"Invalid Fragment File");
    std::string vert_source((std::istreambuf_iterator<char>(vert_file)), std::istreambuf_iterator<char>()),
    frag_source((std::istreambuf_iterator<char>(frag_file)), std::istreambuf_iterator<char>());
    return FromSources(vert_source,frag_source);
}

void Shader::BindAttribLocation(const std::string &name, GLuint location)
{
    glBindAttribLocation(program_handle,location,name.c_str());
}

bool Shader::CompileShader(GLuint handle, const std::string &source, std::string &log)
{
    const char* src_c = source.c_str();
    glShaderSource(handle,1,&src_c,nullptr);
    glCompileShader(handle);
    GLint success = 0;
    glGetShaderiv(handle,GL_COMPILE_STATUS,&success);
    GLint info_log_length;
    glGetShaderiv(handle,GL_INFO_LOG_LENGTH,&info_log_length);
    char info_log[info_log_length];
    glGetShaderInfoLog(handle,info_log_length,nullptr,info_log);
    log = info_log;
    return success;
}

bool Shader::LinkProgram(GLuint handle, std::string &log)
{
      
    glLinkProgram(handle);

    GLint success = 0;
    glGetProgramiv(handle,GL_LINK_STATUS,&success);
    
    GLint info_log_length;
    glGetProgramiv(handle,GL_INFO_LOG_LENGTH,&info_log_length);
    
    char info_log[info_log_length];
    glGetProgramInfoLog(handle,info_log_length,nullptr,info_log);
    
    log = info_log;
    return success;
}

std::string Shader::ProcessIncludes(const std::string &input,std::unordered_set<std::string>& processedFiles)
{
    std::regex includeRegex(R"(#include\s*[<"]([^>"]+)[>"])");
    std::smatch match;
    std::string output = input;
    
    while (std::regex_search(output, match, includeRegex)) {
        std::string filename = match[1].str();
        if (processedFiles.count(filename)) {
            output.replace(match.position(0), match.length(0), "// Skipping already included: " + filename);
            continue;
        }
        processedFiles.insert(filename);
        std::ifstream included_file(pre_path+filename);
        assert(included_file.is_open() && "Included File does not exist");
        std::string fileContent((std::istreambuf_iterator<char>(included_file)), std::istreambuf_iterator<char>());
        output.replace(match.position(0), match.length(0), fileContent);
    }
    return output;
}
