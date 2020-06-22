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
#include "contour.h"
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
	void main(Mat source_img){

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
	    Kuwahara_Filter_Gray_With_Sum_Table(gray_img, kuhawara_img, integral_img, squared_integral_img, 5);//Applying kuwahara filter to output using integral_image.
	    /*Memory deallocation*/
		for(int i = 0; i < gray_img.cols+1; ++i) {
			delete [] integral_img[i];
			delete [] squared_integral_img[i];
		}
		delete [] integral_img;
		delete [] squared_integral_img;
	}


	Size get_ori_size_img(){
		return original_img.size();
	}

	Size get_ku_size_img(){
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


class Kuhawara_ROI2
{

private:
	bool is_initialize_success;
	// Kuhawara source1;
	// Kuhawara source2;
	// Kuhawara source3;

	// Mat temp_output1;
	// Mat temp_output2;
	// Mat temp_output;

	// Mat test;
	
	Mat *ROI;
	// Mat ROI1;
	// Mat ROI2;
	// Mat ROI3;
	// Mat ROI4;
	// Mat ROI5;


	Mat *samp_output;
	Mat merged_samp_output;
	// Mat samp_output1;
	// Mat samp_output2;
	// Mat samp_output3;
	// Mat samp_output4;
	// Mat samp_output;


	Mat ROI_gray;
	Mat ROI_thresholded;
	Mat blob_window;
	Mat drawing;
	Mat *some_temp_output;
	Mat temp_output;

	Point *p_start_roi_window;
	Point *p_end_roi_window;
	Point p_center_of_object;

	double pixel_value;

	int p_center;
	int total_numb;
	
	


public:
	Kuhawara_ROI2(){

	}
	void main(Kuhawara *image, int total, int target_index){
		total_numb=total;
		some_temp_output=new Mat[2];

		if(target_index==0){
			temp_output=image[target_index+1].get_kuhawara_img()-image[target_index].get_kuhawara_img();
		}else if(target_index==total-1){
			temp_output=image[target_index-1].get_kuhawara_img()-image[target_index].get_kuhawara_img();
		}else{
			some_temp_output[0]=image[target_index-1].get_kuhawara_img()-image[target_index].get_kuhawara_img();
			some_temp_output[1]=image[target_index+1].get_kuhawara_img()-image[target_index].get_kuhawara_img();
			temp_output=some_temp_output[0]+some_temp_output[1];	
		}

		// cout<<"THis is point 1"<<endl;
		// some_temp_output[0]=image[target_index-1].get_kuhawara_img()-image[target_index].get_kuhawara_img();
		// some_temp_output[1]=image[target_index+1].get_kuhawara_img()-image[target_index].get_kuhawara_img();
		// temp_output=some_temp_output[0]+some_temp_output[1];


		// for(int i=0;i<total_numb-1;i++){
		// 	some_temp_output[i]=image[i].get_kuhawara_img()-image[i+1].get_kuhawara_img();
			
		// 	if(i==0){
		// 		temp_output=some_temp_output[i];	
		// 	}else{
		// 		temp_output=temp_output+some_temp_output[i];
		// 	}
		// }
		thresholding_image(temp_output, 50,true,0);
		median_filter(temp_output,temp_output,7);

		p_start_roi_window=new Point[200];//approx numb
		p_end_roi_window=new Point[200];//approx numb

		blob_window=Mat::zeros(image[target_index].get_ori_size_img(),CV_8UC3);//default size initializing

		// cout<<"THis is point 2"<<endl;
		int count_numb=blob(temp_output, blob_window, p_start_roi_window, p_end_roi_window);

		// cout<<"THis is point 2-1"<<endl;
		if(count_numb==-100){
			cout<<"Skip: too much blob"<<endl;
			is_initialize_success=false;
			return;
		}//segmental fault
		// cout<<"THis is point 2-2"<<endl;
		p_center_of_object=draw_rect_box(image[target_index].get_original_img(), p_start_roi_window, p_end_roi_window, 200);

		//make scan the size of object
		

		int cropping_size=400;
		// int number_of_pixel;
		// int jj=0;
		// while(true){
		// 	cout<<"loop inside"<<endl;
		// 	Mat temp_test=Cropping_ROI(temp_output,p_center_of_object,cropping_size);
		// 	number_of_pixel=0;
		// 	for(int i=0;i<temp_test.cols;i++){
		// 		for(int j=0;j<temp_test.rows;j++){
		// 			if( (i==2)||(j==2)||(j==temp_test.rows-3)||(i==temp_test.cols-3) ){
		// 				cout<<"pixel_v: "<<(int)Mpixel(temp_test,i,j)<<endl;
		// 				if( (int)Mpixel(temp_test,i,j)!=0){
		// 					number_of_pixel++;
		// 				}
		// 				jj++;
		// 			}
					
		// 		}
		// 	}
		// 	cout<<"jj: "<<jj<<endl;
		// 	int total_pixel=cropping_size*cropping_size;
			
		// 	if( (temp_test.cols + temp_test.rows)*0.7 < number_of_pixel){
				
		// 	}else{
		// 		break;
		// 	}
		// 	cropping_size=cropping_size+100;
		// }
		// cout<<"number_of_pixel: "<<number_of_pixel<<endl;
		// cout<<"cropping_size: "<<cropping_size<<endl;
		// getchar();

		// cout<<"THis is point 3"<<endl;

		// p_start_roi_window=new Point[200];
		Kuhawara temp_ku[total];
		samp_output = new Mat[total-1];
		ROI=new Mat[total];
		for(int i=0;i<total_numb;i++){
			ROI[i]=Cropping_ROI(image[i].get_original_img(),p_center_of_object,cropping_size);
			temp_ku[i].main(ROI[i]);
		


			// some_temp_output[i]=image[i].get_kuhawara_img()-image[i+1].get_kuhawara_img();
			
			// if(i==0){
			// 	temp_output=some_temp_output[i];	
			// }else{
			// 	temp_output=temp_output+some_temp_output[i];
			// }
		}
		// cout<<"THis is point 4"<<endl;
		int temp_i=0;


		bool the_first_merge=false;
		for(int i=0;i<total_numb;i++){			
			if(i!=target_index){
				// cout<<"i: " <<i<<endl;
				// cout<<"target_index: " <<target_index<<endl;
				samp_output[temp_i]=temp_ku[i].get_kuhawara_img()-temp_ku[target_index].get_kuhawara_img();
				

				if(the_first_merge==false){
					merged_samp_output=samp_output[temp_i].clone();
					the_first_merge=true;
				}else{
					merged_samp_output=merged_samp_output+samp_output[temp_i];	
				}
				temp_i++;	
			}
		}
		// cout<<"THis is point 5"<<endl;

		

		// ROI=Cropping_ROI(image[1].get_original_img(),p_center_of_object,200);

		// cvtColor(ROI, ROI_gray, CV_BGR2GRAY);

		// ROI_thresholded=ROI_gray.clone();
		// // thresholding_image(ROI_thresholded, (int)pixel_value/total,true,0);
		// // cv::threshold(ROI_gray, ROI_thresholded, 0, 255, CV_THRESH_BINARY_INV | CV_THRESH_TRIANGLE);
		thresholding_image(merged_samp_output, 50,true,0);
		median_filter(merged_samp_output,merged_samp_output,7);

		Point ROI_mid_p;

		ROI_mid_p.x=merged_samp_output.rows/2;
		ROI_mid_p.y=merged_samp_output.cols/2;

		std::vector<vector<Point>>contours;

		// findContours(ROI_gray,contours,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_NONE);
		findContours(merged_samp_output,contours,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_NONE);

		int object_i = Find_The_Object_Contour(contours,ROI_mid_p);
		// // cout<<"p4"<<endl;
		// // cout<<"1"<<endl;
		drawing=Mat::zeros(merged_samp_output.size(),CV_8UC3);
		Scalar color=CV_RGB(255,0,0);
		drawContours(drawing,contours,object_i,color,2,8);
		is_initialize_success=true;
		cout<<"success initializing"<<endl;
	}
	// Kuhawara_ROI2(Kuhawara input1, Kuhawara input2, Kuhawara input3){
	// 	// source2=input2;

	// 	temp_output1=input1.get_kuhawara_img()-input2.get_kuhawara_img();
	// 	temp_output2=input3.get_kuhawara_img()-input2.get_kuhawara_img();
	// 	temp_output=temp_output1+temp_output2;

	// 	thresholding_image(temp_output, 50,true,0);
	// 	// cv::threshold(temp_output3, temp_output, 0, 255, CV_THRESH_BINARY_INV | CV_THRESH_OTSU);
	// 	median_filter(temp_output,temp_output,5);

		


	// 	p_start_roi_window=new Point[200];//approx numb
	// 	p_end_roi_window=new Point[200];//approx numb

		

	// 	blob_window=Mat::zeros(input2.get_ori_size_img(),CV_8UC3);

	// 	int count_numb=blob(temp_output,blob_window,p_start_roi_window,p_end_roi_window);

	// 	if(count_numb==-100){
	// 		cout<<"Skip: too much blob"<<endl;
	// 		is_initialize_success=false;
	// 		return;
	// 	}//segmental fault

		
	// 	p_center_of_object=draw_rect_box(input2.get_original_img(), p_start_roi_window, p_end_roi_window, 200);
		


	// 	// test=input2.get_original_img();
	// 	// circle(test,p_center_of_object,3,Scalar(0,0,255));
	// 	ROI=Cropping_ROI(input2.get_original_img(),p_center_of_object,200);

	// 	ROI1=Cropping_ROI(input1.get_original_img(),p_center_of_object,200);
	// 	ROI2=Cropping_ROI(input2.get_original_img(),p_center_of_object,200);
	// 	ROI3=Cropping_ROI(input3.get_original_img(),p_center_of_object,200);

	// 	// Kuhawara samp1(ROI1);
	// 	// Kuhawara samp2(ROI2);
	// 	// Kuhawara samp3(ROI3);

	// 	Kuhawara samp1;
	// 	Kuhawara samp2;
	// 	Kuhawara samp3;


	// 	samp1.main(ROI1);
	// 	samp2.main(ROI2);
	// 	samp3.main(ROI3);

	// 	cout<<"ori size: "<<samp1.get_ori_size_img()<<endl;
	// 	cout<<"ku size: "<<samp1.get_ku_size_img()<<endl;

	// 	Mat samp_output1=samp1.get_kuhawara_img()-samp2.get_gray_img();
	// 	Mat samp_output2=samp3.get_gray_img()-samp2.get_kuhawara_img();
	// 	Mat samp_output=samp_output1+samp_output2;

	// 	ROI=ROI2;

	// 	// cvtColor(ROI, ROI_gray, CV_BGR2GRAY);
	// 	// cvtColor(ROI1, ROI_gray1, CV_BGR2GRAY);
	// 	// cvtColor(ROI2, ROI_gray2, CV_BGR2GRAY);
	// 	// cvtColor(ROI3, ROI_gray3, CV_BGR2GRAY);








	// 	// int total=3;
	// 	// pixel_value=0;
		
	// 	// pixel_value=pixel_value+(int)Mpixel(input2.get_gray_img(),p_center_of_object.x,p_center_of_object.y);
	// 	// pixel_value=pixel_value+(int)Mpixel(input2.get_gray_img(),p_center_of_object.x+1,p_center_of_object.y);
	// 	// pixel_value=pixel_value+(int)Mpixel(input2.get_gray_img(),p_center_of_object.x-1,p_center_of_object.y);
		
	// 	// cout<<"1: "<<(int)Mpixel(input2.get_gray_img(),p_center_of_object.x,p_center_of_object.y)<<endl;
	// 	// cout<<"2: "<<(int)Mpixel(input2.get_gray_img(),p_center_of_object.x,p_center_of_object.y+1)<<endl;
	// 	// cout<<"3: "<<(int)Mpixel(input2.get_gray_img(),p_center_of_object.x,p_center_of_object.y-1)<<endl;

	// 	// ROI_thresholded=ROI_gray.clone();
	// 	ROI_thresholded=samp_output.clone();
	// 	// thresholding_image(ROI_thresholded, (int)pixel_value/total,true,0);
	// 	// cv::threshold(ROI_gray, ROI_thresholded, 0, 255, CV_THRESH_BINARY_INV | CV_THRESH_TRIANGLE);
	// 	thresholding_image(ROI_thresholded, 50,true,0);
	// 	median_filter(ROI_thresholded,ROI_thresholded,5);

	// 	Point ROI_mid_p;

	// 	ROI_mid_p.x=ROI_gray.rows/2;
	// 	ROI_mid_p.y=ROI_gray.cols/2;

	// 	std::vector<vector<Point>>contours;

	// 	// findContours(ROI_gray,contours,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_NONE);
	// 	findContours(ROI_thresholded,contours,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_NONE);

	// 	int object_i = Find_The_Object_Contour(contours,ROI_mid_p);
	// 	// cout<<"p4"<<endl;
	// 	// cout<<"1"<<endl;
	// 	drawing=Mat::zeros(ROI_gray.size(),CV_8UC3);
	// 	Scalar color=CV_RGB(255,0,0);
	// 	drawContours(ROI,contours,object_i,color,2,8);
	// 	is_initialize_success=true;


	// 	// Mat ROI_thresholding=Cropping_ROI(temp_output,center_of_object,200);

	// }
	bool get_initalization_result(){return is_initialize_success;}
	Mat* get_ROI_img(){return ROI;}
	// Mat get_temp_output1(){return temp_output1;}
	// Mat get_temp_output2(){return temp_output2;}
	// Mat get_temp_output(){return temp_output;}
	Mat get_drawing(){return drawing;}
	Mat get_blob(){return blob_window;}
	Mat get_temp_output_img(){return temp_output;}
	Mat get_thresholded_img(){return ROI_thresholded;}
	Mat get_merged_samp_output(){return merged_samp_output;}

	// Mat get_ROI1(){return samp_output1;}
	// Mat get_ROI2(){return samp_output2;}
	// Mat get_ROI3(){return samp_output3;}
	// Mat get_ROI4(){return samp_output4;}
	Mat* get_samp_output(){return samp_output;}
	
};













class Kuhawara_ROI
{

private:
	bool is_initialize_success;
	Kuhawara source1;
	Kuhawara source2;
	Kuhawara source3;

	Mat temp_output1;
	Mat temp_output2;
	Mat temp_output;

	Mat test;
	
	Mat ROI;
	Mat ROI1;
	Mat ROI2;
	Mat ROI3;
	Mat ROI4;
	Mat ROI5;

	Mat samp_output1;
	Mat samp_output2;
	Mat samp_output3;
	Mat samp_output4;
	Mat samp_output;


	Mat ROI_gray;
	Mat ROI_thresholded;
	Mat blob_window;
	Mat drawing;


	Point *p_start_roi_window;
	Point *p_end_roi_window;
	Point p_center_of_object;

	double pixel_value;

	int p_center;



public:
	Kuhawara_ROI(){

	}
	// Kuhawara_ROI(Kuhawara input1, Kuhawara input2, Kuhawara input3, Kuhawara input4, Kuhawara input5){
	// 	// source2=input2;

	// 	temp_output1=input1.get_kuhawara_img()-input2.get_kuhawara_img();
	// 	temp_output2=input3.get_kuhawara_img()-input2.get_kuhawara_img();
	// 	temp_output=temp_output1+temp_output2;

	// 	thresholding_image(temp_output, 50,true,0);
	// 	// cv::threshold(temp_output3, temp_output, 0, 255, CV_THRESH_BINARY_INV | CV_THRESH_OTSU);
	// 	median_filter(temp_output,temp_output,5);

		


	// 	p_start_roi_window=new Point[200];//approx numb
	// 	p_end_roi_window=new Point[200];//approx numb

		

	// 	blob_window=Mat::zeros(input2.get_ori_size_img(),CV_8UC3);

	// 	int count_numb=blob(temp_output,blob_window,p_start_roi_window,p_end_roi_window);

	// 	if(count_numb==-100){
	// 		cout<<"Skip: too much blob"<<endl;
	// 		is_initialize_success=false;
	// 		return;
	// 	}//segmental fault

		
	// 	p_center_of_object=draw_rect_box(input2.get_original_img(), p_start_roi_window, p_end_roi_window, 200);
		


	// 	// test=input2.get_original_img();
	// 	// circle(test,p_center_of_object,3,Scalar(0,0,255));
	// 	ROI=Cropping_ROI(input2.get_original_img(),p_center_of_object,200);

	// 	ROI1=Cropping_ROI(input1.get_original_img(),p_center_of_object,200);
	// 	ROI2=Cropping_ROI(input2.get_original_img(),p_center_of_object,200);
	// 	ROI3=Cropping_ROI(input3.get_original_img(),p_center_of_object,200);

	// 	ROI4=Cropping_ROI(input4.get_original_img(),p_center_of_object,200);
	// 	ROI5=Cropping_ROI(input5.get_original_img(),p_center_of_object,200);

	// 	Kuhawara samp1(ROI1);
	// 	Kuhawara samp2(ROI2);
	// 	Kuhawara samp3(ROI3);
	// 	Kuhawara samp4(ROI4);
	// 	Kuhawara samp5(ROI5);

	// 	cout<<"ori size: "<<samp1.get_ori_size_img()<<endl;
	// 	cout<<"ku size: "<<samp1.get_ku_size_img()<<endl;

	// 	samp_output1=samp1.get_kuhawara_img()-samp2.get_kuhawara_img();
	// 	samp_output2=samp3.get_kuhawara_img()-samp2.get_kuhawara_img();
	// 	samp_output3=samp4.get_kuhawara_img()-samp2.get_kuhawara_img();
	// 	samp_output4=samp5.get_kuhawara_img()-samp2.get_kuhawara_img();
		

	// 	samp_output=samp_output1+samp_output2+samp_output3+samp_output4;

	// 	ROI=ROI2;

	// 	// cvtColor(ROI, ROI_gray, CV_BGR2GRAY);
	// 	// cvtColor(ROI1, ROI_gray1, CV_BGR2GRAY);
	// 	// cvtColor(ROI2, ROI_gray2, CV_BGR2GRAY);
	// 	// cvtColor(ROI3, ROI_gray3, CV_BGR2GRAY);








	// 	// int total=3;
	// 	// pixel_value=0;
		
	// 	// pixel_value=pixel_value+(int)Mpixel(input2.get_gray_img(),p_center_of_object.x,p_center_of_object.y);
	// 	// pixel_value=pixel_value+(int)Mpixel(input2.get_gray_img(),p_center_of_object.x+1,p_center_of_object.y);
	// 	// pixel_value=pixel_value+(int)Mpixel(input2.get_gray_img(),p_center_of_object.x-1,p_center_of_object.y);
		
	// 	// cout<<"1: "<<(int)Mpixel(input2.get_gray_img(),p_center_of_object.x,p_center_of_object.y)<<endl;
	// 	// cout<<"2: "<<(int)Mpixel(input2.get_gray_img(),p_center_of_object.x,p_center_of_object.y+1)<<endl;
	// 	// cout<<"3: "<<(int)Mpixel(input2.get_gray_img(),p_center_of_object.x,p_center_of_object.y-1)<<endl;

	// 	// ROI_thresholded=ROI_gray.clone();
	// 	ROI_thresholded=samp_output.clone();
	// 	// thresholding_image(ROI_thresholded, (int)pixel_value/total,true,0);
	// 	// cv::threshold(ROI_gray, ROI_thresholded, 0, 255, CV_THRESH_BINARY_INV | CV_THRESH_TRIANGLE);
	// 	thresholding_image(ROI_thresholded, 60,true,0);
	// 	median_filter(ROI_thresholded,ROI_thresholded,5);

	// 	Point ROI_mid_p;

	// 	ROI_mid_p.x=ROI_gray.rows/2;
	// 	ROI_mid_p.y=ROI_gray.cols/2;

	// 	std::vector<vector<Point>>contours;

	// 	// findContours(ROI_gray,contours,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_NONE);
	// 	findContours(ROI_thresholded,contours,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_NONE);

	// 	int object_i = Find_The_Object_Contour(contours,ROI_mid_p);
	// 	// cout<<"p4"<<endl;
	// 	// cout<<"1"<<endl;
	// 	drawing=Mat::zeros(ROI_gray.size(),CV_8UC3);
	// 	Scalar color=CV_RGB(255,0,0);
	// 	drawContours(ROI,contours,object_i,color,2,8);
	// 	is_initialize_success=true;


	// 	// Mat ROI_thresholding=Cropping_ROI(temp_output,center_of_object,200);

	// }

	Kuhawara_ROI(Kuhawara input1, Kuhawara input2, Kuhawara input3){
		// source2=input2;

		temp_output1=input1.get_kuhawara_img()-input2.get_kuhawara_img();
		temp_output2=input3.get_kuhawara_img()-input2.get_kuhawara_img();
		temp_output=temp_output1+temp_output2;

		thresholding_image(temp_output, 50,true,0);
		// cv::threshold(temp_output3, temp_output, 0, 255, CV_THRESH_BINARY_INV | CV_THRESH_OTSU);
		median_filter(temp_output,temp_output,5);

		


		p_start_roi_window=new Point[200];//approx numb
		p_end_roi_window=new Point[200];//approx numb

		

		blob_window=Mat::zeros(input2.get_ori_size_img(),CV_8UC3);

		int count_numb=blob(temp_output,blob_window,p_start_roi_window,p_end_roi_window);

		if(count_numb==-100){
			cout<<"Skip: too much blob"<<endl;
			is_initialize_success=false;
			return;
		}//segmental fault

		
		p_center_of_object=draw_rect_box(input2.get_original_img(), p_start_roi_window, p_end_roi_window, 200);
		


		// test=input2.get_original_img();
		// circle(test,p_center_of_object,3,Scalar(0,0,255));
		ROI=Cropping_ROI(input2.get_original_img(),p_center_of_object,200);

		ROI1=Cropping_ROI(input1.get_original_img(),p_center_of_object,200);
		ROI2=Cropping_ROI(input2.get_original_img(),p_center_of_object,200);
		ROI3=Cropping_ROI(input3.get_original_img(),p_center_of_object,200);

		// Kuhawara samp1(ROI1);
		// Kuhawara samp2(ROI2);
		// Kuhawara samp3(ROI3);

		Kuhawara samp1;
		Kuhawara samp2;
		Kuhawara samp3;


		samp1.main(ROI1);
		samp2.main(ROI2);
		samp3.main(ROI3);

		cout<<"ori size: "<<samp1.get_ori_size_img()<<endl;
		cout<<"ku size: "<<samp1.get_ku_size_img()<<endl;

		Mat samp_output1=samp1.get_kuhawara_img()-samp2.get_gray_img();
		Mat samp_output2=samp3.get_gray_img()-samp2.get_kuhawara_img();
		Mat samp_output=samp_output1+samp_output2;

		ROI=ROI2;

		// cvtColor(ROI, ROI_gray, CV_BGR2GRAY);
		// cvtColor(ROI1, ROI_gray1, CV_BGR2GRAY);
		// cvtColor(ROI2, ROI_gray2, CV_BGR2GRAY);
		// cvtColor(ROI3, ROI_gray3, CV_BGR2GRAY);








		// int total=3;
		// pixel_value=0;
		
		// pixel_value=pixel_value+(int)Mpixel(input2.get_gray_img(),p_center_of_object.x,p_center_of_object.y);
		// pixel_value=pixel_value+(int)Mpixel(input2.get_gray_img(),p_center_of_object.x+1,p_center_of_object.y);
		// pixel_value=pixel_value+(int)Mpixel(input2.get_gray_img(),p_center_of_object.x-1,p_center_of_object.y);
		
		// cout<<"1: "<<(int)Mpixel(input2.get_gray_img(),p_center_of_object.x,p_center_of_object.y)<<endl;
		// cout<<"2: "<<(int)Mpixel(input2.get_gray_img(),p_center_of_object.x,p_center_of_object.y+1)<<endl;
		// cout<<"3: "<<(int)Mpixel(input2.get_gray_img(),p_center_of_object.x,p_center_of_object.y-1)<<endl;

		// ROI_thresholded=ROI_gray.clone();
		ROI_thresholded=samp_output.clone();
		// thresholding_image(ROI_thresholded, (int)pixel_value/total,true,0);
		// cv::threshold(ROI_gray, ROI_thresholded, 0, 255, CV_THRESH_BINARY_INV | CV_THRESH_TRIANGLE);
		thresholding_image(ROI_thresholded, 50,true,0);
		median_filter(ROI_thresholded,ROI_thresholded,5);

		Point ROI_mid_p;

		ROI_mid_p.x=ROI_gray.rows/2;
		ROI_mid_p.y=ROI_gray.cols/2;

		std::vector<vector<Point>>contours;

		// findContours(ROI_gray,contours,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_NONE);
		findContours(ROI_thresholded,contours,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_NONE);

		int object_i = Find_The_Object_Contour(contours,ROI_mid_p);
		// cout<<"p4"<<endl;
		// cout<<"1"<<endl;
		drawing=Mat::zeros(ROI_gray.size(),CV_8UC3);
		Scalar color=CV_RGB(255,0,0);
		drawContours(ROI,contours,object_i,color,2,8);
		is_initialize_success=true;


		// Mat ROI_thresholding=Cropping_ROI(temp_output,center_of_object,200);

	}
	bool get_initalization_result(){return is_initialize_success;}
	Mat get_ROI_img(){return ROI;}
	Mat get_temp_output1(){return temp_output1;}
	Mat get_temp_output2(){return temp_output2;}
	Mat get_temp_output(){return temp_output;}
	Mat get_drawing(){return drawing;}
	Mat get_blob(){return blob_window;}
	Mat get_test(){return test;}
	Mat get_temp_output_img(){return temp_output;}
	Mat get_thresholded_img(){return ROI_thresholded;}

	Mat get_ROI1(){return samp_output1;}
	Mat get_ROI2(){return samp_output2;}
	Mat get_ROI3(){return samp_output3;}
	Mat get_ROI4(){return samp_output4;}
	Mat get_ROI5(){return samp_output;}
	
};



#endif