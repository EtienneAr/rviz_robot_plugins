#pragma once

#include <rclcpp/rclcpp.hpp>
#include <rviz_common/display.hpp>

namespace rviz_robot_plugins
{

class JointStateRobotDisplay : public rviz_common::Display
{
  Q_OBJECT

public:
  JointStateRobotDisplay() {}
  ~JointStateRobotDisplay() override = default;

  void onInitialize() override {}
  void reset() override {}

protected:
  void update(float wall_dt, float ros_dt) override {}
};

}  // namespace rviz_robot_plugins
