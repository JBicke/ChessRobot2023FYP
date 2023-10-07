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
	
	//Take the input
	std::string photoName;
	
	std::getline(std::cin, photoName);
	
	// Create New Board
	cv::Mat boardNew = CVRunMain(photoName);
	
	std::cout << boardNew << std::endl;

	// Obtain Old Board
	// Extract the non-numeric and numeric parts
    char non_numeric_part = photoName[0];
    std::string numeric_part = photoName.substr(1);

    // Subtract 1 from the numeric part and convert it back to a string
    int numeric_value = std::stoi(numeric_part) - 1;

    // Concatenate the non-numeric part and the new numeric part
    std::string previous_photoName = non_numeric_part + std::to_string(numeric_value);

	
	cv::Mat boardOld(8,8,CV_32S,cv::Scalar(0));
	boardOld = CVRunMain(previous_photoName);

	std::cout << boardOld << std::endl;


	
	
	// Print Example Board
	// cout << "M = " << endl << " " << board << endl << endl;
	
	// Create Sample Move
	
	
	
	// for (int i =0; i< 8; i++) {
	// 	boardOld.at<int>(i,0) = 1;
	// 	boardOld.at<int>(i,1) = 1;
	// 	boardOld.at<int>(i,6) = -1;
	// 	boardOld.at<int>(i,7) = -1;
	// }
	
	// boardOld.at<int>(4,3) = 1;
	// boardOld.at<int>(4,1) = 0;
	
	// Print Example Move
	// std::cout << "M = " << std::endl << " " << boardnew << std::endl << std::endl;
	
	// Calculate the move from the moves (currently inverted)
	cv::Mat deltaMatrix = deltaCalc(boardOld, boardNew);
	
	std::cout << "M = " << std::endl << " " << deltaMatrix << std::endl << std::endl;
	
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
	
	// Print the move in chess notatin
	cout << moveOld << moveNew << endl;
	
	//string move = moveOld+moveNew;
	
	//vector<string> moves;
	
	//moves.push_back(move);
	// moves.push_back("c7c5");
	// moves.push_back("d2d4");
	// moves.push_back("d7d5");
	
	// Provide the output move (Should say moves)
	string message = "";
	vector<string> messages;
	messages.push_back(message);
	string outputMove = runStock(messages);
	
	cout << outputMove << endl;
}
