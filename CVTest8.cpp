
		
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main()
{
    Mat image = imread("/home/fyp2023jb/ChessRobot2023FYP/State0.jpg"); // read image from file

    // define the region of interest (ROI) using a rectangle
    Rect roi(320, 150, 760, 750); // starting x, starting y, width, height

    // extract the ROI from the input image
    Mat croppedImage = image(roi);

    // display the original and cropped images
    imshow("Original Image", image);
    waitKey(0);
    imshow("Cropped Image", croppedImage);
    waitKey(0);

    return 0;
}
