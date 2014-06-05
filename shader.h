#ifndef SHADER_H_INCLUDED
#define SHADER_H_INCLUDED

#include "common.h"
#include <string>

class ShaderConf {
public:
    virtual void apply(GLuint program) = 0;
};

class Program {
    GLuint m_program;
    GLuint m_vao;
    GLuint m_vbo;

public:
    Program();
    Program(const char *vshader, const char *fshader);
    ~Program();

    void bind(ShaderConf * conf);
    void uploadData(GLfloat *a, int size);
    void drawArray(int start, int num);
    void addAttrib(const char * name, int size, int stride, int offset);
};

#endif
