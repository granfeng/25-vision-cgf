/*
    实现过程：
    0.打开视频
    1.预处理：通道分离、删除己方装甲板颜色、阈值化、膨胀;PnP解算世界坐标
    2.找轮廓
    3.遍历提取灯条（面积限定、矫正灯条、宽高比筛选）
    4.对灯条进行匹配（角差、长度差比率、左右间隔、确定装甲板）
    5.模板匹配 根据装甲板中心的图案判断是不是装甲板
    6.求解相机的世界坐标 计算装甲板中心到相机的距离并显示角度
*/

#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <algorithm>
#include <map>

using namespace cv;
using namespace std;

// 表示敌我颜色、装甲板大小类型、装甲板检测情况
enum EnemyColor { RED, BLUE };
enum ArmorType { SMALL_ARMOR, BIG_ARMOR };
enum ArmorFlag { ARMOR_NO, ARMOR_LOST, ARMOR_LOCAL };

// 基本参数
struct Params
{
    float brightness_threshold;           // 亮度阈值
    float light_min_area;                 // 灯条的最小面积
    float light_max_ratio;                // 灯条的最大宽高比
    float light_contour_min_solidity;     // 灯条轮廓的最小凸度
    float light_color_detect_extend_ratio; // 灯条颜色检测的扩展比率
    float light_max_angle_diff_;          // 灯条之间的最大角度差
    float light_max_height_diff_ratio_;   // 灯条之间的最大高度差比率
    float light_max_y_diff_ratio_;        // 灯条之间的最大y坐标差比率
    float light_min_x_diff_ratio_;        // 灯条之间的最小x坐标差比率
    float armor_max_aspect_ratio_;        // 装甲板的最大宽高比
    float armor_min_aspect_ratio_;        // 装甲板的最小宽高比
    float armor_big_armor_ratio;          // 大装甲板的宽高比
    float armor_small_armor_ratio;        // 小装甲板的宽高比
};

// 灯条特征
struct LightDescriptor
{
    RotatedRect rect;  // 外接旋转矩形
    Point2f center;    // 中心点
    float angle;       // 角度
    float length;      // 长度


    LightDescriptor(const RotatedRect& r) : rect(r)  // 构造函数，初始化灯条的特征
    {
        center = r.center;
        angle = r.angle;
        length = max(r.size.width, r.size.height);
    }
};

// 装甲板特征
struct ArmorDescriptor
{
    LightDescriptor leftLight;   // 左灯条
    LightDescriptor rightLight;  // 右灯条
    ArmorType type;              // 装甲板类型（大装甲板或小装甲板）
    Mat frontImg;                // 装甲板前视图图像
    float rotationScore;         // 旋转程度
    vector<Point2f> vertex;      // 装甲板的顶点

    // 构造函数，初始化装甲板的特征
    ArmorDescriptor(const LightDescriptor& ll, const LightDescriptor& rl, ArmorType t, const Mat& img, float rs, const Params& p)
        : leftLight(ll), rightLight(rl), type(t), frontImg(img), rotationScore(rs) {
        // 根据灯条计算装甲板的顶点位置
        Point2f leftTop = ll.rect.center + Point2f(0, -ll.rect.size.height / 2);
        Point2f leftBottom = ll.rect.center + Point2f(0, ll.rect.size.height / 2);
        Point2f rightTop = rl.rect.center + Point2f(0, -rl.rect.size.height / 2);
        Point2f rightBottom = rl.rect.center + Point2f(0, rl.rect.size.height / 2);
        vertex = { leftTop, rightTop, rightBottom, leftBottom };
    }

    // 判断装甲板是否符合模板匹配
    bool isArmorPattern(std::vector<cv::Mat>& small, std::vector<cv::Mat>& big, int& lastEnemy);
};

// 函数声明
void adjustRec(RotatedRect& rec, int mode);                                                                      // 调整角度和宽高
float distance(const Point2f& p1, const Point2f& p2);                                                            // 计算两点距离
double TemplateMatch(const Mat& img, const Mat& templ, Point& matchLoc, int method);                             // 模板匹配
void calculateWorldCoordinates(const ArmorDescriptor& armor, const Mat& cameraMatrix, const Mat& distCoeffs);    // 计算装甲板在相机坐标系中的位置和姿态

// 主函数
int main()
{
    // 导入视频
    string videoFilePath = "zimiao_test.mp4";
    VideoCapture cap(videoFilePath);
    if (!cap.isOpened())
    {
        cout << "视频无法打开" << endl;
        return -1;
    }

    // 定义相机内参和畸变系数 相机焦距:fx,fy  相机的主点坐标：cx,cy  相机的畸变系数 k1, k2, p1, p2, k3
    double fx = 1000, cx = 320, fy = 1000, cy = 240;
    double k1 = 0, k2 = 0, p1 = 0, p2 = 0, k3 = 0;
    // 相机内参矩阵
    Mat cameraMatrix = (Mat_<double>(3, 3) << fx, 0, cx, 0, fy, cy, 0, 0, 1);
    // 相机畸变系数矩阵
    Mat distCoeffs = (Mat_<double>(5, 1) << k1, k2, p1, p2, k3);

    // 定义并初始化参数结构体
    Params _param ={210.0f,10.0f,1.0f,0.5f,1.1f,7.0f,0.2f,2.0f,0.5f,5.0f,1.0f,3.2f,2.0f};
    // 定义敌方装甲板的颜色
    EnemyColor _enemy_color = RED;
    // 定义小装甲板和大装甲板的模板图像集合
    vector<Mat> _small_Armor_template, _big_Armor_template;
    // 定义上一次检测到的敌方类型
    int lastEnemy = 0;
    // 定义装甲板检测状态
    ArmorFlag _flag = ARMOR_NO;
    // 定义符合的装甲板集合
    vector<ArmorDescriptor> _armors;
    // 定义并初始化目标装甲板
    ArmorDescriptor _targetArmor(LightDescriptor(RotatedRect()), LightDescriptor(RotatedRect()), SMALL_ARMOR, Mat(), 0.0, _param);

    // 运行程序
    Mat frame;
    while (true)
    {
        // 逐帧读取
        cap >> frame;
        if (frame.empty()) break;

        // 分离通道
        vector<Mat> channels;
        split(frame, channels);
        // 生成灰度图像
        Mat grayImg;
        if (_enemy_color == RED)
            grayImg = channels.at(2) - channels.at(0);
        else
            grayImg = channels.at(0) - channels.at(2);

        // 二值化
        Mat binBrightImg;
        threshold(grayImg, binBrightImg, _param.brightness_threshold, 255, THRESH_BINARY);
        // 膨胀
        Mat element = getStructuringElement(MORPH_ELLIPSE, Size(3, 3));
        dilate(binBrightImg, binBrightImg, element);
        
        // 查找轮廓，储存灯条
        vector<vector<Point>> lightContours;
        findContours(binBrightImg.clone(), lightContours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
        // 筛选灯条
        vector<LightDescriptor> lightInfos;
        for (const auto& contour : lightContours)
        {
            // 面积限定
            float lightContourArea = contourArea(contour);
            if (lightContourArea < _param.light_min_area) continue;
            // 检查轮廓点数是否足够
            if (contour.size() < 5) continue;
            // 拟合成旋转矩形
            RotatedRect lightRec = fitEllipse(contour);
            // 调整旋转矩形
            adjustRec(lightRec, 0);
            // 宽高比和凸度
            if (lightRec.size.width / lightRec.size.height > _param.light_max_ratio ||
                lightContourArea / lightRec.size.area() < _param.light_contour_min_solidity)
                continue;
            // 扩展旋转矩形的尺寸
            lightRec.size.width *= _param.light_color_detect_extend_ratio;
            lightRec.size.height *= _param.light_color_detect_extend_ratio;
            // 计算旋转矩形的边界矩形
            Rect lightRect = lightRec.boundingRect();
            // 裁剪边界矩形
            const Rect srcBound(Point(0, 0), frame.size());
            lightRect &= srcBound;
            // 存储符合条件的灯条
            lightInfos.push_back(LightDescriptor(lightRec));
        }
        // 更新筛选结果
        if (lightInfos.empty())
        {
            _flag = ARMOR_NO;
            continue;
        }
        // 灯条排序
        sort(lightInfos.begin(), lightInfos.end(), [](const LightDescriptor& ld1, const LightDescriptor& ld2) 
        {
            return ld1.center.x < ld2.center.x;
        });
        // 匹配灯条 框选装甲板
        _armors.clear();
        // 遍历
        for (size_t i = 0; i < lightInfos.size(); i++)
        {
            for (size_t j = i + 1; j < lightInfos.size(); j++)
            {
                // 获取左右灯条信息
                const LightDescriptor& leftLight = lightInfos[i];
                const LightDescriptor& rightLight = lightInfos[j];
                // 计算角度差和长度差
                float angleDiff_ = abs(leftLight.angle - rightLight.angle);
                float LenDiff_ratio = abs(leftLight.length - rightLight.length) / max(leftLight.length, rightLight.length);
                if (angleDiff_ > _param.light_max_angle_diff_ || LenDiff_ratio > _param.light_max_height_diff_ratio_)
                    continue;
                // 计算距离、平均长度、y 坐标差比率和 x 坐标差比率
                float dis = distance(leftLight.center, rightLight.center);          // 计算左右灯条中心点的距离
                float meanLen = (leftLight.length + rightLight.length) / 2;         // 计算左右灯条的平均长度
                float yDiff = abs(leftLight.center.y - rightLight.center.y);        // 计算左右灯条中心点的 y 坐标差
                float yDiff_ratio = yDiff / meanLen;                                // 计算 y 坐标差比率
                float xDiff = abs(leftLight.center.x - rightLight.center.x);        // 计算左右灯条中心点的 x 坐标差
                float xDiff_ratio = xDiff / meanLen;                                // 计算 x 坐标差比率
                float ratio = dis / meanLen;                                        // 计算距离与平均长度的比率
                if (yDiff_ratio > _param.light_max_y_diff_ratio_ ||
                    xDiff_ratio < _param.light_min_x_diff_ratio_ ||
                    ratio > _param.armor_max_aspect_ratio_ ||
                    ratio < _param.armor_min_aspect_ratio_)
                    continue;
                // 确定装甲板类型和旋转程度
                ArmorType armorType = ratio > _param.armor_big_armor_ratio ? BIG_ARMOR : SMALL_ARMOR;                                                                           // 根据比率确定装甲板类型
                float ratiOff = (armorType == BIG_ARMOR) ? max(_param.armor_big_armor_ratio - ratio, float(0)) : max(_param.armor_small_armor_ratio - ratio, float(0));         // 计算比率偏差
                float yOff = yDiff / meanLen;
                float rotationScore = -(ratiOff * ratiOff + yOff * yOff);                                                                                                       // 计算旋转程度
                // 生成符合的装甲板并存储
                ArmorDescriptor armor(leftLight, rightLight, armorType, channels.at(2), rotationScore, _param);
                _armors.emplace_back(armor);
                break;
            }
        }
        // 更新装甲板筛选结果
        if (_armors.empty())
        {
            _flag = ARMOR_NO;
            continue;
        }
        // 移除不符合模板匹配条件的装甲板
        _armors.erase(remove_if(_armors.begin(), _armors.end(), [&](ArmorDescriptor& i)
        {
            return 0 == (i.isArmorPattern(_small_Armor_template, _big_Armor_template, lastEnemy));
        }), _armors.end());
        // 检测装甲板筛选结果
        if (_armors.empty())
        {
            if (_flag == ARMOR_LOCAL)
            {
                _flag = ARMOR_LOST;
            }
            else
            {
                _flag = ARMOR_NO;
            }
            continue;
        }
        // 选择目标装甲板并计算目标装甲板的世界坐标
        _targetArmor = _armors[0];
        calculateWorldCoordinates(_targetArmor, cameraMatrix, distCoeffs);

         // 显示结果
        imshow("Frame", frame);
        if (waitKey(30) == 27) break;
    }

    // 释放
    cap.release();
    destroyAllWindows();
    return 0;
}

bool ArmorDescriptor::isArmorPattern(std::vector<cv::Mat>& small, std::vector<cv::Mat>& big, int& lastEnemy)
{
    // 获取装甲板的前视图图像
    cv::Mat armorImg = this->frontImg;
    // 选择模板图像集合
    std::vector<cv::Mat>& templates = (this->type == SMALL_ARMOR) ? small : big;
    // 初始化匹配结果
    double maxVal = 0;
    cv::Point matchLoc;
    // 遍历模板图像集合，进行模板匹配
    for (const auto& templ : templates)
    {
        cv::Mat result;
        cv::matchTemplate(armorImg, templ, result, cv::TM_CCOEFF_NORMED);
        double minVal, val;
        cv::Point minLoc, loc;
        cv::minMaxLoc(result, &minVal, &val, &minLoc, &loc);
        // 更新最大匹配值和匹配位置
        if (val > maxVal)
        {
            maxVal = val;
            matchLoc = loc;
        }
    }
    // 判断匹配结果是否符合条件
    if (maxVal > 0.5)
    { 
        // 假设匹配阈值为 0.5
        lastEnemy = (this->type == SMALL_ARMOR) ? 0 : 1; // 假设小装甲板对应敌方类型 0，大装甲板对应敌方类型 1
        return true;
    } 
    else
    {
        return false;
    }
}

void adjustRec(RotatedRect& rec, int mode)
{
    using std::swap;
    float& width = rec.size.width;
    float& height = rec.size.height;
    float& angle = rec.angle;

    while (angle >= 90.0) angle -= 180.0;
    while (angle < -90.0) angle += 180.0;

    if (angle >= 45.0)
    {
        swap(width, height);
        angle -= 90.0;
    }
    else if (angle < -45.0)
    {
        swap(width, height);
        angle += 90.0;
    }
}

float distance(const Point2f& p1, const Point2f& p2)
{
    return sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y));
}

double TemplateMatch(const Mat& img, const Mat& templ, Point& matchLoc, int method)
{
    Mat result;
    matchTemplate(img, templ, result, method);
    double minVal, maxVal;
    Point minLoc, maxLoc;
    minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc);
    matchLoc = (method == TM_SQDIFF || method == TM_SQDIFF_NORMED) ? minLoc : maxLoc;
    return maxVal;
}

void calculateWorldCoordinates(const ArmorDescriptor& armor, const Mat& cameraMatrix, const Mat& distCoeffs)
{
    vector<Point2f> imagePoints = armor.vertex;
    vector<Point3f> objectPoints =
    {
        Point3f(-armor.leftLight.length / 2, armor.leftLight.length / 2, 0),
        Point3f(armor.rightLight.length / 2, armor.rightLight.length / 2, 0),
        Point3f(armor.rightLight.length / 2, -armor.rightLight.length / 2, 0),
        Point3f(-armor.leftLight.length / 2, -armor.leftLight.length / 2, 0)
    };

    Mat rvec, tvec;
    solvePnP(objectPoints, imagePoints, cameraMatrix, distCoeffs, rvec, tvec);

    double distance = norm(tvec);

    Mat R;
    Rodrigues(rvec, R);
    Mat cameraPosition = -R.t() * tvec;
    double pitch = atan2(cameraPosition.at<double>(2), cameraPosition.at<double>(0));
    double yaw = atan2(cameraPosition.at<double>(1), cameraPosition.at<double>(0));

    cout << "相机到装甲板的距离: " << distance << " 米" << endl;
    cout << "俯仰角: " << pitch * 180 / CV_PI << " 度" << endl;
    cout << "偏航角: " << yaw * 180 / CV_PI << " 度" << endl;
}
