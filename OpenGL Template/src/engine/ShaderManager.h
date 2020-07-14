#pragma once
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <array>
#include <unordered_map>
#include <glm\matrix.hpp>
#include <glm\gtc\type_ptr.hpp>

struct ShaderPathList {
	const std::string shaderPath;
	const std::string vertexPath;
	const std::string fragmentPath;
};

class ShaderProgram {
private:
	const bool m_SingleFileShader;
	const ShaderPathList m_Paths;
	unsigned int m_ID;
	std::unordered_map<std::string, int> m_Uniforms;
	
	int getUniform(const char* name);
public:
	ShaderProgram(const char* shaderPath);
	ShaderProgram(const char* vertexPath, const char* fragmentPath);
	~ShaderProgram();

	void setUniform1f(const char* name, float val);
	void setUniform1d(const char* name, double val);
	void setUniformMat4fv(const char* name, glm::mat4&);
	void setUniformBool(const char* name, bool val);
	void setUniformVec2(const char*, glm::vec2& val);
	void Bind();
	void Unbind();
};