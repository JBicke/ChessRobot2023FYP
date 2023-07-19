#include <iostream>
//#include "opencv2/imgcodecs.hpp"
//#include "opencv2/highgui.hpp"
//#include "opencv2/imgproc.hpp"
#include <opencv2/opencv.hpp>


using namespace cv;
using namespace std;

/*
void detectCornerHarris(int,void*)
{
    int blockSize = 2;
    int apertureSize = 3;
    double k = 0.04;
    cv::Mat dst = cv::Mat::zeros( src.size(), CV_32FC1 );
    cv::cornerHarris( src, dst, blockSize, apertureSize, k );
    cv::Mat dst_norm, dst_norm_scaled;
    cv::normalize( dst, dst_norm, 0, 255, cv::NORM_MINMAX, CV_32FC1, cv::Mat() );
    cv::convertScaleAbs( dst_norm, dst_norm_scaled );
    for( int i = 0; i < dst_norm.rows ; i++ )
    {
        for( int j = 0; j < dst_norm.cols; j++ )
        {
            if( (int) dst_norm.at<float>(i,j) > 200 )
            {
                cv::circle( dst_norm_scaled, cv::Point(j,i), 5,  cv::Scalar(255), 2, 8, 0 );
            }
        }
    }
    
    cv::imshow( "harris corners", dst_norm_scaled );
}
*/


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

    //colour detection

    //convert image to HSV
    cv::Mat hsvImage;
    cv::cvtColor(src, hsvImage, cv::COLOR_BGR2HSV);

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
    cv::Mat resultImage = src.clone();
    cv::Scalar contourColour;
    cv::Scalar pinkColour(255, 0, 255);  // Green color for contour bounding boxes
    cv::Scalar blueColour(255,0,0);
    for (size_t i = 0; i < contours.size(); ++i) {
        cv::Rect boundingRect = cv::boundingRect(contours[i]);
        if (cv::countNonZero(pinkMask(boundingRect)) > 0)
            contourColour = pinkColour;
        else
            contourColour = blueColour;
        cv::rectangle(resultImage, boundingRect, contourColour, 2);
    }
    

    cv::imshow( "HSV Image", resultImage);
    


    //corner detection
    int blockSize = 2;
    int apertureSize = 3;
    double k = 0.04;
    cv::Mat dst_corner = cv::Mat::zeros( src_gray.size(), CV_32FC1 );
    cv::cornerHarris( src_gray, dst_corner, blockSize, apertureSize, k );
    cv::Mat dst_norm, dst_norm_scaled;
    cv::normalize( dst_corner, dst_norm, 0, 255, cv::NORM_MINMAX, CV_32FC1, cv::Mat() );
    cv::convertScaleAbs( dst_norm, dst_norm_scaled );
    for( int i = 0; i < dst_norm.rows ; i++ )
    {
        for( int j = 0; j < dst_norm.cols; j++ )
        {
            if( (int) dst_norm.at<float>(i,j) > 180 )
            {
                cv::circle( dst_norm_scaled, cv::Point(j,i), 5,  cv::Scalar(255), 2, 8, 0 );
            }
        }
    }
    
    cv::imshow( "harris corners", dst_norm_scaled );


    // Rect roi(300, 155, 780, 765); // starting x, starting y, width, height

    // extract the ROI from the input image
    // Mat src = srcPre(roi);
    
    // Edge detection
    Canny(src_gray, dst, 100, 150, 3);

    

    // Copy edges to the images that will display the results in BGR
    cvtColor(dst, cdst, COLOR_GRAY2BGR);
    cdstP = cdst.clone();
    cout << cdst.rows << endl;
    //identify chess board columns
    std::vector<cv::Point> square_positions;
    int target_row = 500;
    cv::Mat row = cdst.row(target_row);


    for ( int i = 0; i < row.cols; i++){
        uchar intensity = row.at<uchar>(0, i);

        if (intensity == 255){
            cv::Point location(i, target_row);
            square_positions.push_back(location);

        }

    }
    cout << square_positions << endl;

    // Standard Hough Line Transform
    vector<Vec2f> lines; // will hold the results of the detection
    HoughLines(dst, lines, 1, CV_PI/180, 150, 0, 0 ); // runs the actual detection
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
        line( cdst, pt1, pt2, Scalar(0,0,255), 3, LINE_AA);
    }
    
		//// Initialize a vector to store intersection points
	//std::vector<cv::Point> intersectionPoints;

	//// Loop through all pairs of lines
	//for (size_t i = 0; i < lines.size(); i++) {
		//for (size_t j = i + 1; j < lines.size(); j++) {
			
			//// Get endpoints of the current pair of lines
			//cv::Vec4i line1 = lines[i];
			//cv::Vec4i line2 = lines[j];
			//cv::Point pt1_1(line1[0], line1[1]);
			//cv::Point pt1_2(line1[2], line1[3]);
			//cv::Point pt2_1(line2[0], line2[1]);
			//cv::Point pt2_2(line2[2], line2[3]);
			
			//// Calculate slopes and y-intercepts of the two lines
			//double m1 = (double)(pt1_2.y - pt1_1.y) / (double)(pt1_2.x - pt1_1.x);
			//double b1 = (double)pt1_1.y - m1 * (double)pt1_1.x;
			//double m2 = (double)(pt2_2.y - pt2_1.y) / (double)(pt2_2.x - pt2_1.x);
			//double b2 = (double)pt2_1.y - m2 * (double)pt2_1.x;
			
			//// Check if the lines are parallel or nearly parallel
			//double denom = m1 - m2;
			//if (std::abs(denom) < 1e-6) {
				//continue; // Lines are parallel, no intersection point
			//}
			
			//// Calculate the intersection point of the two lines
			//double x = (b2 - b1) / denom;
			//double y = m1 * x + b1;
			
			//// Check if the intersection point is within the bounds of the image
			//if (x < 0 || x >= cdst.cols || y < 0 || y >= cdst.rows) {
				//continue; // Intersection point is outside image bounds
			//}
			
			//// Add the intersection point to the vector (if not already in it)
			//cv::Point intersection(x, y);
			//if (std::find(intersectionPoints.begin(), intersectionPoints.end(), intersection) == intersectionPoints.end()) {
				//intersectionPoints.push_back(intersection);
			//}
		//}
	//}
    
    
    // Probabilistic Line Transform
    vector<Vec4i> linesP; // will hold the results of the detection
    HoughLinesP(dst, linesP, 1, CV_PI/180, 20, 50, 20 ); // runs the actual detection
    // Draw the lines
    for( size_t i = 0; i < linesP.size(); i++ )
    {
        Vec4i l = linesP[i];
        line( cdstP, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0,0,255), 3, LINE_AA);
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

    // Show results
    imshow("Source", src);
    imshow("Canny", dst);
    imshow("Detected Lines (in red) - Standard Hough Line Transform", cdst);
    imshow("Detected Lines (in red) - Probabilistic Line Transform", cdstP);
    // Wait and Exit
    waitKey();
    return 0;
}
