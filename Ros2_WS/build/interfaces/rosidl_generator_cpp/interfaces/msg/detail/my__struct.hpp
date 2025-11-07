// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from interfaces:msg/My.idl
// generated code does not contain a copyright notice

#ifndef INTERFACES__MSG__DETAIL__MY__STRUCT_HPP_
#define INTERFACES__MSG__DETAIL__MY__STRUCT_HPP_

#include <algorithm>
#include <array>
#include <memory>
#include <string>
#include <vector>

#include "rosidl_runtime_cpp/bounded_vector.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


#ifndef _WIN32
# define DEPRECATED__interfaces__msg__My __attribute__((deprecated))
#else
# define DEPRECATED__interfaces__msg__My __declspec(deprecated)
#endif

namespace interfaces
{

namespace msg
{

// message struct
template<class ContainerAllocator>
struct My_
{
  using Type = My_<ContainerAllocator>;

  explicit My_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->num = 0ll;
    }
  }

  explicit My_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    (void)_alloc;
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->num = 0ll;
    }
  }

  // field types and members
  using _num_type =
    int64_t;
  _num_type num;

  // setters for named parameter idiom
  Type & set__num(
    const int64_t & _arg)
  {
    this->num = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    interfaces::msg::My_<ContainerAllocator> *;
  using ConstRawPtr =
    const interfaces::msg::My_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<interfaces::msg::My_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<interfaces::msg::My_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      interfaces::msg::My_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<interfaces::msg::My_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      interfaces::msg::My_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<interfaces::msg::My_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<interfaces::msg::My_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<interfaces::msg::My_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__interfaces__msg__My
    std::shared_ptr<interfaces::msg::My_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__interfaces__msg__My
    std::shared_ptr<interfaces::msg::My_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const My_ & other) const
  {
    if (this->num != other.num) {
      return false;
    }
    return true;
  }
  bool operator!=(const My_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct My_

// alias to use template instance with default allocator
using My =
  interfaces::msg::My_<std::allocator<void>>;

// constant definitions

}  // namespace msg

}  // namespace interfaces

#endif  // INTERFACES__MSG__DETAIL__MY__STRUCT_HPP_
