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
    cv::Scalar lowerGreen(30, 50, 50);
    cv::Scalar upperGreen(65, 255, 255);

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
    HoughLines(cannyEdges, lines, 1, CV_PI / 180, 125);
    
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

    //imshow("Canny edges", c_cannyEdges);

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
    /* Print the filtered x_points
    std::cout << "Filtered x_points: ";
    for (int x : filtered_x_points) {
        std::cout << x << " ";
    }
    */
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
    /* Print the filtered x_points
    std::cout << "Filtered y_points: ";
    for (int y : filtered_y_points) {
        std::cout << y << " ";
    }
    //cout << endl;
    */
    FilteredPoints filteredPoints;

    filteredPoints.x = filtered_x_points;
    filteredPoints.y = filtered_y_points;

    return filteredPoints;
}

CenterPoints processAndShowContours(const cv::Mat& src) {
    cv::Mat hsvImage;
    cv::cvtColor(src, hsvImage, cv::COLOR_BGR2HSV);
    //cv::imshow("HSV Image", hsvImage);

    cv::Scalar lowerRed1(0, 100, 150);
    cv::Scalar upperRed1(30, 255, 255);

    cv::Scalar lowerRed2(140, 50, 50);
    cv::Scalar upperRed2(179, 255, 255);

    cv::Scalar lowerBlue(100, 150, 150);
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
         if (boundingRect.area() > 250) {
            // Check if the bounding box is too wide
            if (boundingRect.width > 100) {
                cv::Rect leftRect(boundingRect.x, boundingRect.y, boundingRect.width / 2, boundingRect.height);
                cv::Rect rightRect(boundingRect.x + boundingRect.width / 2, boundingRect.y, boundingRect.width / 2, boundingRect.height);

                // Process and store the left rectangle
                if (cv::countNonZero(redMask(leftRect)) > 0)
                    contourColour = pinkColour;
                else
                    contourColour = blueColour;

                cv::rectangle(resultImage, leftRect, contourColour, 2);
                cv::circle(resultImage, cv::Point(centerX - boundingRect.width / 4, centerY), 4, contourColour, -1); // Draw center point
                imshow("Result", resultImage);

                if (contourColour == pinkColour) {
                    pinkCenterPoints.push_back(cv::Point(centerX - boundingRect.width / 4, centerY));
                } else {
                    blueCenterPoints.push_back(cv::Point(centerX - boundingRect.width / 4, centerY));
                }

                // Process and store the right rectangle
                if (cv::countNonZero(redMask(rightRect)) > 0)
                    contourColour = pinkColour;
                else
                    contourColour = blueColour;

                cv::rectangle(resultImage, rightRect, contourColour, 2);
                cv::circle(resultImage, cv::Point(centerX + boundingRect.width / 4, centerY), 4, contourColour, -1); // Draw center point
                imshow("Result", resultImage);

                if (contourColour == pinkColour) {
                    pinkCenterPoints.push_back(cv::Point(centerX + boundingRect.width / 4, centerY));
                } else {
                    blueCenterPoints.push_back(cv::Point(centerX + boundingRect.width / 4, centerY));
                }

            } else if (boundingRect.height > 100) {
                // Check if the bounding box is too tall
                cv::Rect topRect(boundingRect.x, boundingRect.y, boundingRect.width, boundingRect.height / 2);
                cv::Rect bottomRect(boundingRect.x, boundingRect.y + boundingRect.height / 2, boundingRect.width, boundingRect.height / 2);

                // Process and store the top rectangle
                if (cv::countNonZero(redMask(topRect)) > 0)
                    contourColour = pinkColour;
                else
                    contourColour = blueColour;

                cv::rectangle(resultImage, topRect, contourColour, 2);
                cv::circle(resultImage, cv::Point(centerX, centerY - boundingRect.height / 4), 4, contourColour, -1); // Draw center point
                imshow("Result", resultImage);

                if (contourColour == pinkColour) {
                    pinkCenterPoints.push_back(cv::Point(centerX, centerY - boundingRect.height / 4));
                } else {
                    blueCenterPoints.push_back(cv::Point(centerX, centerY - boundingRect.height / 4));
                }

                // Process and store the bottom rectangle
                if (cv::countNonZero(redMask(bottomRect)) > 0)
                    contourColour = pinkColour;
                else
                    contourColour = blueColour;

                cv::rectangle(resultImage, bottomRect, contourColour, 2);
                cv::circle(resultImage, cv::Point(centerX, centerY + boundingRect.height / 4), 4, contourColour, -1); // Draw center point
                imshow("Result", resultImage);

                if (contourColour == pinkColour) {
                    pinkCenterPoints.push_back(cv::Point(centerX, centerY + boundingRect.height / 4));
                } else {
                    blueCenterPoints.push_back(cv::Point(centerX, centerY + boundingRect.height / 4));
                }
            } else {
                // Determine the contour color based on the region
                if (cv::countNonZero(redMask(boundingRect)) > 0)
                    contourColour = pinkColour;
                else
                    contourColour = blueColour;

                // Draw rectangle and center point
                cv::rectangle(resultImage, boundingRect, contourColour, 2);
                cv::circle(resultImage, cv::Point(centerX, centerY), 4, contourColour, -1); // Draw center point
                imshow("Result", resultImage);
                
                // Store center point in the appropriate vector
                if (contourColour == pinkColour) {
                    pinkCenterPoints.push_back(cv::Point(centerX, centerY));
                } else {
                    blueCenterPoints.push_back(cv::Point(centerX, centerY));
                }
            }
        }
    }
    //std::cout << "Pink Center Points:" << std::endl;
for (const cv::Point& center : pinkCenterPoints) {
    int centerX = center.x;
    int centerY = center.y;
    //std::cout << "Center Point: (" << centerX << ", " << centerY << ")" << std::endl;
}

//std::cout << "Blue Center Points:" << std::endl;
for (const cv::Point& center : blueCenterPoints) {
    int centerX = center.x;
    int centerY = center.y;
    //std::cout << "Center Point: (" << centerX << ", " << centerY << ")" << std::endl;
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

double calculateDistance(const cv::Point& point1, const cv::Point& point2) {
    double dx = point1.x - point2.x;
    double dy = point1.y - point2.y;
    return std::sqrt(dx * dx + dy * dy);
}

void rotateChessboard(int chessboard[8][8]) {
    int temp[8][8];

    // Copy the original chessboard to a temporary matrix
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            temp[i][j] = chessboard[i][j];
        }
    }

    // Rotate the chessboard 90 degrees counterclockwise
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            chessboard[i][j] = temp[j][8 - 1 - i];
        }
    }
}

void reflectYAxis(int chessboard[8][8]) {
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8 / 2; ++j) {
            // Swap elements across the y-axis (columns)
            std::swap(chessboard[i][j], chessboard[i][8 - 1 - j]);
        }
    }
}


cv::Mat CVRunMain(std::string photoName){
//int main(){
    try {
        // Declare the output variables
        Mat dst, cdst, cdstP;
        //std::string version = cv::getVersionString();
        std::string photoName = "B1000";
        // Print the version information
        //std::cout << "OpenCV version: " << version << std::endl;
        std::string filenameStr = "Pictures/" + photoName + ".jpg";

        // Convert std::string to const char*
        const char* filename = filenameStr.c_str();
        //const char* filename = "C:/Users/James/Documents/Coding/ChessRobot2023FYP/Pictures/Y1605.jpg";
        //const char* filename = argc >=2 ? argv[1] : default_file;
        // Loads an image
        Mat src = imread( samples::findFile( filename ));
        // Check if image is loaded fine
        if(src.empty()){
            printf(" Error opening image\n");
            printf(" Program Arguments: [image_name -- default %s] \n", filename);
            //return 0;
        }

         // Brightness factor (increase to brighten, decrease to darken)
        //double brightness_factor = 1.5;  // Adjust this value to control brightness

        // Perform addition to brighten the image
        //src = src + cv::Scalar(brightness_factor, brightness_factor, brightness_factor);

        //imshow("src", src);
        // Locate green square positions
        std::vector<cv::Point> crop_points = locateGreenSquares(src);
        int numberOfGreenSquares = crop_points.size();

        /*
        for (const cv::Point& point : crop_points) {
            std::cout << "Points: (" << point.x << ", " << point.y << ")" << std::endl;
        }   
        //cout << numberOfGreenSquares << endl;
        */

        std::vector<cv::Point> filteredCropPoints;
        std::vector<cv::Point> filteredGreenPoints;

        double minDistance = 100.0; // Points within 100 pixels of each other
        double maxDistance = 500.0;

    //filtering out unnecessary points
        for (const Point& point : crop_points) {
            if (point.x > minDistance && point.y > minDistance) {
                bool keepPoint = true;

                for (const Point& filteredPoint : filteredCropPoints) {
                    double distance = calculateDistance(point, filteredPoint);

                    if (distance < minDistance) {
                        // If the point is within 100 pixels of another point, remove it
                        keepPoint = false;
                        break; // No need to check other points
                    } else if (distance < maxDistance) {
                        // If the point is within 500 pixels of another point, remove it
                        keepPoint = false;
                        break; // No need to check other points
                    }
                }
    
                if (keepPoint) {
                    filteredCropPoints.push_back(point);
                    //std::cout << "Saved point: (" << point.x << ", " << point.y << ")" << std::endl;
                }
            }
        }

        // Print the filtered points
        /*
        for (const cv::Point& point : filteredCropPoints) {
            std::cout << "Point: (" << point.x << ", " << point.y << ")" << std::endl;
        }
        */



        //finding points
        cv::Point2f  point1 = filteredCropPoints[0];
        cv::Point2f  point2 = filteredCropPoints[1];
        cv::Point2f  point3 = filteredCropPoints[2];
        cv::Point2f  point4 = filteredCropPoints[3];
        cout << point1 << " " << point2 << " " << point3 << " " << point4 << endl;

        // Initialize min_crop_point and min_crop_point_ind with large values
        int min_crop_point = INT_MAX;
        int min_crop_point_ind = -1;

        // First loop to find the minimum y-value and its index
        for (int i = 0; i < 4; i++) {
            if (filteredCropPoints[i].y < min_crop_point) {
                min_crop_point = filteredCropPoints[i].y;
                min_crop_point_ind = i;
            }
        }

        // Initialize two_min_crop_point and two_min_crop_point_ind with large values
        int two_min_crop_point = INT_MAX;
        int two_min_crop_point_ind = -1;

        // Second loop to find the second minimum y-value and its index, excluding the index found in the first loop
        for (int i = 0; i < 4; i++) {
            if (i != min_crop_point_ind && filteredCropPoints[i].y < two_min_crop_point) {
                two_min_crop_point = filteredCropPoints[i].y;
                two_min_crop_point_ind = i;
            }
        }
        //cout << min_crop_point_ind << " " << two_min_crop_point_ind << endl;
        //cout << filteredCropPoints[min_crop_point_ind] << " " << filteredCropPoints[two_min_crop_point_ind] << endl;
        
        int temp_ind = INT_MAX;
        //rotate image
        if (filteredCropPoints[min_crop_point_ind].x > filteredCropPoints[two_min_crop_point_ind].x){
            //cv::Mat rotatedImage = rotateImageToStraight(src, filteredCropPoints[two_min_crop_point_ind], filteredCropPoints[min_crop_point_ind]);
            temp_ind = min_crop_point_ind;
            min_crop_point_ind = two_min_crop_point_ind;
            two_min_crop_point = temp_ind;
        } 
        
        cv::Mat rotatedImage = rotateImageToStraight(src, filteredCropPoints[min_crop_point_ind], filteredCropPoints[two_min_crop_point_ind]);
        
        int Rwidth = rotatedImage.cols;
        int Rheight = rotatedImage.rows;

        //std::cout << "Width: " << Rwidth << std::endl;
        //std::cout << "Height: " << Rheight << std::endl;

        //display rotated image
        //cv::imshow("Rotated Image", rotatedImage);
        
        //locate green squares in rotated image
        std::vector<cv::Point> greenSquarePositions = locateGreenSquares(rotatedImage);

        /*Display the detected green square positions
        for (const auto& position : greenSquarePositions)
        {
            std::cout << "Green square position: (" << position.x << ", " << position.y << ")" << std::endl;
        }
        */

        //filtering out unnecessary points
        for (const Point& point1 : greenSquarePositions) {
            if (point1.x > 200 && point1.y > 50) {
                bool keepPoint = true;

                for (const Point& filteredGPoint : filteredGreenPoints) {
                    double distance = calculateDistance(point1, filteredGPoint);

                    if (distance < minDistance) {
                        // If the point is within 100 pixels of another point, remove it
                        keepPoint = false;
                        break; // No need to check other points
                    } else if (distance < maxDistance) {
                        // If the point is within 500 pixels of another point, remove it
                        keepPoint = false;
                        break; // No need to check other points
                    }
                }

                if (keepPoint) {
                    filteredGreenPoints.push_back(point1);
                    //std::cout << "Saved point: (" << point1.x << ", " << point1.y << ")" << std::endl;
                }
            }
        }

        //finding positions of chess board corners
        int position1_x = filteredGreenPoints[0].x;
        int position1_y = filteredGreenPoints[0].y;

        int position2_x = filteredGreenPoints[1].x;
        int position2_y = filteredGreenPoints[1].y;

        int position3_x = filteredGreenPoints[2].x;
        int position3_y = filteredGreenPoints[2].y;

        int position4_x = filteredGreenPoints[3].x;
        int position4_y = filteredGreenPoints[3].y;

        int width = 0;
        int height = 0;
        int temp_width, temp_height;

        for (int i=0; i<4; i++){
            temp_width = filteredGreenPoints[0].x - filteredGreenPoints[i].x;
            temp_height = filteredGreenPoints[0].y - filteredGreenPoints[i].y;

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
            if(filteredGreenPoints[i].x < min_x)
                min_x = filteredGreenPoints[i].x;
            
            if(filteredGreenPoints[i].y < min_y)
                min_y = filteredGreenPoints[i].y;
        }
        

        //cout << min_x << " " << min_y << " " << width << " " << height << endl;

        cv::Rect crop_region(min_x, min_y, width, height);

        cv::Mat croppedImage = rotatedImage(crop_region);

        //cv::imshow("Cropped Image", croppedImage);
        
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
        
        std::cout << "Filtered x_points : ";
        for (int x : filtered_x_points) {
            std::cout << x << " ";
        }


        CenterPoints centerPoints = processAndShowContours(croppedImage);

        std::vector<cv::Point> pinkCenterPoints = centerPoints.pink;
        std::vector<cv::Point> blueCenterPoints = centerPoints.blue;

         std::vector<cv::Point> filteredPinkCenterPoints;
        for (const auto& point1 : pinkCenterPoints) {
            bool addPoint = true;
            for (const auto& point2 : filteredPinkCenterPoints) {
                if (calculateDistance(point1, point2) < 50) {
                    addPoint = false;
                    break;
                }
            }
            if (addPoint)
                filteredPinkCenterPoints.push_back(point1);
        }

        // Filter blueCenterPoints
        std::vector<cv::Point> filteredBlueCenterPoints;
        for (const auto& point1 : blueCenterPoints) {
            bool addPoint = true;
            for (const auto& point2 : filteredBlueCenterPoints) {
                if (calculateDistance(point1, point2) < 50) {
                    addPoint = false;
                    break;
                }
            }
            if (addPoint)
                filteredBlueCenterPoints.push_back(point1);
        }

        // Print the filtered points
        std::cout << "Filtered Pink Center Points:" << std::endl;
        for (const auto& point : filteredPinkCenterPoints)
            std::cout << point << std::endl;

        std::cout << "Filtered Blue Center Points:" << std::endl;
        for (const auto& point : filteredBlueCenterPoints)
            std::cout << point << std::endl;

        /*std::cout << "Blue Center Points :" << std::endl;
        for (const cv::Point& center : pinkCenterPoints) {
            int centerX = center.x;
            int centerY = center.y;
            std::cout << "Center Point: (" << centerX << ", " << centerY << ")" << std::endl;
        }
*/
        int chessboardPosition[8][8] = {0};

        //pink chessboard positions

        for(int i=0; i<pinkCenterPoints.size(); i++){
            int pos_x_point_p = pinkCenterPoints.at(i).x;
            int pos_y_point_p = pinkCenterPoints.at(i).y;
            
            //cout << "index x:" << pos_x_point_p << " index y: " << pos_y_point_p << endl;
            // Find the insertion position in the sorted vector
            auto it_x_p = std::lower_bound(filtered_x_points.begin(), filtered_x_points.end(), pos_x_point_p);

            // Calculate the index where the point would sit
            int index_x_p = std::distance(filtered_x_points.begin(), it_x_p);

            // Find the insertion position in the sorted vector
            auto it_y_p = std::lower_bound(filtered_y_points.begin(), filtered_y_points.end(), pos_y_point_p);

            // Calculate the index where the point would sit
            int index_y_p = std::distance(filtered_y_points.begin(), it_y_p);

            //cout << "index x:" << index_x_p << " index y: " << index_y_p << endl;

            if(index_x_p > 0 && index_y_p > 0 && index_y_p < 9 && index_x_p < 9){
                //cout << "index x:" << index_x_p << " index y: " << index_y_p << endl;
                chessboardPosition[index_x_p-1][index_y_p-1] = -1;
            }
        }

        //blue chessboard positions
        int pos_x_point_b = 0;
        int pos_y_point_b = 0;
        int index_x_b = 0;
        int index_y_b = 0;

        for(int i=0; i<blueCenterPoints.size(); ++i){
            pos_x_point_b = blueCenterPoints[i].x;
            pos_y_point_b = blueCenterPoints[i].y;
            
            if (pos_x_point_b > 0 && pos_x_point_b < 10000 && pos_y_point_b > 0 && pos_y_point_b < 10000){
            
                //cout << " x:" << pos_x_point_b << "  y: " << pos_y_point_b << endl;
                
                // Find the insertion position in the sorted vector
                auto it_x_b = std::lower_bound(filtered_x_points.begin(), filtered_x_points.end(), pos_x_point_b);

                // Calculate the index where the point would sit
                int index_x_b = std::distance(filtered_x_points.begin(), it_x_b);

                // Find the insertion position in the sorted vector
                auto it_y_b = std::lower_bound(filtered_y_points.begin(), filtered_y_points.end(), pos_y_point_b);

                // Calculate the index where the point would sit
                int index_y_b = std::distance(filtered_y_points.begin(), it_y_b);

                //cout << "index x:" << index_x_b << " index y: " << index_y_b << endl;
                if(index_x_b > 0 && index_y_b > 0 && index_y_b < 9 && index_x_b < 9){
                    //cout << "index x:" << index_x_b << " index y: " << index_y_b << endl;
                    chessboardPosition[index_x_b-1][index_y_b-1] = 1;
                }
            }
        }
        
        for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            std::cout << std::setw(3) << chessboardPosition[i][j] << " ";
        }
        std::cout << "\n";
    }

        //rotateChessboard(chessboardPosition);
        reflectYAxis(chessboardPosition);


        cv::Mat chessboardMat(8, 8, CV_32S);
        for (int i = 0; i < 8; ++i) {
            for (int j = 0; j < 8; ++j) {
                chessboardMat.at<int>(i, j) = chessboardPosition[i][j];
            }
        }

        // Print the array
        //std::cout << chessboardMat << std::endl;
        //cout << "finished" << endl;
        //cv::waitKey(0);
        //return 0;
        return chessboardMat;
    } catch (...) {
        std::cerr << "Unknown error occurred." << std::endl;
    }

}
