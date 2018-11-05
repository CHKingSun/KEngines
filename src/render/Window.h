//
// Created by KinSun on 2018/07/16
//

#ifndef KENGINES_WINDOW_H
#define KENGINES_WINDOW_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "../KHeader.h"
#include "../Log.h"
#include "../math/vector/vec4.h"
#include "../object/Font.h"
#include "../buffer/FrameBuffer.h"

#ifdef IMGUI_ENABLE
#include <imgui.h>
#include <imgui_impl_glfw_gl3.h>
#endif

namespace KEngines { namespace KRenderer {
	class Window {
		friend class Renderer;

	private:
		Kboolean is_active;
		Kboolean is_focus;

		Kdouble run_time, pause_time;
		Kdouble last_time;
		Kuint frame_count, frame_current;

		GLFWwindow* window;
		std::string title;

		Kboolean initGL(Kint width, Kint height) {
			if (!glfwInit()) {
				window = nullptr;
				return false;
			}

			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef KDEBUG
			glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
			// comment this line in a release build!										
			// enable OpenGL debug context if context allows for debug context
#endif

			window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
			if (!window) {
				glfwTerminate();
				return false;
			}
			glfwSetWindowPos(window, 120, 120);
			glfwMakeContextCurrent(window);

			if (glewInit() != GLEW_OK) {
				Log::error("GLEW initial failed!");
				glfwDestroyWindow(window);
				glfwTerminate();
				return false;
			}

#ifdef KDEBUG
			GLint flags; glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
			if (flags & GL_CONTEXT_FLAG_DEBUG_BIT) {
				glEnable(GL_DEBUG_OUTPUT);
				glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS); // makes sure errors are displayed synchronously
				glDebugMessageCallback(glDebugOutput, nullptr);
				glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
			}
#endif

			setClearColor();
			glfwSwapInterval(1);

#ifdef IMGUI_ENABLE
			// Setup ImGui binding
			ImGui::CreateContext();
			//ImGuiIO& io = ImGui::GetIO(); (void)io;
			//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
			//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls
			ImGui_ImplGlfwGL3_Init(window, true);

			// Setup style
			ImGui::StyleColorsDark();
			//ImGui::StyleColorsClassic();
#endif

			Log::info("Version: ", glGetString(GL_VERSION));
			is_active = true;
			run_time = 0;
			pause_time = 0;

			return true;
		}

	public:
		Window(const std::string &title, Kint width = 1200, Kint height = 900) : title(title) {
			if (!initGL(width, height)) {
				Log::error("Create window failed!");
				return;
			}
		}
		~Window() {
#ifdef IMGUI_ENABLE
			ImGui_ImplGlfwGL3_Shutdown();
			ImGui::DestroyContext();
#endif
			glfwDestroyWindow(window);
			glfwTerminate();
		}

		void resize(Kint w, Kint h) {
			//Log::info("Window resized with ", w, ", ", h);
			glViewport(0, 0, w, h);
			KObject::Font::setViewport(ivec2(w, h));
			KBuffer::FrameBuffer::setViewport(ivec2(w, h));
		}

		Kboolean closed()const { return glfwWindowShouldClose(window) == GLFW_TRUE; }

		Kboolean actived()const { return is_active; }

		Kboolean focused()const { return is_focus; }

		void closeWindow()const { glfwSetWindowShouldClose(window, GLFW_TRUE); }

		void clear() { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); }

		void setClearColor(const KVector::vec4& color = KVector::vec4(0.17f, 0.17f, 0.17f, 1.0f)) {
			glClearColor(color.r, color.g, color.b, color.a);
			//glClearColor(1.f, 0.f, 1.f, 0.f); //test
		}

		void update() {
			glfwSwapBuffers(window);
			glfwPollEvents();

			Kdouble now_time = glfwGetTime();
			if (is_active) {
				run_time = now_time - pause_time;
			}
			else {
				pause_time = now_time - run_time;
			}

			++frame_count;
			Kdouble delta_time = now_time - last_time;
			if (delta_time >= 0.2f) {
				last_time = now_time;
				frame_current = (Kuint)((Kdouble)frame_count / delta_time);
				frame_count = 0;
			}
		}

		void setTitle(const std::string& title) {
			this->title = title;
			glfwSetWindowTitle(window, this->title.c_str());
		}

		void resetTime() {
			run_time = 0;
			pause_time = 0;
		}

		inline const Kdouble getRunTime()const { return run_time; }

		inline const Kdouble getCurrentTime()const { return glfwGetTime(); }

		inline const Kuint getCurrentFrame()const { return frame_current; }
	};
} }

#endif // !KENGINES_WINDOW_H

