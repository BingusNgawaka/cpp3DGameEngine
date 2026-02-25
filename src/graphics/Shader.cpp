#include "../../include/graphics/Shader.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

std::string ShaderManager::readFile(const char* filePath) {
    std::ifstream fileStream(filePath, std::ios::in);
    if (!fileStream.is_open()) {
        throw std::runtime_error("Impossible to open " + std::string(filePath) + ". Are you in the right directory?");
    }
    std::stringstream sstr;
    sstr << fileStream.rdbuf();
    return sstr.str();
}

GLuint ShaderManager::compileShader(const char* source, GLenum shaderType) {
    GLuint shaderID = glCreateShader(shaderType);
    glShaderSource(shaderID, 1, &source, nullptr);
    glCompileShader(shaderID);

    GLint result;
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &result);
	
    if (!result) {
        GLint logLength;
        glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &logLength);
        std::vector<char> errorMessage(logLength);
        glGetShaderInfoLog(shaderID, logLength, nullptr, errorMessage.data());
        std::cerr << errorMessage.data() << std::endl;
        glDeleteShader(shaderID);
        throw std::runtime_error("Shader compilation failed!");
    }
    return shaderID;
}

GLuint ShaderManager::linkProgram(GLuint vertexShaderID, GLuint fragmentShaderID) {
    GLuint programID = glCreateProgram();
    glAttachShader(programID, vertexShaderID);
    glAttachShader(programID, fragmentShaderID);
    glLinkProgram(programID);

    GLint result;
    glGetProgramiv(programID, GL_ATTACHED_SHADERS, &result);
	std::cout << "Attached Shaders: " << result << std::endl;

	glValidateProgram(programID);
	GLint validationResult;
	glGetProgramiv(programID, GL_VALIDATE_STATUS, &validationResult);
	std::cout << "Valid Program: " << validationResult << std::endl;
	std::cout << "ID: " << programID << std::endl;



    if (!result) {
        GLint logLength;
        glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &logLength);
        std::vector<char> errorMessage(logLength);
        glGetProgramInfoLog(programID, logLength, nullptr, errorMessage.data());
        std::cerr << errorMessage.data() << std::endl;
        glDeleteProgram(programID);
        throw std::runtime_error("Program linking failed!");
    }

    return programID;
}

GLuint ShaderManager::loadShaders(const char* vertexFilePath, const char* fragmentFilePath) {
    std::string vertexCode = readFile(vertexFilePath);
    std::string fragmentCode = readFile(fragmentFilePath);

    GLuint vertexShaderID, fragmentShaderID;
    try {
        vertexShaderID = compileShader(vertexCode.c_str(), GL_VERTEX_SHADER);
        fragmentShaderID = compileShader(fragmentCode.c_str(), GL_FRAGMENT_SHADER);
        GLuint programID = linkProgram(vertexShaderID, fragmentShaderID);
        return programID;
    } catch (...) {
        glDeleteShader(vertexShaderID);
        glDeleteShader(fragmentShaderID);
        throw;
    }
}

ShaderManager::ShaderManager(const char* vertexPath, const char* fragmentPath) {
    shaderID = loadShaders(vertexPath, fragmentPath);
}
ShaderManager::ShaderManager(){
}

ShaderManager::~ShaderManager() {
    //glDeleteProgram(shaderID);
}

GLuint ShaderManager::getShaderID() const {
    return shaderID;
}

void ShaderManager::sendMatrix4(const char* varName, const glm::mat4& matrix) const {
    GLint varLocation = glGetUniformLocation(shaderID, varName);
    glUniformMatrix4fv(varLocation, 1, GL_FALSE, &matrix[0][0]);
}

void ShaderManager::sendVec3(const char* varName, const glm::vec3& vector) const {
    GLint varLocation = glGetUniformLocation(shaderID, varName);
    glUniform3fv(varLocation, 1, &vector[0]);
}

void ShaderManager::sendFloat(const char* varName, GLfloat value) const {
    GLint varLocation = glGetUniformLocation(shaderID, varName);
    glUniform1f(varLocation, value);
}

void ShaderManager::sendInt(const char* varName, GLint value) const {
    GLint varLocation = glGetUniformLocation(shaderID, varName);
    glUniform1i(varLocation, value);
}
