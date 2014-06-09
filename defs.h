#ifndef DEFS_H_INCLUDED
#define DEFS_H_INCLUDED

//API
#define NX_LINE 1
#define NX_QUADRATIC 2
#define NX_BEZIER 3

typedef int NxSegmentType;

//Internal
#define MAX_SEGSIZE 5
#define MAX_SHADER_CONFS 4
#define FAIL(message) printf("nxvg: %s", message); exit(EXIT_FAILURE);
#define GLCHECK(message) GLenum err = glGetError(); if(err != GL_NO_ERROR) { printf("%s\nGl error code %u", message, err); exit(EXIT_FAILURE); }

#endif
