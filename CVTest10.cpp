
		
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main()
{
    // Load the image
    cv::Mat img = cv::imread("/home/fyp2023jb/ChessRobot2023FYP/Online.png");

    // Convert the image to grayscale
    cv::Mat gray;
    cv::cvtColor(img, gray, cv::COLOR_BGR2GRAY);

    // Define the chessboard size (in this case, an 8x8 board)
    cv::Size boardSize(7, 7);

    // Find the chessboard corners
    std::vector<cv::Point2f> corners;
    bool patternWasFound = cv::findChessboardCorners(gray, boardSize, corners, cv::CALIB_CB_FAST_CHECK);

    // If the corners are found, refine the corners to subpixel accuracy
    if (patternWasFound) {
        cv::TermCriteria criteria(cv::TermCriteria::EPS + cv::TermCriteria::MAX_ITER, 30, 0.001);
        cv::cornerSubPix(gray, corners, cv::Size(11, 11), cv::Size(-1, -1), criteria);
    }

    // Draw the corners on the image
    cv::drawChessboardCorners(img, boardSize, corners, patternWasFound);

    // Display the image
    cv::imshow("Chessboard Corners", img);
    cv::waitKey(0);

    return 0;
}
