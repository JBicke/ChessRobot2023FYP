// g++ -o OpenCV_Testing OpenCV_Testing.cpp -I/usr/include/opencv4 `pkg-config --cflags --libs opencv4`
// /OpenCV_Testing

#include <opencv2/opencv.hpp>
#include <vector>
#include <iostream>

using namespace cv;
using namespace std;

int main() {
	Mat image = imread("/home/fyp2023jb/ChessRobot2023FYP/State0.jpg"); // read image from file
	if (image.empty()) // check if image was loaded successfully
	{
		cout << "Error: image not found" << endl;
		return 1;
	}
	
	Mat gray;
	cvtColor(image, gray, COLOR_BGR2GRAY);

	// Size boardSize(8, 8);
	// vector<Point2f> corners;
	// bool found = findChessboardCorners(image, boardSize, corners);

	Mat edges;
	Canny(gray, edges, 50, 150,3);
	
	imshow("Edges",edges); // show image in a window
	waitKey(0); // wait for a key press
	
	vector<Vec2f> lines;
	HoughLines(edges,lines,1,CV_PI/180,100);
	
	
	
	Mat output;
    cvtColor(edges, output, COLOR_GRAY2BGR);
    for (const auto& line : lines)
    {
        float rho = line[0], theta = line[1];
        Point pt1, pt2;
        double a = cos(theta), b = sin(theta);
        double x0 = a*rho, y0 = b*rho;
        pt1.x = cvRound(x0 + 1000*(-b));
        pt1.y = cvRound(y0 + 1000*(a));
        pt2.x = cvRound(x0 - 1000*(-b));
        pt2.y = cvRound(y0 - 1000*(a));
        
		Vec<float, 2> color = {0, 0, 255};
		line(output, pt1, pt2, Scalar(0,0,255), 3, LINE_AA);
    }
    
	imshow("Output", output);
    waitKey(0);
	
	return 0;
}

		
