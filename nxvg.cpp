#include "nxvg.h"
#include "context.h"
#include "draw.h"

#define MAX_SEGSIZE 5

using namespace std;

void glSetup(Context *nxvg) {
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
}

void* nxvgNew(int resX, int resY) {
    Context *nxvg = new Context(resX, resY);
    glSetup(nxvg);
    return (void*)(nxvg);
}

void nxvgDelete(void *ctx) {
    delete (Context *)(ctx);
}

NxColor nxvgColor(GLfloat r, GLfloat g, GLfloat b, GLfloat a) {
    NxColor c = {r, g, b, a};
    return c;
}

//int getSegSize(NxSegmentType type) {
//    switch (type) {
//    case NX_LINE: return 1;
//    case NX_QUADRATIC: return 2;
//    case NX_BEZIER: return 3;
//    }
//
//    FAIL("Unhandled segment type.");
//    return 0;
//}

void nxvgDrawPath(void *ctx, GLfloat *coords, NxSegmentType *types, int seglen, const NxColor & color) {
    Context *nxvg = static_cast<Context *>(ctx);

    Drawer::SegmentList segments;
    auto fp = new FillerPoly(coords);
    coords+=2;
    for (int i=0; i<seglen; ++i) {
        NxSegmentType type = types[i];
        Stenciler * seg = NULL;
        if (type == NX_QUADRATIC) {
            seg = new QuadraticCurve(coords-2);
            coords+=2;
            fp->addPoint(coords);
            coords+=2;
        } else if (type == NX_BEZIER) {
            seg = new CubicCurve(coords-2);
            coords+=4;
            fp->addPoint(coords);
            coords+=2;
        } else if (type == NX_LINE) {
            fp->addPoint(coords);
            coords+=2;
        }
        segments.push_back(unique_ptr<Stenciler>(seg));
    }

    nxvg->drawer().drawPath(segments, color);
}
