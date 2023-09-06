#include <iostream>
//#include "opencv2/imgcodecs.hpp"
//#include "opencv2/highgui.hpp"
//#include "opencv2/imgproc.hpp"
#include <opencv2/opencv.hpp>


using namespace cv;
using namespace std;

struct CenterPoints {
    std::vector<cv::Point> pink;
    std::vector<cv::Point> blue;
};

struct FilteredPoints {
    std::vector<int> x;
    std::vector<int> y;
};

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

FilteredPoints cannyEdgeDetection(const cv::Mat& image){
    cv::Mat c_cannyEdges, p_cannyEdges;
    cv::Mat cannyEdges;
    Canny(image, cannyEdges, 100, 150, 3);
    //imshow("edges", cannyEdges);
    cvtColor(cannyEdges, c_cannyEdges, COLOR_GRAY2BGR);
    p_cannyEdges = cannyEdges.clone();

    vector<Vec2f> lines; // will hold the results of the detection

    // Run Hough Line Transform for horizontal lines
    HoughLines(cannyEdges, lines, 1, CV_PI / 180, 150);
    
    vector<double> x_points;
    vector<double> y_points;

    // Draw the detected horizontal lines on the original image
    for (size_t i = 0; i < lines.size(); ++i) {
        float rho = lines[i][0];
        float theta = lines[i][1];
        
        double a = cos(theta);
        double b = sin(theta);
        double x0 = a * rho;
        double y0 = b * rho;
        
        x_points.push_back(x0);
        y_points.push_back(y0);
    }

    for (auto it = x_points.begin(); it != x_points.end();) {
        if (*it < 10) {
            it = x_points.erase(it);
        } else {
            ++it;
        }
    }

    for (auto it = y_points.begin(); it != y_points.end();) {
        if (*it < 10) {
            it = y_points.erase(it);
        } else {
            ++it;
        }
    }
    
    std::vector<int> filtered_x_points;

    for (int x : x_points) {
        bool shouldAdd = true;

        for (int filtered_x : filtered_x_points) {
            if (std::abs(x - filtered_x) <= 40) {
                shouldAdd = false;
                break;
            }
        }

        if (shouldAdd) {
            filtered_x_points.push_back(x);
        }
    }
    std::sort(filtered_x_points.begin(), filtered_x_points.end());
    // Print the filtered x_points
    std::cout << "Filtered x_points: ";
    for (int x : filtered_x_points) {
        std::cout << x << " ";
    }
    cout << endl;

    std::vector<int> filtered_y_points;

    for (int y : y_points) {
        bool shouldAdd = true;

        for (int filtered_y : filtered_y_points) {
            if (std::abs(y - filtered_y) <= 40) {
                shouldAdd = false;
                break;
            }
        }

        if (shouldAdd) {
            filtered_y_points.push_back(y);
        }
    }
    std::sort(filtered_y_points.begin(), filtered_y_points.end());
    // Print the filtered x_points
    std::cout << "Filtered y_points: ";
    for (int y : filtered_y_points) {
        std::cout << y << " ";
    }
    cout << endl;

    FilteredPoints filteredPoints;

    filteredPoints.x = filtered_x_points;
    filteredPoints.y = filtered_y_points;

    return filteredPoints;
}

CenterPoints processAndShowContours(const cv::Mat& src) {
    cv::Mat hsvImage;
    cv::cvtColor(src, hsvImage, cv::COLOR_BGR2HSV);
    //cv::imshow("HSV Image", hsvImage);

    cv::Scalar lowerRed1(0, 50, 50);
    cv::Scalar upperRed1(10, 255, 255);

    cv::Scalar lowerRed2(160, 50, 50);
    cv::Scalar upperRed2(179, 255, 255);

    cv::Scalar lowerBlue(100, 100, 100);
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

    vector<cv::Point> pinkCenterPoints;
    vector<cv::Point> blueCenterPoints;

    for (size_t i = 0; i < contours.size(); ++i) {
        cv::Rect boundingRect = cv::boundingRect(contours[i]);
        
        // Calculate center point
        int centerX = (boundingRect.x + boundingRect.x + boundingRect.width) / 2;
        int centerY = (boundingRect.y + boundingRect.y + boundingRect.height) / 2;

        // Determine the contour color based on the region
        if (cv::countNonZero(redMask(boundingRect)) > 0)
            contourColour = pinkColour;
        else
            contourColour = blueColour;

        // Draw rectangle and center point
        cv::rectangle(resultImage, boundingRect, contourColour, 2);
        cv::circle(resultImage, cv::Point(centerX, centerY), 4, contourColour, -1); // Draw center point
        imshow("Result", resultImage);
<<<<<<< HEAD
        
=======
>>>>>>> 26066980009ab8ec05a448f32f7bdc7916a36d1d
        // Store center point in the appropriate vector
        if (contourColour == pinkColour) {
            pinkCenterPoints.push_back(cv::Point(centerX, centerY));
        } else {
            blueCenterPoints.push_back(cv::Point(centerX, centerY));
        }
    }
    std::cout << "Pink Center Points:" << std::endl;
for (const cv::Point& center : pinkCenterPoints) {
    int centerX = center.x;
    int centerY = center.y;
    std::cout << "Center Point: (" << centerX << ", " << centerY << ")" << std::endl;
}

std::cout << "Blue Center Points:" << std::endl;
for (const cv::Point& center : blueCenterPoints) {
    int centerX = center.x;
    int centerY = center.y;
    std::cout << "Center Point: (" << centerX << ", " << centerY << ")" << std::endl;
}

    CenterPoints centerPoints;
    
    centerPoints.pink = pinkCenterPoints;
    centerPoints.blue = blueCenterPoints;

    return centerPoints;
}

std::vector<std::vector<int>> transposeMatrix(const std::vector<std::vector<int>>& matrix) {
    int rows = matrix.size();
    int cols = matrix[0].size();

    std::vector<std::vector<int>> transposed(cols, std::vector<int>(rows, 0));

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            transposed[j][i] = matrix[i][j];
        }
    }

    return transposed;
}

int main(int argc, char** argv)
{
    // Declare the output variables
    Mat dst, cdst, cdstP;
<<<<<<< HEAD
    const char* default_file ="/home/fyp2023jb/ChessRobot2023FYP/1001.jpg";
=======
    const char* default_file ="E:/UNI/ece4078/ChessRobot2023FYP/1001.jpg";
>>>>>>> 26066980009ab8ec05a448f32f7bdc7916a36d1d
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
    cv::Point2f  point3 = crop_points[2];
    cv::Point2f  point4 = crop_points[3];
    cout << point1 << " " << point2 << " " << point3 << " " << point4 << endl;

    int min_crop_point = 10000; 
int min_crop_point_ind = 0;
int two_min_crop_point = 10000;
int two_min_crop_point_ind = 0;

for(int i = 0; i < 4; i++) {
    if(crop_points[i].y < min_crop_point) {
        min_crop_point = crop_points[i].y;
        min_crop_point_ind = i;
    }
}


for(int i = 0; i < 4; i++) {
    if((crop_points[i].y < two_min_crop_point) && (i != min_crop_point_ind)) {
        two_min_crop_point = crop_points[i].y;
        two_min_crop_point_ind = i;  
    }
}

    cout << crop_points[min_crop_point_ind] << " " << crop_points[two_min_crop_point_ind] << endl;
    // rotate image
    cv::Mat rotatedImage = rotateImageToStraight(src, crop_points[min_crop_point_ind], crop_points[two_min_crop_point_ind]);
    
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

    int min_x = 10000;
    int min_y = 10000;

    for (int i=0; i<4; i++){
        if(greenSquarePositions[i].x < min_x)
            min_x = greenSquarePositions[i].x;
        
        if(greenSquarePositions[i].y < min_y)
            min_y = greenSquarePositions[i].y;
    }
    

    cout << min_x << " " << min_y << " " << width << " " << height << endl;

    cv::Rect crop_region(min_x, min_y, width, height);

    cv::Mat croppedImage = rotatedImage(crop_region);

    cv::imshow("Cropped Image", croppedImage);
    
    //convert to gray_scale
    cv::Mat src_gray;
    cvtColor(croppedImage, src_gray, COLOR_BGR2GRAY);
    
    //canny edge detection
    FilteredPoints filteredPoints = cannyEdgeDetection(src_gray);

    std::vector<int> filtered_x_points = filteredPoints.x;
    std::vector<int> filtered_y_points = filteredPoints.y;

    std::cout << "Filtered y_points : ";
    for (int y : filtered_y_points) {
        std::cout << y << " ";
    }
    cout << endl;


    CenterPoints centerPoints = processAndShowContours(croppedImage);

    std::vector<cv::Point> pinkCenterPoints = centerPoints.pink;
    std::vector<cv::Point> blueCenterPoints = centerPoints.blue;


    std::cout << "Blue Center Points :" << std::endl;
    for (const cv::Point& center : pinkCenterPoints) {
        int centerX = center.x;
        int centerY = center.y;
        std::cout << "Center Point: (" << centerX << ", " << centerY << ")" << std::endl;
    }

    int chessboardPosition[8][8] = {0};

    //pink chessboard positions

    for(int i=0; i<pinkCenterPoints.size(); ++i){
        int pos_x_point_p = pinkCenterPoints[i].x;
        int pos_y_point_p = pinkCenterPoints[i].y;

        // Find the insertion position in the sorted vector
        auto it_x_p = std::lower_bound(filtered_x_points.begin(), filtered_x_points.end(), pos_x_point_p);

        // Calculate the index where the point would sit
        int index_x_p = std::distance(filtered_x_points.begin(), it_x_p);

        // Find the insertion position in the sorted vector
        auto it_y_p = std::lower_bound(filtered_y_points.begin(), filtered_y_points.end(), pos_y_point_p);

        // Calculate the index where the point would sit
        int index_y_p = std::distance(filtered_y_points.begin(), it_y_p);

        cout << "index x:" << index_x_p << " index y: " << index_y_p << endl;

        chessboardPosition[index_x_p - 1][index_y_p - 1] = -1;
    }

    //blue chessboard positions
    int pos_x_point_b = 0;
    int pos_y_point_b = 0;
    int index_x_b = 0;
    int index_y_b = 0;

    for(int i=0; i<blueCenterPoints.size(); ++i){
        pos_x_point_b = blueCenterPoints[i].x;
        pos_y_point_b = blueCenterPoints[i].y;

        // Find the insertion position in the sorted vector
        auto it_x_b = std::lower_bound(filtered_x_points.begin(), filtered_x_points.end(), pos_x_point_b);

        // Calculate the index where the point would sit
        int index_x_b = std::distance(filtered_x_points.begin(), it_x_b);

        // Find the insertion position in the sorted vector
        auto it_y_b = std::lower_bound(filtered_y_points.begin(), filtered_y_points.end(), pos_y_point_b);

        // Calculate the index where the point would sit
        int index_y_b = std::distance(filtered_y_points.begin(), it_y_b);

        cout << "index x:" << index_x_b << " index y: " << index_y_b << endl;

        chessboardPosition[index_x_b - 1][index_y_b - 1] = 1;
    }
    int chessboardPositionFinal[8][8] = {0};

    for (int i = 0; i < 8; ++i)
      for (int j = 0; j < 8; ++j) {
         chessboardPositionFinal[j][i] = chessboardPosition[i][j];
      }

    cv::Mat chessboardMat(8, 8, CV_32S);
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            chessboardMat.at<int>(i, j) = chessboardPositionFinal[i][j];
        }
    }

    // Print the array
    std::cout << chessboardMat << std::endl;
    cout << "finished" << endl;
    cv::waitKey(0);

    return 0;
}
