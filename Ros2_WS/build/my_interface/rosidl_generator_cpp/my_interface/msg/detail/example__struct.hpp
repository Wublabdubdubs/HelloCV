// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from my_interface:msg/Example.idl
// generated code does not contain a copyright notice

#ifndef MY_INTERFACE__MSG__DETAIL__EXAMPLE__STRUCT_HPP_
#define MY_INTERFACE__MSG__DETAIL__EXAMPLE__STRUCT_HPP_

#include <algorithm>
#include <array>
#include <memory>
#include <string>
#include <vector>

#include "rosidl_runtime_cpp/bounded_vector.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


#ifndef _WIN32
# define DEPRECATED__my_interface__msg__Example __attribute__((deprecated))
#else
# define DEPRECATED__my_interface__msg__Example __declspec(deprecated)
#endif

namespace my_interface
{

namespace msg
{

// message struct
template<class ContainerAllocator>
struct Example_
{
  using Type = Example_<ContainerAllocator>;

  explicit Example_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->data1 = 0l;
      this->data2 = 0ll;
    }
  }

  explicit Example_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    (void)_alloc;
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->data1 = 0l;
      this->data2 = 0ll;
    }
  }

  // field types and members
  using _data1_type =
    int32_t;
  _data1_type data1;
  using _data2_type =
    int64_t;
  _data2_type data2;

  // setters for named parameter idiom
  Type & set__data1(
    const int32_t & _arg)
  {
    this->data1 = _arg;
    return *this;
  }
  Type & set__data2(
    const int64_t & _arg)
  {
    this->data2 = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    my_interface::msg::Example_<ContainerAllocator> *;
  using ConstRawPtr =
    const my_interface::msg::Example_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<my_interface::msg::Example_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<my_interface::msg::Example_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      my_interface::msg::Example_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<my_interface::msg::Example_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      my_interface::msg::Example_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<my_interface::msg::Example_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<my_interface::msg::Example_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<my_interface::msg::Example_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__my_interface__msg__Example
    std::shared_ptr<my_interface::msg::Example_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__my_interface__msg__Example
    std::shared_ptr<my_interface::msg::Example_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const Example_ & other) const
  {
    if (this->data1 != other.data1) {
      return false;
    }
    if (this->data2 != other.data2) {
      return false;
    }
    return true;
  }
  bool operator!=(const Example_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct Example_

// alias to use template instance with default allocator
using Example =
  my_interface::msg::Example_<std::allocator<void>>;

// constant definitions

}  // namespace msg

}  // namespace my_interface

#endif  // MY_INTERFACE__MSG__DETAIL__EXAMPLE__STRUCT_HPP_
