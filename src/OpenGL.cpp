#include "OpenGL.hpp"

OpenGL::OpenGL()
{
    glGetBooleanv(GL_DEPTH_TEST, &_depthtest_enable);
}

decltype(OpenGL::INSTANCE) OpenGL::GetInstance()
{
    if (!INSTANCE)
        INSTANCE = std::make_shared<OpenGL>();
    return INSTANCE;
}

void OpenGL::DepthTest(GLboolean yes)
{
    FetchAndSet(enable_disable_func, GL_DEPTH_TEST, _depthtest_enable, yes);
}

void checkOpenGLError()
{
    GLenum error = glGetError();
#ifdef _DEBUG
    if (error != GL_NO_ERROR)
    {
        std::string errorStr;
        switch (error)
        {
        case GL_INVALID_ENUM:
        errorStr = "GL_INVALID_ENUM: An unacceptable value is specified for an enumerated argument.";
            break;
        case GL_INVALID_VALUE:
            errorStr = "GL_INVALID_VALUE: A numeric argument is out of range.";
            break;
        case GL_INVALID_OPERATION:
            errorStr = "GL_INVALID_OPERATION: The specified operation is not allowed in the current state.";
            break;
        case GL_OUT_OF_MEMORY:
            errorStr = "GL_OUT_OF_MEMORY: There is not enough memory to execute the command.";
            break;
        case GL_INVALID_FRAMEBUFFER_OPERATION:
            errorStr = "GL_INVALID_FRAMEBUFFER_OPERATION: The framebuffer is not complete.";
            break;
        default:
            errorStr = "Unknown OpenGL Error";
            break;
        }
        std::cerr << "OPENGL ERROR: " << errorStr << std::endl;
    }

#endif
    assert(error == GL_NO_ERROR && "OPENGL ERROR");
}
