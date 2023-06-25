
		
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main()
{
    // Load the image
    Mat cal = imread("/home/fyp2023jb/ChessRobot2023FYP/State0.jpg", IMREAD_COLOR);
	imshow("Initial Image", cal);
    waitKey(0);
    // Set the number of inner corners of the chessboard
    cv::Size patternsize(7,7); //number of centers
	std::vector<cv::Point2f> centers; //this will be filled by the detected centers

	bool found = cv::findChessboardCorners( cal, patternsize, centers, cv::CALIB_CB_ADAPTIVE_THRESH );

	std::cout << found << "\n";
	if(found){      
		cv::drawChessboardCorners(cal,patternsize,centers,found);
		// I need the first corner at top-left
		if(centers.front().y > centers.back().y){
			std::cout << "Reverse order\n";
			std::reverse(centers.begin(),centers.end());
		}
		for(size_t r=0;r<patternsize.height;r++){
			for(size_t c=0;c<patternsize.width;c++){
				std::ostringstream oss;
				oss << "("<<r<<","<<c<<")";
				cv::putText(cal, oss.str(), centers[r*patternsize.width+c], cv::FONT_HERSHEY_PLAIN, 3, CV_RGB(0, 255, 0), 3);
			}
		}
	}
}
