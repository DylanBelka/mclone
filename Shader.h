#ifndef SHADER_H
#define SHADER_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include <glew.h>

class Shader
{
public:
	Shader() = default;
	Shader(std::string vertShaderPath, std::string fragShaderPath);
	~Shader();

	void use();
	void setUniformMat4(const std::string& name, const glm::mat4& trans) { glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(trans)); }
	void setUniformMat4(const GLuint uniformLoc, const glm::mat4& trans) { glUniformMatrix4fv(uniformLoc, 1, GL_FALSE, glm::value_ptr(trans)); }
	const GLuint getUniformLocation(const std::string& name) const { return glGetUniformLocation(ID, name.c_str()); }

private:
	unsigned int ID = -1;

	std::string loadShaderFromFile(std::string shaderFilePath);
	unsigned int compileShaderFromSrc(std::string shaderSrc, unsigned int shaderType);
};

void checkGlErr(int line);

#endif // SHADER_H