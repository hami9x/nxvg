#ifndef NXVG_DRAW_H_INCLUDED
#define NXVG_DRAW_H_INCLUDED

#include "common.h"

class Context;

namespace glm {
    class vec2;
}

class Stenciler {
    virtual void stencil() = 0;
}

class QuadraticCurve: public Stenciler {
    const glm::vec2 m_points[3];

public:
    QuadraticCurve(GLfloat const * const dataptr);

    virtual void stencil(const Context * const sp);
};

class CubicCurve: public Stenciler {
    const glm::vec2 m_points[4];

public:
    CubicCurve(GLfloat const * const dataptr);

    virtual void stencil(const Context * const sp);
};

class FillerPoly: public Stenciler {
    vector<glm::vec2> m_points;
    FillerPoly(GLfloat const * const startPtr);

    void addPoint(GLfloat const * const dataPtr);

    virtual void stencil(const Context * const sp);
}

class Drawer {
    const Context * const m_ctx;

public:
    typedef vector<unique_ptr<Stenciler>> SegmentList;
    void drawPath(const SegmentList & segs);
};

#endif
