#pragma once

#include "gl/draw_type.hpp"
#include "gl/gl.hpp"
#include "gl/type_traits.hpp"

#include <array>
#include <vector>

namespace gl {
template <std::size_t N> class vertex_array {
public:
  vertex_array() {}

  void set_draw_type(draw_type dtype) { _draw_type = dtype; }

  template <typename K>
  void set_buffer(std::size_t index, const std::vector<K> &buffer,
                  std::size_t n_elem = 0, std::size_t draw_size = 0) {
    static_assert(gl_type<K>::value != 0, "Unknown buffer value type.");
    static_assert(has_value_type<K>::value,
                  "Type K doesn't have typedef value_type.");

    auto ptr = reinterpret_cast<const std::uint8_t *>(buffer.data());
    _buffers[index] =
        std::vector<std::uint8_t>{ptr, ptr + buffer.size() * sizeof(K)};

    if (_array_id == 0) {
      glGenVertexArrays(1, &_array_id);
    }

    glBindVertexArray(_array_id);

    if (_buffers_id[index] == 0) {
      glGenBuffers(1, &_buffers_id[index]);
    }

    glBindBuffer(GL_ARRAY_BUFFER, _buffers_id[index]);
    glBufferData(GL_ARRAY_BUFFER, _buffers[index].size(),
                 (void *)_buffers[index].data(), GL_STREAM_DRAW);

    if (n_elem) {
      glVertexAttribPointer(index, n_elem, gl_type<K>::value, GL_FALSE, 0,
                            nullptr);
    } else {
      glVertexAttribPointer(index, sizeof(K) / sizeof(typename K::value_type),
                            gl_type<K>::value, GL_FALSE, 0, nullptr);
    }

    if (draw_size) {
      _draw_size = draw_size;
    } else {
      _draw_size = buffer.size();
    }
  }

  template <typename K, std::size_t Size>
  void set_buffer(std::size_t index, const K (&buffer)[Size]) {
    static_assert(gl_type<K>::value != 0, "Unknown buffer value type.");
    static_assert(has_value_type<K>::value,
                  "Type K doesn't have typedef value_type.");

    auto ptr = reinterpret_cast<const std::uint8_t *>(&buffer[0]);
    _buffers[index] = std::vector<std::uint8_t>{ptr, ptr + Size * sizeof(K)};

    if (_array_id == 0) {
      glGenVertexArrays(1, &_array_id);
    }

    glBindVertexArray(_array_id);

    if (_buffers_id[index] == 0) {
      glGenBuffers(1, &_buffers_id[index]);
    }

    glBindBuffer(GL_ARRAY_BUFFER, _buffers_id[index]);
    glBufferData(GL_ARRAY_BUFFER, _buffers[index].size(),
                 (void *)_buffers[index].data(), GL_STREAM_DRAW);
    glVertexAttribPointer(index, sizeof(K) / sizeof(typename K::value_type),
                          gl_type<K>::value, GL_FALSE, 0, nullptr);

    _draw_size = Size;
  }

  template <typename K, std::size_t Size>
  void set_buffer(std::size_t index, const K (&buffer)[Size],
                  std::size_t n_elem) {
    static_assert(gl_type<K>::value != 0, "Unknown buffer value type.");
    static_assert(has_value_type<K>::value == false,
                  "Call set_buffer without n_elem for custom types.");

    auto ptr = reinterpret_cast<const std::uint8_t *>(&buffer[0]);
    _buffers[index] = std::vector<std::uint8_t>{ptr, ptr + Size * sizeof(K)};

    if (_array_id == 0) {
      glGenVertexArrays(1, &_array_id);
    }

    glBindVertexArray(_array_id);

    if (_buffers_id[index] == 0) {
      glGenBuffers(1, &_buffers_id[index]);
    }

    glBindBuffer(GL_ARRAY_BUFFER, _buffers_id[index]);
    glBufferData(GL_ARRAY_BUFFER, _buffers[index].size(),
                 (void *)_buffers[index].data(), GL_STREAM_DRAW);
    glVertexAttribPointer(index, n_elem, gl_type<K>::value, GL_FALSE, 0,
                          nullptr);

    _draw_size = Size / n_elem;
  }

  void draw() {
    for (std::size_t i = 0; i < N; i++) {
      if (_buffers_id[i]) {
        glEnableVertexAttribArray(i);
      }
    }

    glBindVertexArray(_array_id);
    glDrawArrays(to_value(_draw_type), 0, _draw_size);

    for (std::size_t i = 0; i < N; i++) {
      if (_buffers_id[i]) {
        glDisableVertexAttribArray(i);
      }
    }
  }

private:
  std::array<std::vector<std::uint8_t>, N> _buffers;
  std::array<std::uint32_t, N> _buffers_id = {{0}};
  std::uint32_t _array_id = 0;
  std::uint32_t _draw_size = 0;
  draw_type _draw_type = draw_type::triangles;
};
} // namespace gl
