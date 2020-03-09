#include "ShaderProgram.h"
#include "Shaders.h"
#include <iostream>

using namespace std;
ShaderProgram *shProgram;

ShaderProgram::ShaderProgram()
{
	cout << "Loading vertex shader..." << endl;
	m_vertexShader = loadShader(GL_VERTEX_SHADER);

	cout << "Loading fragment shader..." << endl;
	m_fragmentShader = loadShader(GL_FRAGMENT_SHADER);


	m_shaderProgram = glCreateProgram();


	glAttachShader(m_shaderProgram, m_vertexShader);
	glAttachShader(m_shaderProgram, m_fragmentShader);


	glLinkProgram(m_shaderProgram);

	int infologLength = 0;
	int charsWritten  = 0;
	char *infoLog;

	glGetProgramiv(m_shaderProgram, GL_INFO_LOG_LENGTH, &infologLength);

	if (infologLength > 1)
	{
		infoLog = new char[infologLength];
		glGetProgramInfoLog(m_shaderProgram, infologLength, &charsWritten, infoLog);
		cout << infoLog << endl;
		delete []infoLog;
	}

	cout << "Shader program created" << endl;
}

GLuint ShaderProgram::loadShader(GLenum shaderType)
{
	GLuint shader;
    shader = glCreateShader(shaderType);

	switch(shaderType)
	{
        case GL_VERTEX_SHADER:
            glShaderSource(shader, 1, &vertexShaderCode, NULL);
        break;

        case GL_FRAGMENT_SHADER:
            glShaderSource(shader, 1, &fragmentShaderCode, NULL);
        break;

        default:
            cerr << "Wrong shader type to load" << endl;
            abort();
	}

	glCompileShader(shader);

	int infologLength = 0;
	int charsWritten  = 0;
	char *infoLog;

	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infologLength);

	if(infologLength > 1)
    {
		infoLog = new char[infologLength];
		glGetShaderInfoLog(shader, infologLength, &charsWritten, infoLog);
        cerr << infoLog << endl;
		delete []infoLog;
	}

	return shader;
}

void ShaderProgram::use()
{
	glUseProgram(m_shaderProgram);
}

GLuint ShaderProgram::getUniform(string variableName)
{
	return glGetUniformLocation(m_shaderProgram, variableName.c_str());
}

GLuint ShaderProgram::getAttribute(string variableName)
{
	return glGetAttribLocation(m_shaderProgram, variableName.c_str());
}

ShaderProgram::~ShaderProgram()
{
	glDetachShader(m_shaderProgram, m_vertexShader);
	glDetachShader(m_shaderProgram, m_fragmentShader);

	glDeleteShader(m_vertexShader);
	glDeleteShader(m_fragmentShader);

	glDeleteProgram(m_shaderProgram);
}
