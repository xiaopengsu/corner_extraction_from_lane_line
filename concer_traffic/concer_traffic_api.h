#ifndef CONCER_TRAFFIC_API
#define CONCER_TRAFFIC_API
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <vector>
#include <algorithm>

void findroadconcers(cv::Mat srcImage,cv::Mat maskimage, std::vector<std::vector<cv::Point> > & roadlinepoints);

#endif
