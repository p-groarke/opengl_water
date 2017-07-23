#pragma once

#include "gl/gl.hpp"
#include <bitset>
#include <cstdint>
#include <string>

namespace gl {
class shader {
public:
  enum class type {
    vertex,
    fragment,
    geometry,
    tessellation_control,
    tessellation_evaluation,
    none
  };

  inline shader() : _type(type::none) {}

  inline shader(type type, std::string &&content)
      : _content(std::move(content)), _type(type) {}

  bool compile();

  std::string get_error_string() const;

  inline std::uint32_t get_id() const { return _id; }

  inline type get_type() const { return _type; }

  inline bool is_compiled() const { return _is_compiled; }

private:
  std::string _content;
  type _type;
  std::uint32_t _id = 0;
  bool _is_compiled = false;
};

class program {
public:
  bool link();

  void activate() const;

  inline void attach_shader(const shader &shr) {
    if (!shr.is_compiled()) {
      return;
    }

    switch (shr.get_type()) {
    case shader::type::vertex:
      _vertex_id = shr.get_id();
      return;
    case shader::type::fragment:
      _fragment_id = shr.get_id();
      return;
    case shader::type::geometry:
      _geometry_id = shr.get_id();
      return;
    case shader::type::tessellation_control:
      _tess_ctrl_id = shr.get_id();
      return;
    case shader::type::tessellation_evaluation:
      _tess_eval_id = shr.get_id();
      return;
    default:
      return;
    }
  }

  inline std::uint32_t get_id() const { return _id; }

  inline bool is_linked() const { return _is_linked; }

  void bind_attribute(std::uint32_t index, const char *name) const;

  void bind_attribute(std::uint32_t index, const std::string &name) const;

  std::uint32_t get_uniform_location(const char *name) const;

  std::uint32_t get_uniform_location(const std::string &name) const;

  std::uint32_t get_attribute_location(const char *name) const;

  std::uint32_t get_attribute_location(const std::string &name) const;

  std::string get_error_string() const;

private:
  std::uint32_t _id = 0;
  bool _is_linked = false;
  std::uint32_t _vertex_id = 0;
  std::uint32_t _fragment_id = 0;
  std::uint32_t _geometry_id = 0;
  std::uint32_t _tess_ctrl_id = 0;
  std::uint32_t _tess_eval_id = 0;
};
} // namespace gl
