#include "Shader.h"

Shader::Shader(std::string vertShaderPath, std::string fragShaderPath)
{
	std::string vertShaderSrc = loadShaderFromFile(vertShaderPath);
	std::string fragShaderSrc = loadShaderFromFile(fragShaderPath);

	unsigned int vertShader = compileShaderFromSrc(vertShaderSrc, GL_VERTEX_SHADER);
	unsigned int fragShader = compileShaderFromSrc(fragShaderSrc, GL_FRAGMENT_SHADER);

	if (ID == -1)
	{
		ID = glCreateProgram();
	}
	glAttachShader(ID, vertShader);
	glAttachObjectARB(ID, fragShader);
	glLinkProgram(ID);

	glUseProgram(ID);
	glDeleteShader(vertShader);
	glDeleteShader(fragShader);

	GLenum err = glGetError();

	if (err != GL_NO_ERROR)
	{
		std::cout << "OpenGL shader error. Error code: " << err << "\nPrinting log...\n\n";
		const int logLen = 1024;
		int len = 0;
		char log[logLen];
		glGetProgramInfoLog(ID, logLen, &len, log);
		std::cout << log << std::endl;
	}
}

Shader::~Shader()
{
	glDeleteProgram(ID);
}

void Shader::use()
{
	glUseProgram(ID);
}

std::string Shader::loadShaderFromFile(std::string shaderFilePath)
{
	std::ifstream shaderFileStream;
	std::string line; // current line
	shaderFileStream.open(shaderFilePath);
	
	if (!shaderFileStream.is_open())
	{
		if (!shaderFileStream.good())
		{
			std::cout << "failed to load file " << shaderFilePath << std::endl;
			return "FAILED TO LOAD FILE " + shaderFilePath;
		}
	}

	std::stringstream ss;
	ss << shaderFileStream.rdbuf();
	shaderFileStream.close();
	return ss.str();
}

unsigned int Shader::compileShaderFromSrc(std::string shaderSrc, unsigned int shaderType)
{
	unsigned int shaderID;
	const char* src = shaderSrc.c_str();
	shaderID = glCreateShader(shaderType);
	glShaderSource(shaderID, 1, &src, NULL);
	glCompileShader(shaderID);

	int success;
	char infoLog[512];
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shaderID, 512, NULL, infoLog);
		std::cout << "Error compiling shader type: " << shaderType << "\n" << infoLog << std::endl;
	}
	return shaderID;
}

void checkGlErr(int line)
{
	GLenum err = glGetError();

	if (err != GL_NO_ERROR)
	{
		std::cout << "OpenGL error code: " << err << " on line " << line << std::endl;
		system("pause");
	}
}