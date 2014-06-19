#include "draw.h"

#include "context.h"
#include "bluaa.h"

using namespace glm;

void QuadraticCurve::stencil(const Context * const ctx) const {
    auto sp = &(ctx->m_quad_shader);
    sp->bind();
    GLfloat vertices[] = {
        m_points[0].x, m_points[0].y, 0.0f, 0.0f,
        m_points[1].x, m_points[1].y, 0.5f, 0.0f,
        m_points[2].x, m_points[2].y, 1.0f, 1.0f
    };
    sp->uploadData(vertices, sizeof(vertices));
    sp->drawArray(0, 3);
}

void CubicCurve::stencil(const Context * const ctx) const {
}

void FillerPoly::stencil(const Context * const ctx) const {
}

void Drawer::drawPath(const Drawer::SegmentList & segs, const NxColor & color) const {
    glStencilMask(0xff);
    glClearStencil(0);

    auto & fbo = m_ctx->fbo();

    //! start FBO bind
    fbo.clear();
    fbo.bind();

    glStencilMask(0xff);
    //stencil step
    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    glStencilFunc(GL_ALWAYS, 0, 0xff);
    glStencilOp(GL_KEEP, GL_KEEP, GL_INVERT);
    for (auto & seg: segs) {
        seg->stencil(m_ctx);
    }
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);


    //fill the stenciled area
    glStencilFunc(GL_LESS, 0, 0xff);
    auto & nsp = m_ctx->m_nothing_shader;
    nsp.bind();
    float quad[] = {
        -1.0f,  1.0f,	// top left corner
        -1.0f, -1.0f,	// bottom left corner
         1.0f,  1.0f,   // top right corner
         1.0f, -1.0f	// bottom right corner
    };
    nsp.uploadData(quad, sizeof(quad));
    draw0313(nsp);

    glStencilFunc(GL_ALWAYS, 0, 0xff);
    fbo.nextPass();
    aaProcess(m_ctx);
    fbo.unbind();
    //! end of FBO bind

    //antialiasing and fill to the screen
    glStencilFunc(GL_ALWAYS, 0, 0xff);
    GlColorConf cc(color);
    m_ctx->m_fill_shader.bind(cc); draw0313(m_ctx->m_fill_shader);
}
