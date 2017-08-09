#include "window.h"
#include "globals.h"
#include "engine/entity.h"

#include <cstdlib>

namespace {
inline Window* get_this(GLFWwindow* window)
{
	return reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
}
} // namespace

Window::Window(const char* window_name)
{
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
	if (!(window = glfwCreateWindow(width, height, window_name, nullptr, nullptr))) {
		printf("Couldn't create window.\nExiting...\n");
		std::exit(-1);
	}

	glfwMakeContextCurrent(window);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
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
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
}

Window::~Window()
{
	glfwDestroyWindow(window);
	glfwTerminate();
}

void Window::post_render()
{
	glfwSwapBuffers(window);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glfwPollEvents();
}

/* Callbacks. */
void Window::error_callback(int error, const char* description)
{
	OUTPUT_ERROR("Error: %d - %s", error, description);
}

void Window::key_callback(GLFWwindow* window_, int key, int, int action, int)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window_, GLFW_TRUE);
		return;
	}
}

void Window::resize_callback(GLFWwindow* window, int width_, int height_)
{
	auto w = get_this(window);
	w->width = width_;
	w->height = height_;
	w->window_ratio = w->width / static_cast<float>(w->height);
	glViewport(0, 0, w->width, w->height);
}

void Window::mouse_callback(GLFWwindow*, double, double)
{
}

void Window::scroll_callback(GLFWwindow*, double, double)
{
}

Window* Window::main = nullptr;
