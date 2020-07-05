#include "GLSLProgram.h"
GLSLProgram::GLSLProgram() {
    linked = false;
    logString = "";
    handle = glCreateProgram();
    if( 0 == handle ) {
        std::cout<< "Error creating shader program\n";
    }
}
void GLSLProgram::printLog() {
    if(logString != "" ) {
        std::cout<<logString<<std::endl;
    }
    std::cout<<"No log found"<<std::endl;
}
bool GLSLProgram::compileShaderFromFile(const char * fileName,GLSLShader::GLSLShaderType type) {
    if(handle == 0) {
        std::cout<<"Program handle is null\n";
        return false;
    }
    GLuint vertShader = glCreateShader(type);
    if( 0 == vertShader ) {
        std::cout<<"Error creating shader.\n";
        return false;
    }
    std::string vertShaderStr = readFile(fileName);
    if(vertShaderStr == "") {
        std::cout<<"Couldn't open shader file"<<std::endl;
        return false;
    }
    const GLchar * shaderCode = vertShaderStr.c_str();
    const GLchar* codeArray[] = {shaderCode};

    glShaderSource( vertShader, 1, codeArray, NULL );
    glCompileShader( vertShader );

    GLint result;
    glGetShaderiv( vertShader, GL_COMPILE_STATUS, &result );
    if( GL_FALSE == result ) {
        std::cout<<"Vertex shader compilation failed!\n";
        GLint logLen;
        glGetShaderiv( vertShader, GL_INFO_LOG_LENGTH, &logLen );
        if( logLen > 0 ) {
            char * log = new char[logLen];
            GLsizei written;
            glGetShaderInfoLog(vertShader, logLen, &written, log);
            logString = log;
            delete[] log;
            return false;
        }
    }
    glAttachShader( handle, vertShader );
    return true;
}
std::string GLSLProgram::readFile(const char *filePath) {
    std::string content;
    std::ifstream fileStream(filePath);

    if(!fileStream.is_open()) {
        std::cout << "Could not read file " << filePath << ". File does not exist." << std::endl;
        return "";
    }

    std::string line = "";
    while(!fileStream.eof()) {
        std::getline(fileStream, line);
        content.append(line + "\n");
    }

    fileStream.close();
    return content;
}
bool GLSLProgram::link() {
    if(linked) {
        std::cout<<"warning: program already linked\n";
    }
    if(handle == 0) {
        std::cout<<"Program handle is null\n";
        return false;
    }
    glLinkProgram( handle );
    GLint status;
    glGetProgramiv( handle, GL_LINK_STATUS, &status );
    if( GL_FALSE == status ) {
        std::cout<<"Failed to link shader program!\n";
        GLint logLen;
        glGetProgramiv(handle, GL_INFO_LOG_LENGTH,&logLen);
        if( logLen > 0 ) {
            char * log = new char[logLen];
            GLsizei written;
            glGetProgramInfoLog(handle, logLen,
                                &written, log);
            logString = log;
            delete[] log;
        }
    }
    linked = true;
    return true;
}
void GLSLProgram::use() {
    if(!linked) {
        std::cout<<"Error, Program not linked\n";
    } else if(handle == 0) {
        std::cout<<"Program handle is null\n";
    } else {
        glUseProgram( handle );
    }
}
int GLSLProgram::getHandle() {
    if(handle == 0) {
        std::cout<<"Program handle is null\n";
        return 0;
    }
    return handle;
}
bool GLSLProgram::isLinked() {
    return linked;
}
void GLSLProgram::bindAttribLocation(GLuint location, const char * name) {
    if(handle == 0) {
        std::cout<<"Program handle is null\n";
    } else if(linked) {
        std::cout<<"Error, Program already linked\n";
    } else {
        glBindAttribLocation(handle, location, name);
    }
}
void GLSLProgram::bindFragDataLocation(GLuint location, const char * name) {
    if(handle == 0) {
        std::cout<<"Program handle is null\n";
    } else if(linked) {
        std::cout<<"Error, Program already linked\n";
    } else {
        glBindFragDataLocation(handle, location, name);
    }
}
int GLSLProgram::getUniformLocation(const char * name ) {
    if(handle == 0) {
        std::cout<<"Program handle is null\n";
        return 0;
    }
    return glGetUniformLocation(handle,name);
}
void GLSLProgram::printActiveAttribs() {
    if(linked) {
        std::cout<<"warning: program already linked\n";
    }
    if(handle == 0) {
        std::cout<<"Program handle is null\n";
    } else {
        GLint maxLength, nAttribs;
        glGetProgramiv(handle, GL_ACTIVE_ATTRIBUTES,
                       &nAttribs);
        glGetProgramiv(handle, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH,
                       &maxLength);

        GLchar * name = new GLchar[maxLength];
        GLint sizee, location;
        GLsizei written;
        GLenum type;
        std::cout<<" Index | Name\n";
        std::cout<<"------------------------------------------------\n";
        for( int i = 0; i < nAttribs; i++ ) {
            glGetActiveAttrib( handle, i, maxLength, &written,
                               &sizee, &type, name );
            location = glGetAttribLocation(handle, name);
            std::cout<<location<<" | "<<name<<std::endl;
        }
        delete[] name;

    }
}
void GLSLProgram::printActiveUniforms() {
    if(linked) {
        std::cout<<"warning: program already linked\n";
    }
    if(handle == 0) {
        std::cout<<"Program handle is null\n";
    } else {
        GLint nUniforms, maxLength;
        glGetProgramiv( handle, GL_ACTIVE_UNIFORM_MAX_LENGTH,
                        &maxLength);
        glGetProgramiv( handle, GL_ACTIVE_UNIFORMS,
                        &nUniforms);

        GLchar * name = new GLchar[maxLength];
        GLint sizee, location;
        GLsizei written;
        GLenum type;
        std::cout<<" Location | Name\n";
        std::cout<<"------------------------------------------------\n";
        for( int i = 0; i < nUniforms; i++ ) {
            glGetActiveUniform( handle, i, maxLength, &written,
                                &sizee, &type, name );
            location = glGetUniformLocation(handle, name);
            std::cout<<location<<" | "<<name<<std::endl;
        }
        delete[] name;

    }
}
void GLSLProgram::setUniform(const char *name, const glm::vec3 & v) {
    if(handle == 0) {
        std::cout<<"Program handle is null\n";
    } else {
        GLuint location =getUniformLocation(name);
        if( location >= 0 ) {
            glUniform3fv(location, 1,&v[0]);
        }
    }
}
void GLSLProgram::setUniform(const char *name, const glm::vec4 & v) {
    if(handle == 0) {
        std::cout<<"Program handle is null\n";
    } else {
        GLuint location =getUniformLocation(name);
        if( location >= 0 ) {
            glUniform4fv(location, 1,&v[0]);
        }
    }
}
void GLSLProgram::setUniform(const char *name, const glm::mat3 & v) {
    if(handle == 0) {
        std::cout<<"Program handle is null\n";
    }else {
        GLuint location =getUniformLocation(name);
        if( location >= 0 ) {
            glUniformMatrix3fv(location, 1, GL_FALSE,&v[0][0]);
        }
    }
}
void GLSLProgram::setUniform(const char *name, const glm::mat4 & v) {
    if(handle == 0) {
        std::cout<<"Program handle is null\n";
    } else {
        GLuint location =getUniformLocation(name);
        if( location >= 0 ) {
            glUniformMatrix4fv(location, 1, GL_FALSE,&v[0][0]);
        }
    }
}
void GLSLProgram::setUniform(const char *name, float val) {
    if(handle == 0) {
        std::cout<<"Program handle is null\n";
    } else {
        GLuint location =getUniformLocation(name);
        if( location >= 0 ) {
            glUniform1f(location, val);
        }
    }
}
void GLSLProgram::setUniform(const char *name, int val) {
    if(handle == 0) {
        std::cout<<"Program handle is null\n";
    } else {
        GLuint location =getUniformLocation(name);
        if( location >= 0 ) {
            glUniform1i(location, val);
        }
    }
}
