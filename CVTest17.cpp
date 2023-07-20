#include <iostream>
//#include "opencv2/imgcodecs.hpp"
//#include "opencv2/highgui.hpp"
//#include "opencv2/imgproc.hpp"
#include <opencv2/opencv.hpp>


using namespace cv;
using namespace std;

int main(int argc, char** argv)
{
    // Declare the output variables
    Mat dst, cdst, cdstP;
    const char* default_file ="E:/UNI/ece4078/ChessRobot2023FYP/4.jpg";
    const char* filename = argc >=2 ? argv[1] : default_file;
    // Loads an image
    Mat src = imread( samples::findFile( filename ));
    // Check if image is loaded fine
    if(src.empty()){
        printf(" Error opening image\n");
        printf(" Program Arguments: [image_name -- default %s] \n", default_file);
        return -1;
    }

    cv::Mat src_gray;
    cvtColor(src, src_gray, COLOR_BGR2GRAY); 
    cv::Mat src_binary;

    /*
    cvtColor(src, src_gray, COLOR_BGR2GRAY); 
    cv::adaptiveThreshold(src_gray, src_binary, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY, 11, 2);

    imshow("binary image", src_binary);

    //find chessboard corners
    std::vector<cv::Point2f> corners;
    cv::Size boardSize(8,8);
    bool found = cv::findChessboardCorners(src_binary, boardSize, corners, cv::CALIB_CB_ADAPTIVE_THRESH + cv::CALIB_CB_NORMALIZE_IMAGE);

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
        cv::Mat resultImage = src.clone();
        cv::Scalar outlineColor(0, 255, 0); // Green color for contours
        cv::drawContours(resultImage, contours, -1, outlineColor, 2);

        // Display the resulting image with square outlines
        imshow("Result", resultImage);
    }
    else {
        cout << "found not found" << endl;
    }
    */

    
    //convert image to HSV
    cv::Mat hsvImage;
    cv::cvtColor(src, hsvImage, cv::COLOR_BGR2HSV);

    cv::Mat transMask;
    inRange(hsvImage, cv::Scalar (0,0,143), cv::Scalar (179,61,252), transMask);
    imshow("Binary", transMask);
    
    //colour detection
    //define colour ranges for pink and blue
    cv::Scalar lowerPink(140,50,50);
    cv::Scalar upperPink(170, 255, 255);

    cv::Scalar lowerBlue(90,50,50);
    cv::Scalar upperBlue(130,255,255);

    //cv::Scalar lowerBlack(0,0,0);
    //cv::Scalar upperBlack(255, 255, 30);
    
    //perform masking for both blue and pink intensities
    cv::Mat pinkMask;
    cv::Mat blueMask;
    //cv::Mat blackMask;

    cv::inRange(hsvImage, lowerPink, upperPink, pinkMask);
    cv::inRange(hsvImage, lowerBlue, upperBlue, blueMask);
    //cv::inRange(hsvImage, lowerBlack, upperBlack, blackMask);

    cv::Mat resultMask = pinkMask | blueMask;

    // Perform morphological operations on the combined mask
    int morphSize = 3;  // Adjust the size as per your requirement
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(morphSize, morphSize));
    cv::morphologyEx(resultMask, resultMask, cv::MORPH_OPEN, kernel);

    // Find contours in the combined mask
    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;
    cv::findContours(resultMask, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    // Draw bounding rectangles around the detected contours
    cv::Mat PinkBlueImage = src.clone();
    cv::Scalar contourColour;
    cv::Scalar pinkColour(255, 0, 255);  // Green color for contour bounding boxes
    cv::Scalar blueColour(255,0,0);
    for (size_t i = 0; i < contours.size(); ++i) {
        cv::Rect boundingRect = cv::boundingRect(contours[i]);
        if (cv::countNonZero(pinkMask(boundingRect)) > 0)
            contourColour = pinkColour;
        else
            contourColour = blueColour;
        cv::rectangle(PinkBlueImage, boundingRect, contourColour, 2);
    }
    

    cv::imshow( "HSV Image", PinkBlueImage);

    cv::Mat adaptiveThreshold;
    cv::adaptiveThreshold(src_gray, adaptiveThreshold, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY_INV, 11, 2);

    cv::Mat c_cannyEdges;
    cv::Mat cannyEdges;
    Canny(src_gray, cannyEdges, 100, 150, 3);

    cvtColor(cannyEdges, c_cannyEdges, COLOR_GRAY2BGR);

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
        cout << "Point 1: "<< pt1.x << " " << pt1.y << " Point 2: " << pt2.x << " " << pt2.y << endl;
        line( c_cannyEdges, pt1, pt2, Scalar(0,0,255), 3, LINE_AA);
    }

    cv::imshow("Canny EDges Hough", c_cannyEdges);

    cv::Mat blackSquareEdges = adaptiveThreshold & ~resultMask;

    std::vector<std::vector<cv::Point>> blackContours;
    cv::findContours(blackSquareEdges, blackContours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    cv::Mat blackSquaresImage = src.clone();
    cv::drawContours(blackSquaresImage, blackContours, -1, cv::Scalar(0, 0, 255), 2);
    cv::imshow("Black Squares Image", blackSquaresImage);

    /*
    std::vector<cv::Point> pinkPiecePositions, bluePiecePositions;
    for (const auto& contour : blackContours)
    {   
        
        double area = cv::contourArea(contour);
        if (area > 0 && area < 500) // Adjust the area range based on the piece size
        {
            
            // Determine if the piece is pink or blue by checking color mask intersections
            cv::Mat mask = cv::Mat::zeros(src.size(), CV_8U);
            if (contour.size() > 0)
            {
                std::vector<std::vector<cv::Point>> temp_contour;
                temp_contour.push_back(contour);
                cv::drawContours(mask, temp_contour, 0, cv::Scalar(255), -1);
                
                int blueIntersection = cv::countNonZero(resultMask & mask);
                int pinkIntersection = cv::countNonZero(pinkMask & mask);

               if (blueIntersection > pinkIntersection)
                {
                    // Calculate the centroid of the contour for blue piece
                    cv::Moments moments = cv::moments(contour, false); // Use binaryImage=false to get accurate moments
                    cv::Point centroid(moments.m10 / moments.m00, moments.m01 / moments.m00);

                    // Ensure the centroid is within the valid image range
                    if (centroid.x >= 0 && centroid.x < src.cols && centroid.y >= 0 && centroid.y < src.rows)
                        bluePiecePositions.push_back(centroid);
                    else
                        std::cout << "Invalid centroid for blue piece: " << centroid << std::endl;
                }
                else
                {
                    // Calculate the centroid of the contour for pink piece
                    cv::Moments moments = cv::moments(contour, false); // Use binaryImage=false to get accurate moments
                    cv::Point centroid(moments.m10 / moments.m00, moments.m01 / moments.m00);

                    // Ensure the centroid is within the valid image range
                    if (centroid.x >= 0 && centroid.x < src.cols && centroid.y >= 0 && centroid.y < src.rows)
                        pinkPiecePositions.push_back(centroid);
                    else
                        std::cout << "Invalid centroid for pink piece: " << centroid << std::endl;
                }
            }
        }
    }
    
    // Create 8x8 matrix to represent chessboard positions
    const int rows = 8;
    const int cols = 8;
    cv::Mat chessboard(rows, cols, CV_8S, cv::Scalar(0)); // Initialize matrix with 0 (empty)
    cout << pinkPiecePositions << endl;
    cout << bluePiecePositions << endl;
    
    // Assign -1 for pink pieces and 1 for blue pieces in the matrix
    for (const auto& pinkPiecePos : pinkPiecePositions)
        chessboard.at<char>(pinkPiecePos) = -1;
    for (const auto& bluePiecePos : bluePiecePositions)
        chessboard.at<char>(bluePiecePos) = 1;

    // Display the chessboard positions matrix
    std::cout << chessboard << std::endl;
    */


    cv::waitKey(0);
}