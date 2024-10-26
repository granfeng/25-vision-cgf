#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/point_cloud2.hpp>
#include <pcl_conversions/pcl_conversions.h>
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl/filters/voxel_grid.h>
#include <pcl/io/pcd_io.h>

class PointCloudProcessor : public rclcpp::Node
{
public:
  PointCloudProcessor() : Node("point_cloud_processor")
  {
    // 读取原始点云
    pcl::PointCloud<pcl::PointXYZI>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZI>);
    pcl::io::loadPCDFile("basic_task3.pcd", *cloud);

    // 发布原始点云
    pub_origin_ = this->create_publisher<sensor_msgs::msg::PointCloud2>("/pcd_origin", 10);
    sensor_msgs::msg::PointCloud2::SharedPtr msg_origin(new sensor_msgs::msg::PointCloud2);
    pcl::toROSMsg(*cloud, *msg_origin);
    msg_origin->header.frame_id = "map";
    pub_origin_->publish(*msg_origin);

    // 降采样
    pcl::VoxelGrid<pcl::PointXYZI> voxel_filter;
    voxel_filter.setInputCloud(cloud);
    voxel_filter.setLeafSize(0.3, 0.3, 0.3);
    pcl::PointCloud<pcl::PointXYZI>::Ptr cloud_filtered(new pcl::PointCloud<pcl::PointXYZI>);
    voxel_filter.filter(*cloud_filtered);

    // 剔除高度大于0.8m的点云
    pcl::PointCloud<pcl::PointXYZI>::Ptr cloud_final(new pcl::PointCloud<pcl::PointXYZI>);
    for (const auto& point : *cloud_filtered)
    {
      if (point.z <= 0.8)
      {
        cloud_final->points.push_back(point);
      }
    }

    // 保存处理后的点云
    pcl::io::savePCDFile("basic_task3_result.pcd", *cloud_final);

    // 发布处理后的点云
    pub_result_ = this->create_publisher<sensor_msgs::msg::PointCloud2>("/pcd_result", 10);
    sensor_msgs::msg::PointCloud2::SharedPtr msg_result(new sensor_msgs::msg::PointCloud2);
    pcl::toROSMsg(*cloud_final, *msg_result);
    msg_result->header.frame_id = "map";
    pub_result_->publish(*msg_result);
  }

private:
  rclcpp::Publisher<sensor_msgs::msg::PointCloud2>::SharedPtr pub_origin_;
  rclcpp::Publisher<sensor_msgs::msg::PointCloud2>::SharedPtr pub_result_;
};

int main(int argc, char **argv)
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<PointCloudProcessor>());
  rclcpp::shutdown();
  return 0;
}