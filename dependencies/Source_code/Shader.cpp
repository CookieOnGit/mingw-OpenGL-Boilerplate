#include "Shader.hpp"

Shader::Shader(std::string_view vertexShaderPath, std::string_view fragmentShaderPath): shaderProgramID{glCreateProgram()}
{
    std::ifstream vertexShaderFile;
    std::ifstream fragmentShaderFile;
    std::string temp_fragmentShaderCode;
    std::string temp_vertexShaderCode;

    vertexShaderFile.exceptions   (std::ifstream::failbit | std::ifstream::badbit);
    fragmentShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        vertexShaderFile.open(vertexShaderPath.data());
        std::ostringstream vShaderStream;
        vShaderStream << vertexShaderFile.rdbuf();
        temp_vertexShaderCode = vShaderStream.str();
        
        fragmentShaderFile.open(fragmentShaderPath.data());
        std::ostringstream fShaderStream;
        fShaderStream << fragmentShaderFile.rdbuf();
        temp_fragmentShaderCode = fShaderStream.str();
        
    }
    catch(const std::exception& e)
    {
        std::cerr << "An error occured while reading from the shader program" << e.what() << '\n';
    }
    const char* vertexShaderCode{temp_vertexShaderCode.c_str()};
    unsigned int vertexShader{glCreateShader(GL_VERTEX_SHADER)};
    
    glShaderSource(vertexShader, 1, &vertexShaderCode, nullptr);
    glCompileShader(vertexShader);
    CheckStatus(vertexShader, VertexShader);
    
    const char* fragmentShaderCode{temp_fragmentShaderCode.c_str()};
    unsigned int fragmentShader{glCreateShader(GL_FRAGMENT_SHADER)};

    glShaderSource(fragmentShader, 1, &fragmentShaderCode, nullptr);
    glCompileShader(fragmentShader);
    CheckStatus(fragmentShader, FragmentShader);

    glAttachShader(shaderProgramID, vertexShader);
    glAttachShader(shaderProgramID, fragmentShader);
    glLinkProgram(shaderProgramID);
    CheckStatus(shaderProgramID, Linker);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

}

void Shader::CheckStatus(GLuint shader, statusCheck t)
{
    int success;
    char infolog[1024];
    switch (t)
    {
        case VertexShader:
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if(!success)
            {
                glGetShaderInfoLog(shader, 1024, nullptr, infolog);
                std::cerr << "PROGRAM VERTEX COMPILE ERROR: " << infolog << "\n";
            }
            break;
        case FragmentShader:
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if(!success)
            {
                glGetShaderInfoLog(shader, 1024, nullptr, infolog);
                std::cerr << "PROGRAM FRAGMENT COMPILE ERROR: " << infolog << "\n";
            }
            break;
        case Linker:
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if(!success)
            {
                glGetProgramInfoLog(shader, 1024, nullptr, infolog);
                std::cerr << "PROGRAM LINK ERROR: "<< infolog << "\n";
            }
            break;
        default:
            std::cout << "Never Meant to be here \n";
    }
}

void Shader::use()
{
    glUseProgram(shaderProgramID);
}

Shader::~Shader()
{
    glDeleteProgram(shaderProgramID);
}
#pragma region setUniformOverloads

template<>
void Shader::setUniform<int>(const std::string& uniformName, int value)
{
    glUniform1i( glGetUniformLocation(shaderProgramID, uniformName.c_str()) , value);
}

template<>
void Shader::setUniform<bool>(const std::string& uniformName, bool value)
{
    glUniform1i( glGetUniformLocation(shaderProgramID, uniformName.c_str()) , static_cast<int>(value) );
}

template<>
void Shader::setUniform<float>(const std::string& uniformName, float value)
{
    glUniform1f( glGetUniformLocation(shaderProgramID, uniformName.c_str() ), value);
}

template<>
void Shader::setUniform<int,int>(const std::string& uniformName, int value1, int value2, int value3, int value4)
{
    glUniform4i( glGetUniformLocation(shaderProgramID, uniformName.c_str()) , value1, value2, value3, value4);
}

template<>
void Shader::setUniform<float,float>(const std::string& uniformName, float value1, float value2, float value3, float value4)
{
    glUniform4f( glGetUniformLocation(shaderProgramID, uniformName.c_str() ), value1, value2, value3, value4);
}

#pragma endregion