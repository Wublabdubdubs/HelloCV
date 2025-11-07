// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from interfaces:msg/My.idl
// generated code does not contain a copyright notice

#ifndef INTERFACES__MSG__DETAIL__MY__BUILDER_HPP_
#define INTERFACES__MSG__DETAIL__MY__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "interfaces/msg/detail/my__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace interfaces
{

namespace msg
{

namespace builder
{

class Init_My_num
{
public:
  Init_My_num()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  ::interfaces::msg::My num(::interfaces::msg::My::_num_type arg)
  {
    msg_.num = std::move(arg);
    return std::move(msg_);
  }

private:
  ::interfaces::msg::My msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::interfaces::msg::My>()
{
  return interfaces::msg::builder::Init_My_num();
}

}  // namespace interfaces

#endif  // INTERFACES__MSG__DETAIL__MY__BUILDER_HPP_
