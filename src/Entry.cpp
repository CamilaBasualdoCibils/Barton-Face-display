
// #include <GL/glew.h>
#include "Window.hpp"
#include "Shader.hpp"
#include "OpenGL.hpp"
#include "VertexArray.hpp"
#define FULLSCREEN 0
#define DECORATED 1
#define RESIZEABLE 1
int main()
{

    Window::Init({800, 480},
                 WindowHints::eFULLSCREEN * FULLSCREEN |
                     WindowHints::eDECORATED * DECORATED |
                     WindowHints::eRESIZEABLE * RESIZEABLE);

    std::shared_ptr<Window> window = Window::GetInstance();

    std::shared_ptr<Shader> shader = Shader::FromFiles("test.vert", "test.frag");
    shader->BindAttribLocation("a_position",0);
    shader->BindAttribLocation("a_texcoord",1);
    const vec2 quad_data[] = {
    {-1, 1},       {0.0f, 1.0f}, // Top-left
    { 1, 1},       {1.0f, 1.0f}, // Top-right
    { 1, -1},      {1.0f, 0.0f}, // Bottom-right
    {-1, -1},      {0.0f, 0.0f}  // Bottom-left
    };
    unsigned int quad_indicies[] = {
        0, 1, 2,  // First triangle: top-left, top-right, bottom-right
        2, 3, 0   // Second triangle: bottom-right, bottom-left, top-left
    };
    const std::vector<VertexArrayAttrib> attribs = {{GL_FLOAT,2,false},{GL_FLOAT,2,false}};
    std::shared_ptr<VertexArray> vert_array = 
    std::make_shared<VertexArray>((void*)quad_data,sizeof(quad_data),attribs) ;
    vert_array->SetElementBuffer((void*)quad_indicies,sizeof(quad_indicies),GL_UNSIGNED_INT);
    OpenGL::GetInstance()->DepthTest(false);

    auto lastTime = std::chrono::high_resolution_clock::now();
    int frameCount = 0;
    while (!window->ShouldClose() && window->GetKey(GLFW_KEY_ESCAPE) != GLFW_PRESS)
    {
        auto currentTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> deltaTime = currentTime - lastTime;
        
        shader->Use();
        vert_array->Draw(6);


        // If 1 second has passed, calculate and print framerate
        if (deltaTime.count() >= 1.0f) {
            float fps = frameCount / deltaTime.count();
            std::cout << "FPS: " << fps << std::endl;
            frameCount = 0;
            lastTime = currentTime;
        }

        frameCount++;
        
        window->SwapAndPoll();
        
    }
}
