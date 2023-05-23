#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>

using namespace cv;
using namespace std;
int main(int argc, char** argv)
{
    // Declare the output variables
    Mat dst, cdst, cdstP;
    const char* default_file = "1.jpg";
    const char* filename = argc >=2 ? argv[1] : default_file;
    // Loads an image
    Mat src = imread( samples::findFile( filename ), IMREAD_GRAYSCALE );
    // Check if image is loaded fine
    if(src.empty()){
        printf(" Error opening image\n");
        printf(" Program Arguments: [image_name -- default %s] \n", default_file);
        return -1;
    }
    
    // Rect roi(300, 155, 780, 765); // starting x, starting y, width, height

    // extract the ROI from the input image
    // Mat src = srcPre(roi);
    
    // Edge detection
    Canny(src, dst, 50, 200, 3);
    // Copy edges to the images that will display the results in BGR
    cvtColor(dst, cdst, COLOR_GRAY2BGR);
    cdstP = cdst.clone();
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
    HoughLinesP(dst, linesP, 1, CV_PI/180, 20, 50, 10 ); // runs the actual detection
    // Draw the lines
    for( size_t i = 0; i < linesP.size(); i++ )
    {
        Vec4i l = linesP[i];
        line( cdstP, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0,0,255), 3, LINE_AA);
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
