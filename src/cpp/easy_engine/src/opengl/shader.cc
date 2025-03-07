// shader.cc - 着色器实现
// Version: 1.0
// Author: Wang Zhuowei wang.zhuowei@foxmail.com
// Copyright: (c) wang.zhuowei@foxmail.com All rights reserved.
// Last Change: 2020 Aug 14
// License: GPL.v3

#include "include/opengl/shader.h"

#include <fstream>
#include <map>
#include <sstream>

#include "include/opengl/exception.h"
#include "include/opengl/glad/glad.h"

namespace {

const int InfoLen = 512;  // 信息数组长度

// 根据类型得到着色器的名称
// @param type: 着色器类型
std::string GetErrorType(int type) {
  std::map<int, std::string> msg_tab{
      {GL_FRAGMENT_SHADER, "fragment shader"},
      {GL_VERTEX_SHADER, "vertex shader"},
      {GL_GEOMETRY_SHADER, "geometry shader"},
  };
  return msg_tab[type];
}

}  // namespace

namespace easy_engine {
namespace opengl {

Shader::~Shader() {
  int result;
  glGetShaderiv(_id, GL_DELETE_STATUS, &result);
  if (!result) {
    Delete();
  }
}

void Shader::Delete() { glDeleteShader(_id); }

void Shader::Init(const std::string& shader_path, int type) {
  std::ifstream path(shader_path);
  if (!path.is_open()) {
    std::string msg("Open: " + shader_path + " failed");
    throw GlException(msg);
  }
  std::stringstream source;
  source << path.rdbuf();
  path.close();
  std::string shader_str(source.str());
  const char* shader_code = shader_str.c_str();

  int  success;
  char info_log[InfoLen];
  _id = glCreateShader(type);
  glShaderSource(_id, 1, &shader_code, NULL);
  glCompileShader(_id);
  glGetShaderiv(_id, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(_id, InfoLen, NULL, info_log);
    std::string msg("Type: " + GetErrorType(type) +
                    " Msg: " + std::string(info_log));
    throw GlException(msg);
  }
}

VertexShader::VertexShader(const std::string& shader_path) {
  Init(shader_path, GL_VERTEX_SHADER);
}

GeometryShader::GeometryShader(const std::string& shader_path) {
  Init(shader_path, GL_GEOMETRY_SHADER);
}

FragmentShader::FragmentShader(const std::string& shader_path) {
  Init(shader_path, GL_FRAGMENT_SHADER);
}

ShaderProgram::ShaderProgram() : _id(0), _is_compile(false) {
  _id = glCreateProgram();
}

ShaderProgram::~ShaderProgram() {
  int result;
  glGetProgramiv(_id, GL_DELETE_STATUS, &result);
  if (false == result) {
    glDeleteProgram(_id);
  }
}

void ShaderProgram::Attach(std::shared_ptr<Shader> shader) {
  _v_shader_ids.push_back(shader);
}

void ShaderProgram::Compile() {
  for (auto& shader : _v_shader_ids) {
    glAttachShader(_id, shader->Id());
  }
  glLinkProgram(_id);
  int  success;
  char info_log[InfoLen];
  glGetProgramiv(_id, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(_id, InfoLen, NULL, info_log);
    std::string msg("Link error,Msg: " + std::string(info_log));
    throw GlException(msg);
  }
  _is_compile = true;
  for (auto& shader : _v_shader_ids) {
    shader->Delete();
  }
}

void ShaderProgram::Bind() {
  if (!_is_compile) {
    Compile();
  }
  glUseProgram(_id);
}

void ShaderProgram::SetUniform(const std::string& name, bool value) const {
  glUniform1i(glGetUniformLocation(_id, name.c_str()), (int)value);
}

void ShaderProgram::SetUniform(const std::string& name, int value) const {
  glUniform1i(glGetUniformLocation(_id, name.c_str()), value);
}

void ShaderProgram::SetUniform(const std::string& name,
                               unsigned int       value) const {
  glUniform1i(glGetUniformLocation(_id, name.c_str()), value);
}

void ShaderProgram::SetUniform(const std::string& name, float value) const {
  glUniform1f(glGetUniformLocation(_id, name.c_str()), value);
}

void ShaderProgram::SetUniform(const std::string& name,
                               const vec2&        value) const {
  auto pos = glGetUniformLocation(_id, name.c_str());
  glUniform2f(pos, value.x, value.y);
}

void ShaderProgram::SetUniform(const std::string& name,
                               const vec3&        value) const {
  auto pos = glGetUniformLocation(_id, name.c_str());
  glUniform3f(pos, value.x, value.y, value.z);
}

void ShaderProgram::SetUniform(const std::string& name,
                               const vec4&        value) const {
  auto pos = glGetUniformLocation(_id, name.c_str());
  glUniform4f(pos, value.x, value.y, value.z, value.w);
}

void ShaderProgram::SetUniform(const std::string& name,
                               const mat3&        value) const {
  auto pos = glGetUniformLocation(_id, name.c_str());
  glUniformMatrix3fv(pos, 1, GL_FALSE, value_ptr(value));
}

void ShaderProgram::SetUniform(const std::string& name,
                               const mat4&        value) const {
  auto pos = glGetUniformLocation(_id, name.c_str());
  glUniformMatrix4fv(pos, 1, GL_FALSE, value_ptr(value));
}

};  // namespace opengl
};  // namespace easy_engine
