// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from my_interface:msg/Example.idl
// generated code does not contain a copyright notice

#ifndef MY_INTERFACE__MSG__DETAIL__EXAMPLE__BUILDER_HPP_
#define MY_INTERFACE__MSG__DETAIL__EXAMPLE__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "my_interface/msg/detail/example__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace my_interface
{

namespace msg
{

namespace builder
{

class Init_Example_data2
{
public:
  explicit Init_Example_data2(::my_interface::msg::Example & msg)
  : msg_(msg)
  {}
  ::my_interface::msg::Example data2(::my_interface::msg::Example::_data2_type arg)
  {
    msg_.data2 = std::move(arg);
    return std::move(msg_);
  }

private:
  ::my_interface::msg::Example msg_;
};

class Init_Example_data1
{
public:
  Init_Example_data1()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_Example_data2 data1(::my_interface::msg::Example::_data1_type arg)
  {
    msg_.data1 = std::move(arg);
    return Init_Example_data2(msg_);
  }

private:
  ::my_interface::msg::Example msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::my_interface::msg::Example>()
{
  return my_interface::msg::builder::Init_Example_data1();
}

}  // namespace my_interface

#endif  // MY_INTERFACE__MSG__DETAIL__EXAMPLE__BUILDER_HPP_
