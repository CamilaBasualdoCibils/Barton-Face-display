#pragma once
#include <pch.hpp>

void checkOpenGLError();

class OpenGL {

    private:
    using func_void_glenum = std::function<void(GLenum)>;
    func_void_glenum enable_disable_func[2] = {glDisable,glEnable};

    GLboolean _depthtest_enable;
    void FetchAndSet(func_void_glenum func[2],GLenum parameter,GLboolean& v,GLboolean yes) {
        if (v ==yes) return;
        func[yes](parameter);

        v = yes;
    }
    static inline std::shared_ptr<OpenGL> INSTANCE;
    public:
    OpenGL();
    static decltype(INSTANCE) GetInstance();
    void DepthTest(GLboolean yes);

};
