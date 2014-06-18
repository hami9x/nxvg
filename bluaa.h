#ifndef BLUAA_H_INCLUDED
#define BLUAA_H_INCLUDED
#include "shader.h"

class Context;

class GlModeConf: public ShaderConf {
    int m_mode;
public:
    inline GlModeConf(int mode): m_mode(mode) {}

    virtual void apply(GLuint program) const;
};

void aaProcess(Context *ctx);

#endif
