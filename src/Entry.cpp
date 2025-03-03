
// #include <GL/glew.h>
#include "Window.hpp"
#include "Shader.hpp"
#include "OpenGL.hpp"
#include "VertexArray.hpp"
#include "Framebuffer.hpp"
#define FULLSCREEN 0
#define DECORATED 1
#define RESIZEABLE 1

#define RESOLUTION_SCALE 0.10f
const vec2 quad_data[] = {
    {-1, 1}, {0.0f, 1.0f}, // Top-left
    {1, 1},
    {1.0f, 1.0f}, // Top-right
    {1, -1},
    {1.0f, 0.0f}, // Bottom-right
    {-1, -1},
    {0.0f, 0.0f} // Bottom-left
};
uint8_t quad_indicies[] = {
    0, 1, 2, // First triangle: top-left, top-right, bottom-right
    2, 3, 0  // Second triangle: bottom-right, bottom-left, top-left
};

int main(int argc, char **argv)
{

    Window::Init({800, 480},
                 WindowHints::eDECORATED * DECORATED |
                     WindowHints::eRESIZEABLE * RESIZEABLE
#ifdef _RELEASE
                     | WindowHints::eFULLSCREEN
#endif
    );

    std::shared_ptr<Window> window = Window::GetInstance();

    std::shared_ptr<Shader> face_shader = Shader::FromFiles("face.vert", "face.frag");
    // face_shader->BindAttribLocation("a_position", 0);
    // face_shader->BindAttribLocation("a_texcoord", 1);
    std::shared_ptr<Shader> resize_shader = Shader::FromFiles("Project.vert", "Project.frag");

    glReleaseShaderCompiler(); // NO MORE SHADER COMPILES AFTER THIS <========

    const std::vector<VertexArrayAttrib> attribs = {{GL_FLOAT, 2, false}, {GL_FLOAT, 2, false}};

    std::shared_ptr<VertexArray> vert_array =
        std::make_shared<VertexArray>((void *)quad_data, sizeof(quad_data), attribs);

    vert_array->SetElementBuffer((void *)quad_indicies, sizeof(quad_indicies), GL_UNSIGNED_BYTE);

    OpenGL::GetInstance()->DepthTest(false);

    auto lastTime = std::chrono::high_resolution_clock::now();
    int frameCount = 0;

    float resolution_scale = RESOLUTION_SCALE;
    if (argc >= 2)
    {
        resolution_scale = std::stof(argv[1]);
    }
    uvec2 final_resolution = (vec2)window->Size() * resolution_scale;
    bool round_to_pow_2 = argc >= 3 ? std::stoi(argv[2]) : 0;
    if (round_to_pow_2 || argc < 3)
    {
        std::cout << "Rounding resolution to power of 2\n";
        for (int i = 0; i < 2; i++)
        {
            float n = final_resolution[i];
            if (n <= 0)
                final_resolution[i] = 1; // Edge case: return 1 if n is 0 or negative

            int lower = 1 << (int)std::floor(std::log2(n));
            int upper = 1 << (int)std::ceil(std::log2(n));

            final_resolution[i] = (n - lower < upper - n) ? lower : upper;
        }
    }
    std::cout << "Upscale Resolution " << final_resolution.x << ", " << final_resolution.y << std::endl;
    std::shared_ptr<Framebuffer_Low> render_fbo = std::make_shared<Framebuffer_Low>(final_resolution);

    while (!window->ShouldClose() && window->GetKey(GLFW_KEY_ESCAPE) != GLFW_PRESS)
    {
        auto currentTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> deltaTime = currentTime - lastTime;

        render_fbo->Use();
        face_shader->Use();
        glUniform1f(0, (float)glfwGetTime());
        vert_array->Draw(6);

        // If 1 second has passed, calculate and print framerate
        if (deltaTime.count() >= 1.0f)
        {
            float fps = frameCount / deltaTime.count();
            std::cout << "FPS: " << fps << std::endl;
            frameCount = 0;
            lastTime = currentTime;
        }

        glViewport(0, 0, window->Size().x, window->Size().y);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        resize_shader->Use();
        glBindTexture(GL_TEXTURE_2D, render_fbo->GetTexture());
        vert_array->Draw(6);
        frameCount++;

        window->SwapAndPoll();
    }
}
