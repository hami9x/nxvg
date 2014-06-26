#ifndef NXVG_AA_H_INCLUDED
#define NXVG_AA_H_INCLUDED

#include <vector>
#include "shader.h"
#include "framebuffer.h"

class Context;

class GlModeConf: public ShaderConf {
    int m_mode;
public:
    inline GlModeConf(int mode): m_mode(mode) {}

    virtual void apply(GLuint program) const;
};

class SMAA {
    Framebuffer m_blendTex, m_edgesTex;
    GLuint m_areaTex, m_searchTex;
    const Program m_edgedet_shader;
    const Program m_bweight_shader;
    const Program m_neighbor_shader;
    Framebuffer * m_infbo;
    friend class Drawer;

public:
    SMAA(Context * ctx);
    void init();
    void process();
};

#endif
