#include "ImageBufferScene.hpp"

#include <chrono>
#include <GL/glew.h>
#include <iostream>
#include <ShaderProgram.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

/*
    TODO NEXT TIME:
    Make a struct that is a vertex (3 floats) and then make an array of that, where array sizes are adjusted 
    (vertices will be 4 instead of 12) and then just pass that to opengl and see if it just does everything properly.

*/ 

/**
 * @brief Construct a new Test Scene:: Test Scene object
 * This will be where all of the objects that I want to make are loaded/created
 * For now, just as a simple example, I won't pull in obj or classes that 
 * construct objects...yet
 */
ImageBufferScene::ImageBufferScene() {
    this->setup();
};

void ImageBufferScene::setup() {
    // Load the image
    unsigned char* image = stbi_load("src/textures/brick.jpg", &width, &height, &channels, 0);

    glGenFramebuffers(1, &fbo);
    glGenTextures(1, &tex);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex, 0);
}

void ImageBufferScene::render() {
    glBlitNamedFramebuffer(fbo, 0, 0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
    // glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


/**
 * @brief Things to do at the beginning of the frame
 * This will be where we get inputs and set up any start of frame timing logic
 * no real drawing stuff should be done here. I believe this would be "physics ticks"
 */
void ImageBufferScene::notifyBeginFrame() {};

/**
 * @brief This is where the blit happens
 * You will want to clear screen at the start of this
 */
void ImageBufferScene::notifyDisplayFrame() {
    glClearColor(0.53, 0.81, 0.92, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    float timeValue = (float)SDL_GetTicks() / 1000.0;
    float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
    this->render();
};

void ImageBufferScene::notifyKeyboardInput(unsigned char key, bool pressed) {};

void ImageBufferScene::notifyKeyboardUp(SDL_Keysym key) {};

void ImageBufferScene::notifyMouseInput(int button, int state, int x, int y) {};

void ImageBufferScene::notifyMouseMovementInput(int x, int y) {};

void ImageBufferScene::notifyEndFrame() {};

void ImageBufferScene::notifyReshape(int width, int height, int previous_width, int previous_height) {}

ImageBufferScene::ImageBufferScene(Core::WindowInfo windowInfo) : ImageBufferScene() {
    this->windowInfo = windowInfo;
}