#pragma once

#include <cstdio>
//#include <GLFW/glfw3.h>
#include "glfw.h"
#include <vector>

struct Water {
    Water(int w, int h);
    
    void update();
    
    GLuint texture_id;
    int width, height;
    std::vector<float> _water_texture;
};
