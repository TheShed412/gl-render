#include "InitSDL.hpp"

#include "InitGLEW.hpp"

using namespace Core::Init;

Core::IListener* InitSDL::listener = NULL;
Core::WindowInfo InitSDL::windowInformation;
SDL_Window* InitSDL::sdlWindow = NULL;
SDL_GLContext InitSDL::glContext = NULL;
bool InitSDL::quit = false;

void InitSDL::init(const Core::WindowInfo& windowInfo,
                     const Core::ContextInfo& contextInfo)
{
    int bpp = 0;
    int flags = 0;

    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "::SDL INIT ERROR::" << std::endl;
        exit(EXIT_FAILURE);
    }

    int width = windowInfo.width;
    int height = windowInfo.height;
    
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 5);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);

    SDL_SetRelativeMouseMode(SDL_FALSE);// TODO: make this a config option

    flags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE;

    sdlWindow = SDL_CreateWindow(
        windowInfo.name.c_str(),
        windowInfo.position_x,
        windowInfo.position_y,
        windowInfo.width,
        windowInfo.height,
        flags
    );

    if (sdlWindow == NULL) {
        std::cerr << "::SDL WINDOW CREATION ERROR::" << std::endl;
        std::cerr << SDL_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }

    glContext = SDL_GL_CreateContext(sdlWindow);

    if (glContext == NULL) {
        std::cerr << "::SDL GL CONTEXT CREATION ERROR::" << std::endl;
        std::cerr << SDL_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }

    InitGLEW::Init();
}

void InitSDL::run() {
    int xpos = windowInformation.width / 2;
    int ypos = windowInformation.height / 2;
    while(!quit) {
        SDL_Event event;
        while (SDL_PollEvent(&event) && !quit){
            switch(event.type){
                case SDL_QUIT:
                quit = true;
                break;

                case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    quit = true;
                } else {
                    keyboardCallback(event.key.keysym.sym, true);
                }
                break;

                case SDL_KEYUP:
                keyboardCallback(event.key.keysym.sym, false);
                break;

                case SDL_MOUSEMOTION:
                xpos += event.motion.xrel;
                ypos += event.motion.yrel;
                mouseMovementCallback(event.motion.xrel, event.motion.yrel);
                break;

                case SDL_MOUSEBUTTONDOWN:
                mouseCallback(event.button.button, 1, xpos, ypos);
                break;

                case SDL_MOUSEBUTTONUP:
                mouseCallback(event.button.button, 0, event.motion.xrel, event.motion.yrel);
                break;
            }
        }
        displayCallback();
    }
    closeCallback();
    close();
}

void InitSDL::close() {
    SDL_DestroyWindow(sdlWindow);
    SDL_GL_DeleteContext(glContext); 
    SDL_Quit();
}
 
void InitSDL::enterFullscreen() {
    if (SDL_SetWindowFullscreen(sdlWindow, SDL_WINDOW_FULLSCREEN) < 0) {
        std::cerr << "::FAILED TO FULLSCREEN::" << std::endl;
        std::cerr << SDL_GetError() << std::endl;
    }
}

void InitSDL::exitFullscreen() {
    if (SDL_SetWindowFullscreen(sdlWindow, 0) < 0) {
        std::cerr << "::FAILED TO WINDOW::" << std::endl;
        std::cerr << SDL_GetError() << std::endl;
    }
}

void InitSDL::idleCallback(void) {}

void InitSDL::displayCallback(void) {
    if (listener){
        listener->notifyBeginFrame();
        listener->notifyDisplayFrame();
        SDL_GL_SwapWindow(sdlWindow);
   
        listener->notifyEndFrame();
    }
}

void InitSDL::reshapeCallback(int width, int height) {
    if (windowInformation.isReshapable == true) {
        if (listener){
            listener->notifyReshape(width,
                                 height,
                                 windowInformation.width,
                                 windowInformation.height);
        }
        windowInformation.width = width;
        windowInformation.height = height;
    }
}

void InitSDL::keyboardCallback(unsigned char key, bool pressed) {
    if(listener) {
        listener->notifyKeyboardInput(key, pressed);
    }
}

void InitSDL::mouseCallback(int button, int state, int x, int y) {
    if(listener) {
        listener->notifyMouseInput(button, state, x, y);
    }
}

void InitSDL::mouseMovementCallback(int x, int y) {
    if(listener) {
        listener->notifyMouseMovementInput(x, y);
    }
}

void InitSDL::closeCallback(){}

void InitSDL::keyboardUp(SDL_Keysym key, int x, int y) {
    if(listener) {
        listener->notifyKeyboardUp(key);
    }
}

void InitSDL::setListener(Core::IListener*& iListener)
{
   listener = iListener;
}

void InitSDL::printOpenGLInfo(const Core::WindowInfo& windowInfo, const Core::ContextInfo& contextInfo) {
    // const unsigned char* renderer = glGetString(GL_RENDERER);
    // const unsigned char* vendor = glGetString(GL_VENDOR);
    // const unsigned char* version = glGetString(GL_VERSION);
    
    std::cout << "******************************************************************************" << std::endl;
    std::cout << "SDL:Initialise" << std::endl;
}