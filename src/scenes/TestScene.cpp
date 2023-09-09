#include "TestScene.hpp"

#include <GL/glew.h>
#include <iostream>
#include <ShaderLoader.hpp>

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
TestScene::TestScene() {
    this->setupVBO();
};

void TestScene::setupVBO() {
    // I want to make a vertex structure that will encompass this particular type of thing properly
    // I want the abstraction of "each element in the array is a vertex" feel, so that it is easier to think about
    // This might even mean wrapping the filling of buffers in this structure, but I'm down with it lol
    // If I make this structure the size of 3 floats, and then just tell opengl to jump a single float ahead or whatever for each point,
    // will it just work? Need to look in to that
    // Just setting points, all in normal device coords
    // Top right of rectangle "0"
    verts[0].x = 0.5;
    verts[0].y = 0.5;
    verts[0].z = 0;


    // Bottom right of rectangle "1"
    verts[1].x = 0.5;
    verts[1].y = -0.5;
    verts[1].z = 0;

    // Bottom left of rectangle "2"
    verts[2].x = -0.5;
    verts[2].y = -0.5;
    verts[2].z = 0;

    // Top left of rectangle "3"
    verts[3].x = -0.6;
    verts[3].y = 0.5;
    verts[3].z = 0;

    // Said lib mentioned above should also have a way of easily getting indecies, I think
    // I need to work with this more to figure out where things can be simplified for ebos
    // Now we order the vertecies in the order they will connect, essentially
    // First 3 are the first triangle of the rectangle
    eboIndecies[0] = 0;
    eboIndecies[1] = 1;
    eboIndecies[2] = 3;
    // 2nd 3 are the 2nd traiangle of the rectangle
    eboIndecies[3] = 1;
    eboIndecies[4] = 2;
    eboIndecies[5] = 3;
    
    // Boy here are VAOs
    // What the VAO seems to do is almost keep track of state assigned once it binds
    // Once you bind a VAO, anything that happens to VBOs after that binding, will save it to a vao state basically.
    // And once we have that vao, we can then bind that for an object in a dingle call, instead of 6 or 7 calls for VBOs
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // This gives me a list of usable buffer ids. It doesn't actually create a buffer
    // Idea: could I batch this in a fun way to create a bunch of usable ids at the startof the program?
    glGenBuffers(1, &vbo);

    // This will actually create and link the buffer to the id on vbo
    // Also, from here on, any operation on GL_ARRAY_BUFFER will be done on this buffer
    // Each buffer type kinda has dedicated space and can only have one buffer in it at a time
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    // This actually moves data to the buffer bound to GL_ARRAY_BUFFER.
    // The last arg is important to tell the GPU how the data will be used.
    // • GL_STREAM_DRAW: the data is set only once and used by the GPU at most a few times.
    // • GL_STATIC_DRAW: the data is set only once and used many times.
    // • GL_DYNAMIC_DRAW: the data is changed a lot and used many times.
    // If something is changed a lot on draw, then we will want to tell the GPU that so it can optimize
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

    // Setup the ebo which allows us to use indecies on the vertex array so we don't need to send multiople of the same vertex
    // Setup is the same as all other buffers as explained above
    glGenBuffers(1, &this->ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(eboIndecies), eboIndecies, GL_STATIC_DRAW);

    // Now we need to describe how things are stored in the vertex buffers so the right
    // data can be read from the shader
    // The vertex buffer (vbo) is set up:
    //     - each number is a 4 byte float
    //     - so each vertex (x, y, z) is 12 bytes
    //     - there is no space between values
    //     - there is no offset at the start
    // All of that translates to:
    glVertexAttribPointer(0, // the index of where to start in the buffer (matches 'layout in vertex shader')
                          3, // since they are vec3s, we pass 3
                          GL_FLOAT, // the type of the data coming in
                          GL_FALSE, // should we normalize? we did that by hand so no
                          3 * sizeof(float),// the stride (basically the width of each x,y,z group)
                          (void*)0); // The offset of the start of the buffer
    glEnableVertexAttribArray(0);// Pass the vertex attrib location (first arg of glVertexAttribPointer)

    shed::ShaderLoader shaderLoader = shed::ShaderLoader();
    std::string vertPath = "src/shaders/vertex.glsl";
    std::string fragPath = "src/shaders/fragment.glsl";
    shaderProg = shaderLoader.CreateProgram(vertPath.c_str(), fragPath.c_str());
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void TestScene::renderVBO() {
    glBindVertexArray(vao);
    // Use drawElements so opengl knows we are using an ebo
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}


/**
 * @brief Things to do at the beginning of the frame
 * This will be where we get inputs and set up any start of frame timing logic
 * no real drawing stuff should be done here. I believe this would be "physics ticks"
 */
void TestScene::notifyBeginFrame() {};

/**
 * @brief This is where the blit happens
 * You will want to clear screen at the start of this
 */
void TestScene::notifyDisplayFrame() {
    glClearColor(0.53, 0.81, 0.92, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shaderProg);
    this->renderVBO();
};

void TestScene::notifyKeyboardInput(unsigned char key, bool pressed) {};

void TestScene::notifyKeyboardUp(SDL_Keysym key) {};

void TestScene::notifyMouseInput(int button, int state, int x, int y) {};

void TestScene::notifyMouseMovementInput(int x, int y) {};

void TestScene::notifyEndFrame() {};

void TestScene::notifyReshape(int width, int height, int previous_width, int previous_height) {}

TestScene::TestScene(Core::WindowInfo windowInfo) : TestScene() {
    this->windowInfo = windowInfo;
}