#pragma once

#include <cstdio>
#include "glfw.h"
#include <vector>

struct RGB {
    float r, g, b;
};


struct Water {
    Water(int w, int h);

    void update();

    GLuint texture_id;
    int width, height;
    std::vector<RGB> _water_texture;
};
