#ifndef NXVG_SHADER_H_INCLUDED
#define NXVG_SHADER_H_INCLUDED

#include "common.h"
#include <string>

class ShaderConf {
public:
    virtual void apply(GLuint program) = 0;
};

class ConfApplier {
    vector<ShaderConf *> m_confs;
    static const MAX_SHADER_CONFS;

public:
    ConfApplier(int num, ...);
    ConfApplier(ShaderConf * conf);

    void apply(GLuint program);
};

class Program {
    GLuint m_program;
    GLuint m_vao;
    GLuint m_vbo;

public:
    Program();
    Program(const char *vshader, const char *fshader);
    ~Program();

    void bind(ConfApplier confs);
    void bind();
    void uploadData(GLfloat *a, int size);
    void drawArray(int start, int num);
    void addAttrib(const char * name, int size, int stride, int offset);
};

#endif
