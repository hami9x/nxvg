#ifndef NXVG_FRAMEBUFFER_H_INCLUDED
#define NXVG_FRAMEBUFFER_H_INCLUDED
#include "common.h"

class Framebuffer {
    GLuint m_fbos[2];
    GLuint m_texs[2];
    GLuint m_rbos[2];
    int m_w;
    int m_h;
    int m_current;

    void activateBuffer(int i);

public:
    ~Framebuffer();
    Framebuffer(int width, int height);

    void bind();
    void unbind() const;
    void nextPass();
    void clear();
};

#endif
