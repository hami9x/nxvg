#ifndef NXVG_DEFS_H_INCLUDED
#define NXVG_DEFS_H_INCLUDED

#define FAIL(message) printf("nxvg: %s", message); exit(EXIT_FAILURE);
#define GLCHECK(message) GLenum err = glGetError(); if(err != GL_NO_ERROR) { printf("%s\nGl error code %u", message, err); exit(EXIT_FAILURE); }

#endif
