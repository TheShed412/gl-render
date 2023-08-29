#include "TestScene.hpp"

void TestScene::notifyBeginFrame() {};

void TestScene::notifyDisplayFrame() {};

void TestScene::notifyKeyboardInput(unsigned char key, bool pressed) {};

void TestScene::notifyKeyboardUp(SDL_Keysym key) {};

void TestScene::notifyMouseInput(int button, int state, int x, int y) {};

void TestScene::notifyMouseMovementInput(int x, int y) {};

void TestScene::notifyEndFrame() {};

void TestScene::notifyReshape(int width, int height, int previous_width, int previous_height) {}

TestScene::TestScene(Core::WindowInfo windowInfo) : TestScene() {
    this->windowInfo = windowInfo;
}