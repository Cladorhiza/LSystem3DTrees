#include "Shader.h"
#include <GL/glew.h>

#include "external_code/glm/gtc/type_ptr.hpp"

Shader::Shader(const std::string& filepath)
	: filePath(filepath), rendererID(0)
{
	ShaderProgramSource source = ParseShader(filepath);
	rendererID = CreateShader(source.vertexSource, source.fragmentSource, source.geometrySource);
}

Shader::~Shader()
{
	glDeleteProgram(rendererID);
}

void Shader::Bind() const
{
	glUseProgram(rendererID);
}

void Shader::Unbind() const
{
	glUseProgram(0);
}

void Shader::SetUniformMat4f(const std::string& name, const glm::mat4& matrix) {

	glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(matrix));
}


void Shader::SetUniform1iv(const std::string& name, const int size, const int* const data) {

	glUniform1iv(GetUniformLocation(name), 32, data);
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
	glUniform4f(GetUniformLocation(name), v0, v1, v2, v3);
}

void Shader::SetUniformvec3f(const std::string& name, float v0, float v1, float v2) {
	glUniform3f(GetUniformLocation(name), v0, v1, v2);
}

void Shader::SetUniform1i(const std::string& name, int vi) {
	glUniform1i(GetUniformLocation(name), vi);
}

void Shader::SetUniform1f(const std::string& name, float f) {
	glUniform1f(GetUniformLocation(name), f);
}

int Shader::GetUniformLocation(const std::string& name)
{
	if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
		return m_UniformLocationCache[name];

	int location = glGetUniformLocation(rendererID, name.c_str());

	if (location == -1)
		std::cout << "Warning: Uniform '" << name << "' doesn't exist!" << std::endl;

	m_UniformLocationCache[name] = location;
	return location;
}

ShaderProgramSource Shader::ParseShader(const std::string& filepath) {

	std::ifstream stream(filepath);

	enum class ShaderType {
		NONE = -1, VERTEX = 0, FRAGMENT = 1, GEOMETRY = 2
	};

	std::string line;
	std::stringstream ss[3];
	ShaderType type = ShaderType::NONE;
	while (getline(stream, line)) {

		if (line.find("#shader") != std::string::npos) {

			if (line.find("vertex") != std::string::npos) {
				type = ShaderType::VERTEX;
			}
			else if (line.find("fragment") != std::string::npos) {
				type = ShaderType::FRAGMENT;
			}
			else if (line.find("geometry") != std::string::npos) {
				type = ShaderType::GEOMETRY;
			}

		}
		else {
			ss[(int)type] << line << '\n';
		}

	}
	return { ss[0].str(), ss[1].str(), ss[2].str() };

}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source) {

	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE) {
		int length = 0;
		char message[4096];
		glGetShaderInfoLog(id, 4096, &length, message);
		std::string shaderType = "";
		if (type == GL_VERTEX_SHADER)
			shaderType = "vertex";
		else if (type == GL_FRAGMENT_SHADER)
			shaderType = "fragment";
		else if (type == GL_GEOMETRY_SHADER)
			shaderType = "geometry";
		std::cout << "Failed to compile " << shaderType << " shader: " << message << std::endl;
		glDeleteShader(id);
		return 0;
	}

	return id;

}

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader, const std::string& geometryShader) {

	unsigned int program = glCreateProgram();
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);
	unsigned int gs = 0;
	if (!geometryShader.empty())
		gs = CompileShader(GL_GEOMETRY_SHADER, geometryShader);
	

	glAttachShader(program, vs);
	glAttachShader(program, fs);
	if (gs != 0) glAttachShader(program, gs);
	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(vs);
	glDeleteShader(fs);
	glDeleteShader(gs);

	return program;

}