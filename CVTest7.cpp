
		
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main()
{
    Mat image = imread("/home/fyp2023jb/ChessRobot2023FYP/State0.jpg");
    imshow("Original", image);
	waitKey(0);

	// Define the source points
	vector<Point2f> src_points;
	src_points.push_back(Point2f(0, 0));  // top-left
	src_points.push_back(Point2f(image.cols - 1, 0));  // top-right
	src_points.push_back(Point2f(image.cols - 1, image.rows - 1));  // bottom-right
	src_points.push_back(Point2f(0, image.rows - 1));  // bottom-left

	// Define the destination points
	vector<Point2f> dst_points;
	dst_points.push_back(Point2f(0, 0));  // top-left
	dst_points.push_back(Point2f(7 * 100, 0));  // top-right
	dst_points.push_back(Point2f(7 * 100, 7 * 100));  // bottom-right
	dst_points.push_back(Point2f(0, 7 * 100));  // bottom-left

	// Compute the perspective transform matrix
	Mat M = getPerspectiveTransform(src_points, dst_points);

	// Apply the perspective transform to the image
	Mat warped;
	warpPerspective(image, warped, M, Size(7 * 100, 7 * 100));

	// Display the result
	imshow("warped", warped);
	waitKey(0);
}
