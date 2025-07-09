#include <pluginlib/class_list_macros.hpp>
#include <rosidl_runtime_cpp/traits.hpp>

#include "rviz_robot_plugins/joint_state_robot_display.hpp"
#include "rviz_default_plugins/robot/robot_link.hpp"

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
    // Init ros node and topics
    rviz_ros_node_ = context_->getRosNodeAbstraction();
    topic_property_->initialize(rviz_ros_node_);

    // Init visual robot
    robot_ = std::make_unique<rviz_default_plugins::robot::Robot>(scene_node_, context_, "Robot: ", this);

    // Read topics
    updateRobotDescriptionTopic();
}

void JointStateRobotDisplay::updateRobotDescriptionTopic()
{
    if (topic_property_->isEmpty()) {
      setStatus(
        rviz_common::properties::StatusProperty::Error, "Robot Description topic", QString("Error subscribing: Empty topic name"));
      return;
    }

    try {
        setStatus(rviz_common::properties::StatusProperty::Warn, "URDF", "Waiting to receive message");

        robot_model_subscription_.reset();

        rclcpp::QoS qosLatching = rclcpp::QoS(rclcpp::KeepLast(1));
        qosLatching.transient_local();

        rclcpp::Node::SharedPtr node = rviz_ros_node_.lock()->get_raw_node();
        robot_model_subscription_ =
            node->create_subscription<std_msgs::msg::String>(
            topic_property_->getTopicStd(),
            qosLatching,
            std::bind(&JointStateRobotDisplay::updateRobotModel, this, std::placeholders::_1)
        );
        setStatus(rviz_common::properties::StatusProperty::Ok, "Robot Description topic", "Valid");
    } catch (rclcpp::exceptions::InvalidTopicNameError & e) {
      setStatus(
        rviz_common::properties::StatusProperty::Error, "Robot Description topic", QString("Error subscribing: ") + e.what());
    }
}

void JointStateRobotDisplay::updateRobotModel(std_msgs::msg::String::ConstSharedPtr msg)
{
    // Robot description message received
    setStatus(rviz_common::properties::StatusProperty::Ok, "URDF", "Received");

    // Parsing urdf
    urdf::Model descr;
    if (!descr.initString(msg->data)) {
        setStatus(rviz_common::properties::StatusProperty::Error, "URDF", "URDF failed Model parse");
        return;
    }
    setStatus(rviz_common::properties::StatusProperty::Ok, "URDF", "URDF parsed OK");

    // Loading robot
    robot_->clear();
    robot_->load(descr);

    // Getting load errors
    std::stringstream ss;
    for (const auto & name_link_pair : robot_->getLinks()) {
        const std::string err = name_link_pair.second->getGeometryErrors();
        if (!err.empty()) {
            ss << "\n• for link '" << name_link_pair.first << "':\n" << err;
        }
    }

    // Display load errors
    if (ss.tellp()) {
        setStatus(rviz_common::properties::StatusProperty::Error, "URDF", QString("Errors loading geometries:").append(ss.str().c_str()));
    }
}

} //namespace rviz_robot_plugins

PLUGINLIB_EXPORT_CLASS(rviz_robot_plugins::JointStateRobotDisplay, rviz_common::Display)