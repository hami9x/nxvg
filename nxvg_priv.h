#ifndef NXVGPRIV_H_INCLUDED
#define NXVGPRIV_H_INCLUDED
#include "nxvg.h"
#include "shader.h"
#include "framebuffer.h"

class GlColorConf: public ShaderConf {
    NxColor m_color;
public:
    GlColorConf(NxColor color);
    virtual void apply(GLuint program);
};

class GlResoConf: public ShaderConf {
    int m_x;
    int m_y;
public:
    GlResoConf(int x, int y);
    virtual void apply(GLuint program);
};

struct Nxvg {
    int resX;
    int resY;

    Nxvg(int resX, int resY);

    Program fillSh;
    Program quadSh;
    Program oblurvSh;
    Program oblurhSh;
    Program blurvSh;
    Program blurhSh;
    Program simpleSh;
    Framebuffer fbo;
};

#endif // NXVGPRIV_H_INCLUDED
