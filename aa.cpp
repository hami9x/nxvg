#include "aa.h"
#include "vendor/smaa/Textures/AreaTex.h"
#include "vendor/smaa/Textures/SearchTex.h"
#include "context.h"

using namespace std;
using namespace utils;

void GlModeConf::apply(GLuint program) const {
    GLint loc = glGetUniformLocation(program, "mode");
    if (loc == -1) {
        FAIL("Uniform `mode` not available in the program.");
    }

    glUniform1i(loc, m_mode);
}

void genTex(GLuint * tex) {
    glGenTextures( 1, tex );
    glBindTexture(GL_TEXTURE_2D, *tex );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
}

SMAA::SMAA(Context * ctx):
    m_blendTex(ctx->rx(), ctx->ry()),
    m_edgesTex(ctx->rx(), ctx->ry()),
    m_areaTex(0),
    m_searchTex(0),
    m_edgedet_shader("vs_smaa.glsl", "fs_smaa_edgedet.glsl"),
    m_bweight_shader("vs_smaa.glsl", "fs_smaa_weightcalc.glsl"),
    m_neighbor_shader("vs_smaa.glsl", "fs_smaa_neighborcalc.glsl"),
    m_infbo(&ctx->fbo())
{
}

void SMAA::init() {
    postexAttrs(m_edgedet_shader);
    postexAttrs(m_bweight_shader);
    postexAttrs(m_neighbor_shader);

    uploadFullscreenQuad(m_edgedet_shader);
    uploadFullscreenQuad(m_bweight_shader);
    uploadFullscreenQuad(m_neighbor_shader);
    genTex(&m_areaTex);
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RG8, ( GLsizei )AREATEX_WIDTH, ( GLsizei )AREATEX_HEIGHT, 0, GL_RG, GL_UNSIGNED_BYTE, areaTexBytes);

    genTex(&m_searchTex);
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RG8, ( GLsizei )SEARCHTEX_WIDTH, ( GLsizei )SEARCHTEX_HEIGHT, 0, GL_RG, GL_UNSIGNED_BYTE, searchTexBytes);
}

void SMAA::process() {
    glDisable(GL_BLEND);
    m_blendTex.clear();
    m_edgesTex.clear();

    m_edgedet_shader.bind(GlModeConf(0));
    m_edgesTex.bind();
    glBindTexture(GL_TEXTURE_2D, m_infbo->texture());
    draw0313(m_edgedet_shader);
    m_edgesTex.unbind();

    auto sp = m_bweight_shader;
    sp.bind(GlModeConf(1));
    m_blendTex.bind();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_edgesTex.texture());
    glUniform1i(glGetUniformLocation(sp.program(), "edgesTex"), 0);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_areaTex);
    glUniform1i(glGetUniformLocation(sp.program(), "areaTex"), 1);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, m_searchTex);
    glUniform1i(glGetUniformLocation(sp.program(), "searchTex"), 2);
    draw0313(sp);
    m_blendTex.unbind();
//
    sp = m_neighbor_shader;
    sp.bind(GlModeConf(2));
    m_infbo->bind();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_infbo->texture());
    glUniform1i(glGetUniformLocation(sp.program(), "colorTex"), 0);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_blendTex.texture());
    glUniform1i(glGetUniformLocation(sp.program(), "blendTex"), 1);
    m_infbo->nextPass();
    draw0313(sp);

    //glEnable(GL_BLEND);

    glActiveTexture(GL_TEXTURE0);
}
