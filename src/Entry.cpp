
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
GLFWwindow *window;
GLFWmonitor *monitor;
const GLFWvidmode *video_mode;

void ERROR_CALLBACK(int code, const char *desc)
{
    std::cerr << code << " " << desc << "\n";
}
int main()
{

    glfwInit();
    glfwSetErrorCallback(ERROR_CALLBACK);
    monitor = glfwGetPrimaryMonitor();
    video_mode = glfwGetVideoMode(monitor);
    glfwWindowHint(GLFW_CONTEXT_CREATION_API, GLFW_EGL_CONTEXT_API); // Use EGL
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);             // Request OpenGL ES
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 1);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    window = glfwCreateWindow(video_mode->width, video_mode->height, "Barton Display", nullptr, nullptr);
    std::cout << "hello\n";

    glfwMakeContextCurrent(window);
    int major, minor;
    major = glfwGetWindowAttrib(window, GLFW_CONTEXT_VERSION_MAJOR);
    minor = glfwGetWindowAttrib(window, GLFW_CONTEXT_VERSION_MINOR);
    std::cout << "OpenGL ES Context Version: " << major << "." << minor << std::endl;
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
}