#include <pch.hpp>
using WindowHintsBitMask = uint32_t;
enum WindowHints : WindowHintsBitMask
{
    eFULLSCREEN = 0b1,
    eRESIZEABLE = 0b10,
    eDECORATED = 0b100
};
class Window
{
    const static inline uvec2 GLES_VERSION = {2,1};
    GLFWwindow *window_ptr;
    GLFWmonitor *monitor;
    const GLFWvidmode *video_mode;
    uvec2 size;
    static inline std::shared_ptr<Window> instance;

public:
    static decltype(instance) GetInstance();
    static void Init(uvec2 size, WindowHintsBitMask flags);
    Window(uvec2 size, WindowHintsBitMask flags);
    ~Window();
    bool ShouldClose() const;
    void SwapAndPoll();
    uvec2 Size() const {return size;}
    float AspectRatio() const {return size.y/size.x;}
    int GetKey(int key);
};