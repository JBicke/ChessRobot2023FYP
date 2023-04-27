// Make Move Attempt 2

#include "deltaInterpretation.h"
#include "opencv2/opencv.hpp"
#include <iostream>

cv::Mat deltaCalc(cv::Mat oldPosition, cv::Mat newPosition) {
	cv::Mat diffMat;
	diffMat = newPosition - oldPosition;
	return diffMat;
}


int main() {
	cv::Mat board(8,8,CV_32F,cv::Scalar(0));
	
	for (int i =0; i< 8; i++) {
		board.at<float>(i,0) = 1;
		board.at<float>(i,1) = 1;
		board.at<float>(i,6) = -1;
		board.at<float>(i,7) = -1;
	}
	
	std::cout << "M = " << std::endl << " " << board << std::endl << std::endl;
	
	cv::Mat boardnew(8,8,CV_32F,cv::Scalar(0));
	
	for (int i =0; i< 8; i++) {
		boardnew.at<float>(i,0) = 1;
		boardnew.at<float>(i,1) = 1;
		boardnew.at<float>(i,6) = -1;
		boardnew.at<float>(i,7) = -1;
	}
	
	boardnew.at<float>(4,3) = 1;
	boardnew.at<float>(4,1) = 0;
	
	std::cout << "M = " << std::endl << " " << boardnew << std::endl << std::endl;
	
	cv::Mat deltaMatrix = deltaCalc(board, boardnew);
	
	std::cout << "M = " << std::endl << " " << deltaMatrix << std::endl << std::endl;
	
	int result = deltaInterp(deltaMatrix);

}


