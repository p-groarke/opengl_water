#pragma once

#include <cstdio>
#include "glfw.h"
#include <vector>

struct RGB {
    typedef float value_type;
    float r, g, b;
};


struct Water {
    Water(int w, int h);

    void update(float dt);
    
    int get_index(int x, int y);

    GLuint texture_id;
    int width, height;
    std::vector<RGB> _water_texture;
};
