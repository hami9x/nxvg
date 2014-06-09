#include "nxvg.h"
#include "common.h"
#include "nxvg_priv.h"

void uploadFullscreenQuad(Program & sp) {
    sp.bind();
    float quad[] = {
        -1.0f,  1.0f, 0.0f, 1.0f,	// top left corner
        -1.0f, -1.0f, 0.0f, 0.0f,	// bottom left corner
         1.0f,  1.0f, 1.0f, 1.0f,// top right corner
         1.0f, -1.0f, 1.0f, 0.0f	// bottom right corner
    };
    sp.uploadData(quad, sizeof(quad));
}

Nxvg::Nxvg(int rx, int ry): fillSh("fill_vshader.glsl", "fill_fshader.glsl"),
            quadSh("fill_vshader.glsl", "quadratic_fshader.glsl"),
            blurvSh("fill_vshader.glsl", "blurvert_fshader.glsl"),
            blurhSh("fill_vshader.glsl", "blurhori_fshader.glsl"),
            oblurvSh("fill_vshader.glsl", "oblurv_fshader.glsl"),
            oblurhSh("fill_vshader.glsl", "oblurh_fshader.glsl"),
            simpleSh("simple_vshader.glsl", "simple_fshader.glsl"),
            fbo(rx, ry)
{
    resX = rx;
    resY = ry;

    auto basicAttrs = [](Program & sp) {
        sp.bind();
        sp.addAttrib("position", 2, 4, 0);
        sp.addAttrib("texcoord", 2, 4, 2);
    };
    basicAttrs(fillSh);
    basicAttrs(quadSh);
    basicAttrs(blurhSh);
    basicAttrs(blurvSh);
    basicAttrs(oblurhSh);
    basicAttrs(oblurvSh);
    simpleSh.bind(); simpleSh.addAttrib("position", 2, 2, 0);

    uploadFullscreenQuad(fillSh);
    uploadFullscreenQuad(blurhSh);
    uploadFullscreenQuad(blurvSh);
    uploadFullscreenQuad(oblurhSh);
    uploadFullscreenQuad(oblurvSh);
}

GlColorConf::GlColorConf(NxColor color) {
    m_color = color;
}

void GlColorConf::apply(GLuint program) {
    GLint loc = glGetUniformLocation(program, "color");
    if (loc == -1) {
        FAIL("Uniform `color` not available in the program.");
    }

    //printf("color: %f %f %f %f\n", m_color.r, m_color.g, m_color.b, m_color.a);
    glUniform4f(loc, m_color.r, m_color.g, m_color.b, m_color.a);
}

GlResoConf::GlResoConf(int x, int y) {
    m_x = x;
    m_y = y;
}

void GlResoConf::apply(GLuint program) {
    GLint loc = glGetUniformLocation(program, "resolution");
    if (loc == -1) {
        FAIL("Uniform `resolution` not available in the program.");
    }

    //printf("Reso: %f %f %f %f\n", m_Reso.r, m_Reso.g, m_Reso.b, m_Reso.a);
    glUniform2f(loc, m_x, m_y);
}

void gInit(Nxvg *nxvg) {
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
}

void* nxvgNew(int resX, int resY) {
    Nxvg *nxvg = new Nxvg(resX, resY);
    gInit(nxvg);
    return (void*)(nxvg);
}

void nxvgDelete(void *ctx) {
    free(ctx);
}

NxColor nxvgColor(GLfloat r, GLfloat g, GLfloat b, GLfloat a) {
    NxColor c = {r, g, b, a};
    return c;
}

int gSegSize(NxSegmentType type) {
    switch (type) {
    case NX_LINE: return 3;
    case NX_QUADRATIC: return 3;
    case NX_BEZIER: return 4;
    }

    FAIL("Unhandled segment type.");
    return 0;
}

//class GlCornersConf: ShaderConf {
//    GLfloat m_cornersx[4];
//    GLfloat m_cornersy[4];
//    int m_n;
//
//public:
//    GlCornersConf(int ncorners, GLfloat skip, GLfloat *data) {
//        m_n = ncorners;
//        for (int i=0, idx=0; idx<ncorners; i+=skip+2, idx++) {
//            m_cornersx[idx] = data[i];
//            m_cornersy[idx] = data[i+1];
//        }
//
//        for (int i=0; i<3; i++) {
//            printf(": %f, %f\n", m_cornersx[i], m_cornersy[i]);
//        }
//    }
//
//    virtual void apply(GLuint program) {
//        GLint loc = glGetUniformLocation(program, "cornersx");
//        if (loc == -1) {
//            FAIL("Uniform `cornersx` not available in the program.");
//        }
//
//        printf("Reso: %f %f %f %f\n", m_Reso.r, m_Reso.g, m_Reso.b, m_Reso.a);
//        glUniform1fv(loc, m_n, m_cornersx);
//
//        loc = glGetUniformLocation(program, "cornersy");
//        if (loc == -1) {
//            FAIL("Uniform `cornersy` not available in the program.");
//        }
//
//        printf("Reso: %f %f %f %f\n", m_Reso.r, m_Reso.g, m_Reso.b, m_Reso.a);
//        glUniform1fv(loc, m_n, m_cornersy);
//    }
//};

void gDrawSegment(Nxvg *ctx, NxSegmentType type, GLfloat *points, int size, NxColor color) {
//    for (int i=0; i<size; i++) {
//        printf("%d (%f, %f)\n", i, points[i*2], points[i*2+1]);
//    }
    GlColorConf colorConf(color);
    GlColorConf rcolorConf(nxvgColor(1, 0, 0, 1));
    GlColorConf dcolorConf(nxvgColor(0, 0, 0, 1));
    GlResoConf resoConf(ctx->resX, ctx->resY);
    auto drawQuad = [] (Program & sp) {
        sp.drawArray(0, 3);
        sp.drawArray(1, 3);
    };

    ctx->fbo.clear();

    if (type == NX_QUADRATIC) {
        ctx->fbo.bind(); //{
            auto sp = &(ctx->quadSh);
            sp->bind(); //{
                GLfloat vertices[] = {
                    points[0], points[1], 0.0f, 0.0f,
                    points[2], points[3], 0.5f, 0.0f,
                    points[4], points[5], 1.0f, 1.0f
                };
                sp->uploadData(vertices, sizeof(vertices));
                sp->drawArray(0, 3);
            //}
        //}
        ctx->fbo.unbind();
    } else if (type == NX_LINE) {
//        ctx->fbo.bind(); //{
//            GLfloat corners[] = {
//                    points[0], points[1],
//                    points[2], points[3],
//                    points[4], points[5],
//                };
//            GlCornersConf cc(3, 0, corners);

            auto sp = &(ctx->simpleSh);
            sp->bind(&dcolorConf); //{
                GLfloat vertices[] = {
                    points[0], points[1],
                    points[2], points[3],
                    points[4], points[5],
                };
//                for (int i=0; i<3; i++) {
//                    printf("v: %f %f\n", points[i*2], points[i*2+1]);
//                }
                sp->uploadData(vertices, sizeof(vertices));
                sp->drawArray(0, 3);
            //}
        //}
        ctx->fbo.unbind();
    }

    ctx->fbo.bind(); //{
        ctx->fbo.nextPass();
        ctx->oblurvSh.bind(&resoConf); //{
            drawQuad(ctx->oblurvSh);
        //}
        ctx->fbo.nextPass();
        ctx->oblurhSh.bind(&resoConf); //{
            drawQuad(ctx->oblurhSh);
        //}
        for (int i=0; i<2; i++) {
            //continue;
            ctx->fbo.nextPass();
            ctx->blurvSh.bind(&resoConf); //{
                drawQuad(ctx->blurvSh);
            //}
            ctx->fbo.nextPass();
            ctx->blurhSh.bind(&resoConf); //{
                drawQuad(ctx->blurhSh);
            //}
        }
    //}
    ctx->fbo.unbind();

    ctx->fillSh.bind(&colorConf); //{
            drawQuad(ctx->fillSh);
    //}

//    auto sp = &(ctx->simpleSh);
//    sp->bind(&rcolorConf); //{
//        GLfloat vertices[] = {
//            points[0], points[1],
//            points[2], points[3],
//            points[4], points[5],
//        };
//    sp->uploadData(vertices, sizeof(vertices));
//    sp->drawArray(0, 3);
}

void nxvgDrawPath(void *ctx, GLfloat *points, NxSegmentType *types, int seglen, NxColor color) {
    Nxvg *nxvg = (Nxvg *)ctx;

    int pIdx = 0;
    for (int i=0; i<seglen; i++) {
        int segsize = gSegSize(types[i]);
        GLfloat segpts[MAX_SEGSIZE*2];
        for (int j=0; j<segsize; j++) {
            segpts[j*2] = points[pIdx];
            segpts[j*2+1] = points[pIdx+1];
            pIdx+=2;
        }
        gDrawSegment(nxvg, types[i], segpts, segsize, color);
    }
}
