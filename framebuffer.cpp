#include "framebuffer.h"

Framebuffer::~Framebuffer() {
    glDeleteFramebuffers(2, m_fbos);
    glDeleteRenderbuffers(2, m_rbos);
    glDeleteTextures(2, m_texs);
}

void Framebuffer::activateBuffer(int i) {
    m_current = i;
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbos[i]);
    glBindRenderbuffer(GL_RENDERBUFFER, m_rbos[i]);
}

void Framebuffer::nextPass() {
    glBindTexture(GL_TEXTURE_2D, m_texs[m_current]);
    activateBuffer(1-m_current);
}

Framebuffer::Framebuffer(int w, int h):
    m_w(w),
    m_h(h),
    m_current(0)
{
    for (int i=0; i<=1; i++) {
        GLuint framebufferName = 0;
        glGenFramebuffers(1, &framebufferName);
        glBindFramebuffer(GL_FRAMEBUFFER, framebufferName);

        // The texture we're going to render to
        GLuint renderedTexture;
        glGenTextures(1, &renderedTexture);

        // "Bind" the newly created texture : all future texture functions will modify this texture
        glBindTexture(GL_TEXTURE_2D, renderedTexture);

        // Give an empty image to OpenGL ( the last "0" )
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

        // Poor filtering. Needed !
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

        //
        GLuint depthrenderbuffer;
        glGenRenderbuffers(1, &depthrenderbuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, depthrenderbuffer);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, w, h);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthrenderbuffer);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depthrenderbuffer);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderedTexture, 0);

        if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            printf("Framebuffer initialization failed with error %d", glCheckFramebufferStatus(GL_FRAMEBUFFER));
            exit(EXIT_FAILURE);
        }

        m_fbos[i] = framebufferName;
        m_rbos[i] = depthrenderbuffer;
        m_texs[i] = renderedTexture;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

void Framebuffer::unbind() const {
    glBindTexture(GL_TEXTURE_2D, m_texs[m_current]);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glEnable(GL_BLEND);
}

void Framebuffer::bind() {
    glDisable(GL_BLEND);
    activateBuffer(m_current);
}

void Framebuffer::clear() {
    activateBuffer(m_current);
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);
    unbind();
}
