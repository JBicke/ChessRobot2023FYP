// g++ -o OpenCV_Testing OpenCV_Testing.cpp -I/usr/include/opencv4 `pkg-config --cflags --libs opencv4`
// /OpenCV_Testing

#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main()
{
	Mat image = imread("/home/fyp2023jb/ChessRobot2023FYP/State0.jpg"); // read image from file
	int square_size = 100;
	
	imshow("before",image); // show image in a window
	waitKey(0); // wait for a key press
	
	// convert image to HSV color space
	Mat hsv;
	cvtColor(image, hsv, COLOR_BGR2HSV);
	
	imshow("hsv",hsv); // show image in a window
	waitKey(0); // wait for a key press
	
	// define range of blue color in HSV
	Scalar lower_blue = Scalar(100, 50, 50);
	Scalar upper_blue = Scalar(130, 255, 255);

	// define range of pink color in HSV
	Scalar lower_pink = Scalar(140, 100, 100);
	Scalar upper_pink = Scalar(170, 255, 255);

	// create binary masks for blue and pink areas
	Mat blue_mask, pink_mask;
	inRange(hsv, lower_blue, upper_blue, blue_mask);
	inRange(hsv, lower_pink, upper_pink, pink_mask);

	// create matrix to store the chessboard state
	Mat state = Mat::zeros(8, 8, CV_8SC1);

	// loop over each square in the chessboard
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			// compute the pixel coordinates of the center of the square
			int x = j * square_size + square_size / 2;
			int y = i * square_size + square_size / 2;

			// check if the center pixel is blue or pink
			if (blue_mask.at<uchar>(y, x) > 0) {
				state.at<char>(i, j) = 1; // white piece
			} else if (pink_mask.at<uchar>(y, x) > 0) {
				state.at<char>(i, j) = -1; // black piece
			} else {
				state.at<char>(i, j) = 0; // no piece
			}
		}
	}
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			cout << (int) state.at<char>(i, j) << " ";
		}
    cout << endl;
	}
}
		
