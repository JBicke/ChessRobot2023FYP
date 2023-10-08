#include "opencv2/opencv.hpp"
#include <iostream>
#include <array>

using namespace std;

array<int,4> deltaInterp(cv::Mat deltaMatrix) {
	int previousRow = 0;
	int previousCol = 0;
	int newRow = 0;
	int newCol = 0;
	bool previousFound = false;
	bool newFound = false;
	bool enPass1 = false;
	int enRow1 = 0;
	int enCol1 = 0;
	int enRow2 = 0;
	int enCol2 = 0;
	int enRow3 = 0;
	int enCol3 = 0;
	// cout << "delta = " << endl << " " << deltaMatrix << endl << endl;
	int countOld = 0;
	int countNew = 0;
	for (int i = 0; i < deltaMatrix.rows; i++) {
        for (int j = 0; j < deltaMatrix.cols; j++) {
            if (deltaMatrix.at<int>(i, j) == -1) {
                countOld++;
            } else if (deltaMatrix.at<int>(i, j) == 1) {
                countNew++;
			} else if (deltaMatrix.at<int>(i, j) == 2) {
                countNew++;
			}
        }
    }
    cout << "-1: " << countOld << ", 1:" << countNew << endl;
	if ((countOld == 1) && (countNew == 1)) {
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++) {
				if((previousFound == false) && (deltaMatrix.at<int>(i,j) == -1)) {
					previousRow = i;
					previousCol = j;
					previousFound = true;
				} else if ((newFound == false) && (deltaMatrix.at<int>(i,j) == 1)) {
					newRow = i;
					newCol = j;
					newFound = true;
				} else if ((newFound == false) && (deltaMatrix.at<int>(i,j) == 2)) {
					newRow = i;
					newCol = j;
					newFound = true;
				} 
			}
		}
	} else if ((countOld == 2) && (countNew == 1)) { // This if statement only occurs during Enpassant
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++) {
				if (deltaMatrix.at<int>(i,j) == 1) {
					enRow1 = i;
					enCol1 = j;
				} else if ((enPass1 == false) && (deltaMatrix.at<int>(i,j) == -1)) {
					enRow2 = i;
					enCol2 = j;
					enPass1 == true;
				} else if ((enPass1 == true) && (deltaMatrix.at<int>(i,j) == -1)) {
					enRow3 = i;
					enCol3 = j;
					enPass1 == false;
				}
			}
		}
		if (enRow1 == enRow2) {
			previousRow = enRow3;
			previousCol = enCol3;
		} else if (enRow1 == enRow3) {
			previousRow = enRow2;
			previousCol = enCol2;
		}
		newRow = enRow1;
		newCol = enCol1;

	}
	
	// Enpasssent would be a 1 in column 7, and 2 -1 in column 6.
	// Castling would be 2 -1 and 2 1s in column 1. 
	// Promotion would be just like a normal move from row 7 to row 8, which makes it hard. Make it so you have to press the piece it promotes to, hence different intepretation is not needed.

	// std::cout << "Found -1 at row " << previousRow << " and column " << previousCol << std::endl;
	// std::cout << "Found 1 at row " << newRow << " and column " << newCol << std::endl;


	array<int,4> positions = {previousRow, previousCol, newRow, newCol};
	
	
	return positions;
}
