// g++ -o OpenCV_Testing OpenCV_Testing.cpp -I/usr/include/opencv4 `pkg-config --cflags --libs opencv4`
// /OpenCV_Testing

#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main()
{
    // Load image
    Mat image = imread("/home/fyp2023jb/ChessRobot2023FYP/State0.jpg");
    cout << "Image type: " << image.type() << endl;
    // Convert to grayscale
    Mat gray;
    
	imshow("RGB",image); // show image in a window
	waitKey(0); // wait for a key press
	
    // cvtColor(image, gray, COLOR_BGR2GRAY);
    
    // imshow("RGB",gray); // show image in a window
	// waitKey(0); // wait for a key press
	
    // Apply thresholding
    Mat binary;
    threshold(image, binary, 100, 255, THRESH_BINARY_INV);
    
    // Find contours
    vector<vector<Point>> contours;
    findContours(binary, contours, CV_RETR_FLOODFILL, CHAIN_APPROX_SIMPLE);
    
    // Create chessboard matrix
    Mat chessboard = Mat::zeros(8, 8, CV_8SC1);
    
    // Loop over contours
    for (const auto& contour : contours)
    {
        // Compute contour moments
        Moments moments = cv::moments(contour);
        
        // Compute contour center
        Point center(moments.m10 / moments.m00, moments.m01 / moments.m00);
        
        // Determine color of piece
        Scalar color = image.at<uchar>(center);
        int piece = color[0] == 255 ? 1 : (color[0] == 127 ? 0 : -1);
        
        // Map contour to chessboard square
        int row = center.y / (image.rows / 8);
        int col = center.x / (image.cols / 8);
        
        // Set value in chessboard matrix
        chessboard.at<signed char>(row, col) = piece;
    }
    
    // Print chessboard matrix
    cout << chessboard << endl;
    
    return 0;
}
		
