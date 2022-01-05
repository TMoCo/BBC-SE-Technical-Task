/*
* Author: Thomas Moreno Cooper
* Date Created: 01/01/2022
* Copyright 2022 under MIT License
*
* Shader class definition
*/

#include <glad/glad.h>
#include <fstream>
#include <Shader.h>

Shader::Shader()
  : id{ 0 }, valid{ false }
{}

Shader::Shader(const char* vs_path, const char* fs_path) 
  : id{ 0 }, valid{ false } 
{
  const std::vector<char> vs_data = getShaderCode(vs_path);
  const std::vector<char> fs_data = getShaderCode(fs_path);

  uint32_t vs = glCreateShader(GL_VERTEX_SHADER);
  const char* p_vs_data = vs_data.data();
  glShaderSource(vs, 1, &p_vs_data, NULL);
  glCompileShader(vs);
  checkShaderCompile(vs, "vertex");

  uint32_t fs = glCreateShader(GL_FRAGMENT_SHADER);
  const char* p_fs_data = fs_data.data();
  glShaderSource(fs, 1, &p_fs_data, NULL);
  glCompileShader(fs);
  checkShaderCompile(fs, "fragment");

  id = glCreateProgram();
  glAttachShader(id, vs);
  glAttachShader(id, fs);
  glLinkProgram(id);

  if (checkShaderCompile(id, "program") == 0)
  {
    valid = true;
  }

  glDeleteShader(vs);
  glDeleteShader(fs);
}

Shader::~Shader()
{
  glDeleteProgram(id);
}

void Shader::use()
{
  glUseProgram(id);
}

bool Shader::isValid() const
{
  return valid;
}

void Shader::setBool(const char* name, bool value) const
{
  glUniform1i(glGetUniformLocation(id, name), (int)value);
}

void Shader::setInt(const char* name, int value) const
{
  glUniform1i(glGetUniformLocation(id, name), value);
}

void Shader::setFloat(const char* name, float value) const
{
  glUniform1f(glGetUniformLocation(id, name), value);
}

void Shader::setVec2(const char* name, const Vector2& value) const
{
  glUniform2fv(glGetUniformLocation(id, name), 1, &value[0]);
}

void Shader::setVec3(const char* name, const Vector3& value) const
{
  glUniform3fv(glGetUniformLocation(id, name), 1, &value[0]);
}

void Shader::setVec4(const char* name, const Vector4& value) const
{
  glUniform4fv(glGetUniformLocation(id, name), 1, &value[0]);
}

void Shader::setMatrix4(const char* name, const Matrix4& value) const
{
  glUniformMatrix4fv(glGetUniformLocation(id, name), 1, GL_FALSE, value[0]);
}


const std::vector<char> Shader::getShaderCode(const char* path)
{
  std::ifstream shaderStream(path, std::ios::ate);

  std::vector<char> data;

  if (!shaderStream.is_open())
  {
    std::cerr << "Error! Failed to open file stream!\npath provided: " << path << std::endl;
  }

  uint32_t len = (uint32_t)shaderStream.tellg();
  data.resize(len);

  shaderStream.seekg(0);
  shaderStream.read(data.data(), len);

  shaderStream.close();

  return data;
}

int Shader::checkShaderCompile(uint32_t shader, const char* type)
{
  int success;
  static char infoLog[1024];

  if (strcmp(type, "program") != 0)
  {
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
      glGetShaderInfoLog(shader, sizeof(infoLog), NULL, infoLog);
      std::cerr << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << std::endl;
      return -1;
    }
  }
  else
  {
    glGetProgramiv(shader, GL_LINK_STATUS, &success);
    if (!success)
    {
      glGetProgramInfoLog(shader, sizeof(infoLog), NULL, infoLog);
      std::cerr << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << std::endl;
      return -1;
    }
  }
  return 0;
}