
		
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main()
{
    // Load the image
    Mat image = imread("/home/fyp2023jb/ChessRobot2023FYP/State0.jpg", IMREAD_COLOR);
	

    // define the region of interest (ROI) using a rectangle
    Rect roi(320, 150, 760, 750); // starting x, starting y, width, height

    // extract the ROI from the input image
    Mat croppedImage = image(roi);

    // display the original and cropped images
    imshow("Original Image", image);
    waitKey(0);
    imshow("Cropped Image", croppedImage);
    waitKey(0);
    
    // Set the number of inner corners of the chessboard
    Size boardSize(7, 7);

    // Find the corners of the chessboard
    Mat gray;
    cvtColor(croppedImage, gray, COLOR_BGR2GRAY);
    vector<Point2f> corners;
    bool found = findChessboardCorners(gray, boardSize, corners);

    // If the corners are found, draw them on the image
    if (found) {
        drawChessboardCorners(croppedImage, boardSize, corners, found);
        cout << "found";
    }
	
    // Display the image
    imshow("Detected Chessboard", croppedImage);
    waitKey(0);

    return 0;
}
