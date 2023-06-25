#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main()
{
    // Load the image
    Mat img = imread("/home/fyp2023jb/ChessRobot2023FYP/State0.jpg");

    // Convert the image to grayscale
    Mat gray;
    cvtColor(img, gray, COLOR_BGR2GRAY);

    // Define the parameters for Harris corner detection
    int blockSize = 2;
    int apertureSize = 3;
    double k = 0.06;
    Mat dst = Mat::zeros(img.size(), CV_32FC1);

    // Apply Harris corner detection
    cornerHarris(gray, dst, blockSize, apertureSize, k);

    // Normalize the output image
    Mat dst_norm, dst_norm_scaled;
    normalize(dst, dst_norm, 0, 255, NORM_MINMAX, CV_32FC1, Mat());
    convertScaleAbs(dst_norm, dst_norm_scaled);

    // Draw circles around the corners
    for (int i = 0; i < dst_norm.rows; i++) {
        for (int j = 0; j < dst_norm.cols; j++) {
            if ((int)dst_norm.at<float>(i, j) > 200) {
                circle(img, Point(j, i), 5, Scalar(0, 0, 255), 2, 8, 0);
            }
        }
    }

    // Display the image
    imshow("Harris Corner Detection", dst_norm_scaled);
    waitKey(0);
    
    imshow("Harris Corner Detection", img);
    waitKey(0);

    return 0;
}



