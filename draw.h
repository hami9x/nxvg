#ifndef NXVG_DRAW_H_INCLUDED
#define NXVG_DRAW_H_INCLUDED
#include <array>
#include <vector>
#include <memory>
#include <glm/vec2.hpp>

#include "common.h"

class Context;

using glm::vec2;

class Stenciler {
public:
    virtual void stencil(const Context * const ctx) const = 0;
};

class QuadraticCurve: public Stenciler {
    const std::array<vec2, 3> m_points;

public:
    QuadraticCurve(GLfloat const * const p):
        m_points({vec2(p[0], p[1]), vec2(p[2], p[3]), vec2(p[4], p[5])})
    {}


    virtual void stencil(const Context * const ctx) const;
};

class CubicCurve: public Stenciler {
    const std::array<vec2, 4> m_points;

public:
    CubicCurve(GLfloat const * const p):
        m_points({
            vec2(p[0], p[1]),
            vec2(p[2], p[3]),
            vec2(p[4], p[5]),
            vec2(p[6], p[7])
        })
    {}

    virtual void stencil(const Context * const sp) const;
};

class FillerPoly: public Stenciler {
    std::vector<vec2> m_points;

public:
    inline FillerPoly(GLfloat const * const startPtr): m_points(1, vec2(startPtr[0], startPtr[1])) {}

    inline void addPoint(GLfloat const * const p) { m_points.push_back(vec2(p[0], p[1])); };

    virtual void stencil(const Context * const sp) const;
};

class Drawer {
    Context * const m_ctx;

public:
    inline Drawer(Context * ctx): m_ctx(ctx) {}
    typedef std::vector<std::unique_ptr<Stenciler>> SegmentList;
    void drawPath(const Drawer::SegmentList & segs, const NxColor & color) const;
};

#endif
