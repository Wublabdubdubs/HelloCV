cmake_minimum_required(VERSION 3.8)
project(topic)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(my_interface REQUIRED)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # comment the line when a copyright and license is added to all source files
  set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # comment the line when this package is in a git repo and when
  # a copyright and license is added to all source files
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()

add_executable(TopicPublisher src/Topic_Publisher.cpp)
add_executable(TopicSubscriber src/Topic_Subscriber.cpp)
ament_target_dependencies(TopicPublisher rclcpp std_msgs my_interface)
ament_target_dependencies(TopicSubscriber rclcpp std_msgs my_interface)

install(
  TARGETS TopicPublisher
  DESTINATION lib/${PROJECT_NAME}
)

install(
  TARGETS TopicSubscriber
  DESTINATION lib/${PROJECT_NAME}
)

