#pragma once
#include <string>
#include <GL/glew.h>


class GLSLProgram
{
public:
	GLSLProgram();
	~GLSLProgram();

	//Read shaders from file and compile it
	void compileShaders(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath);

	//Link the shaders
	void linkShaders();

	void addAttribute(const std::string& attributeName);

	GLint getUniformLocation(const std::string& uniformName);

	void useProgram();

	void unuseProgram();

private:

	void compileShader(const std::string& filePath, GLuint id);

	//ID of the whole program
	GLuint _programID;

	GLuint _vertexShaderID;
	GLuint _fragmentShaderID;

	//Count the number of attributes
	int _numAttributes;
};

