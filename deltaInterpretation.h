#ifndef DELTAINTERPRETATION_H
#define DELTAINTERPRETATION_H
#include "opencv2/opencv.hpp"
#include <array>
#include <iostream>

using namespace std;

array<int,4> deltaInterp(cv::Mat a);
int bi_popen(const char* const command, FILE** const in, FILE** const out);
std::string runStock(vector<string> move);


#endif
