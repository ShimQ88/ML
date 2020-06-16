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

Mat Cropping_ROI(Mat imput_image,Point center_of_object, int kernel_size);

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
	Kuhawara(){

	}
	Kuhawara(Mat source_img){

		/*process storing original image*/
		if(source_img.cols>1000){//resize if source img too big
			cout<<"resize operating"<<endl;
			resize(source_img, original_img, cv::Size(), 0.5, 0.5);	
		}else{
			original_img=source_img;
		}
		/********************************/

		/*process making gray image*/
		cvtColor(original_img, gray_img, COLOR_BGR2GRAY);//color orginal image to gray scale
		/***************************/
		
		/*process making kuhawara image*/
		proc_make_kuhawara();
		/*******************************/
		
		
	}
	void proc_make_kuhawara(){
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

	Size get_size_img(){
		return original_img.size();
	}

	Mat get_original_img(){
		return original_img;
	}

	Mat get_gray_img(){
		return gray_img;
	}
	Mat get_kuhawara_img(){
		return kuhawara_img;
	}
	
};


class Kuhawara_ROI
{

private:
	Kuhawara source1;
	Kuhawara source2;
	Kuhawara source3;

	Mat temp_output1;
	Mat temp_output2;
	Mat temp_output;

	Mat test;
	
	Mat ROI;
	Mat ROI_gray;
	Mat ROI_thresholded;
	Mat blob_window;

	Point *p_start_roi_window;
	Point *p_end_roi_window;
	Point p_center_of_object;

	double pixel_value;

	int p_center;



public:
	Kuhawara_ROI(){

	}
	Kuhawara_ROI(Kuhawara input1, Kuhawara input2, Kuhawara input3){
		// source2=input2;

		temp_output1=input1.get_kuhawara_img()-input2.get_kuhawara_img();
		temp_output2=input3.get_kuhawara_img()-input2.get_kuhawara_img();
		temp_output=temp_output1+temp_output2;

		thresholding_image(temp_output, 30,true,0);
		median_filter(temp_output,temp_output,11);

		
		p_start_roi_window=new Point[200];//approx numb
		p_end_roi_window=new Point[200];//approx numb

		

		blob_window=Mat::zeros(input2.get_size_img(),CV_8UC3);

		int count_numb=blob(temp_output,blob_window,p_start_roi_window,p_end_roi_window);

		if(count_numb==-100){
			cout<<"Skip: too much blob"<<endl;
			return;
		}//segmental fault

		
		p_center_of_object=draw_rect_box(input2.get_original_img(), p_start_roi_window, p_end_roi_window, 200);
		


		test=input2.get_original_img();
		circle(test,p_center_of_object,3,Scalar(0,0,255));
		ROI=Cropping_ROI(input2.get_original_img(),p_center_of_object,200);


		cvtColor(ROI, ROI_gray, CV_BGR2GRAY);

		// int total=3;
		// pixel_value=0;
		
		// pixel_value=pixel_value+(int)Mpixel(input2.get_gray_img(),p_center_of_object.x,p_center_of_object.y);
		// pixel_value=pixel_value+(int)Mpixel(input2.get_gray_img(),p_center_of_object.x+1,p_center_of_object.y);
		// pixel_value=pixel_value+(int)Mpixel(input2.get_gray_img(),p_center_of_object.x-1,p_center_of_object.y);
		
		// cout<<"1: "<<(int)Mpixel(input2.get_gray_img(),p_center_of_object.x,p_center_of_object.y)<<endl;
		// cout<<"2: "<<(int)Mpixel(input2.get_gray_img(),p_center_of_object.x,p_center_of_object.y+1)<<endl;
		// cout<<"3: "<<(int)Mpixel(input2.get_gray_img(),p_center_of_object.x,p_center_of_object.y-1)<<endl;

		ROI_thresholded=ROI_gray.clone();
		// thresholding_image(ROI_thresholded, (int)pixel_value/total,true,0);
		thresholding_image(ROI_thresholded, 40,true,0);



		// Mat ROI_thresholding=Cropping_ROI(temp_output,center_of_object,200);

	}
	Mat get_cropped_img(){return ROI;}
	Mat get_test(){return test;}
	Mat get_blob(){return blob_window;}
	Mat get_temp_output_img(){return temp_output;}
	Mat get_thresholded_img(){return ROI_thresholded;}

};



#endif