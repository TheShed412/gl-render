#include <iostream>
#include <fstream>
#include <vector>

#include "init/InitSDL.hpp"
#include "scenes/TestScene.hpp"

int main(int argc, char **argv)
{
    int width = 1200;
    int height = 800;

    Core::WindowInfo window(std::string("Test"),
                            400, 200,//position
                            width, height, //size
                            true);//reshape
    
    Core::ContextInfo context(4, 5, true);

    Core::Init::InitSDL::init(window, context);
    Core::IListener* scene = new TestScene(window);
    Core::Init::InitSDL::setListener(scene);
    Core::Init::InitSDL::run();
    
    return 0;
}
