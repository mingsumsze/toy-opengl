#include <glad/glad.h>
#include "shader.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include "error.h"


// lvalue & rvalue. Return string ref instead?
static std::string ParseShaderSourceFromFile(const std::string& shaderSourcePath) {
  std::ifstream fileStream(shaderSourcePath);
  std::stringstream stringStream;

  std::string line;
  while (getline(fileStream, line)) {
    stringStream << line << '\n';
  }
  return stringStream.str();
}

// static function here? inline?
static unsigned int CompileShader(unsigned int type, const std::string& shaderSource) {
  unsigned int shaderObj = glCreateShader(type);
  const char* shaderSource_cstr = shaderSource.c_str();

  // ID; no. strings; shader source; null
  GLCheckError(glShaderSource(shaderObj, 1, &shaderSource_cstr, nullptr));
  GLCheckError(glCompileShader(shaderObj));

  int success;
  GLCheckError(glGetShaderiv(shaderObj, GL_COMPILE_STATUS, &success));
  if (!success) {
    char infoLog[512];
    GLCheckError(glGetShaderInfoLog(shaderObj, 512, NULL, infoLog));
    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    GLCheckError(glDeleteShader(shaderObj));
    return -1;
  }

  return shaderObj;
}

static unsigned int CreateShaderProgram(const std::string& vertexShaderSource, const std::string& fragmentShaderSource) {
  unsigned int shaderProgram = glCreateProgram();
  unsigned int vertexShaderObj = CompileShader(GL_VERTEX_SHADER, vertexShaderSource);
  unsigned int fragmentShaderObj = CompileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

  GLCheckError(glAttachShader(shaderProgram, vertexShaderObj));
  GLCheckError(glAttachShader(shaderProgram, fragmentShaderObj));
  GLCheckError(glLinkProgram(shaderProgram));
  GLCheckError(glValidateProgram(shaderProgram));

  // Delete shader objects after link
  GLCheckError(glDeleteShader(vertexShaderObj));
  GLCheckError(glDeleteShader(fragmentShaderObj));

  return shaderProgram;
}

Shader::Shader(const std::string& vertexShaderSourcePath, const std::string& fragmentShaderSourcePath) {
  std::string vertexShaderSource = ParseShaderSourceFromFile(vertexShaderSourcePath);
  std::string fragmentShaderSource = ParseShaderSourceFromFile(fragmentShaderSourcePath);

  this->ID = CreateShaderProgram(vertexShaderSource, fragmentShaderSource);
}