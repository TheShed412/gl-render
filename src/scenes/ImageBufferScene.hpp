#pragma
#include <WindowInfo.hpp>
#include <IListener.hpp>
#include <Types.hpp>
#include <ShaderProgram.hpp>

using namespace Core;

class ImageBufferScene : public IListener {
    private:
        Core::WindowInfo windowInfo;

        GLuint fbo, pbo, tex;

        void setup();
        void render();

        int width, height, channels;

    public:
        void notifyBeginFrame() override;
        void notifyDisplayFrame() override;
        void notifyKeyboardInput(unsigned char key, bool pressed) override;
        void notifyKeyboardUp(SDL_Keysym key) override;
        void notifyMouseInput(int button, int state, int x, int y) override;
        void notifyMouseMovementInput(int x, int y) override;
        void notifyEndFrame() override;
        void notifyReshape(int width,
                                  int height,
                                  int previous_width,
                                  int previous_height) override;
        ImageBufferScene();
        ImageBufferScene(Core::WindowInfo windowInfo);
};