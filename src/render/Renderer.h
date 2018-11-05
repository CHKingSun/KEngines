//
// Create by KingSun on 2018/07/29
//

#ifndef KENGINES_RENDERER_H
#define KENGINES_RENDERER_H

#include "Window.h"

#include <functional>

namespace KEngines { namespace KRenderer {
	class Renderer {
		friend void window_size_callback(GLFWwindow* window, int width, int height);
		friend void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
		friend void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
		friend void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
		friend void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
		friend void cursor_enter_callback(GLFWwindow* window, int entered);
		friend void window_iconify_callback(GLFWwindow* window, int iconified);

	protected:
		Window* window;
		Kboolean keys[512]; //-1, 32-162, 256-248
		Kboolean mouse[3]; //left, right, wheel
		Kboolean mouse_hide;

		ivec2 w_size;
		vec2 mouse_pos;

		void initAction() {
			glfwSetWindowSizeCallback(window->window, window_size_callback);
			glfwSetKeyCallback(window->window, key_callback);
			glfwSetMouseButtonCallback(window->window, mouse_button_callback);
			glfwSetScrollCallback(window->window, scroll_callback);
			glfwSetCursorPosCallback(window->window, cursor_position_callback);
			glfwSetCursorEnterCallback(window->window, cursor_enter_callback);

			Kdouble mx, my;
			glfwGetCursorPos(window->window, &mx, &my);
			mouse_pos.x = mx, mouse_pos.y = my;
		}

	protected:
		Renderer(const std::string& title, Ksize swidth = 1000, Ksize sheight = 700) :
			window(nullptr), w_size(swidth, sheight), mouse_hide(false) {
			window = new Window(title, swidth, sheight);
			if (window->actived()) {
				glfwSetWindowUserPointer(window->window, this);
				initAction();
			}
		}

		virtual void keyEvent(Kint key, Kint action) {
			keys[key] = (action != GLFW_RELEASE);
			if (keys[GLFW_KEY_ESCAPE]) {
				window->closeWindow();
			}
		}

		virtual void mouseEvent(Kint button, Kint action) {
			mouse[button] = (action == GLFW_PRESS);
		}

		virtual void mouseWheelEvent(Kdouble yoffset) {
			//if(is_focus) std::cout << "Yaxis scroll: " << yoffset << std::endl;
		}

		virtual void cursorEvent(Kdouble xpos, Kdouble ypos) {
			mouse_pos.x = xpos; mouse_pos.y = ypos;
			//if(mouse[GLFW_MOUSE_BUTTON_LEFT]) 
			//	std::cout << "Pointer at: " << mx << ", " << my << std::endl;
		}

		virtual void focusEvent(bool focused) {
			window->is_focus = focused;
		}

		virtual void iconifiedEvent(bool iconified) {
			window->is_active = window->is_active && !iconified;
		}

	public:
		virtual ~Renderer() {
			delete window;
		}

		virtual void exec() = 0;

		virtual void resize(Kint w, Kint h) {
			w_size.x = w;
			w_size.y = h;
			window->resize(w, h);
		}

		void hideMouse(Kboolean hide = true) {
			mouse_hide = hide;
			if (hide) {
				glfwSetInputMode(window->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
				//hide the mouse pointer, and also the mouse pointer will not be limited to window size.
			}
			else {
				glfwSetInputMode(window->window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			}
		}

		const vec2& getMouse()const { return mouse_pos; }
	};


	static void window_size_callback(GLFWwindow* window, int width, int height)
	{
		auto renderer = static_cast<Renderer*>(glfwGetWindowUserPointer(window));
		if (renderer != nullptr) renderer->resize(width, height);
		renderer = nullptr;
	}

	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
		auto renderer = static_cast<Renderer*>(glfwGetWindowUserPointer(window));
		if (renderer != nullptr) renderer->keyEvent(key, action);
		renderer = nullptr;
	}

	static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
		auto renderer = static_cast<Renderer*>(glfwGetWindowUserPointer(window));
		if (renderer != nullptr) renderer->mouseEvent(button, action);
		renderer = nullptr;
	}

	static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
		auto renderer = static_cast<Renderer*>(glfwGetWindowUserPointer(window));
		if (renderer != nullptr) renderer->mouseWheelEvent(yoffset);
		renderer = nullptr;
	}

	static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
		auto renderer = static_cast<Renderer*>(glfwGetWindowUserPointer(window));
		if (renderer != nullptr) renderer->cursorEvent(xpos, ypos);
		renderer = nullptr;
	}

	static void cursor_enter_callback(GLFWwindow* window, int entered) {
		auto renderer = static_cast<Renderer*>(glfwGetWindowUserPointer(window));
		if (renderer != nullptr) renderer->focusEvent(entered != GLFW_FALSE);
		renderer = nullptr;
	}

	static void window_iconify_callback(GLFWwindow* window, int iconified) {
		auto renderer = static_cast<Renderer*>(glfwGetWindowUserPointer(window));
		if (renderer != nullptr) renderer->iconifiedEvent(iconified != GLFW_FALSE);
		renderer = nullptr;
	}
} }

#endif // !KENGINES_RENDERER_H

