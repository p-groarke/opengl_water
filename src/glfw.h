#pragma once

#include <cstdio>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cstdlib>

struct Glfw {
	Glfw(const char* window_name) {
		if (!glfwInit()) {
			printf("Couldn't initialize GLFW.\nExiting...\n");
			std::exit(-1);
		}


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

		/* Callbacks */
		glfwSetKeyCallback(window, key_callback);
		glfwSetFramebufferSizeCallback(window, resize_callback);
	}

	~Glfw() {
		glfwDestroyWindow(window);
		glfwTerminate();
	}

	inline void post_render() {
		glfwSwapBuffers(window);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glfwPollEvents();
	}

	/* Callbacks. */
	static inline void error_callback(int error, const char* description) {
		fprintf(stderr, "Error: %d - %s\n", error, description);
	}

	static inline void key_callback(GLFWwindow* window_, int key, int
			, int action, int)
	{
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
			glfwSetWindowShouldClose(window_, GLFW_TRUE);
	}

	static inline void resize_callback([[maybe_unused]] GLFWwindow* window_
			, int width_, int height_)
	{
		width = width_;
		height = height_;
		window_ratio = width / static_cast<float>(height);
		glViewport(0, 0, width, height);
	}

	GLFWwindow* window;
	const unsigned int swap_interval = 1;
	int samples = 4;
	static int width, height;
	static float window_ratio;
};
