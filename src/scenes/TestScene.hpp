#pragma
#include <WindowInfo.hpp>
#include <IListener.hpp>

using namespace Core;

class TestScene : public IListener {
    private:
        Core::WindowInfo windowInfo;
        // TRIANGLE STUFF //
        const char *vertexShaderSource = "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "void main()\n"
        "{\n"
        " gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
        "}\0";

        const char *fragShaderSource = "#version 330 core\n"
        "out vec4 FragColor;\n"
        "void main()\n"
        "{\n"
        " FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
        "}\0";

        float vertices[9];
        unsigned int vbo;
        unsigned int vao;

        unsigned int vShader;
        unsigned int fShader;
        unsigned int shaderProg;

        // TRIANGLE STUFF //

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