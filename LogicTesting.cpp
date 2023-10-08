// -I/usr/include/opencv4 `pkg-config --cflags --libs opencv4`
// g++ -o chessRobot main.cpp deltaInterpretation.cpp callStockfish.cpp -I/usr/include/opencv4 `pkg-config --cflags --libs opencv4`
#include "deltaInterpretation.h"
#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>
#include <array>
#include <vector>

using namespace std;

cv::Mat deltaCalc(cv::Mat oldPosition, cv::Mat newPosition) {
	cv::Mat diffMat(8,8,CV_32S);
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

	cv::Mat boardNew(8,8,CV_32S,cv::Scalar(0));
	
	cv::Mat boardOld(8,8,CV_32S,cv::Scalar(0));
	

	//std::cout << boardOld << std::endl;


	
	
	// Print Example Board
	// cout << "M = " << endl << " " << board << endl << endl;
	
	// Create Sample Move
	
	
	
	for (int i =0; i< 8; i++) {
	 	boardOld.at<int>(i,0) = 1;
		boardOld.at<int>(i,1) = 1;
	 	boardOld.at<int>(i,6) = -1;
	 	boardOld.at<int>(i,7) = -1;
	}
	
    for (int i =0; i< 8; i++) {
	 	boardNew.at<int>(i,0) = 1;
		boardNew.at<int>(i,1) = 1;
	 	boardNew.at<int>(i,6) = -1;
	 	boardNew.at<int>(i,7) = -1;
	}

	boardOld.at<int>(0,0) = 1;
	boardOld.at<int>(1,0) = 0;
	boardOld.at<int>(2,0) = 0;
	boardOld.at<int>(3,0) = 0;
	boardOld.at<int>(4,0) = 1;
	boardOld.at<int>(5,0) = 0;
	boardOld.at<int>(6,0) = 0;
	boardOld.at<int>(7,0) = 1;
	
	boardNew.at<int>(0,0) = 1;
	boardNew.at<int>(1,0) = 0;
	boardNew.at<int>(2,0) = 0;
	boardNew.at<int>(3,0) = 0;
	boardNew.at<int>(4,0) = 0;
	boardNew.at<int>(5,0) = 1;
	boardNew.at<int>(6,0) = 1;
	boardNew.at<int>(7,0) = 0;
	
	// Print Example Move
	// std::cout << "M = " << std::endl << " " << boardnew << std::endl << std::endl;
	
	// Calculate the move from the moves (currently inverted)
	cv::Mat deltaMatrix = deltaCalc(boardOld, boardNew);
	
	//std::cout << "M = " << std::endl << " " << deltaMatrix << std::endl << std::endl;
	
	// Interpret the information
	array<int,4> result = deltaInterp(deltaMatrix);
	
	
	// Gather data from the information
	int chessRowOld = get<0>(result);
	int chessColOld = get<1>(result);
	int chessRowNew = get<2>(result);
	int chessColNew = get<3>(result);
	
	// Convert the move to chess notation
	string moveOld = chessNotation(chessRowOld,chessColOld);
	string moveNew = chessNotation(chessRowNew,chessColNew);
	
	//string move = moveOld+moveNew;
	
	//vector<string> moves;
	
	//moves.push_back(move);
	// moves.push_back("c7c5");
	// moves.push_back("d2d4");
	// moves.push_back("d7d5");
	
	// Provide the output move (Should say moves)
	string message = moveOld+moveNew;
	cout << message << endl;
}
