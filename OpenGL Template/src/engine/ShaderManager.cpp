#include <glad\glad.h>
#include "ShaderManager.h"
#include "Config.h"
#include "FileLoader.h"

enum {
	INVALID = -2, NONE = -1, VERTEX_SOURCE = 0, FRAGMENT_SOURCE = 1
};

struct ShaderSourceCode {
	std::string vertSrc;
	std::string fragSrc;
};

unsigned int loadShader(const std::string& source, unsigned int type)
{
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, NULL);
	glCompileShader(id);

	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);

	if (result == GL_FALSE) {
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)alloca(length * sizeof(char));

		glGetShaderInfoLog(id, length, &length, message);

		std::cout << "Error while compiling " << (type == GL_VERTEX_SHADER ? "Vertex Shader" : "Fragment Shader") << std::endl;
		std::cout << message << std::endl;

		glDeleteShader(id);
		return 0;
	}

	return id;
}
unsigned int createShader(ShaderSourceCode& sourceCode) {
	unsigned int program = glCreateProgram();
	unsigned int vs = loadShader(sourceCode.vertSrc, GL_VERTEX_SHADER);
	unsigned int fs = loadShader(sourceCode.fragSrc, GL_FRAGMENT_SHADER);

	glAttachShader(program, vs);
	glAttachShader(program, fs);

	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}
/**
	Loads a shader from one file, the shaders must be separated by type using `$vert` and `$frag`
*/
ShaderSourceCode loadShaderFromOneFile(const char* shaderPath) {
	std::fstream shaderFile(shaderPath);
	std::array<std::stringstream, 2> shaderSource;
	std::string line;
	int type = NONE;

	if (shaderFile.is_open()) {
		while (std::getline(shaderFile, line)) {
			if (line.find(c().VERTEX_START_STRING)) {
				type = VERTEX_SOURCE;
			}
			else if (line.find(c().FRAGMENT_START_STRING)) {
				type = FRAGMENT_SOURCE;
			}
			else {
				if (type == INVALID) {
					std::cerr << "ERROR, INVALID SHADER STATE SELECTED" << std::endl;
				}
				else if (type != NONE) {
					shaderSource[type] << line << "\n";
				}
			}
		}
	}
	else {
		printf("SHADERMANAGER  ERROR: No file exists at path: %s", shaderPath);
	}

	shaderFile.close();
	return {
		shaderSource[0].str(), shaderSource[1].str()
	};
}

/**
	Loads a vertex and fragment shader from two separate files, the shaders are then read and the source code for both shaders is returned
*/
ShaderSourceCode loadShaderFromTwoFiles(const char* vertPath, const char* fragPath) {
	return {
		readFile(vertPath), readFile(fragPath)
	};
}


/**
	ShaderProgram Class
*/
ShaderProgram::ShaderProgram(const char* shaderPath)
	: m_SingleFileShader(true), m_Paths({shaderPath, "", "" }), m_Uniforms()
{
	ShaderSourceCode sourceCode = loadShaderFromOneFile(shaderPath);
	m_ID =  createShader(sourceCode);
}
ShaderProgram::ShaderProgram(const char* vertexPath, const char* fragmentPath)
	: m_SingleFileShader(false), m_Paths({ "", vertexPath, fragmentPath})
{
	ShaderSourceCode sourceCode = loadShaderFromTwoFiles(vertexPath, fragmentPath);
	m_ID = createShader(sourceCode);
}

ShaderProgram::~ShaderProgram() {
	glUseProgram(0);
	glDeleteProgram(m_ID);
}

int ShaderProgram::getUniform(const char* name) {
	if (int loc = m_Uniforms[name]) {
		return loc;
	}
	int loc = glGetUniformLocation(m_ID, name);
	m_Uniforms[name] = loc;
	return loc;
}

void ShaderProgram::Bind() {
	glUseProgram(m_ID);
}

void ShaderProgram::Unbind() {
	glUseProgram(0);
}

void ShaderProgram::setUniform1f(const char* name, float val) {
	glUniform1fv(getUniform(name), 1, &val);
}

void ShaderProgram::setUniform1d(const char* name, double val) {
	glUniform1dv(getUniform(name), 1, &val);
}

void ShaderProgram::setUniformMat4fv(const char* name, glm::mat4& matrix) {
	glUniformMatrix4fv(getUniform(name), 1, GL_FALSE, glm::value_ptr(matrix));
}

void ShaderProgram::setUniformBool(const char* name, bool val) {
	const GLint numVal = val ? 1 : 0;
	glUniform1iv(getUniform(name), 1, &numVal);
}

void ShaderProgram::setUniformVec2(const char* name, glm::vec2& vector) {
	glUniform2fv(getUniform(name), 1, glm::value_ptr(vector));
}