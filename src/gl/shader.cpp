#include "gl/shader.hpp"
#include <fstream>
#include <vector>

namespace gl {
GLenum shader_type_to_gl_enum(shader::type type) {
  switch (type) {
  case shader::type::vertex:
    return GL_VERTEX_SHADER;
  case shader::type::fragment:
    return GL_FRAGMENT_SHADER;
  case shader::type::geometry:
    return GL_GEOMETRY_SHADER;
  case shader::type::tessellation_control:
    return GL_TESS_CONTROL_SHADER;
  case shader::type::tessellation_evaluation:
    return GL_TESS_EVALUATION_SHADER;
  default:
    return 0;
  }
}

bool shader::compile() {

  if (_type == type::none) {
    return false;
  }

  if (_content.empty()) {
    return false;
  }

  _id = glCreateShader(shader_type_to_gl_enum(_type));

  const char *data = _content.c_str();
  glShaderSource(_id, 1, &data, nullptr);

  // Compile shader.
  glCompileShader(_id);

  GLint compile_status = 0;
  glGetShaderiv(_id, GL_COMPILE_STATUS, &compile_status);
  _is_compiled = compile_status;

  return _is_compiled;
}

std::string shader::get_error_string() const {
  if (_is_compiled) {
    return "";
  }

  int info_length = 0;
  glGetShaderiv(_id, GL_INFO_LOG_LENGTH, &info_length);

  if (info_length > 1) {
    std::string msg(info_length, 0);
    GLsizei n_char_written = 0;
    glGetShaderInfoLog(_id, info_length, &n_char_written, &msg[0]);
    return msg;
  }

  return "";
}

bool program::link() {
  _id = glCreateProgram();

  if (_vertex_id) {
    glAttachShader(_id, _vertex_id);
  }

  if (_fragment_id) {
    glAttachShader(_id, _fragment_id);
  }

  if (_geometry_id) {
    glAttachShader(_id, _geometry_id);
  }

  if (_tess_ctrl_id) {
    glAttachShader(_id, _tess_ctrl_id);
  }

  if (_tess_eval_id) {
    glAttachShader(_id, _tess_eval_id);
  }

  glLinkProgram(_id);

  int linked = 0;
  glGetProgramiv(_id, GL_LINK_STATUS, &linked);
  _is_linked = linked;
  return _is_linked;
}

void program::activate() const {
  if (!_is_linked) {
    return;
  }
  glUseProgram(_id);
}

void program::bind_attribute(std::uint32_t index, const char *name) const {
  glBindAttribLocation(_id, index, name);
}

void program::bind_attribute(std::uint32_t index,
                             const std::string &name) const {
  glBindAttribLocation(_id, index, name.c_str());
}

std::uint32_t program::get_attribute_location(const char *name) const {
  return glGetAttribLocation(_id, name);
}

std::uint32_t program::get_attribute_location(const std::string &name) const {
  return glGetAttribLocation(_id, name.c_str());
}

std::uint32_t program::get_uniform_location(const char *name) const {
  return glGetUniformLocation(_id, name);
}

std::uint32_t program::get_uniform_location(const std::string &name) const {
  return glGetUniformLocation(_id, name.c_str());
}

std::string program::get_error_string() const {
  if (_is_linked) {
    return "";
  }

  int info_length = 0;
  glGetProgramiv(_id, GL_INFO_LOG_LENGTH, &info_length);

  if (info_length > 1) {
    std::string msg(info_length, 0);
    GLsizei n_char_written = 0;
    glGetProgramInfoLog(_id, info_length, &n_char_written, &msg[0]);
    return msg;
  }

  return "";
}

} // namespace gl
