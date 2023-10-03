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
	
	//Take the input
	std::string message;
	
	
	
	std::getline(std::cin, message);
	
	// Create Example Board
	cv::Mat board = CVRunMain();
	
	std::cout << board << std::endl;
	
	// Print Example Board
	// cout << "M = " << endl << " " << board << endl << endl;
	
	// Create Sample Move
	
	cv::Mat boardnew(8,8,CV_32F,cv::Scalar(0));
	
	for (int i =0; i< 8; i++) {
		boardnew.at<float>(i,0) = 1;
		boardnew.at<float>(i,1) = 1;
		boardnew.at<float>(i,6) = -1;
		boardnew.at<float>(i,7) = -1;
	}
	
	boardnew.at<float>(4,3) = 1;
	boardnew.at<float>(4,1) = 0;
	
	// Print Example Move
	// std::cout << "M = " << std::endl << " " << boardnew << std::endl << std::endl;
	
	// Calculate the move from the moves
	cv::Mat deltaMatrix = deltaCalc(board, boardnew);
	
	// std::cout << "M = " << std::endl << " " << deltaMatrix << std::endl << std::endl;
	
	// Interpret the information
	array<int,4> result = deltaInterp(deltaMatrix);
	
	// for (auto elem : result) {
		// cout << elem << " " << endl;
	// }
	
	// Gather data from the information
	int chessRowOld = get<0>(result);
	int chessColOld = get<1>(result);
	int chessRowNew = get<2>(result);
	int chessColNew = get<3>(result);
	
	// Convert the move to chess notation
	string moveOld = chessNotation(chessRowOld,chessColOld);
	string moveNew = chessNotation(chessRowNew,chessColNew);
	
	// Print the move in chess notatin
	// cout << moveOld << moveNew << endl;
	
	string move = moveOld+moveNew;
	
	vector<string> moves;
	
	moves.push_back(move);
	// moves.push_back("c7c5");
	// moves.push_back("d2d4");
	// moves.push_back("d7d5");
	
	// Provide the output move (Should say moves)
	vector<string> messages;
	messages.push_back(message);
	string outputMove = runStock(messages);
	
	cout << outputMove << endl;
}
