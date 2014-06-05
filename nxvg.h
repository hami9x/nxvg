#ifndef NXVG_H_INCLUDED
#define NXVG_H_INCLUDED

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

typedef struct {
    GLfloat r;
    GLfloat g;
    GLfloat b;
    GLfloat a;
} NxColor;

EXPORT NxColor nxvgColor(GLfloat r, GLfloat g, GLfloat b, GLfloat a);
EXPORT void* nxvgNew(int resolutionX, int resolutionY);
EXPORT void nxvgDrawPath(void *ctx, GLfloat *points, NxSegmentType *types, int seglent, NxColor color);
EXPORT void nxvgDelete(void *ctx);

#endif // NXVG_H_INCLUDED
