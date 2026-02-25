#ifndef SHADER_HPP
#define SHADER_HPP

#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>
#include <string>

class ShaderManager {
private:
    GLuint shaderID;

    std::string readFile(const char* filePath);
    GLuint compileShader(const char* source, GLenum shaderType);
    GLuint linkProgram(GLuint vertexShaderID, GLuint fragmentShaderID);
    GLuint loadShaders(const char* vertexFilePath, const char* fragmentFilePath);

public:
    ShaderManager(const char* vertexPath, const char* fragmentPath);
	ShaderManager();
    ~ShaderManager();

    GLuint getShaderID() const;

    void sendMatrix4(const char* varName, const glm::mat4& matrix) const;
    void sendVec3(const char* varName, const glm::vec3& vector) const;
    void sendFloat(const char* varName, GLfloat value) const;
    void sendInt(const char* varName, GLint value) const;
};

#endif // SHADER_HPP

