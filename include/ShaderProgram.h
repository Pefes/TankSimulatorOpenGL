#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include "GL/glew.h"
#include <string>

class ShaderProgram
{
public:
    ShaderProgram();
    ~ShaderProgram();

    void use();
    GLuint getUniform(std::string variableName);
    GLuint getAttribute(std::string variableName);

private:
    GLuint m_shaderProgram;
	GLuint m_vertexShader;
	GLuint m_fragmentShader;

    GLuint loadShader(GLenum shaderType);
};

extern ShaderProgram *shProgram;

#endif // SHADERPROGRAM_H
