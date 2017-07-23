#pragma once

#include "gl/gl.hpp"
#include <type_traits>

namespace gl {
template <class T> struct Void { typedef void type; };

template <class T, class U = void> struct has_value_type : std::false_type {
  typedef std::false_type type;
};

template <class T>
struct has_value_type<T, typename Void<typename T::value_type>::type>
    : std::true_type {
  typedef std::true_type type;
};

template <std::size_t N> struct gl_real_value {
  static constexpr std::size_t value = 0;
};

template <> struct gl_real_value<4> {
  static constexpr std::size_t value = GL_FLOAT;
};

template <> struct gl_real_value<8> {
  static constexpr std::size_t value = GL_DOUBLE;
};

//----------------------------
template <std::size_t N> struct gl_signed_value {
  static constexpr std::size_t value = 0;
};

template <> struct gl_signed_value<1> {
  static constexpr std::size_t value = GL_BYTE;
};

template <> struct gl_signed_value<2> {
  static constexpr std::size_t value = GL_SHORT;
};

template <> struct gl_signed_value<4> {
  static constexpr std::size_t value = GL_INT;
};

// template<>
// struct gl_signed_value<8> {
//    static constexpr std::size_t value = 29;
//};
//----------------------------

//----------------------------
template <std::size_t N> struct gl_unsigned_value {
  static constexpr std::size_t value = 0;
};

template <> struct gl_unsigned_value<1> {
  static constexpr std::size_t value = GL_UNSIGNED_BYTE;
};

template <> struct gl_unsigned_value<2> {
  static constexpr std::size_t value = GL_UNSIGNED_SHORT;
};

template <> struct gl_unsigned_value<4> {
  static constexpr std::size_t value = GL_UNSIGNED_INT;
};

// template<>
// struct gl_unsigned_value<8> {
//    static constexpr std::size_t value = 37;
//};
//----------------------------

//----------------
template <typename T, typename is_arithmetic, typename is_integral,
          typename is_signed>
struct type_to_gl_type {
  static constexpr std::size_t value = 0;
};

/// signed integer
template <typename T>
struct type_to_gl_type<T, std::true_type, std::true_type, std::true_type> {
  static constexpr std::size_t value = gl_signed_value<sizeof(T)>::value;
};

/// unsigned integer
template <typename T>
struct type_to_gl_type<T, std::true_type, std::true_type, std::false_type> {
  static constexpr std::size_t value = gl_unsigned_value<sizeof(T)>::value;
};

// real.
template <typename T>
struct type_to_gl_type<T, std::true_type, std::false_type, std::true_type> {
  static constexpr std::size_t value = gl_real_value<sizeof(T)>::value;
};
//-------------------

template <typename T> struct open_gl_type {
  static constexpr std::size_t value =
      type_to_gl_type<T, typename std::is_arithmetic<T>::type,
                      typename std::is_integral<T>::type,
                      typename std::is_signed<T>::type>::value;
};

template <typename K, typename has_value, typename is_arith> struct is_blabla {
  static constexpr std::size_t value = 0;
};

template <typename K> struct is_blabla<K, std::true_type, std::false_type> {
  static constexpr std::size_t value =
      open_gl_type<typename K::value_type>::value;
};

template <typename K> struct is_blabla<K, std::false_type, std::true_type> {
  static constexpr std::size_t value = open_gl_type<K>::value;
};

template <typename T> struct gl_type {
  //
  static constexpr std::size_t value =
      is_blabla<T, typename has_value_type<T>::type,
                typename std::is_arithmetic<T>::type>::value;
};
} // namespace gl
