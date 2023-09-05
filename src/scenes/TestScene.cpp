#include "TestScene.hpp"

#include <GL/glew.h>
#include <iostream>

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
    // Just setting points, all in normal device coords
    // Top right of rectangle "0"
    vertices[0] = 0.5;
    vertices[1] = 0.5;
    vertices[2] = 0;

    // Bottom right of rectangle "1"
    vertices[3] = 0.5;
    vertices[4] = -0.5;
    vertices[5] = 0;

    // Bottom left of rectangle "2"
    vertices[6] = -0.5;
    vertices[7] = -0.5;
    vertices[8] = 0;

    // Top left of rectangle "3"
    vertices[9] = -0.5;
    vertices[10] = 0.5;
    vertices[11] = 0;

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
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

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

    // Loading shaders and stuff
    // Just like other calls, this will create an ID associated to the shader
    vShader = glCreateShader(GL_VERTEX_SHADER);
    // This will link the source of the shader to the ID
    glShaderSource(vShader, 1, &vertexShaderSource, NULL);
    // This does the compilations
    glCompileShader(vShader);

    int success;
    char infoLog[512];
    glGetShaderiv(vShader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(vShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" <<
        infoLog << std::endl;
    }

    fShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fShader, 1, &fragShaderSource, NULL);
    glCompileShader(fShader);
    // For both of the things above we can check for compilation errors, skipping for now
    glGetShaderiv(fShader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(fShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAG::COMPILATION_FAILED\n" <<
        infoLog << std::endl;
    }

    shaderProg = glCreateProgram();
    glAttachShader(shaderProg, vShader);
    glAttachShader(shaderProg, fShader);
    glLinkProgram(shaderProg);
    // We can also check for linking errors above, skipping for now
    glGetProgramiv(shaderProg, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(shaderProg, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::LINKING_FAILED\n" <<
            infoLog << std::endl;
    }

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void TestScene::renderVBO() {
    glBindVertexArray(vao);
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