#include "context.h"
#include "common.h"

void uploadFullscreenQuad(const Program & sp) {
    sp.bind();
    float quad[] = {
        -1.0f,  1.0f, 0.0f, 1.0f,	// top left corner
        -1.0f, -1.0f, 0.0f, 0.0f,	// bottom left corner
         1.0f,  1.0f, 1.0f, 1.0f,// top right corner
         1.0f, -1.0f, 1.0f, 0.0f	// bottom right corner
    };
    sp.uploadData(quad, sizeof(quad));
}

void drawFullscreenQuad(const Program & sp) {
    sp.drawArray(0, 3);
    sp.drawArray(1, 3);
}

Context::Context(int rx, int ry):
        m_resx(rx),
        m_resy(ry),
        m_fbo(rx, ry),
        m_reso_conf(rx, ry),
        m_drawer(this),
        m_fill_shader("vs_postex.glsl", "fs_fill.glsl"),
        m_quad_shader("vs_postex.glsl", "fs_quadratic.glsl"),
        m_aa_shader("vs_postex.glsl", "fs_bluaa.glsl"),
        m_nothing_shader("vs_pos.glsl", "fs_nothing.glsl")
{
    auto postexAttrs = [](const Program & sp) {
        sp.bind();
        sp.addAttrib("position", 2, 4, 0);
        sp.addAttrib("texcoord", 2, 4, 2);
    };
    auto posAttrs = [](const Program & sp) {
        sp.bind();
        sp.addAttrib("position", 2, 2, 0);
    };

    postexAttrs(m_fill_shader);
    postexAttrs(m_quad_shader);
    postexAttrs(m_aa_shader);
    posAttrs(m_nothing_shader);

    uploadFullscreenQuad(m_fill_shader);
    uploadFullscreenQuad(m_aa_shader);
}

void GlColorConf::apply(GLuint program) const {
    GLint loc = glGetUniformLocation(program, "color");
    if (loc == -1) {
        FAIL("Uniform `color` not available in the program.");
    }

    //printf("color: %f %f %f %f\n", m_color.r, m_color.g, m_color.b, m_color.a);
    glUniform4f(loc, m_color.r, m_color.g, m_color.b, m_color.a);
}

void GlResoConf::apply(GLuint program) const {
    GLint loc = glGetUniformLocation(program, "resolution");
    if (loc == -1) {
        FAIL("Uniform `resolution` not available in the program.");
    }

    //printf("Reso: %f %f %f %f\n", m_Reso.r, m_Reso.g, m_Reso.b, m_Reso.a);
    glUniform2f(loc, m_x, m_y);
}
