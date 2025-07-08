#include <pluginlib/class_list_macros.hpp>
#include <rosidl_runtime_cpp/traits.hpp>
#include <std_msgs/msg/string.hpp>

#include "rviz_robot_plugins/joint_state_robot_display.hpp"

namespace rviz_robot_plugins
{

JointStateRobotDisplay::JointStateRobotDisplay()
{
    topic_property_ = new rviz_common::properties::RosTopicProperty(
        "Robot Description",
        "/robot_description",
        QString::fromStdString(rosidl_generator_traits::name<std_msgs::msg::String>()),
        "Topic to get robot urdf model",
        this,
        SLOT(updateRobotDescriptionTopic())
    );
}

void JointStateRobotDisplay::onInitialize()
{
    rviz_ros_node_ = context_->getRosNodeAbstraction();
    topic_property_->initialize(rviz_ros_node_);
}

void JointStateRobotDisplay::updateRobotDescriptionTopic()
{
    context_->queueRender();
}

} //namespace rviz_robot_plugins

PLUGINLIB_EXPORT_CLASS(rviz_robot_plugins::JointStateRobotDisplay, rviz_common::Display)