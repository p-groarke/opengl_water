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
	glfwWindowHint(GLFW_SAMPLES, samples);
	if (!(window = glfwCreateWindow(width, height, window_name, nullptr
			, nullptr)))
	{
		printf("Couldn't create window.\nExiting...\n");
		std::exit(-1);
	}

	glfwMakeContextCurrent(window);
	gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
	glfwSwapInterval(swap_interval);

	glGetIntegerv(GL_SAMPLES, &samples);
	if (samples) {
		printf("MSAA enabled with %d samples\n", samples);
	} else {
		printf("MSAA is not be available\n");
	}
	glfwSetWindowUserPointer(window, this);

	/* Callbacks */
	glfwSetKeyCallback(window, key_callback);
	glfwSetFramebufferSizeCallback(window, resize_callback);
	glfwSetCursorPosCallback(window, mouse_pos_callback);
	glfwSetMouseButtonCallback(window, mouse_butt_callback);
	glfwSetScrollCallback(window, scroll_callback);
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

void Window::on_key(std::function<void(int, int, int, int)> callback) {
	key_user_callbacks.emplace_back(callback);
}

void Window::on_mouse_pos(std::function<void(double, double)> callback) {
	mouse_pos_user_callbacks.emplace_back(callback);
}

void Window::on_mouse_button(std::function<void(int, int, int)> callback) {
	mouse_butt_user_callbacks.emplace_back(callback);
}

void Window::on_scroll(std::function<void(double, double)> callback) {
	scroll_user_callbacks.emplace_back(callback);
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

	for (const auto& x : key_user_callbacks) {
		x(key, scancode, action, mods);
	}
}

void Window::mouse_pos_callback(GLFWwindow*, double xpos, double ypos)
{
	for (const auto& x : mouse_pos_user_callbacks) {
		x(xpos, ypos);
	}
}

void Window::mouse_butt_callback(GLFWwindow* window_, int button
		, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT) {
		if (action == GLFW_PRESS) {
			glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
//			glfwGetCursorPos(window_, &cursorX, &cursorY);
		} else {
			glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}
	}

	for (const auto& x : mouse_butt_user_callbacks) {
		x(button, action, mods);
	}
}

void Window::scroll_callback(GLFWwindow*, double xoffset, double yoffset)
{
	for (const auto& x : scroll_user_callbacks) {
		x(xoffset, yoffset);
	}
}

Window* Window::main = nullptr;
std::vector<std::function<void(int, int, int, int)>> Window::key_user_callbacks = {};
std::vector<std::function<void(double, double)>> Window::mouse_pos_user_callbacks = {};
std::vector<std::function<void(int, int, int)>> Window::mouse_butt_user_callbacks = {};
std::vector<std::function<void(double, double)>> Window::scroll_user_callbacks = {};
