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

#ifdef IMGUI_ENABLE
#include <imgui.h>
#include <imgui_impl_glfw_gl3.h>
#endif

namespace KEngines { namespace KRenderer {
	void window_size_callback(GLFWwindow* window, int width, int height);

	class Window {
		friend class Renderer;

	private:
		KVector::ivec2 s_size; //Screen size

		Kboolean is_active;
		Kboolean is_focus;

		Kdouble run_time, pause_time;

		std::string title;
		GLFWwindow* window;

		Kboolean initGL() {
			if (!glfwInit()) {
				window = nullptr;
				return false;
			}

			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef KDEBUG
			glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE); // comment this line in a release build!
																// enable OpenGL debug context if context allows for debug context
			GLint flags; glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
			if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
			{
				glEnable(GL_DEBUG_OUTPUT);
				glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS); // makes sure errors are displayed synchronously
				glDebugMessageCallback(glDebugOutput, nullptr);
				glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
			}
#endif

			window = glfwCreateWindow(s_size.x, s_size.y, title.c_str(), nullptr, nullptr);
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

			glClearColor(0.17f, 0.17f, 0.17f, 1.0f);
			glfwSwapInterval(1);
			glfwSetWindowUserPointer(window, this);
			glfwSetWindowSizeCallback(window, window_size_callback);

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
		Window(const std::string &title, Kint width = 1200, Kint height = 900) :
			title(title), s_size(width, height) {
			if (!initGL()) {
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
			s_size.x = w;
			s_size.y = h;
			//Log::info("Window resized with ", w, ", ", h);
			KObject::Font::setViewport(s_size);
			glViewport(0, 0, s_size.x, s_size.y);
		}

		bool closed()const {
			return glfwWindowShouldClose(window) == GLFW_TRUE;
		}

		bool actived()const {
			return is_active;
		}

		void closeWindow()const {
			glfwSetWindowShouldClose(window, GLFW_TRUE);
		}

		void clear() {
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}

		void setClearColor(const KVector::vec4& color = KVector::vec4(0.17f, 0.17f, 0.17f, 1.0f)) {
			glClearColor(color.r, color.g, color.b, color.a);
		}

		void update() {
			if (is_active) {
				run_time = glfwGetTime() - pause_time;
			}
			else {
				pause_time = glfwGetTime() - run_time;
			}
			glfwSwapBuffers(window);
			glfwPollEvents();
		}

		void setTitle(const std::string& title) {
			this->title = title;
			glfwSetWindowTitle(window, this->title.c_str());
		}

		void resetTime() {
			run_time = 0;
			pause_time = 0;
		}

		inline const Kdouble getRunTime()const {
			return run_time;
		}

		inline const KVector::ivec2& getWindowSize()const {
			return s_size;
		}
	};

	static void window_size_callback(GLFWwindow* window, int width, int height) {
		auto win = static_cast<Window*>(glfwGetWindowUserPointer(window));
		if (win != nullptr) win->resize(width, height);
		win = nullptr;
	}
} }

#endif // !KENGINES_WINDOW_H

