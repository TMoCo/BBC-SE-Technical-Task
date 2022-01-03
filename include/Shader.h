/*
* Author: Thomas Moreno Cooper
* Date Created: 01/01/2022
* Copyright 2022 under MIT License
*
* Shader class declaration
*/

#ifndef SHADER_H
#define SHADER_H 1

#include <thomath.h>
#include <vector>

class Shader
{
public:
  Shader();

  Shader(const char* vs_path, const char* fs_path);

  ~Shader();

  void use();

  bool isValid() const;

  void setBool(const char* name, bool value) const;

  void setInt(const char* name, int value) const; 

  void setFloat(const char* name, float value) const;

  void setVec2(const char* name, const Vector2& value) const;
  
  void setVec3(const char* name, const Vector3& value) const;

  void setVec4(const char* name, const Vector4& value) const;

  void setMatrix4(const char* name, const Matrix4& value) const;

private:
  const std::vector<char> getShaderCode(const char* path);

  int checkShaderCompile(uint32_t shader, const char* type);

  uint32_t id;

  bool valid;
};

#endif // !SHADER_H
