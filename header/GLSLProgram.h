#ifndef GLSLPROGRAM_H
#define GLSLPROGRAM_H
#include <string>
#include <GL/glew.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <fstream>
namespace GLSLShader {
enum GLSLShaderType {
    VERTEX = 0X8B31,
    FRAGMENT = 0X8B30,
    GEOMETRY = 0X8DD9,
    TESS_CONTROL = 0X8E88,
    TESS_EVALUATION = 0X8E87
};
};
class GLSLProgram {
  private:
    int handle;
    bool linked;
    std::string logString;
    int getUniformLocation(const char * name );
    std::string readFile(const char *filePath);
  public:
    GLSLProgram();
    bool compileShaderFromFile( const char * fileName,
                                GLSLShader::GLSLShaderType type );
    bool link();
    void use();
    void printLog();
    int getHandle();
    bool isLinked();
    void bindAttribLocation( GLuint location, const char * name);
    void bindFragDataLocation( GLuint location,
                               const char * name );
    void setUniform(const char *name,float x,float y,
                    float z);
    void setUniform(const char *name, const glm::vec3 & v);
    void setUniform(const char *name, const glm::vec4 & v);
    void setUniform(const char *name, const glm::mat4 & m);
    void setUniform(const char *name, const glm::mat3 & m);
    void setUniform(const char *name, float val );
    void setUniform(const char *name, int val );
    void printActiveUniforms();
    void printActiveAttribs();
};

#endif // GLSLPROGRAM_H
