#ifndef KUWAHARA
#define KUWAHARA

#include <sys/types.h> 
#include <sys/stat.h>
#include <unistd.h>
// #include <stdlib.h> 
#include <stdlib.h>

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

bool creating_folder(string folder_name){
	int check;
	int size = folder_name.length(); 
	
    char dirname[size+1];
    strcpy(dirname, folder_name.c_str()); 
  
    check = mkdir(dirname,0777); 
  
    // check if directory is created or not 
    if(!check){ 
        printf("The folder %s Directory created\n", dirname);
        return true;
    }else{
    	printf("Fail to create %s Directory\n", dirname);
        return false;
    }
}


void write_img(Mat saving_img, string folder_name, string file_name, string file_type){
  	int delimiter=0;
  	while(delimiter!=-1){
  		delimiter = file_name.find('/');
  		file_name = file_name.substr(delimiter+1);
  	}
  	file_name = file_name.substr(0,file_name.find('.'));
  	cout<<"file_name: "<<file_name<<endl;

	string path_ROI=folder_name+file_name+file_type;

	imwrite( path_ROI, saving_img);
}


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
	Mat *ROI;
	Mat *samp_output;
	Mat merged_samp_output;
	Mat merged_samp_output2;
	Mat main_ROI;
	Mat ROI_and_drawing;
	string contour_txt;
	

	Mat ROI_thresholded;
	Mat blob_window;
	Mat drawing;
	Mat *some_temp_output;

	Mat temp_output;

	Point *p_start_roi_window;
	Point *p_end_roi_window;
	Point p_center_of_object;

	double pixel_value;

	// int p_center;
	int total_numb;
	
	


public:
	Kuhawara_ROI2(){
		ROI_and_drawing=Mat::zeros(cv::Size(50,50),CV_8UC3);
		main_ROI=Mat::zeros(cv::Size(50,50),CV_8UC3);

	}
	void main(Kuhawara *image, int total, int target_index){
		
		total_numb=total;//total size storing

		temp_output=sub_prev_and_next_images(image, target_index);

		// thresholding_image(temp_output, 50,true,0);
		cv::threshold(temp_output, temp_output, 0, 255, CV_THRESH_BINARY | THRESH_OTSU);
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

		// cout<<"This is point 2-2"<<endl;
		p_center_of_object=draw_rect_box(image[target_index].get_original_img(), p_start_roi_window, p_end_roi_window, 200);

		int cropping_size=detemine_frame_size(temp_output,20,20);
		cout<<"framesize: "<<cropping_size<<endl;


		Kuhawara temp_ku[total_numb];
		samp_output = new Mat[total_numb-1];
		ROI=new Mat[total_numb];

		for(int i=0;i<total_numb;i++){
			ROI[i]=Cropping_ROI(image[i].get_original_img(),p_center_of_object,cropping_size);
			temp_ku[i].main(ROI[i]);
		}
		// cout<<"THis is point 4"<<endl;
		int temp_i=0;
		main_ROI=ROI[target_index];
		ROI_and_drawing=main_ROI.clone();

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

		
		merged_samp_output2=merged_samp_output.clone();
		// thresholding_image(ROI_thresholded, (int)pixel_value/total,true,0);
		cv::threshold(merged_samp_output, merged_samp_output, 0, 255, CV_THRESH_BINARY | THRESH_OTSU);
		// cv::threshold(merged_samp_output, merged_samp_output, 0, 255, THRESH_TOZERO );


		// thresholding_image(merged_samp_output, 45,true,0);
		median_filter(merged_samp_output,merged_samp_output,9);

		Point ROI_mid_p;

		ROI_mid_p.x=merged_samp_output.rows/2;
		ROI_mid_p.y=merged_samp_output.cols/2;

		std::vector<vector<Point>>contours;

		findContours(merged_samp_output,contours,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_NONE);

		int object_i = Find_The_Object_Contour(contours,ROI_mid_p);
		if(object_i==-1){
			is_initialize_success=false;
			cout<<"contour fail"<<endl;
			contour_txt="";
		}else{
			// // cout<<"p4"<<endl;
			// // cout<<"1"<<endl;
			drawing=Mat::zeros(merged_samp_output.size(),CV_8UC3);
			Scalar color=CV_RGB(255,0,0);
			drawContours(drawing,contours,object_i,color,2,8);
			drawContours(ROI_and_drawing,contours,object_i,color,2,8);

			vector<float>CE;
			cout<<"object_i: "<<object_i<<endl;
			
			EllipticFourierDescriptors(contours[object_i],CE);
			contour_txt="0, ";
			// strcat(contour_txt, "0, ");
			// to_string(CE[i])
			for(int i=1;i<10;i++){
				contour_txt=contour_txt+to_string(CE[i]);
				// strcat(contour_txt ,st );
				if(i==9){
					contour_txt=contour_txt+"\n";
					// strcat(contour_txt, "\n");
				}else{
					contour_txt=contour_txt+",";
					// strcat(contour_txt, ",");
				}
		    	
			}
			
			


			is_initialize_success=true;
			cout<<"success initializing"<<endl;
		}
		

	}
	
	bool get_initalization_result(){return is_initialize_success;}
	// int get_object_i(){return object_i;}
	Mat* get_ROI_img(){return ROI;}
	Mat get_main_ROI_img(){return main_ROI;}
	// Mat get_temp_output1(){return temp_output1;}
	// Mat get_temp_output2(){return temp_output2;}
	// Mat get_temp_output(){return temp_output;}
	Mat get_drawing(){return drawing;}
	Mat sub_prev_and_next_images(Kuhawara *image, int target){
		
		Mat final_output;
		if(target==0){
			final_output = image[target+1].get_kuhawara_img() - image[target].get_kuhawara_img();
		}else if(target==total_numb-1){
			final_output = image[target-1].get_kuhawara_img() - image[target].get_kuhawara_img();
		}else{
			Mat* t_output=new Mat[2];
			t_output[0] = image[target-1].get_kuhawara_img() - image[target].get_kuhawara_img();
			t_output[1] = image[target+1].get_kuhawara_img() - image[target].get_kuhawara_img();
			final_output = t_output[0]+t_output[1];	
		}

		return final_output;
	}
	int detemine_frame_size(Mat input, int s_size, int append_size ){
		bool is_tri=false;
		//size process
		while(true){
			for(int k=p_center_of_object.x-s_size;k<p_center_of_object.x+s_size;k++){
				for(int l=p_center_of_object.y-s_size;l<p_center_of_object.y+s_size;l++){
					if( (k==p_center_of_object.x-s_size)||(k==p_center_of_object.x+s_size-1)||(l==p_center_of_object.y-s_size)||(l==p_center_of_object.y+s_size-1) ){
						Point t_pp;
						t_pp.x=k;
						t_pp.y=l;
						// circle(temp_output,t_pp,3,Scalar(255,255,255));
						if((int)Mpixel(input,k,l)!=0 ){
							is_tri=true;
						}
					}

				}			
			}
			s_size=s_size+append_size;
			if(is_tri==false){
				break;
			}
			is_tri=false;
			if(s_size>250){
				return s_size*2;
				cout<<"error for size"<<endl;
			}
		}
		return s_size*2;
	}
	Mat get_blob(){return blob_window;}
	string get_contour_txt(){return contour_txt;}
	Mat get_ROI_and_drawing(){return ROI_and_drawing;}
	Mat get_temp_output_img(){return temp_output;}
	Mat get_thresholded_img(){return ROI_thresholded;}
	Mat get_merged_samp_output(){return merged_samp_output;}
	Mat get_merged_samp_output2(){return merged_samp_output2;}
	Mat* get_samp_output(){return samp_output;}
	int get_pixel(int target){
		Mat *temp_ROI=get_samp_output();
		Mat target_ROI=temp_ROI[target];
		int to=-999;
		for(int i=0;i<target_ROI.cols;i++){
			for(int j=0;j<target_ROI.rows;j++){
				if( (int)Mpixel(target_ROI,i,j)>to ){
					to=(int)Mpixel(target_ROI,i,j);
				}
			}
		}
		return to;
	}
	
};

#endif