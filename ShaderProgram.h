#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H
#include <glm/glm.hpp>

#include <string>

class CShaderProgram
{
public:
    CShaderProgram(const std::string& strVSPath, const std::string& strFSPath);

    void Use() const;
    void SetUniform(const std::string& name, const glm::vec3& vec) const;
    void SetUniform(const std::string& name, const glm::mat4& mat) const;
    void SetUniform(const std::string& name, int val) const;
    void SetUniform(const std::string& name, float val) const;

    inline unsigned int GetId() const { return m_shaderProgramId; };
private:
    unsigned int m_shaderProgramId;
};

#endif // SHADER_PROGRAM_H