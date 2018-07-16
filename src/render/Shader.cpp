#include "Shader.h"

#include <cstdio>

namespace KEngines { namespace KRenderer {
	Shader::Shader(): uniforms(new std::unordered_map<std::string, Kint>()) {
		program = glCreateProgram();
		if (program == 0) {
			Log::error("Create program failed!");
		}
	}

	Shader::Shader(const std::string& vs_filename, const std::string& fs_filename):
	uniforms(new std::unordered_map<std::string, Kint>()) {
		createProgram(vs_filename, fs_filename);
	}
	Shader::~Shader() {
		uniforms->clear();
		delete uniforms;
		if (isValid()) glDeleteProgram(program);
	}

	Kboolean Shader::readFile(const std::string& filename, std::string& content) const {
#if 1
		FILE* file = std::fopen(filename.c_str(), "rt"); //read text
		if (file == nullptr) {
			Log::error("File ", filename, " read failed!");
			return false;
		}
		std::fseek(file, 0, SEEK_END);
		unsigned long length = std::ftell(file) + 1;
		auto *data = new char[length];
		memset(data, 0, length);
		std::fseek(file, 0, SEEK_SET);
		std::fread(data, 1, length, file);
		std::fclose(file);
		content = data;
		//std::cout << content << std::endl;
		delete[] data;
		return true;
#else
		std::ifstream file(filename, std::ios::in);
		if (!file.is_open())
		{
			Log::error("File ", filename, " read failed!");
			return false;
		}
		content = std::string(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
#endif
	}

	Kint Shader::getLocation(const char* name) const {
		auto it = uniforms->find(name);
		if (it == uniforms->end()) {
			Kint loc = glGetUniformLocation(program, name);
			uniforms->emplace(name, loc);
			if (loc == -1) Log::error("Wrong unifrom name: ", name);
			return loc;
		}
		return it->second;
	}

	Kuint Shader::createShader(GLenum type, const std::string& filename)const {
		Kuint shader = glCreateShader(type);
		if (shader == 0) {
			Log::error("Create shader failed with type: ", type);
			return shader;
		}

		std::string content;
		if (!readFile(filename, content)) {
			Log::error("Read file: ", filename, " failed!");
			glDeleteShader(shader);
			return 0;
		}

		const char *sources = content.c_str();
		glShaderSource(shader, 1, &sources, nullptr);
		glCompileShader(shader);
		//delete[] sources; make sure to do not delete data in content string
		sources = nullptr;

		GLint flag;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &flag);
		if (flag == GL_FALSE) {
			Kint length;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
			GLchar error_log[1024];
			glGetShaderInfoLog(shader, length, nullptr, error_log);
			Log::error("Error compiling ",
				(type == GL_VERTEX_SHADER ? "vectex" : "fragment"),
				" shader: \n", error_log);
			glDeleteShader(shader);
			return 0;
		}

		return shader;
	}

	Kboolean Shader::addShader(Kuint shader)const {
		if (!glIsShader(shader) || !isValid()) return false;

		glAttachShader(program, shader);
		glLinkProgram(program);

		GLint flag;
		GLchar error_log[1024];
		glGetProgramiv(program, GL_LINK_STATUS, &flag);
		if (GL_FALSE == flag) {
			glGetProgramInfoLog(program, 1024, nullptr, error_log);
			Log::error("Error linking program: ", error_log);
			return false;
		}

		return true;
	}

	Kboolean Shader::addShader(GLenum type, const std::string& filename)const {
		Kuint shader = createShader(type, filename);
		if (glIsShader(shader)) {
			Kboolean flag = addShader(shader);
			glDeleteShader(shader);
			return flag;
		}
		return false;
	}

	Kboolean Shader::createProgram(const std::string& vs_filename, const std::string& fs_filename) {
		program = glCreateProgram();
		if (program == 0) {
			Log::error("Create program failed!");
			return false;
		}

		Kuint vert_shader = createShader(GL_VERTEX_SHADER, vs_filename);
		Kuint frag_shader = createShader(GL_FRAGMENT_SHADER, fs_filename);
		if (!glIsShader(vert_shader) || !glIsShader(frag_shader))return false;

		glAttachShader(program, vert_shader);
		glAttachShader(program, frag_shader);
		glLinkProgram(program);

		glDeleteShader(vert_shader);
		glDeleteShader(frag_shader);

		GLint flag;
		GLchar error_log[1024];
		glGetProgramiv(program, GL_LINK_STATUS, &flag);
		if (GL_FALSE == flag) {
			glGetProgramInfoLog(program, 1024, nullptr, error_log);
			Log::error("Error linking program: ", error_log);
			glDeleteProgram(program);
			return false;
		}

		glValidateProgram(program);
		glGetProgramiv(program, GL_VALIDATE_STATUS, &flag);
		if (GL_FALSE == flag) {
			glGetProgramInfoLog(program, 1024, nullptr, error_log);
			Log::error("Program valid: ", error_log);
			glDeleteProgram(program);
			return false;
		}
		return true;
	}

} }
