#include <opencv2/opencv.hpp>
#include <string>
using namespace cv;
using namespace std;

string detectTrafficLightColor(Mat frame) {
    Mat hsv;
    cvtColor(frame, hsv, COLOR_BGR2HSV);

    // 红色HSV阈值
    Scalar lower_red1 = Scalar(0, 150, 150);
    Scalar upper_red1 = Scalar(10, 255, 255);
    Scalar lower_red2 = Scalar(160, 150, 150);
    Scalar upper_red2 = Scalar(180, 255, 255);
    // 绿色HSV阈值
    Scalar lower_green = Scalar(20, 50, 50);
    Scalar upper_green = Scalar(80, 255, 255);

    Mat mask_red1, mask_red2, mask_green;
    inRange(hsv, lower_red1, upper_red1, mask_red1);
    inRange(hsv, lower_red2, upper_red2, mask_red2);
    Mat mask_red = mask_red1 | mask_red2;
    inRange(hsv, lower_green, upper_green, mask_green);

    int red_pixels = countNonZero(mask_red);
    int green_pixels = countNonZero(mask_green);

    if (red_pixels > green_pixels && red_pixels > 500) return "Red";
    else if (green_pixels > 500) return "Green";
    else return "Unknown";
}

int main() {
    // 1. 读取输入视频
    VideoCapture cap("22/TrafficLight.mp4");
    if (!cap.isOpened()) {
        cerr << "无法打开视频文件 TrafficLight.mp4！" << endl;
        return -1;
    }

    // 获取视频基础参数
    double original_fps = cap.get(CAP_PROP_FPS);
    int width = (int)cap.get(CAP_PROP_FRAME_WIDTH);
    int height = (int)cap.get(CAP_PROP_FRAME_HEIGHT);

    // 2. 加快播放速度的核心设置
    double speed_multiplier = 3;
    int delay = 1000.0 / (original_fps * speed_multiplier);
    int skip_frames = 0;
    int frame_count = 0;

    // 3. 创建输出视频
    VideoWriter writer("result.avi", VideoWriter::fourcc('M', 'J', 'P', 'G'), original_fps, Size(width, height));
    if (!writer.isOpened()) {
        cerr << "无法创建输出视频文件 result.avi！" << endl;
        cap.release();
        return -1;
    }

    // 4. 逐帧处理
    Mat frame;
    while (cap.read(frame)) {
        if (frame.empty()) break;

        // 帧跳过逻辑
        if (frame_count % (skip_frames + 1) != 0) {
            frame_count++;
            continue;
        }

        // 检测交通灯颜色
        string color = detectTrafficLightColor(frame);

        // 标出发光区域
        Mat hsv, mask;
        cvtColor(frame, hsv, COLOR_BGR2HSV);
        if (color == "Red") {
            inRange(hsv, Scalar(0, 150, 150), Scalar(10, 255, 255), mask);
            Mat mask2;
            inRange(hsv, Scalar(160, 150, 150), Scalar(180, 255, 255), mask2);
            mask = mask | mask2;
        } else if (color == "Green") {
            inRange(hsv, Scalar(20, 20, 20), Scalar(80, 255, 255), mask);
        } else {
            // 未知颜色，不进行标记
            mask = Mat::zeros(frame.size(), CV_8UC1);
        }

        // 形态学操作去噪
        Mat kernel = getStructuringElement(MORPH_ELLIPSE, Size(7, 7));
        morphologyEx(mask, mask, MORPH_OPEN, kernel);
        morphologyEx(mask, mask, MORPH_CLOSE, kernel);

        // 找到发光区域并严格过滤
        vector<vector<Point>> contours;
        findContours(mask, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
        
        for (auto& cnt : contours) {
            double area = contourArea(cnt);
            
            // 面积过滤：只保留中等大小的区域（排除小反光点和大面积错误检测）
            if (area < 1000) continue;
            
            Rect rect = boundingRect(cnt);
            
            // 宽高比过滤：近似圆形（0.7-1.5之间）
            double ratio = (double)rect.width / rect.height;
            if (ratio < 0.7 || ratio > 1.5) continue;
            
            // 圆形度过滤
            double perimeter = arcLength(cnt, true);
            double circularity = 0;
            if (perimeter > 0) {
                circularity = 4 * CV_PI * area / (perimeter * perimeter);
            }
            // 圆形度阈值（1表示完美圆形，0.6表示比较圆）
            if (circularity < 0.3) continue;
            
            // 通过所有过滤条件，标记真正的红绿灯
            Scalar box_color = (color == "Red") ? Scalar(0, 0, 255) : Scalar(0, 255, 0);
            rectangle(frame, rect, box_color, 3);
            
            // 在框上方显示颜色和面积信息（调试用）
            string info = color + " A:" + to_string((int)area) + " C:" + to_string(circularity).substr(0,4);
            putText(frame, info, Point(rect.x, rect.y - 10), 
                   FONT_HERSHEY_SIMPLEX, 0.5, box_color, 1);
        }

        // 在左上角显示信号灯颜色
        string status_text = "Traffic Light: " + color;
        putText(frame, status_text, Point(20, 30), FONT_HERSHEY_SIMPLEX, 0.8, Scalar(0, 0, 255), 2);

        // 固定窗口大小+显示
        resizeWindow("Traffic Light Detection", 640, 480);
        imshow("Traffic Light Detection", frame);

        // 写入输出视频
        writer.write(frame);

        // 按调整后的延时播放，ESC退出
        if (waitKey(delay) == 27) break;
        frame_count++;
    }

    // 释放资源
    cap.release();
    writer.release();
    destroyAllWindows();
    return 0;
}