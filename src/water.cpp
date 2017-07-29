#include "water.hpp"
#include "globals.h"
#include <random>
#include <cmath>

Water::Water(int w, int h) : width(w), height(h) {
    glGenTextures(1, &texture_id);

    glBindTexture(GL_TEXTURE_2D, texture_id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

    float color[] = { 1.0f, 0.0f, 1.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, color);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    _water_texture.resize(width * height);

    int i = 0;
    for(auto& n : _water_texture) {
        float v = 0.5 + 0.5 * std::sinf(2.0f * 4.0f * M_PI * i++ / (float)height);
        n = RGB{v, 1.0f, 0.0f};
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1024, 1024, 0, GL_RGB, GL_FLOAT, &_water_texture[0]);

    glBindTexture(GL_TEXTURE_2D, 0);
	GL_CHECK_ERROR();
}

int Water::get_index(int x, int y) {
    return y * width + x;
}

void Water::update(float) {
	glBindTexture(GL_TEXTURE_2D, texture_id);

	//https://www.packtpub.com/books/content/introduction-modern-opengl
	const float freq = 8.0f;
	for(int y = 0; y < height; y++) {
		for(int x = 0; x < width; x++) {
			float distance = sqrt(pow((0.5 - x) / (float)width, 2.0) + pow((0.5 - y) / (float)height, 2.0));
			float v = 0.5 + 0.5 * std::sinf(distance * freq * -M_PI + 8.0f * glfwGetTime());
			_water_texture[get_index(y, x)] = RGB{v, 1.0f, 0.0f};
		}
	}

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1024, 1024, 0, GL_RGB, GL_FLOAT, &_water_texture[0]);
}
