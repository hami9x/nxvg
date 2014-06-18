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
    //activate stencil here
    for (auto & seg: segs) {
        seg->stencil(m_ctx);
    }

    GlColorConf cc(color);

    auto & fbo = m_ctx->fbo();
    fbo.clear();

    aaProcess(m_ctx);
    drawFullscreenQuad(m_ctx->m_fill_shader);
}
