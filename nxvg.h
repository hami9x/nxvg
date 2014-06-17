#ifndef NXVG_H_INCLUDED
#define NXVG_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

#if defined(_MSC_VER)
    //  Microsoft
    #define EXPORT __declspec(dllexport)
    #define IMPORT __declspec(dllimport)
#elif defined(_GCC)
    //  GCC
    #define EXPORT __attribute__((visibility("default")))
    #define IMPORT
#else
    #define EXPORT
    #define IMPORT
#endif

#include "defs.h"
#include "gl.h"
#include "shader.h"

//API
#define NX_LINE 1
#define NX_QUADRATIC 2
#define NX_BEZIER 3

typedef int NxSegmentType;

typedef struct {
    GLfloat r;
    GLfloat g;
    GLfloat b;
    GLfloat a;
} NxColor;

EXPORT NxColor nxvgColor(GLfloat r, GLfloat g, GLfloat b, GLfloat a);
EXPORT void* nxvgNew(int resolutionX, int resolutionY);
EXPORT void nxvgDrawPath(void *ctx, GLfloat *coords, NxSegmentType *types, int seglent, const NxColor & color);
EXPORT void nxvgDelete(void *ctx);

#ifdef __cplusplus
}
#endif

#endif // NXVG_H_INCLUDED
