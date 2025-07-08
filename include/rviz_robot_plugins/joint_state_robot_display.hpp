#pragma once

#include <rclcpp/rclcpp.hpp>
#include <rviz_common/display.hpp>
#include <rviz_common/display_context.hpp>
#include "rviz_common/properties/ros_topic_property.hpp"
#include "rviz_common/ros_integration/ros_node_abstraction_iface.hpp"

namespace rviz_robot_plugins
{

class JointStateRobotDisplay: public rviz_common::Display
{
  Q_OBJECT

public:
  JointStateRobotDisplay();
  virtual ~JointStateRobotDisplay() override = default;

protected:
  void onInitialize() override;
  void onEnable() override {};
  void onDisable() override {};

protected Q_SLOTS:
  void updateRobotDescriptionTopic();

private:
  rviz_common::ros_integration::RosNodeAbstractionIface::WeakPtr rviz_ros_node_;
  rviz_common::properties::RosTopicProperty * topic_property_;
};

}  // namespace rviz_robot_plugins
