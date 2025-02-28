
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
GLFWwindow* window;
GLFWmonitor* monitor;
const GLFWvidmode* video_mode;

void ERROR_CALLBACK(int code, const char* desc) {
    std::cerr << code << " " << desc << "\n";

}
int main() {
    

    glfwInit();
    glfwSetErrorCallback(ERROR_CALLBACK);
    monitor = glfwGetPrimaryMonitor();
    video_mode = glfwGetVideoMode(monitor);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_COMPAT_PROFILE);
    glfwWindowHint(GLFW_CLIENT_API,GLFW_OPENGL_ES_API);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,1);
    window = glfwCreateWindow(video_mode->width,video_mode->height,"Barton Display",nullptr,nullptr);
    std::cout << "hello\n";

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
}