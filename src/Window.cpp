#include "Window.hpp"
#include "OpenGL.hpp"
void GLAPIENTRY debugMessageCallback(GLenum source, GLenum type, GLuint id,
                                     GLenum severity, GLsizei length,
                                     const GLchar *message, const void *userParam)
{
    (void)length;
    (void)userParam;
    std::cerr << "[OpenGL Debug Message] (" << id << "): " << message << "\n";
}

static void GLFW_ERROR_CALLBACK(int code, const char *desc)
{
    std::cerr << code << " " << desc << "\n";
}
decltype(Window::instance) Window::GetInstance()
{
    assert(instance && "Window Does not exist");
    return instance;
}
void Window::Init(uvec2 size, WindowHintsBitMask flags)
{
    assert(!instance && "Instance already exists");
    instance = std::make_shared<Window>(size, flags);
}
Window::Window(uvec2 _size, WindowHintsBitMask flags)
{
    size = _size;
    glfwInit();
    glfwSetErrorCallback(GLFW_ERROR_CALLBACK);
    monitor = glfwGetPrimaryMonitor();
    video_mode = glfwGetVideoMode(monitor);
    // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_CREATION_API, GLFW_EGL_CONTEXT_API);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
#ifdef _DEBUG
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
#endif

    glfwWindowHint(GLFW_RESIZABLE, flags & WindowHints::eRESIZEABLE);
    glfwWindowHint(GLFW_DECORATED, flags & WindowHints::eDECORATED);

    window_ptr = glfwCreateWindow(size.x, size.y, "Barton Display", flags & WindowHints::eFULLSCREEN ? monitor : nullptr, nullptr);
    std::cout << "Window created\n";
    glfwMakeContextCurrent(window_ptr);
#ifdef _DEBUG
    PFNGLDEBUGMESSAGECALLBACKKHRPROC glDebugMessageCallbackKHR =
        (PFNGLDEBUGMESSAGECALLBACKKHRPROC)glfwGetProcAddress("glDebugMessageCallbackKHR");
    glfwSwapInterval(0);
    if (glDebugMessageCallbackKHR)
    {
        std::cout << "glDebugMessageCallbackKHR is available!" << std::endl;
        glEnable(GL_DEBUG_OUTPUT_KHR);
        
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS_KHR);
        
        glDebugMessageCallbackKHR(debugMessageCallback,nullptr);
    
    }
    else
    {
        std::cout << "Failed to load glDebugMessageCallbackKHR!" << std::endl;
    }
 
    // glDebugMessageCallback(debugMessageCallback, nullptr);
#endif
    glViewport(0, 0, size.x, size.y);
    
    int major, minor;
    major = glfwGetWindowAttrib(window_ptr, GLFW_CONTEXT_VERSION_MAJOR);
    minor = glfwGetWindowAttrib(window_ptr, GLFW_CONTEXT_VERSION_MINOR);
    std::cout << "OpenGL ES Context Version: " << major << "." << minor << std::endl;
}

Window::~Window()
{
    glfwDestroyWindow(window_ptr);
    std::cout << "Window Destroyed" << std::endl;
    glfwTerminate();
    std::cout << "GLFW Terminated" << std::endl;
}

bool Window::ShouldClose() const
{
    return glfwWindowShouldClose(window_ptr);
}

void Window::SwapAndPoll()
{
    glfwPollEvents();
    glfwSwapBuffers(window_ptr);
}

int Window::GetKey(int key)
{
    return glfwGetKey(window_ptr,key);
}
