/*This piece of code was originally from Lazy Foo' Productions
(http://lazyfoo.net/)*/
#include "shader.h"
#include <stdlib.h>
#include <stdio.h>
#include <cstdarg>
#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>

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

static inline std::string &ltrim(std::string &s) {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
        return s;
}

const std::string IMPORTDEF = "#include";
const std::string SHADERBASE = "shaders/";

void getShaderSource(std::stringstream & sstream, std::string filepath) {
    std::ifstream sourceFile(SHADERBASE+filepath);
    if (sourceFile) {
        for (std::string line; getline(sourceFile, line);) {
            if (line.substr(0, IMPORTDEF.size()) == IMPORTDEF) {
                auto ifile = line.substr(IMPORTDEF.size()+2, line.size()-IMPORTDEF.size()-3);
                getShaderSource(sstream, SHADERBASE+ifile);
            } else {
                sstream << line << std::endl;
            }
        }
    } else {
        printf("Unable to open file %s\n", filepath.c_str());
        FAIL("");
    }
}

GLuint loadShaderFromFile(std::string path, GLenum shaderType) {
    //Open file
    GLuint shaderID = 0;
    std::string shaderString;

    std::stringstream source;

    getShaderSource(source, path);
    //Get shader source
    shaderString = source.str();

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

    return shaderID;
}

Program::Program(): m_program(0), m_vao(0), m_vbo(0) {
}

Program::Program(const char *vshader, const char *fshader): m_program(0), m_vao(0), m_vbo(0) {
    glGenVertexArrays(1, &m_vao);

    // Create a Vertex Buffer Object and copy the vertex data to it
    glGenBuffers(1, &m_vbo);

    GLuint vertexShader = loadShaderFromFile(vshader, GL_VERTEX_SHADER);
    GLuint fragmentShader = loadShaderFromFile(fshader, GL_FRAGMENT_SHADER);

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
