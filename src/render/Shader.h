//
// Created by KingSun on 2018/07/11
//

#ifndef KENGINES_SHADER_H
#define KENGINES_SHADER_H

#include <GL/glew.h>
#include <unordered_map>

#include "../KHeader.h"
#include "../Log.h"
#include "../math/matrix/mat4.h"

namespace KEngines { namespace KRenderer {
	static Kuint getSize(GLenum type) {
		switch (type)
		{
		case GL_FLOAT: return sizeof(GLfloat);
		case GL_UNSIGNED_INT: return sizeof(GLuint);
		case GL_UNSIGNED_SHORT: return sizeof(GLushort);
		case GL_UNSIGNED_BYTE: return sizeof(GLubyte);
		case GL_FLOAT_VEC3: return sizeof(GLfloat) * 4;
		case GL_FLOAT_VEC4: return sizeof(GLfloat) * 4;
		case GL_FLOAT_MAT3: return sizeof(GLfloat) * 12;
		//When we use mat3, it will divide into 3 vec3,
		//but when GLSL read the buffer of vec3, it will read 4 value and discard the last value,
		//thus, be sure to push mat4x3(3 vec4) data into the buffer or you may get what you don't want.
		case GL_FLOAT_MAT4: return sizeof(GLfloat) * 16;
		case GL_FLOAT_MAT2: return sizeof(GLfloat) * 4;
		case GL_FLOAT_VEC2: return sizeof(GLfloat) * 2;
		case GL_BOOL: return sizeof(GLboolean);
		case GL_INT: return sizeof(GLint);
		case GL_SHORT: return sizeof(GLshort);
		case GL_BYTE: return sizeof(GLbyte);
		case GL_DOUBLE: return sizeof(GLdouble);
		default: return 0;
		}
	}

	using namespace KVector;
	using namespace KMatrix;

	class Shader {

	private:
		Kuint program;
		std::unordered_map<std::string, Kint>* uniforms;

		bool readFile(const std::string& filename, std::string& content)const;

		Kint getLocation(const char* name)const;

	public:
		Shader();
		Shader(const std::string& vs_filename, const std::string& fs_filename);
		~Shader();

		inline Kboolean isValid()const {
			if (glIsProgram(program) != GL_TRUE) {
				Log::error("Program is not valid!");
				return false;
			}
			return true;
		}
		inline void bind()const {
			if (isValid()) glUseProgram(program);
		}
		inline void unBind()const {
			glUseProgram(0);
		}

		Kuint createShader(GLenum type, const std::string& filename)const;
		Kboolean addShader(Kuint shader)const;
		Kboolean addShader(GLenum type, const std::string& filename)const;
		Kboolean createProgram(const std::string& vs_filename, const std::string& fs_filename);

		inline Kint getAttribLocation(const char* name)const {
			if (!isValid()) return -2;
			return glGetAttribLocation(program, name);
		}

		inline void bindUniform1i(const char* name, Kint value)const {
			if (!isValid()) return;
			glUniform1i(getLocation(name), value);
		}

		inline void bindUniform1f(const char* name, Kfloat value)const {
			if (!isValid()) return;
			glUniform1f(getLocation(name), value);
		}

		inline void bindUniform2f(const char* name, const vec2& v)const {
			if (!isValid()) return;
			glUniform2fv(getLocation(name), 1, v.data());
			//note: if we set count = 2 and with 2 vector data in,
			//then this location will be set with v[0].data()
			//and next location will be set with v[1].data()
		}

		inline void bindUniform3f(const char* name, const vec3& v)const {
			if (!isValid()) return;
			glUniform3fv(getLocation(name), 1, v.data());
		}

		inline void bindUniform4f(const char* name, const vec4& v)const {
			if (!isValid()) return;
			glUniform4fv(getLocation(name), 1, v.data());
		}

		inline void bindUniformMat3f(const char* name, const mat3 &m)const {
			if (!isValid()) return;
			glUniformMatrix3fv(getLocation(name), 1, GL_TRUE, m.data()); //our matrix is row-first
		}

		inline void bindUniformMat4f(const char* name, const mat4 &m)const {
			if (!isValid()) return;
			glUniformMatrix4fv(getLocation(name), 1, GL_TRUE, m.data()); //our matrix is row-first
		}
	};

} }

#endif // !KENGINES_SHADER_H
