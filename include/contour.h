#ifndef CONTOUR_H
#define CONTOUR_H

#include <fstream>
#include <iostream>
#include <string>
#include <chrono>
#include <ctime>
#include <vector>
#include <cstdio>
#include <bits/stdc++.h>
#include <glob.h>
#include"opencv2/highgui/highgui.hpp"
#include"opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/ml/ml.hpp"
#include "opencv2/opencv.hpp"


using namespace cv;
using namespace std;
using namespace cv::ml;
using namespace chrono;

using namespace cv;
#define n_CE 10


#define PI 3.1415926535897932384626433832795029L

#define pixelR(image,x,y) image.data[image.step[0]*y+image.step[1]*x+2]
#define pixelB(image,x,y) image.data[image.step[0]*y+image.step[1]*x]
#define pixelG(image,x,y) image.data[image.step[0]*y+image.step[1]*x+1]

template<typename T>
static Ptr<T> load_classifier(const string& filename_to_load);

void 
EllipticFourierDescriptors(std::vector<Point>& contour, std::vector<float> &CE);

void 
SkinTresholding(Mat3b& frame);

int 
FindTheLargestContour(std::vector<vector<Point>>contours);

void run_contour(char* argv);

#endif //end of CONTOUR_H