#pragma
#include <WindowInfo.hpp>
#include <IListener.hpp>
#include <Types.hpp>
#include <ShaderProgram.hpp>

using namespace Core;

class TestScene : public IListener {
    private:
        Core::WindowInfo windowInfo;
        shed::ShaderProgram* shaders; 
        // TRIANGLE STUFF //

        // The benefit of the ebo is, originally, we'd have 18 float in this to define 
        // already shared verts, with the ebo and being able to define indecies, we only need
        // the unique verts
        // This isn't a spectacular demo of the space saving since the total size of the ebo+fbo is the same as
        // an fbo that does the same thing. But on larger objects that have more shared verts, it gets much better
        shed::vec3 verts[4];
        unsigned int vbo;
        unsigned int vao;

        // An array storing the order of the indecies to draw, referencing the above array
        unsigned int eboIndecies[6];
        unsigned int ebo;

        unsigned int vShader;
        unsigned int fShader;
        unsigned int shaderProg;

        // TRIANGLE STUFF //
        void setupVBO();
        void renderVBO();

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
        TestScene();
        TestScene(Core::WindowInfo windowInfo);
};