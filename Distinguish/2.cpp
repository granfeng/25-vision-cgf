#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace cv;
using namespace std;

// 计算两个点之间的距离
double getDistance(Point2f point1, Point2f point2) {
    return sqrt(pow(point1.x - point2.x, 2) + pow(point1.y - point2.y, 2));
}

// 定义灯条信息的结构体
struct LightInfo {
    RotatedRect rect;              // 最小外接矩形
    float angle;                   // 角度
    float length;                  // 长度
    Point2f center;                // 中心点
    Point2f points[4];             // 四个角点
    Point2f point2[2];             // 临时计算的点

    // 构造函数，计算中心点并初始化角度和长度
    LightInfo(const RotatedRect& r)
        : rect(r), center(r.center), angle(r.angle), length(max(r.size.width, r.size.height)) {
        
        // 将四个角点的信息存入points中，顺序是左上开始顺时针。
        r.points(points);

        // 计算宽的两个中点
        Point2f temp1 = (points[0] + points[1]) / 2;
        Point2f temp2 = (points[2] + points[1]) / 2;

        double dis1 = getDistance(temp1, center);
        double dis2 = getDistance(temp2, center);

        // 选择出宽的两个中点，且point2[0]在另一个中点之下
        if (dis1 >= dis2) {
            point2[0] = (temp1.y < center.y) ? temp1 : (2 * center - temp1);
            point2[1] = (temp1.y < center.y) ? (2 * center - temp1) : temp1;
        } else {
            point2[0] = (temp2.y < center.y) ? temp2 : (2 * center - temp2);
            point2[1] = (temp2.y < center.y) ? (2 * center - temp2) : temp2;
        }
    }
};

int main()
{
    // 初始化参数
    Mat frame;
    const string videoPath = "zimiao_test.mp4";
    VideoCapture cap(videoPath);

    if (!cap.isOpened()) {
        cerr << "无法打开视频文件: " << videoPath << endl;
        return -1;
    }

    // 参数设置
    const int kThreshold = 210;                // 二值化阈值
    const int kMaxVal = 255;                   // 最大值
    const Size kGaussianBlurSize = Size(5, 5); // 高斯模糊核大小

    cap >> frame; // 读取视频一帧确定分辨率
    int frameH = (int)frame.rows;
    int frameW = (int)frame.cols;

    Point2d a = Point2d(0, frameH / 2);
    Point2d b = Point2d(frameW, frameH / 2);

    // PnP解算世界坐标
    vector<Point3d> model_points = {
        Point3d(-69.0f, +26.0f, 0), // 左上角
        Point3d(+69.0f, +26.0f, 0),
        Point3d(+69.0f, -26.0f, 0),
        Point3d(-69.0f, -26.0f, 0)  // 右下角
    };

    Mat camera_matrix = (Mat_<double>(3, 3) << 1000, 0, frameW / 2, 0, 1000, frameH / 2, 0, 0, 1); // 相机参数
    Mat dist_coeffs = (Mat_<double>(5, 1) << 0, 0, 0, 0, 0); // 畸变系数
    Mat rotation_vector; // 旋转向量
    Mat translation_vector; // 平移向量

    // 灯条检测参数
    const int lightMinArea = 20;
    const int lightMaxAspectRatio = 6; // 最大宽长比
    const double lightContourMinSolidity = 0.6; // 外接矩形填充度
    const double maxAngleDiff = 7.0; // 角度差
    const double maxHeightDiffRatio = 0.2; // 高度差
    const float armored_plate_length_width_ratio1 = 0.5f; // 装甲板宽长比
    const float armored_plate_length_width_ratio2 = 3.0f;

    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;
    vector<LightInfo> detectedLights;

    while (true) {
        cap >> frame; // 读取视频帧
        if (frame.empty()) {
            cerr << "无法读取视频帧，可能已到达视频末尾" << endl;
            break;
        }

        // 复制当前帧用于绘制
        Mat frameToDraw = frame.clone();

        // 分离颜色通道并计算差异
        Mat channels[3];
        split(frame, channels);
        Mat diff = channels[2] - channels[0];

        // 二值化处理
        Mat binary;
        threshold(diff, binary, kThreshold, kMaxVal, THRESH_BINARY);

        // 应用高斯模糊
        Mat gaussianBlurred;
        GaussianBlur(binary, gaussianBlurred, kGaussianBlurSize, 0);

        // 膨胀操作
        Mat element = getStructuringElement(MORPH_RECT, Size(9, 9));
        Mat dilated;
        dilate(gaussianBlurred, dilated, element); // 使灯条变宽

        // 查找轮廓
        findContours(dilated, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);

        // 清空检测到的灯条信息
        detectedLights.clear();

        // 遍历所有轮廓
        for (const auto& contour : contours) {
            double ContourArea = contourArea(contour);
            if (ContourArea < lightMinArea) continue;

            RotatedRect lightRect = minAreaRect(contour);
            double aspectRatio = lightRect.size.width / lightRect.size.height;
            double solidity = ContourArea / lightRect.size.area();

            // 过滤不合格的灯条
            if (aspectRatio > lightMaxAspectRatio || solidity < lightContourMinSolidity) continue;

            // 添加检测到的灯条信息
            detectedLights.emplace_back(lightRect);
        }

        // 按灯条中心x坐标排序
        sort(detectedLights.begin(), detectedLights.end(), [](const LightInfo& li1, const LightInfo& li2) {
            return li1.center.x < li2.center.x;
        });

        // 遍历所有灯条进行匹配
        for (size_t i = 0; i < detectedLights.size(); ++i) {
            for (size_t j = i + 1; j < detectedLights.size(); ++j) {
                const LightInfo& leftLight = detectedLights[i];
                const LightInfo& rightLight = detectedLights[j];

                // 计算相似度指标
                float angleDiff = abs(leftLight.angle - rightLight.angle);
                float lenDiffRatio = abs(leftLight.length - rightLight.length) / max(leftLight.length, rightLight.length);
                float len = (leftLight.length + rightLight.length) / 2.0f; // 均长度
                float wid = getDistance(leftLight.center, rightLight.center); // 中心距离近似为宽度
                float len_wid_ratio = wid / len;

                // 检查匹配条件
                if (angleDiff > maxAngleDiff || lenDiffRatio > maxHeightDiffRatio ||
                    len_wid_ratio < armored_plate_length_width_ratio1 || len_wid_ratio > armored_plate_length_width_ratio2) {
                    continue;
                }

                // 匹配成功，绘制结果
                Point2f vertices[4]; // 四个角点
                vertices[0] = leftLight.point2[0];
                vertices[1] = rightLight.point2[0];
                vertices[2] = rightLight.point2[1];
                vertices[3] = leftLight.point2[1];

                // 绘制角点和连接线
                for (int k = 0; k < 4; k++) {
                    circle(frameToDraw, vertices[k], 3, Scalar(0, 255, 0), FILLED);
                    putText(frameToDraw, to_string(k + 1), vertices[k], FONT_HERSHEY_SIMPLEX, 0.5, Scalar(255, 255, 255), 1, 1);
                }

                // 连接灯条中心
                Point center = Point((leftLight.center.x + rightLight.center.x) / 2, (leftLight.center.y + rightLight.center.y) / 2);
                for (int k = 0; k < 4; k++) {
                    line(frameToDraw, vertices[k], vertices[(k + 1) % 4], Scalar(0, 0, 255), 1);
                }
                circle(frameToDraw, center, 3, Scalar(255, 0, 0), FILLED);
                line(frameToDraw, leftLight.center, rightLight.center, Scalar(0, 255, 0), 1);

                // PnP解算像素坐标
                vector<Point2d> image_points = {
                    Point2d(vertices[0]),
                    Point2d(vertices[1]),
                    Point2d(vertices[2]),
                    Point2d(vertices[3])
                };

                // 解算PnP
                solvePnP(model_points, image_points, camera_matrix, dist_coeffs, rotation_vector, translation_vector);
                Mat Rvec, Tvec;
                rotation_vector.convertTo(Rvec, CV_32F);  // 旋转向量转换格式
                translation_vector.convertTo(Tvec, CV_32F); // 平移向量转换格式 

                Mat_<float> rotMat(3, 3);
                Rodrigues(Rvec, rotMat); // 旋转向量转成旋转矩阵

                // 求解相机的世界坐标
                Mat P_oc = rotMat.inv() * Tvec; // 求相机到物体的距离，单位是mm
                float Dis = (pow(pow(P_oc.at<float>(0, 0), 2) + pow(P_oc.at<float>(0, 1), 2) + pow(P_oc.at<float>(0, 2), 2), 0.5) * 10) / 100;
                string distanceStr = to_string(int(Dis)) + "." + to_string(int(Dis * 10 - int(Dis) * 10)) + "cm";
                putText(frameToDraw, distanceStr, (vertices[0] + vertices[3]) / 2, FONT_HERSHEY_COMPLEX, 0.5, Scalar(255, 255, 255), 1, 3);

                // 计算并显示角度
                float angle = (atan(P_oc.at<float>(0, 1) / P_oc.at<float>(0, 2))) * 180 / CV_PI; // 使用PI常量
                string angleStr = to_string(int(angle)) + "." + to_string(int(abs(angle * 10 - int(angle) * 10))) + "Du";
                putText(frameToDraw, angleStr, a, FONT_HERSHEY_COMPLEX, 0.5, Scalar(255, 255, 255), 1, 3);
            }
        }

        // 显示结果帧
        imshow("Detected Lights", frameToDraw);
        if (waitKey(30) == 27) break; // 按ESC键退出
    }

    // 释放视频捕获对象  
    cap.release();
    return 0;

}