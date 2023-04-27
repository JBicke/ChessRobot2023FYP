// g++ -o OpenCV_Testing OpenCV_Testing.cpp -I/usr/include/opencv4 `pkg-config --cflags --libs opencv4`
// /OpenCV_Testing

#include "opencv2/opencv.hpp"
#include <iostream>

using namespace cv;
using namespace std;

int main() {
	Mat image = imread("chessboard.jpg"); // read image from file
	if (image.empty()) // check if image was loaded successfully
	{
		cout << "Error: image not found" << endl;
		return 1;
	}
	
	
	Mat gray;
	cvtColor(image, gray, COLOR_BGR2GRAY);
	
	Mat edges;
	Canny(gray, edges, 50, 150);
	
	imshow("Edges",edges); // show image in a window
	waitKey(0); // wait for a key press
	
	
	return 0;
}

		
