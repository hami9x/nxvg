#ifndef NXVG_SHADER_H_INCLUDED
#define NXVG_SHADER_H_INCLUDED

#include "common.h"
#include <vector>
#include <string>

class ShaderConf {
public:
    virtual void apply(GLuint program) const = 0;
};

class Program {
    GLuint m_program;
    GLuint m_vao;
    GLuint m_vbo;

public:
    Program();
    Program(const char *vshader, const char *fshader);
    ~Program();

    void bind(const ShaderConf & confs) const;
    void bind(const std::vector<ShaderConf> & confs) const;
    void bind() const;
    void uploadData(GLfloat *a, int size) const;
    void drawArray(int start, int num) const;
    void addAttrib(const char * name, int size, int stride, int offset) const;
};

#endif
