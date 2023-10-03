#ifndef VISION_H
#define VISION_H
#include "opencv2/opencv.hpp"
#include <array>
#include <iostream>

using namespace cv;
using namespace std;

std::vector<cv::Point> locateGreenSquares(const cv::Mat& inputImage);
cv::Mat rotateImageToStraight(const cv::Mat& image, cv::Point2f point1, cv::Point2f point2);
// FilteredPoints cannyEdgeDetection(const cv::Mat& image);
// CenterPoints processAndShowContours(const cv::Mat& src);
std::vector<std::vector<int>> transposeMatrix(const std::vector<std::vector<int>>& matrix);
double calculateDistance(const cv::Point& point1, const cv::Point& point2);
void rotateChessboard(int chessboard[8][8]);
void reflectYAxis(int chessboard[8][8]);
cv::Mat CVRunMain(std::string photoName);




#endif
