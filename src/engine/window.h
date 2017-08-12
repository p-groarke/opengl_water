#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <functional>
#include <vector>

struct Window {
	Window(const char* window_name);
	~Window();

	void post_render();

	static void on_key(std::function<void(int, int, int, int)> callback);
	static void on_mouse_pos(std::function<void(double, double)> callback);
	static void on_mouse_button(std::function<void(int, int, int)> callback);
	static void on_scroll(std::function<void(double, double)> callback);

	/* GLFW callbacks. */
private:
	static void error_callback(int error, const char* description);
	static void resize_callback(GLFWwindow* window_
			, int width_, int height_);
	static void key_callback(GLFWwindow* window_, int key, int scancode
			, int action, int mods);
	static void mouse_pos_callback(GLFWwindow* window_
			, double xpos, double ypos);
	static void mouse_butt_callback(GLFWwindow* window_, int button
			, int action, int mods);
	static void scroll_callback(GLFWwindow* window_
			, double xoffset, double yoffset);

public:
	GLFWwindow* window;
	const unsigned int swap_interval = 1;
	int samples = 4;
	int width = 1024;
	int height = 576;
	float window_ratio = width / static_cast<float>(height);

	static Window* main;

private:
	static std::vector<std::function<void(int, int, int, int)>> key_user_callbacks;
	static std::vector<std::function<void(double, double)>> mouse_pos_user_callbacks;
	static std::vector<std::function<void(int, int, int)>> mouse_butt_user_callbacks;
	static std::vector<std::function<void(double, double)>> scroll_user_callbacks;
};
