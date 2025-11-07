// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from my_interface:msg/Example.idl
// generated code does not contain a copyright notice

#ifndef MY_INTERFACE__MSG__DETAIL__EXAMPLE__TRAITS_HPP_
#define MY_INTERFACE__MSG__DETAIL__EXAMPLE__TRAITS_HPP_

#include <stdint.h>

#include <sstream>
#include <string>
#include <type_traits>

#include "my_interface/msg/detail/example__struct.hpp"
#include "rosidl_runtime_cpp/traits.hpp"

namespace my_interface
{

namespace msg
{

inline void to_flow_style_yaml(
  const Example & msg,
  std::ostream & out)
{
  out << "{";
  // member: data1
  {
    out << "data1: ";
    rosidl_generator_traits::value_to_yaml(msg.data1, out);
    out << ", ";
  }

  // member: data2
  {
    out << "data2: ";
    rosidl_generator_traits::value_to_yaml(msg.data2, out);
  }
  out << "}";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const Example & msg,
  std::ostream & out, size_t indentation = 0)
{
  // member: data1
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "data1: ";
    rosidl_generator_traits::value_to_yaml(msg.data1, out);
    out << "\n";
  }

  // member: data2
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "data2: ";
    rosidl_generator_traits::value_to_yaml(msg.data2, out);
    out << "\n";
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const Example & msg, bool use_flow_style = false)
{
  std::ostringstream out;
  if (use_flow_style) {
    to_flow_style_yaml(msg, out);
  } else {
    to_block_style_yaml(msg, out);
  }
  return out.str();
}

}  // namespace msg

}  // namespace my_interface

namespace rosidl_generator_traits
{

[[deprecated("use my_interface::msg::to_block_style_yaml() instead")]]
inline void to_yaml(
  const my_interface::msg::Example & msg,
  std::ostream & out, size_t indentation = 0)
{
  my_interface::msg::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use my_interface::msg::to_yaml() instead")]]
inline std::string to_yaml(const my_interface::msg::Example & msg)
{
  return my_interface::msg::to_yaml(msg);
}

template<>
inline const char * data_type<my_interface::msg::Example>()
{
  return "my_interface::msg::Example";
}

template<>
inline const char * name<my_interface::msg::Example>()
{
  return "my_interface/msg/Example";
}

template<>
struct has_fixed_size<my_interface::msg::Example>
  : std::integral_constant<bool, true> {};

template<>
struct has_bounded_size<my_interface::msg::Example>
  : std::integral_constant<bool, true> {};

template<>
struct is_message<my_interface::msg::Example>
  : std::true_type {};

}  // namespace rosidl_generator_traits

#endif  // MY_INTERFACE__MSG__DETAIL__EXAMPLE__TRAITS_HPP_
