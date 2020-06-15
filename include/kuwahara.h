#ifndef KUWAHARA
#define KUWAHARA

// System Headers
#include "opencv2/opencv.hpp"
#include <stdio.h>
#include <chrono>
#include <ctime>
#include <iostream>
#include <bits/stdc++.h>
#include <opencv2/core/core.hpp>
#include <vector>
#include <string>
#include <glob.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

//namespace
using namespace cv;
using namespace std;
using namespace chrono;




#define Mpixel(image,x,y) ((uchar *)((image).data +(y)*((image).step)))[(x)*((image).channels())]//gray color space
#define pixelB(image,x,y) image.data[image.step[0]*y+image.step[1]*x]	//Blue color space
#define pixelG(image,x,y) image.data[image.step[0]*y+image.step[1]*x+1]	//Green color space
#define pixelR(image,x,y) image.data[image.step[0]*y+image.step[1]*x+2]	//Red color space

float binary_histogram(Mat image, int window_size, double histogram[]);

void thresholding_image(Mat image, int value, bool inverted, int window_size);

bool check_numb_in_a_row(string prev_name, string cur_name);

Point draw_rect_box(Mat input_image, Point* p1, Point* p2, int loop_number);

Mat Cropping_ROI(Mat imput_image,Point center_of_object);

int blob(Mat image2, Mat image3,Point *p1_rec, Point *p2_rec);

void Grey_to_Color(Mat3b source_image, Mat filtered_image,Mat3b output_image);

void Determining_ROI_Size(Mat source_image, int *small_x, int *small_y,
							int *large_x,int *large_y);

void Image_stitching(Mat source_image,Mat filtered_image,Mat output_image);

void median_filter(Mat image1,Mat image2,int window_size);

int FindTheLargestContour2(std::vector<vector<Point>>contours);

void Integral_test(Mat image1, double **integral_image, double **squared_integral_image);

void Integral_Gray_Initialize(Mat image1, double **integral_image,double **squared_integral_image);

void Integral_Color_Initialize(Mat3b image1, double **integral_image_B, 
								double **integral_image_G, double **integral_image_R, 
								double **squared_integral_image_B, double **squared_integral_image_G, 
								double **squared_integral_image_R);

void Filter_Gray_Integral(Mat image1, Mat image2, double** integral_image,
							double** squared_integral_image, int window_size);

void Filter_Gray_Integral2(Mat image1, Mat image2, double** integral_image,double** squared_integral_image, 
							int window_size);

void Kuwahara_Filter_Gray_With_Sum_Table(Mat source_image, Mat output_image, double** integral_image, 
										double** squared_integral_image, int window_size);

void Kuwahara_Filter_Color_With_Sum_Table(Mat3b source_image, Mat3b output_image, 
										double **integral_image_B, double **integral_image_G, 
										double **integral_image_R, double **squared_integral_image_B, 
										double **squared_integral_image_G, 
										double **squared_integral_image_R, int window_size);

void Filter_Gray_Integral4(Mat image1, Mat image2, double** integral_image,
							double** squared_integral_image, int window_size);

void Kuwahara_Filter_Gray_Without_Sum_Table(Mat source_image, Mat output_image, int window_size);

void Filter_Gray(Mat image1, Mat image2, int window_size);
int run_kuwahara(int argc,char *argv[]);

class Kuhawara
{

private:
	Mat original_img;
	Mat gray_img;
	Mat kuhawara_img;
public:
	void main(Mat source_img);
	
};

void Kuhawara::main(Mat source_img){
	if(source_img.cols>1000){
		cout<<"resize operating"<<endl;
		resize(source_img, original_img, cv::Size(), 0.5, 0.5);	
	}else{
		original_img=source_img;
	}
	cvtColor(original_img, gray_img, COLOR_BGR2GRAY);//color orginal image to gray scale
	
	double **integral_img=new double*[gray_img.cols+1];
	double **squared_integral_img=new double*[gray_img.cols+1];
	for(int i = 0; i < gray_img.cols+1; ++i){
		integral_img[i] = new double[gray_img.rows+1];
		squared_integral_img[i] = new double[gray_img.rows+1];
	}
	kuhawara_img=Mat::zeros(gray_img.size(),IMREAD_GRAYSCALE);//initialize the value of output metrices to zero
	Integral_Gray_Initialize(gray_img, integral_img, squared_integral_img);//create summed-table to integral_image array.
    Kuwahara_Filter_Gray_With_Sum_Table(gray_img, kuhawara_img, integral_img, squared_integral_img, 3);//Applying kuwahara filter to output using integral_image.
    /*Memory deallocation*/
	for(int i = 0; i < gray_img.cols+1; ++i) {
		delete [] integral_img[i];
		delete [] squared_integral_img[i];
	}
	delete [] integral_img;
	delete [] squared_integral_img;
	
}

class Kuhawara_ROI
{

private:
	Kuhawara **source;
	
	Mat ROI;
	Mat ROI_gray;

	Point p_start_roi_window;
	Point p_end_roi_window;
	Point p_center_of_object;

	int p_center;



public:
	Kuhawara_ROI(){
	
	}
	void Cropping_image(Mat source_img);
	

};

void Kuhawara_ROI::Cropping_image(Mat source_img){
	
	
}


#endif