#ifndef NXVG_DRAW_H_INCLUDED
#define NXVG_DRAW_H_INCLUDED
#include "draw.h"

#include <vendor/glm/glm.hpp>
#include "common.h"

using namespace glm;

class QuadraticCurve: public Stenciler {
    const glm::vec2 m_points[3];

public:
    QuadraticCurve(GLfloat const * const dataptr);

    virtual void stencil(const Context * const sp);
};

inline QuadraticCurve::QuadraticCurve(GLfloat const * const p) {
    m_points[0] = vec2(p[0], p[1]);
    m_points[1] = vec2(p[2], p[3]);
    m_points[2] = vec2(p[4], p[5]);
}

void QuadraticCurve::stencil(const Context * const sp) {
    auto sp = &(ctx->quadSh);
    sp->bind();
    GLfloat vertices[] = {
        m_points[0].x, m_points[0].y, 0.0f, 0.0f,
        m_points[1].x, m_points[1].y, 0.5f, 0.0f,
        m_points[2].x, points[2].y, 1.0f, 1.0f
    };
    sp->uploadData(vertices, sizeof(vertices));
    sp->drawArray(0, 3);
}

inline CubicCurve::CubicCurve(GLfloat const * const p) {
    m_points[0] = vec2(p[0], p[1]);
    m_points[1] = vec2(p[2], p[3]);
    m_points[2] = vec2(p[4], p[5]);
    m_points[3] = vec2(p[6], p[7]);
}

void CubicCurve::stencil(const Context * const sp) {
}

inline void FillerPoly::addPoint(GLfloat const * const p) {
    m_points.push_back(vec2(p[0], p[1]));
}

void FillerPoly::stencil(const Context * const sp) {
}

void Drawer::drawPath(const Drawer::SegmentList & segs, const NxColor & color) {

    //activate stencil here
    for (auto seg: segs) {
        seg.stencil();
    }

    GlColorConf colorConf(color);
    auto drawQuad = [] (Program & sp) {
        sp.drawArray(0, 3);
        sp.drawArray(1, 3);
    };

    auto fbo = m_ctx->fbo();
    fbo.clear();

    ctx->fbo.bind(); //{
        ctx->fbo.nextPass();
        ctx->oblurvSh.bind(&resoConf); //{
            drawQuad(ctx->oblurvSh);
        //}
        ctx->fbo.nextPass();
        ctx->oblurhSh.bind(&resoConf); //{
            drawQuad(ctx->oblurhSh);
        //}
        for (int i=0; i<2; i++) {
            //continue;
            ctx->fbo.nextPass();
            ctx->blurvSh.bind(&resoConf); //{
                drawQuad(ctx->blurvSh);
            //}
            ctx->fbo.nextPass();
            ctx->blurhSh.bind(&resoConf); //{
                drawQuad(ctx->blurhSh);
            //}
        }
    //}
    ctx->fbo.unbind();

    ctx->fillSh.bind(&colorConf); //{
            drawQuad(ctx->fillSh);
    //}

//    auto sp = &(ctx->simpleSh);
//    sp->bind(&rcolorConf); //{
//        GLfloat vertices[] = {
//            points[0], points[1],
//            points[2], points[3],
//            points[4], points[5],
//        };
//    sp->uploadData(vertices, sizeof(vertices));
//    sp->drawArray(0, 3);
}
