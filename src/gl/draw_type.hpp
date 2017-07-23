#pragma once

#include <cstdint>

namespace gl {
enum class draw_type {
  points = GL_POINTS,
  lines = GL_LINES,
  line_loop = GL_LINE_LOOP,
  line_strip = GL_LINE_STRIP,
  triangles = GL_TRIANGLES,
  triangle_strip = GL_TRIANGLE_STRIP,
  triangle_fan = GL_TRIANGLE_FAN
};

inline std::uint32_t to_value(draw_type dtype) {
  return static_cast<std::uint32_t>(dtype);
}

} // namespace gl
