#include "ShaderProgram.hpp" 
#include <iostream>
#include <fstream>
#include <vector>
 
using namespace shed;
 
ShaderProgram::ShaderProgram(void) {}
ShaderProgram::~ShaderProgram(void) {}
 
std::string ShaderProgram::ReadShader(const std::string& filename) {
    std::string shaderCode;
    std::ifstream file(filename, std::ios::in);
 
    if(!file.good()) {
       std::cout<< "Can't read file " << filename <<std::endl;
       std::terminate();
    }
    
    // reading in file for shader
    file.seekg(0, std::ios::end);
    shaderCode.resize((unsigned int)file.tellg());
    file.seekg(0, std::ios::beg);
    file.read(&shaderCode[0], shaderCode.size());
    file.close();

    return shaderCode;
}
 
GLuint ShaderProgram::CreateShader(GLenum shaderType, const std::string& source, const char* shaderName) {
    int compile_result = 0;
 
    GLuint shader = glCreateShader(shaderType);
    const char *shader_code_ptr = source.c_str();
    const int shader_code_size = source.size();
 
    glShaderSource(shader, 1, &shader_code_ptr, &shader_code_size);
    glCompileShader(shader);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compile_result);
 
    //check for errors
    if (compile_result == GL_FALSE) {
        int info_log_length = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &info_log_length);
        std::vector<char> shader_log(info_log_length);
        glGetShaderInfoLog(shader, info_log_length, NULL, &shader_log[0]);
        std::cout << "ERROR compiling shader: " << shaderName << std::endl << &shader_log[0] << std::endl;
        return 0;
    }

    return shader;
}
 
int ShaderProgram::CreateProgram(const std::string& vertexShaderFilename, const std::string& fragmentShaderFilename) {
    //read the shader files and save the code
    std::string vertex_shader_code = ReadShader("src/shaders/" + vertexShaderFilename);
    std::string fragment_shader_code = ReadShader("src/shaders/" + fragmentShaderFilename);

    GLuint vertex_shader = CreateShader(GL_VERTEX_SHADER, vertex_shader_code, (char*)"vertex shader");
    GLuint fragment_shader = CreateShader(GL_FRAGMENT_SHADER, fragment_shader_code, (char*)"fragment shader");
    
    int link_result = 0;
    //create the program handle, attatch the shaders and link it
    GLuint program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    
    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &link_result);

    //check for link errors
    if (link_result == GL_FALSE) {
        int info_log_length = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &info_log_length);
        std::vector<char> program_log(info_log_length);
        glGetProgramInfoLog(program, info_log_length, NULL, &program_log[0]);
        std::cout << "Shader Loader : LINK ERROR" << std::endl << &program_log[0] << std::endl;
        return link_result;
    }

    this->_id = program;

    return 0;
}

// TODO: I kinda want these helpers to do a bit more than simple wrappers
// like, maybe having uniforms be compeletely controlled by this class as well
void ShaderProgram::Use() {
    glUseProgram(id);
}

void ShaderProgram::SetUniform(const std::string &name, bool value) {
    glUniform1i(glGetUniformLocation(id, name.c_str()), (int)value);
}

void ShaderProgram::SetUniform(const std::string &name, float value) {
    glUniform1f(glGetUniformLocation(id, name.c_str()), value);
}

void ShaderProgram::SetUniform(const std::string &name, int value) {
    glUniform1i(glGetUniformLocation(id, name.c_str()), value);
}

void ShaderProgram::SetUniform(const std::string &name, shed::vec4 value) {
    glUniform4f(glGetUniformLocation(id, name.c_str()), value.x, value.y, value.z, value.w);
}

