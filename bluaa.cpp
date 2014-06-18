#include "bluaa.h"

#include "context.h"

void GlModeConf::apply(GLuint program) const {
    if (m_mode < 1 || m_mode > 4) {
        FAIL("Invalid mode for bluAA's GlModeConf.");
    }

    GLint loc = glGetUniformLocation(program, "mode");
    if (loc == -1) {
        FAIL("Uniform `mode` not available in the program.");
    }

    //printf("color: %f %f %f %f\n", m_color.r, m_color.g, m_color.b, m_color.a);
    glUniform1i(loc, m_mode);
}

void aaProcess(Context * ctx) {
    auto & sp = ctx->m_aa_shader;
    auto & fbo = ctx->fbo();
    fbo.bind();
        sp.bind(GlModeConf(1)); drawFullscreenQuad(sp);
        fbo.nextPass();
        sp.bind(GlModeConf(2)); drawFullscreenQuad(sp);
        fbo.nextPass();

        for (int i=0; i<2; i++) {
            fbo.nextPass();
            sp.bind(GlModeConf(3)); drawFullscreenQuad(sp);
            fbo.nextPass();
            sp.bind(GlModeConf(4)); drawFullscreenQuad(sp);
        }
    fbo.unbind();
}
