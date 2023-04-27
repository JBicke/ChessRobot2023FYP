#include "opencv2/opencv.hpp"
#include <iostream>

int deltaInterp(cv::Mat deltaMatrix) {
	int row = 0;
	int col = 0;
	bool found = false;
	std::cout << "delta = " << std::endl << " " << deltaMatrix << std::endl << std::endl;
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if((found == false) && (deltaMatrix.at<float>(i,j) == -1)) {
				row = i;
				col = j;
				found = true;
			}
		}
	}
	
	if (row != -1 && col != -1) {
		std::cout << "Found -1 at row " << row << " and column " << col << std::endl;
	} else {
		std::cout << "-1 not found in matrix." << std::endl;
	}
	
	return row+col;
}

