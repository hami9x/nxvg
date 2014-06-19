/*This piece of code was originally from Lazy Foo' Productions
(http://lazyfoo.net/)*/
#include "shader.h"
#include <stdlib.h>
#include <stdio.h>
#include <cstdarg>
#include <fstream>
#include <iostream>

using namespace std;

void printShaderLog(GLuint shader) {
    //Make sure name is shader
    if(glIsShader(shader)) {
        //Shader log length
        int infoLogLength = 0;
        int maxLength = infoLogLength;

        //Get info string length
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

        //Allocate string
        char* infoLog = new char[maxLength];

        //Get info log
        glGetShaderInfoLog(shader, maxLength, &infoLogLength, infoLog);
        if(infoLogLength > 0) {
            //Print Log
            printf("%s\n", infoLog);
        }

        //Deallocate string
        delete[] infoLog;
    }
    else {
        printf("Name %d is not a shader\n", shader);
    }
}

GLuint loadShaderFromFile(std::string path, GLenum shaderType) {
    //Open file
    GLuint shaderID = 0;
    std::string shaderString;
    std::ifstream sourceFile(path.c_str());

    //Source file loaded
    if(sourceFile) {
        //Get shader source
        shaderString.assign((std::istreambuf_iterator< char >(sourceFile)), std::istreambuf_iterator< char >());
        //Create shader ID
        shaderID = glCreateShader(shaderType);

        //Set shader source
        const GLchar* shaderSource = shaderString.c_str();
        glShaderSource(shaderID, 1, (const GLchar**)&shaderSource, NULL);

        //Compile shader source
        glCompileShader(shaderID);

        //Check shader for errors
        GLint shaderCompiled = GL_FALSE;
        glGetShaderiv(shaderID, GL_COMPILE_STATUS, &shaderCompiled);
        if(shaderCompiled != GL_TRUE) {
            printf("Unable to compile shader %d!\n\nSource:\n%s\n", shaderID, shaderSource);
            printShaderLog(shaderID);
            glDeleteShader(shaderID);
            shaderID = 0;
        }
    }
    else {
        printf("Unable to open file %s\n", path.c_str());
    }

    return shaderID;
}

Program::Program(): m_program(0), m_vao(0), m_vbo(0) {
}

Program::Program(const char *vshader, const char *fshader): m_program(0), m_vao(0), m_vbo(0) {
    std::string shaderBase("shaders/");
    glGenVertexArrays(1, &m_vao);

    // Create a Vertex Buffer Object and copy the vertex data to it
    glGenBuffers(1, &m_vbo);

    GLuint vertexShader = loadShaderFromFile(shaderBase+vshader, GL_VERTEX_SHADER);
    GLuint fragmentShader = loadShaderFromFile(shaderBase+fshader, GL_FRAGMENT_SHADER);

    m_program = glCreateProgram();
    glAttachShader(m_program, vertexShader);
    glAttachShader(m_program, fragmentShader);
    //glBindFragDataLocation(m_program, 0, "outColor");
    glLinkProgram(m_program);
    glUseProgram(m_program);
    GLCHECK("Could not use the shader.");
    glDetachShader(m_program, vertexShader);
	glDetachShader(m_program, fragmentShader);
}

Program::~Program() {
    glDeleteBuffers(1, &m_vao);
    glDeleteBuffers(1, &m_vbo);
}

void Program::bind() const {
    if (m_program == 0) {
        FAIL("Program not initialized.");
    }

    glUseProgram(m_program);
    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
}

void Program::bind(const ShaderConf & conf) const {
    bind();
    conf.apply(m_program);
}

void Program::bind(const vector<const ShaderConf *> & confs) const {
    bind();
    for (auto & conf: confs) {
        conf->apply(m_program);
    }
}

void Program::addAttrib(const char * name, int size, int stride, int offset) const {
    GLint attr = glGetAttribLocation(m_program, name);
    glVertexAttribPointer(attr, size, GL_FLOAT, GL_FALSE, stride*sizeof(GLfloat), (void*)(offset*sizeof(GLfloat)));
    glEnableVertexAttribArray(attr);
}

void Program::uploadData(GLfloat * a, int size) const {
    glBufferData(GL_ARRAY_BUFFER, size, a, GL_STATIC_DRAW);
}

void Program::drawArray(int start, int num) const {
    glDrawArrays(GL_TRIANGLES, start, num);
}
