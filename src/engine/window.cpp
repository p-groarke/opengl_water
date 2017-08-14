#include "window.h"
#include "globals.h"

#include <cstdlib>

namespace {
	inline Window* get_this(GLFWwindow* window) {
		return reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
	}
} // anonymous

Window::Window(const char* window_name) {
	if (!glfwInit()) {
		printf("Couldn't initialize GLFW.\nExiting...\n");
		std::exit(-1);
	}

	if (Window::main == nullptr)
		Window::main = this;

	glfwSetErrorCallback(error_callback);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//	glfwWindowHint(GLFW_SAMPLES, samples);
	if (!(window = glfwCreateWindow(width, height, window_name, nullptr
			, nullptr)))
	{
		printf("Couldn't create window.\nExiting...\n");
		std::exit(-1);
	}

	glfwMakeContextCurrent(window);
	gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
	glfwSwapInterval(swap_interval);

//	glGetIntegerv(GL_SAMPLES, &samples);
//	if (samples) {
//		printf("MSAA enabled with %d samples\n", samples);
//	} else {
//		printf("MSAA is not be available\n");
//	}
	glfwSetWindowUserPointer(window, this);

	/* Callbacks */
	glfwSetKeyCallback(window, key_callback);
	glfwSetFramebufferSizeCallback(window, resize_callback);
	glfwSetCursorPosCallback(window, mouse_pos_callback);
	glfwSetMouseButtonCallback(window, mouse_butt_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwFocusWindow(window);
}

Window::~Window() {
	glfwDestroyWindow(window);
	glfwTerminate();
}

void Window::post_render() {
	glfwSwapBuffers(window);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glfwPollEvents();
}

void Window::set_on_key(const std::function<void(int, int, int, int)>& callback) {
	key_user_callbacks.emplace_back(callback);
}

void Window::set_on_mouse_pos(const std::function<void(double, double)>& callback) {
	mouse_pos_user_callbacks.emplace_back(callback);
}

void Window::set_on_mouse_button(const std::function<void(int, int, int)>& callback) {
	mouse_butt_user_callbacks.emplace_back(callback);
}

void Window::set_on_mouse_scroll(const std::function<void(double, double)>& callback) {
	mouse_scroll_user_callbacks.emplace_back(callback);
}

/* glfw callbacks. */
void Window::error_callback(int error, const char* description) {
	OUTPUT_ERROR("Error: %d - %s", error, description);
}

void Window::resize_callback(GLFWwindow* window, int width_, int height_)
{
	auto w = get_this(window);
	w->width = width_;
	w->height = height_;
	w->window_ratio = w->width / static_cast<float>(w->height);
	glViewport(0, 0, w->width, w->height);
}

void Window::key_callback(GLFWwindow* window_, int key, int scancode
		, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window_, GLFW_TRUE);

	auto w = get_this(window_);
	for (size_t i = 0; i < w->key_user_callbacks.size(); ++i) {
		w->key_user_callbacks[i](key, scancode, action, mods);
	}
}

void Window::mouse_pos_callback(GLFWwindow* window_, double xpos, double ypos)
{
	auto w = get_this(window_);
	for (size_t i = 0; i < w->mouse_pos_user_callbacks.size(); ++i) {
		w->mouse_pos_user_callbacks[i](xpos, ypos);
	}
}

void Window::mouse_butt_callback(GLFWwindow* window_, int button
		, int action, int mods)
{
	auto w = get_this(window_);
	for (size_t i = 0; i < w->mouse_butt_user_callbacks.size(); ++i) {
		w->mouse_butt_user_callbacks[i](button, action, mods);
	}
}

void Window::scroll_callback(GLFWwindow* window_, double xoffset, double yoffset)
{
	auto w = get_this(window_);
	for (size_t i = 0; i < w->mouse_scroll_user_callbacks.size(); ++i) {
		w->mouse_scroll_user_callbacks[i](xoffset, yoffset);
	}
}

Window* Window::main = nullptr;
