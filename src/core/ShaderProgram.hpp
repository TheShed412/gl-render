#pragma once
 
#include <GL/glew.h>
#include <iostream>
#include <string>
#include <Types.hpp>
 
namespace shed {
 
    class ShaderProgram {
        private:
            GLuint _id;

            std::string ReadShader(const std::string& filename);
            // TODO: make an error type for this to return
            GLuint CreateShader(GLenum shaderType,
                                const std::string& source,
                                const char* shaderName);
    
        public:
            const GLuint& id = _id;
            ShaderProgram(void);
            ~ShaderProgram(void);
            int CreateProgram(const std::string& VertexShaderFilename,
                                const std::string& FragmentShaderFilename);
            void Use();
            void SetUniform(const std::string &name, bool);
            void SetUniform(const std::string &name, float);
            void SetUniform(const std::string &name, int);
            void SetUniform(const std::string &name, shed::vec4 value);
    
    };
}