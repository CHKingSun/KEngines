//
// Created by KingSun on 2018/07/11
//

#ifndef KENGINES_DEBUG_H
#define KENGINES_DEBUG_H

//Stream header
#include <iostream>
#include <iomanip>
#include <cstdio>
#include <string>

namespace KEngines {
	class Log {
	public:
		template <typename... Ts>
		static void d(const char* format, Ts... args) {
			//Pay more attention to format,
			//when you use %f for integer in release mode,
			//you will get the wrong answer.
			printf(format, args...);
		}

		template <typename... Ts>
		static void i(const char* format, Ts... args) {
			//later will use file to save
			printf(format, args...);
		}

		template <typename... Ts>
		static void e(const char* format, Ts... args) {
			//later will use file to save
			printf(format, args...);
		}

		static void debug() {
			std::clog << std::endl;
		}

		template <typename T>
		static void debug(const T& value) {
			std::clog << value << std::endl;
			//std::clog << value;
			//debug();
		}

		template <typename T, typename... Ts>
		static void debug(const T& value, const Ts&... args) {
			std::clog << value;
			debug(args...);
		}

		static void info() {
			std::cout << std::endl;
		}

		template <typename T>
		static void info(const T& value) {
			std::cout << value << std::endl;
		}

		template <typename T, typename... Ts>
		static void info(const T& value, const Ts&... args) {
			std::cout << value;
			info(args...);
		}

		static void error() {
			std::cerr << std::endl;
		}

		template <typename T>
		static void error(const T& value) {
			std::cerr << value << std::endl;
		}

		template <typename T, typename... Ts>
		static void error(const T& value, const Ts&... args) {
			std::cerr << value;
			error(args...);
		}
	};

}

#define KDEBUG
#ifdef KDEBUG

//Debug
//OpenGL header
#include <cassert>
#include <GL/glew.h>
#define	glCall(x)	x; \
					glCheckError(#x, __FILE__, __LINE__)

inline void glClearError() { while (glGetError() != GL_NO_ERROR); } //discard all errors;
inline bool glCheckError(const char* fun, const char* file, int line) {
	GLenum error;
	if ((error = glGetError()) != GL_NO_ERROR) {
		KEngines::Log::error("OpenGL error at: ", fun, " in file: ", file, " at line: ",
			line, " with error code: 0x0", std::setbase(16), error);
		return false;
	}
	return true;
}


static void GLAPIENTRY glDebugOutput(GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar *message,
	const void *userParam)
{
	if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;
	// ignore these non-significant error codes

	KEngines::Log::debug("---------------");
	KEngines::Log::debug("Debug message (0x", std::setbase(16), std::setiosflags(std::ios::uppercase), id, "): ", message);

	switch (source)
	{
	case GL_DEBUG_SOURCE_API:             KEngines::Log::debug("Source: API"); break;
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   KEngines::Log::debug("Source: Window System"); break;
	case GL_DEBUG_SOURCE_SHADER_COMPILER: KEngines::Log::debug("Source: Shader Compiler"); break;
	case GL_DEBUG_SOURCE_THIRD_PARTY:     KEngines::Log::debug("Source: Third Party"); break;
	case GL_DEBUG_SOURCE_APPLICATION:     KEngines::Log::debug("Source: Application"); break;
	case GL_DEBUG_SOURCE_OTHER:           KEngines::Log::debug("Source: Other"); break;
	}

	switch (type)
	{
	case GL_DEBUG_TYPE_ERROR:               KEngines::Log::debug("Type: Error"); break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: KEngines::Log::debug("Type: Deprecated Behaviour"); break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  KEngines::Log::debug("Type: Undefined Behaviour"); break;
	case GL_DEBUG_TYPE_PORTABILITY:         KEngines::Log::debug("Type: Portability"); break;
	case GL_DEBUG_TYPE_PERFORMANCE:         KEngines::Log::debug("Type: Performance"); break;
	case GL_DEBUG_TYPE_MARKER:              KEngines::Log::debug("Type: Marker"); break;
	case GL_DEBUG_TYPE_PUSH_GROUP:          KEngines::Log::debug("Type: Push Group"); break;
	case GL_DEBUG_TYPE_POP_GROUP:           KEngines::Log::debug("Type: Pop Group"); break;
	case GL_DEBUG_TYPE_OTHER:               KEngines::Log::debug("Type: Other"); break;
	}

	switch (severity)
	{
	case GL_DEBUG_SEVERITY_HIGH:         KEngines::Log::debug("Severity: high"); break;
	case GL_DEBUG_SEVERITY_MEDIUM:       KEngines::Log::debug("Severity: medium"); break;
	case GL_DEBUG_SEVERITY_LOW:          KEngines::Log::debug("Severity: low"); break;
	case GL_DEBUG_SEVERITY_NOTIFICATION: KEngines::Log::debug("Severity: notification"); break;
	}
	KEngines::Log::debug();
	
	//assert(GL_DEBUG_TYPE_ERROR != type);
	assert(false);
}

#else

#define glCall(x) x

#endif // KDEBUG

#endif // !KENGINES_DEBUG_H

