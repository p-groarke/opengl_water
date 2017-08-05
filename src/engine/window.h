#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

struct Window {
	Window(const char* window_name);
	~Window();

	void post_render();

	/* Callbacks. */
	static void error_callback(int error, const char* description);
	static void key_callback(GLFWwindow* window_, int key, int
			, int action, int);
	static void resize_callback(GLFWwindow* window_
			, int width_, int height_);
	static void mouse_callback(GLFWwindow* window_
			, double xpos, double ypos);
	static void scroll_callback(GLFWwindow* window_
			, double xoffset, double yoffset);

	GLFWwindow* window;
	const unsigned int swap_interval = 1;
	int samples = 4;
	int width = 1024;
	int height = 576;
	float window_ratio = width / static_cast<float>(height);

	static Window* main;
};
