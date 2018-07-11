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
			std::cout << std::endl;
		}

		template <typename T>
		static void debug(const T& value) {
			std::cout << value << std::endl;
			//std::cout << value;
			//debug();
		}

		template <typename T, typename... Ts>
		static void debug(const T& value, const Ts&... args) {
			std::cout << value;
			debug(args...);
		}

		static void info() {
			std::clog << std::endl;
		}

		template <typename T>
		static void info(const T& value) {
			std::clog << value << std::endl;
		}

		template <typename T, typename... Ts>
		static void info(const T& value, const Ts&... args) {
			std::clog << value;
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

//Debug
//OpenGL header
#include <GL/glew.h>
#define	glCall(x)	x; \
					glCheckError(#x, __FILE__, __LINE__);

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

#endif // !KENGINES_DEBUG_H

