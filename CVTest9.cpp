
		
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main()
{
    Mat img = imread("/home/fyp2023jb/ChessRobot2023FYP/State0.jpg"); // read image from file

    // define the region of interest (ROI) using a rectangle
    Rect roi(305, 150, 780, 775); // starting x, starting y, width, height

    // extract the ROI from the input image
    Mat croppedImage = img(roi);

    // display the original and cropped images
    imshow("Original Image", img);
    waitKey(0);
    imshow("Cropped Image", croppedImage);
    waitKey(0);


    // Convert the image to grayscale
    cv::Mat gray;
    cv::cvtColor(croppedImage, gray, cv::COLOR_BGR2GRAY);

    // Define the chessboard size (in this case, an 8x8 board)
    cv::Size boardSize(7, 7);

    // Use Harris corner detector to detect corners
    std::vector<cv::Point2f> corners;
    cv::cornerHarris(gray, corners, 2, 3, 0.04);

    // Refine the corners to subpixel accuracy
    cv::TermCriteria criteria(cv::TermCriteria::EPS + cv::TermCriteria::MAX_ITER, 30, 0.001);
    cv::cornerSubPix(gray, corners, cv::Size(11, 11), cv::Size(-1, -1), criteria);

    // Draw the corners on the image
    for (int i = 0; i < corners.size(); i++) {
        cv::circle(img, corners[i], 3, cv::Scalar(0, 0, 255), cv::FILLED);
    }

    // Display the image
    cv::imshow("Chessboard Corners", img);
    cv::waitKey(0);

    return 0;
}
