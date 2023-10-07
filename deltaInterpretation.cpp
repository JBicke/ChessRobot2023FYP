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
	// cout << "delta = " << endl << " " << deltaMatrix << endl << endl;
	int countOld;
	int countNew;
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
	if ((countOld == 1) && (countNew == 1))) {
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
	}
	
	// std::cout << "Found -1 at row " << previousRow << " and column " << previousCol << std::endl;
	// std::cout << "Found 1 at row " << newRow << " and column " << newCol << std::endl;

	array<int,4> positions = {previousRow, previousCol, newRow, newCol};
	
	
	return positions;
}
