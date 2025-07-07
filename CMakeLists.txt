cmake_minimum_required(VERSION 3.8)
project(rviz_robot_plugins)

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(rviz_common REQUIRED)
find_package(rviz_rendering REQUIRED)
find_package(rviz_default_plugins REQUIRED)

set(CMAKE_AUTOMOC ON)
qt5_wrap_cpp(MOC_FILES
  include/rviz_robot_plugins/joint_state_robot_display.hpp
)
add_library(${PROJECT_NAME} SHARED
  src/joint_state_robot_display.cpp  ${MOC_FILES}
)

target_include_directories(${PROJECT_NAME}
  PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
    "$<INSTALL_INTERFACE:include>")

ament_target_dependencies(${PROJECT_NAME}
  rclcpp
  pluginlib
  rviz_common
  rviz_rendering
  rviz_default_plugins
)

# # Causes the visibility macros to use dllexport rather than dllimport (for Windows, when your plugin should be used as library)
target_compile_definitions(${PROJECT_NAME} PRIVATE "RVIZ_ROBOT_PLUGINS_BUILDING_LIBRARY")

install(
  DIRECTORY include/
  DESTINATION include
)

install(
  TARGETS ${PROJECT_NAME}
  EXPORT export_${PROJECT_NAME}
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  RUNTIME DESTINATION bin
)

ament_export_targets(export_${PROJECT_NAME} HAS_LIBRARY_TARGET)

pluginlib_export_plugin_description_file(rviz_common plugins_description.xml)

install(FILES
  package.xml
  DESTINATION share/${PROJECT_NAME}
)

ament_package()