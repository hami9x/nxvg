#ifndef NXVG_CONTEXT_H_INCLUDED
#define NXVG_CONTEXT_H_INCLUDED
#include "nxvg.h"
#include "shader.h"
#include "framebuffer.h"
#include "draw.h"

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

class Context {
    int m_resx;
    int m_resy;
    Framebuffer m_fbo;
    const GlResoConf m_reso_conf;
    Drawer m_drawer;

public:
    Context(int rx, int ry);
    const Program m_fill_shader;
    const Program m_quad_shader;
    const Program m_aa_shader;
    const Program m_nothing_shader;

    inline Framebuffer & fbo() const { return m_fbo; }
    inline const GlResoConf & resoConf() const { return m_reso_conf; }
    inline const Drawer & drawer() const { return m_drawer; }
};

#endif // NXVGPRIV_H_INCLUDED
