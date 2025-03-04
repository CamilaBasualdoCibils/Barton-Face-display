#pragma once
#include <pch.hpp>
template <GLenum internal_format, GLenum format, GLenum internal_type>
class Framebuffer
{

    GLuint fbo, fb_texture;
    uvec2 size;

public:
    Framebuffer(uvec2 _size)
    {
        size = _size;
        glGenFramebuffers(1, &fbo);
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);

        // Create a low-precision RGB565 texture
        glGenTextures(1, &fb_texture);
        glBindTexture(GL_TEXTURE_2D, fb_texture);
        glTexImage2D(GL_TEXTURE_2D, 0, internal_format, size.x, size.y, 0, format, internal_type, NULL);

        // Set texture filtering (bilinear upscaling)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBindTexture(GL_TEXTURE_2D, 0);

        // Attach texture to framebuffer
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fb_texture, 0);

        // Check if framebuffer is complete
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        {
            printf("Framebuffer is not complete!\n");
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0); // Unbind framebuffer
    }

    void Bind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    }
    void Use() {
        Bind();
        glViewport(0,0,size.x,size.y);
    }
    GLuint GetTexture() {return fb_texture;}
};

using Framebuffer_Low = Framebuffer<GL_RGB565, GL_RGB, GL_UNSIGNED_SHORT_5_6_5>;