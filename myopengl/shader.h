#pragma once
#include <glm/glm.hpp>
#include <string>
#include <map>
#include "texture.h"

class Shader
{
public:
    unsigned int id;
    std::string name;

public:
    // constructor generates the shader on the fly
    // ------------------------------------------------------------------------
    Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr);
    Shader() = default;
    // activate the shader
    // ------------------------------------------------------------------------
    void use();

    // utility uniform functions
    // ------------------------------------------------------------------------
    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;
    void setVec2(const std::string& name, const glm::vec2& value) const;
    void setVec3(const std::string& name, const glm::vec3& value) const;
    void setVec4(const std::string& name, const glm::vec4& value) const;
    void setMat4(const std::string& name, const glm::mat4& mat) const;

private:

    // utility function for checking shader compilation/linking errors.
    // ------------------------------------------------------------------------
    void checkCompileErrors(unsigned int shader, std::string type);

private:
    /*std::map<int, std::unique_ptr<Texture>> m_TextureUniformNameAndBindUnitAndTC;*/
};


 