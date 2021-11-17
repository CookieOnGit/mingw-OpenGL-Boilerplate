#ifndef SHADER_HPP
#define SHADER_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <string_view>
#include <fstream>
#include <sstream>

class Shader 
{
public:
    unsigned int shaderProgramID;
    Shader(std::string_view vertexShaderPath, std::string_view fragmentShaderPath);
    void use();

    template<typename T>
    void setUniform(const std::string& uniformName, T value);

    template<typename T,typename U>
    void setUniform(const std::string& uniformName, T value1, U value2, U value3, U value4);

    ~Shader();
private:
    enum statusCheck {VertexShader, FragmentShader, Linker};
    void CheckStatus(unsigned int shader, statusCheck t);
};


#endif