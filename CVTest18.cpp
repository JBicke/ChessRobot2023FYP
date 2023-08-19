#include <iostream>
//#include "opencv2/imgcodecs.hpp"
//#include "opencv2/highgui.hpp"
//#include "opencv2/imgproc.hpp"
#include <opencv2/opencv.hpp>


using namespace cv;
using namespace std;

std::vector<cv::Point> locateGreenSquares(const cv::Mat& inputImage)
{
    std::vector<cv::Point> greenSquarePositions;

    // Convert the input image to the HSV color space
    cv::Mat hsvImage;
    cv::cvtColor(inputImage, hsvImage, cv::COLOR_BGR2HSV);

    // Define lower and upper bounds for green color (in HSV)
    cv::Scalar lowerGreen(35, 50, 50);
    cv::Scalar upperGreen(85, 255, 255);

    // Create a mask for the green color range
    cv::Mat greenMask;
    cv::inRange(hsvImage, lowerGreen, upperGreen, greenMask);

    // Find contours in the green mask
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(greenMask, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    // Filter contours based on area
    double minAreaThreshold = 100.0; // Adjust this threshold as needed
    for (const auto& contour : contours)
    {
        double area = cv::contourArea(contour);
        if (area > minAreaThreshold)
        {
            cv::Moments moments = cv::moments(contour, false);
            if (moments.m00 != 0) // Avoid division by zero
            {
                cv::Point centroid(moments.m10 / moments.m00, moments.m01 / moments.m00);
                greenSquarePositions.push_back(centroid);
   
            }
        }
    }

    return greenSquarePositions;
}

cv::Mat rotateImageToStraight(const cv::Mat& image, cv::Point2f point1, cv::Point2f point2) {
  // Calculate the rotation angle based on the y-coordinate difference
    double angle = std::atan2(point2.y - point1.y, point2.x - point1.x) * 180.0 / CV_PI;

    // Get the rotation matrix
    cv::Point2f center(image.cols / 2, image.rows / 2);
    cv::Mat rotationMatrix = cv::getRotationMatrix2D(center, angle, 1.0);

    // Rotate the image
    cv::Mat rotatedImage;
    cv::warpAffine(image, rotatedImage, rotationMatrix, image.size());

    return rotatedImage;
}

cv::Mat cannyEdgeDetection(const cv::Mat& image){
    cv::Mat c_cannyEdges, p_cannyEdges;
    cv::Mat cannyEdges;
    Canny(image, cannyEdges, 100, 150, 3);

    cvtColor(cannyEdges, c_cannyEdges, COLOR_GRAY2BGR);
    p_cannyEdges = cannyEdges.clone();

    vector<Vec2f> lines; // will hold the results of the detection
    HoughLines(cannyEdges, lines, 1, CV_PI/180, 150, 0, 0 ); // runs the actual detection
    // Draw the lines
    for( size_t i = 0; i < lines.size(); i++ )
    {
        float rho = lines[i][0], theta = lines[i][1];
        
        Point pt1, pt2;
        double a = cos(theta), b = sin(theta);
        double x0 = a*rho, y0 = b*rho;
        pt1.x = cvRound(x0 + 1000*(-b));
        pt1.y = cvRound(y0 + 1000*(a));
        pt2.x = cvRound(x0 - 1000*(-b));
        pt2.y = cvRound(y0 - 1000*(a));
        //cout << "Point 1: "<< pt1.x << " " << pt1.y << " Point 2: " << pt2.x << " " << pt2.y << endl;
        line( c_cannyEdges, pt1, pt2, Scalar(0,0,255), 3, LINE_AA);
    }

    return c_cannyEdges;
}

cv::Mat processAndShowContours(const cv::Mat& src) {
    cv::Mat hsvImage;
    cv::cvtColor(src, hsvImage, cv::COLOR_BGR2HSV);
    cv::imshow("HSV Image", hsvImage);

    cv::Scalar lowerRed1(0, 50, 50);
    cv::Scalar upperRed1(10, 255, 255);

    cv::Scalar lowerRed2(160, 50, 50);
    cv::Scalar upperRed2(179, 255, 255);

    cv::Scalar lowerBlue(90, 50, 50);
    cv::Scalar upperBlue(130, 255, 255);

    cv::Mat redMask;
    cv::Mat blueMask;

    cv::inRange(hsvImage, lowerRed1, upperRed1, redMask);
    cv::inRange(hsvImage, lowerRed2, upperRed2, redMask);
    cv::inRange(hsvImage, lowerBlue, upperBlue, blueMask);

    cv::Mat resultMask = redMask | blueMask;

    int morphSize = 3;
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(morphSize, morphSize));
    cv::morphologyEx(resultMask, resultMask, cv::MORPH_OPEN, kernel);

    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;
    cv::findContours(resultMask, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    cv::Mat resultImage = src.clone();
    cv::Scalar contourColour;
    cv::Scalar pinkColour(255, 0, 255);
    cv::Scalar blueColour(255, 0, 0);
    for (size_t i = 0; i < contours.size(); ++i) {
        cv::Rect boundingRect = cv::boundingRect(contours[i]);
        if (cv::countNonZero(redMask(boundingRect)) > 0)
            contourColour = pinkColour;
        else
            contourColour = blueColour;
        cv::rectangle(resultImage, boundingRect, contourColour, 2);
    }

    return resultImage;
}

cv::Mat probEdgeDetection(const cv::Mat& image){
    cv::Mat probDetection;
    vector<Vec4i> linesP; // will hold the results of the detection
    HoughLinesP(image, linesP, 1, CV_PI/180, 20, 50, 20 ); // runs the actual detection
    // Draw the lines
    for( size_t i = 0; i < linesP.size(); i++ )
    {
        Vec4i l = linesP[i];
        line( probDetection, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0,0,255), 3, LINE_AA);
    }

    for ( size_t i = 0; i < linesP.size(); i++){
        for ( size_t j = i + 1; j < linesP.size(); j++){

            cv::Vec4i line1 = linesP[i];
			cv::Vec4i line2 = linesP[j];
			cv::Point pt1_1(line1[0], line1[1]);
			cv::Point pt1_2(line1[2], line1[3]);
			cv::Point pt2_1(line2[0], line2[1]);
			cv::Point pt2_2(line2[2], line2[3]);
            
            //cout << line1 << line2 << endl;
        }

    }

    return probDetection;
}
int main(int argc, char** argv)
{
    // Declare the output variables
    Mat dst, cdst, cdstP;
    const char* default_file ="E:/UNI/ece4078/ChessRobot2023FYP/chess_board_photos/chess_board_7.jpg";
    const char* filename = argc >=2 ? argv[1] : default_file;
    // Loads an image
    Mat src = imread( samples::findFile( filename ));
    // Check if image is loaded fine
    if(src.empty()){
        printf(" Error opening image\n");
        printf(" Program Arguments: [image_name -- default %s] \n", default_file);
        return -1;
    }
    
    // Locate green square positions
    std::vector<cv::Point> crop_points = locateGreenSquares(src);

    //finding points
    cv::Point2f  point1 = crop_points[0];
    cv::Point2f  point2 = crop_points[1];

    cout << point1 << " " << point2 << endl;

    // rotate image
    cv::Mat rotatedImage = rotateImageToStraight(src, point1, point2);
    
    int Rwidth = rotatedImage.cols;
    int Rheight = rotatedImage.rows;

    std::cout << "Width: " << Rwidth << std::endl;
    std::cout << "Height: " << Rheight << std::endl;

    //display rotated image
    cv::imshow("Rotated Image", rotatedImage);
    
    //locate green squares in rotated image
    std::vector<cv::Point> greenSquarePositions = locateGreenSquares(rotatedImage);

    // Display the detected green square positions
    for (const auto& position : greenSquarePositions)
    {
        std::cout << "Green square position: (" << position.x << ", " << position.y << ")" << std::endl;
    }

    //finding positions of chess board corners
    int position1_x = greenSquarePositions[0].x;
    int position1_y = greenSquarePositions[0].y;

    int position2_x = greenSquarePositions[1].x;
    int position2_y = greenSquarePositions[1].y;

    int position3_x = greenSquarePositions[2].x;
    int position3_y = greenSquarePositions[2].y;

    int position4_x = greenSquarePositions[3].x;
    int position4_y = greenSquarePositions[3].y;

    int width = 0;
    int height = 0;
    int temp_width, temp_height;

    for (int i=0; i<4; i++){
        temp_width = greenSquarePositions[0].x - greenSquarePositions[i].x;
        temp_height = greenSquarePositions[0].y - greenSquarePositions[i].y;

        if (temp_width < 0){
            temp_width = temp_width * -1;
        }

        if (temp_height < 0){
            temp_height = temp_height * -1;
        }

        if (temp_width > width){
            width = temp_width;
        }

        if (temp_height > height){
            height = temp_height;
        }
    }

    

    cout << position1_x << " " << position1_y << " " << position3_x << " " << position3_y << " " << width << " " << height << endl;

    cv::Rect crop_region(position3_x, position3_y, width, height);

    cv::Mat croppedImage = rotatedImage(crop_region);

    cv::imshow("Cropped Image", croppedImage);
    
    //convert to gray_scale
    cv::Mat src_gray;
    cvtColor(croppedImage, src_gray, COLOR_BGR2GRAY);
    
    //convert to binary
    cv::Mat src_binary;
    cv::adaptiveThreshold(src_gray, src_binary, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY, 11, 2);
    //imshow("binary", src_binary);

    /*
    //find chessboard corners
    std::vector<cv::Point2f> corners;
    cv::Size boardSize(7,7);
    int found = cv::findChessboardCorners(src_gray, boardSize, corners, cv::CALIB_CB_ADAPTIVE_THRESH + cv::CALIB_CB_NORMALIZE_IMAGE );
    //cout<< corners << endl;
    if (found){
        cv::cornerSubPix(src_binary, corners, cv::Size(5, 5), cv::Size(-1, -1), cv::TermCriteria(cv::TermCriteria::EPS + cv::TermCriteria::COUNT, 30, 0.1));

        cv::Mat mask = cv::Mat::zeros(src.size(), CV_8U);
        std::vector<cv::Point> polyCorners;
        polyCorners.push_back(corners[0]);
        polyCorners.push_back(corners[boardSize.width - 1]);
        polyCorners.push_back(corners[boardSize.width * (boardSize.height - 1)]);
        polyCorners.push_back(corners[boardSize.width * boardSize.height - 1]);
        cv::fillConvexPoly(mask, polyCorners, cv::Scalar(255));

        // Find contours in the mask
        std::vector<std::vector<cv::Point>> contours;
        cv::findContours(mask, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

        // Draw the outlines of each square
        cv::Mat resultImage = croppedImage.clone();
        cv::Scalar outlineColor(0, 255, 0); // Green color for contours
        cv::drawContours(resultImage, contours, -1, outlineColor, 2);

        // Display the resulting image with square outlines
        imshow("Result", resultImage);
    }
    else {
        cout << "found not found" << endl;
    }
    */

    //canny edge detection
    cv::Mat cannyEdges = cannyEdgeDetection(src_gray); 
    cv::imshow("Canny Edges Hough", cannyEdges);


    cv::Mat maskImage = processAndShowContours(croppedImage);
    cv::imshow("Mask Image", maskImage);

    cv::waitKey(0);

    return 0;
}