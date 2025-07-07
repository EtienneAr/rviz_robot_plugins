#pragma once

#include <rclcpp/rclcpp.hpp>
#include <rviz_common/display.hpp>

namespace rviz_robot_plugins
{

class JointStateRobotDisplay: public rviz_common::Display
{
  Q_OBJECT

public:
  JointStateRobotDisplay() {};
  virtual ~JointStateRobotDisplay() override = default;

protected:
  void onInitialize() override {};
  void onEnable() override {};
  void onDisable() override {};
};

}  // namespace rviz_robot_plugins
