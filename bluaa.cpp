#include "bluaa.h"

#include "context.h"
#include <iostream>

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
    auto mconf = GlModeConf(1);
    auto confRet = [ctx, &mconf](int mode) {
        mconf = GlModeConf(mode);
        return std::vector<const ShaderConf *>({
            dynamic_cast<const ShaderConf *>(&mconf),
            dynamic_cast<const ShaderConf *>(ctx->resoConf())
        });
    };
    sp.bind(confRet(1)); draw0313(sp);
    fbo.nextPass();
    sp.bind(confRet(2)); draw0313(sp);
    fbo.nextPass();

    for (int i=0; i<2; i++) {
        fbo.nextPass();
        sp.bind(confRet(3)); draw0313(sp);
        fbo.nextPass();
        sp.bind(confRet(4)); draw0313(sp);
    }
}
