// Move Calculation
#include <iostream>
#include <vector>
//#include <opencv2/opencv.hpp>


using namespace std;

//void visualiseMatrix(cv::Mat matrix) {
	//// Create a grayscale image to display the matrix
	//cv::Mat image(8,8, CV_8UC1);
	
	//// Copy the matrix values to the image
	//for(int i =0; i<matrix.rows; i++) {
		//for(int j=0; j<matrix.cols; j++) {
			//image.at<uchar>(i,j) = (matrix.at<int>(i,j)+1)*127;
		//}
	//}
	
	//// Display the Image
	////cv::resizeWindow("Matrix Visualisation",400,400);
	////imshow("Matrix Visualization", image);
	////cv::waitKey(0);
//}

std::vector<std::vector<int>> init_board(vector<vector<int>>& board) {
	board.resize(8,vector<int>(8,0));
	for (int i =0; i< 8; i++) {
		board[i][0] = 1;
		board[i][1] = 1;
		board[i][6] = -1;
		board[i][7] = -1;
	}
	//cv::Mat matrix(8,8,CV_32SC1,board);
	//visualiseMatrix(matrix);
	//return 0;
	return board;
}



int main() {
	bool game_start = true;
	int matrix[8][8];
	
	if(game_start) {
		int board1 = init_board(matrix);
	}
	cout << matrix << endl;
	//cv:: Mat board1_mat = cv::Mat(board1.size(), board1[0].size(), CV_32S);
	//for (int i =0; i < board1.size();i++) {
		//for (int j=0;j<board1[i].size();j++) {
			//board1_mat.at<int>(i,j) = board1[i][j];
		//}
	//}
	
	//visualiseMatrix(board1_mat);
	return 0;
}
		
