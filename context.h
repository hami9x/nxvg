#ifndef NXVG_CONTEXT_H_INCLUDED
#define NXVG_CONTEXT_H_INCLUDED
#include "nxvg.h"
#include "shader.h"
#include "framebuffer.h"
#include "aa.h"
#include "draw.h"

class GlColorConf: public ShaderConf {
    const NxColor m_color;

public:
    inline GlColorConf(const NxColor & color): m_color(color) {}
    virtual void apply(GLuint program) const;
};

class GlResoConf: public ShaderConf {
    const int m_x;
    const int m_y;
public:
    inline GlResoConf(int x, int y): m_x(x), m_y(y) {}
    virtual void apply(GLuint program) const;
};

class Context {
    const int m_resx;
    const int m_resy;
    Framebuffer m_fbo;
    const GlResoConf m_reso_conf;
    const Drawer m_drawer;
    SMAA m_aa;

public:
    Context(int rx, int ry);
    const Program m_fill_shader;
    const Program m_quad_shader;
    const Program m_nothing_shader;

    inline SMAA * aa() { return &m_aa; }
    inline Framebuffer & fbo() { return m_fbo; }
    inline const GlResoConf * resoConf() const { return &m_reso_conf; }
    inline const Drawer & drawer() const { return m_drawer; }
    inline int rx() const { return m_resx; }
    inline int ry() const { return m_resy; }
};

namespace utils {
    void uploadFullscreenQuad(const Program & sp);
    void draw0313(const Program & sp);
    void postexAttrs(const Program & sp);
    void posAttrs(const Program & sp);
}

#endif // NXVGPRIV_H_INCLUDED
