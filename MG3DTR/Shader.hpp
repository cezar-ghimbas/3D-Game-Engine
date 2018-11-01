#include <string>
#include <fstream>
#include <vector>
#include <iostream>

namespace Engine {

	std::string ReadFile(const std::string &fileName)
	{
		std::ifstream fileIn(fileName);
		std::string ret;
		ret.assign(std::istreambuf_iterator<char>(fileIn), std::istreambuf_iterator<char>());

		return ret;
	}

	class Shader {
	public:

		Shader()
		{
		}

		Shader(const std::string &vertexShaderFile, const std::string &fragmentShaderFile)
		{
			std::string vertexShader   = ReadFile(vertexShaderFile);
			std::string fragmentShader = ReadFile(fragmentShaderFile);
			program = glCreateProgram();
			AttachShaderObject(vertexShader, GL_VERTEX_SHADER);
			AttachShaderObject(fragmentShader, GL_FRAGMENT_SHADER);
			glLinkProgram(program);
		}

		Shader(const std::string &vertexShaderFile, const std::string &geometryShaderFile, const std::string &fragmentShaderFile)
		{
			std::string vertexShader = ReadFile(vertexShaderFile);
			std::string geometryShader = ReadFile(geometryShaderFile);
			std::string fragmentShader = ReadFile(fragmentShaderFile);
			program = glCreateProgram();
			AttachShaderObject(vertexShader, GL_VERTEX_SHADER);
			AttachShaderObject(geometryShader, GL_GEOMETRY_SHADER);
			AttachShaderObject(fragmentShader, GL_FRAGMENT_SHADER);
			glLinkProgram(program);
		}

		unsigned int GetProgram() const
		{
			return program;
		}

		~Shader()
		{
			glDeleteProgram(program);
		}
	private:
		unsigned int program;
		
		void AttachShaderObject(const std::string &shader, unsigned int type)
		{
			unsigned int shaderObject = glCreateShader(type);
			int shaderSize = shader.size();
			const char *vertex_shader_ptr = shader.c_str();
			glShaderSource(shaderObject, 1, &vertex_shader_ptr, &shaderSize);
			glCompileShader(shaderObject);
			glAttachShader(program, shaderObject);

			int compileResult, infoLogLength;
			glGetShaderiv(shaderObject, GL_COMPILE_STATUS, &compileResult);

			if (compileResult == GL_FALSE) {
				glGetShaderiv(shaderObject, GL_INFO_LOG_LENGTH, &infoLogLength);
				std::vector<char> shaderLog(infoLogLength);
				glGetShaderInfoLog(shaderObject, infoLogLength, NULL, &shaderLog[0]);
				std::cout << &shaderLog[0] << std::endl;
			}
		}
	};



}