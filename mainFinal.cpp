// -I/usr/include/opencv4 `pkg-config --cflags --libs opencv4`
// g++ -o chessRobot main.cpp deltaInterpretation.cpp callStockfish.cpp -I/usr/include/opencv4 `pkg-config --cflags --libs opencv4`
#include "deltaInterpretation.h"
#include "Vision.h"
#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>
#include <array>
#include <vector>

using namespace std;

cv::Mat deltaCalc(cv::Mat oldPosition, cv::Mat newPosition) {
	cv::Mat diffMat;
	diffMat = newPosition - oldPosition;
	return diffMat;
}

string chessNotation(int row, int col) {
	char rowChar = 'a' + row;
	// cout << rowChar << "inline" << endl;
	col = col+1;
	// cout << col << "inline" << endl;
	string result = "";
	result += rowChar;
	result += to_string(col);
	return result; 
}

int main() {
	
	// Create Example Board
	cv::Mat board = CVRunMain();
	
	std::cout << board << std::endl;

	
}