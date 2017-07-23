#include "water.hpp"
#include <random>

Water::Water(int w, int h) : width(w), height(h) {
    glGenTextures(1, &texture_id);
    
    glBindTexture(GL_TEXTURE_2D, texture_id);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    
    float color[] = { 1.0f, 0.0f, 0.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, color);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    std::default_random_engine generator;
    std::uniform_real_distribution<double> distribution(0.0,1.0);
    _water_texture.resize(width * height);
    
    for(auto& n : _water_texture) {
        n = distribution(generator);
    }
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, 1024, 1024, 0, GL_RED, GL_FLOAT, &_water_texture[0]);
}

void Water::update() {
    
}
