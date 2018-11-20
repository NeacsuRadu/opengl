#include <ShaderProgram.h>
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <fstream>
#include <sstream>

CShaderProgram::CShaderProgram(const std::string& strVSPath, const std::string& strFSPath)
{
    std::ifstream inVS(strVSPath);
    std::ifstream inFS(strFSPath);

    std::stringstream ssVS, ssFS;
    ssVS << inVS.rdbuf();
    ssFS << inFS.rdbuf();

    inVS.close();
    inFS.close();

    std::string sourceCode = ssVS.str();
    const char * code = sourceCode.c_str();
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &code, nullptr);
    glCompileShader(vertexShader);

    int success;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        char log[512];
        glGetShaderInfoLog(vertexShader, 512, nullptr, log);
        std::cout << "VERTEX SHADER COMPILE - " << log << std::endl;
    }

    sourceCode = ssFS.str();
    code = sourceCode.c_str();
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &code, nullptr);
    glCompileShader(fragmentShader);


    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        char log[512];
        glGetShaderInfoLog(fragmentShader, 512, nullptr, log);
        std::cout << "FRAGMENT SHADER COMPILE - " << log << std::endl;
    }

    m_shaderProgramId = glCreateProgram();
    glAttachShader(m_shaderProgramId, vertexShader);
    glAttachShader(m_shaderProgramId, fragmentShader);
    glLinkProgram(m_shaderProgramId);
    glGetProgramiv(m_shaderProgramId, GL_LINK_STATUS, &success);
    if (!success)
    {
        char log[512];
        glGetProgramInfoLog(m_shaderProgramId, 512, nullptr, log);
        std::cout << "SHADER PROGRAM LINK - " << log << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void CShaderProgram::Use() const
{
    glUseProgram(m_shaderProgramId);
}

void CShaderProgram::SetUniform(const std::string& name, const glm::vec3& vec) const
{
    glUniform3f(glGetUniformLocation(m_shaderProgramId, name.c_str()), vec.x, vec.y, vec.z);
}

void CShaderProgram::SetUniform(const std::string& name, const glm::mat4& mat) const
{
    glUniformMatrix4fv(glGetUniformLocation(m_shaderProgramId, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
}

void CShaderProgram::SetUniform(const std::string& name, int val) const
{
    glUniform1i(glGetUniformLocation(m_shaderProgramId, name.c_str()), val);
}

void CShaderProgram::SetUniform(const std::string& name, float val) const
{
    glUniform1f(glGetUniformLocation(m_shaderProgramId, name.c_str()), val);
}