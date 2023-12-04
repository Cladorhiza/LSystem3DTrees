#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>

#include "glm.hpp"
#include "gtc/matrix_transform.hpp"

struct ShaderProgramSource {

	std::string vertexSource;
	std::string fragmentSource;
	std::string geometrySource;

};

class Shader {
private:
	std::string filePath;
	unsigned int rendererID;
	std::unordered_map<std::string, int> m_UniformLocationCache;
public:
	Shader(const std::string& filepath);
	~Shader();

	void Bind() const;
	void Unbind() const;

	//Set uniforms
	void SetUniformMat4f(const std::string& name, const glm::mat4& matrix);
	void SetUniform1iv(const std::string& name, const int size, const int* const data);
	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
	void SetUniformvec3f(const std::string& name, float v0, float v1, float v2);
	void SetUniform1i(const std::string& name, int vi);
	void SetUniform1f(const std::string& name, float f);
private:
	unsigned int CompileShader(unsigned int type, const std::string& source);
	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader, const std::string& geometryShader);
	ShaderProgramSource ParseShader(const std::string& filepath);
	int GetUniformLocation(const std::string& name);
};