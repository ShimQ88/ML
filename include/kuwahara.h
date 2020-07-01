#ifndef KUWAHARA
#define KUWAHARA

#include <sys/types.h> 
#include <sys/stat.h>
#include <unistd.h>
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

int determine_frame_size(Mat input, Point obj_center, int frame_size, int append_size ){
	bool is_tri=false;
	// cout<<"input.size(): "<<input.size()<<endl;
	// cout<<"obj_center.x: "<<obj_center.x<<endl;
	// cout<<"obj_center.y: "<<obj_center.y<<endl;

 	if( (obj_center.x-frame_size < 0) || (obj_center.y-frame_size < 0 ) ){//the case1 of out of frame boundary
 		// cout<<"return case1"<<endl;
 		return -1;
	}

	if( (obj_center.x+frame_size > input.cols) || (obj_center.y+frame_size > input.rows) ){//the case2 of out of frame boundary
		// cout<<"return case2"<<endl;
 		return -2;
	}

	while(true){
		if( (obj_center.x-frame_size<=0)||(obj_center.y-frame_size<=0) ){//The frame size bigger than the left and top side margin so finish expanding more frame size
			// cout<<"return case3"<<endl;
			return frame_size*2;
		}

		if( (obj_center.x+frame_size >= input.cols)||(obj_center.y+frame_size >= input.rows) ){//The frame size bigger than the left and top side margin so finish expanding more frame size
			// cout<<"return case4"<<endl;
			return frame_size*2;
		}

		for(int k=obj_center.x-frame_size;k<obj_center.x+frame_size;k++){			
			for(int l=obj_center.y-frame_size;l<obj_center.y+frame_size;l++){

				// cout<<"k: "<<k<<endl;
				// cout<<"l: "<<l<<endl;
				// cout<<"obj_center.x: "<<obj_center.x<<endl;
				// cout<<"obj_center.y: "<<obj_center.y<<endl;
				if( (k==obj_center.x-frame_size)||(k==obj_center.x+frame_size-1)||(l==obj_center.y-frame_size)||(l==obj_center.y+frame_size-1) ){
					// Point t_pp;
					// t_pp.x=k;
					// t_pp.y=l;
					// circle(subtracted_frame,t_pp,3,Scalar(255,255,255));
					if((int)Mpixel(input,k,l)!=0 ){
						is_tri=true;
					}
				}


			}	
		}

		
		
		frame_size=frame_size+append_size;

		// cout<<"frame_size: "<<frame_size<<endl;
		if(is_tri==false){
			// cout<<"normal case: "<<endl;
			// cout<<"frame_size: "<<frame_size*2<<endl;
			break;
		}
		is_tri=false;

		if(frame_size>=250){//maximum size
			return frame_size*2;
			cout<<"error for size"<<endl;
		}
	}
	return frame_size*2;
}


bool find_contour_and_name_file(glob_t glob_sub_result, string *path_contour, string *path_name){
	bool is_find_contour_path=false;
	bool is_find_name_path=false;

	for(unsigned int z=0; z<glob_sub_result.gl_pathc; z++){
		int delimiter=0;
		string file_name=glob_sub_result.gl_pathv[z];
		/*process extract the file name*/
		while(delimiter!=-1){
	  		delimiter = file_name.find('/');
	  		file_name = file_name.substr(delimiter+1);
	  	}
	  	/*********************************/
		delimiter = file_name.find('.');
		string type= file_name.substr(delimiter+1);
		if(type.compare("txt")==0){
			delimiter = file_name.find('_');
			string test=file_name.substr(0,delimiter);
			if(test.compare("contour")==0){
				*path_contour=glob_sub_result.gl_pathv[z];
				is_find_contour_path=true;
			
			}else if(test.compare("name")==0){
				*path_name=glob_sub_result.gl_pathv[z];
				is_find_name_path=true;
			}
		}
	}

	if(is_find_contour_path==true&&is_find_name_path==true){
		return true;//find both of path
	}else{
		return false;//couldn't find both of path
	}
}


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
    	printf("Fail to create %s Directory or the folder %s exist\n ", dirname, dirname);
        return false;
    }
}


string write_img(Mat saving_img, string folder_name, string file_name, string file_type){
  	int delimiter=0;
  	while(delimiter!=-1){
  		delimiter = file_name.find('/');
  		file_name = file_name.substr(delimiter+1);
  	}
  	file_name = file_name.substr(0,file_name.find('.'));
  	cout<<"file_name: "<<file_name<<endl;

  	string output_name=file_name+file_type;
	string path_ROI=folder_name+file_name+file_type;

	imwrite( path_ROI, saving_img);
	return output_name;
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

int run_filtering(int argc,char *argv[]);

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
	Mat target_ROI;
	Mat ROI_and_drawing;
	string contour_txt;
	

	Mat ROI_thresholded;
	Mat blob_window;
	Mat drawing;
	Mat *some_temp_output;

	Mat subtracted_frame;

	Point *p_start_roi_window;
	Point *p_end_roi_window;
	Point p_center_of_object;

	double pixel_value;

	// int p_center;
	int total_numb;
	
	


public:
	Kuhawara_ROI2(){
		ROI_and_drawing=Mat::zeros(cv::Size(50,50),CV_8UC3);
		target_ROI=Mat::zeros(cv::Size(50,50),CV_8UC3);

	}
	void main(Kuhawara *image, int total, int target_index){
		
		total_numb=total;//total size storing
		
		subtracted_frame=sub_prev_and_next_images(image, target_index);

		// thresholding_image(subtracted_frame, 50,true,0);
		cv::threshold(subtracted_frame, subtracted_frame, 0, 255, CV_THRESH_BINARY | THRESH_OTSU);
		median_filter(subtracted_frame,subtracted_frame,7);

		

		p_start_roi_window=new Point[200];//approx numb
		p_end_roi_window=new Point[200];//approx numb

		blob_window=Mat::zeros(image[target_index].get_ori_size_img(),CV_8UC3);//default size initializing

		// cout<<"THis is point 2"<<endl;
		int count_numb=blob(subtracted_frame, blob_window, p_start_roi_window, p_end_roi_window);
		
		// cout<<"THis is point 2-1"<<endl;
		if(count_numb==-100){
			cout<<"Skip: too much blob"<<endl;
			is_initialize_success=false;
			return;
		}//segmental fault

		// cout<<"This is point 2-2"<<endl;
		cout<<"pp1"<<endl;
		p_center_of_object=draw_rect_box(image[target_index].get_original_img(), p_start_roi_window, p_end_roi_window, 200);
		cout<<"pp2"<<endl;

		
		/*process cropping image*/
		int cropping_size=determine_frame_size(subtracted_frame,p_center_of_object,20,20);
		/*error section*/
		//error==-1 obj_center.x-frame_size or obj_center.y-frame_size is less than the minimum size of frame.
		//( min=0 )
		//error==-2 obj_center.x+frame_size or obj_center.y+frame_size is bigger than the maximum size of frame.
		//(max= input.cols or input.rows)
		if(cropping_size==-1||cropping_size==-2){
			cout<<"sizeing error"<<endl;
			is_initialize_success=false;
			return;
		}
		/*process cropping image*/
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
		target_ROI=ROI[target_index];
		ROI_and_drawing=target_ROI.clone();

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
			return;
		}else{
			// // cout<<"p4"<<endl;
			// // cout<<"1"<<endl;
			drawing=Mat::zeros(merged_samp_output.size(),CV_8UC3);
			Scalar color=CV_RGB(255,0,0);
			drawContours(drawing,contours,object_i,color,2,8);
			drawContours(ROI_and_drawing,contours,object_i,color,2,8);
			drawContours(merged_samp_output,contours,object_i,color,2,8);

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
					cout<<"enter"<<endl;

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
	Mat* get_ROI_img(){return ROI;}
	Mat get_target_ROI_img(){return target_ROI;}
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

	// int detemine_frame_size(Mat input, int s_size, int append_size ){
	// 	bool is_tri=false;
	// 	//size process
	// 	// imshow("f",input);
	// 	// int key=waitkey(0);
	// 	cout<<"input.size(): "<<input.size()<<endl;
	// 	// cout<<"input.cols: "<<input.cols<<endl;
	// 	// cout<<"input.rows: "<<input.rows<<endl;
	// 	cout<<"p_center_of_object.x: "<<p_center_of_object.x<<endl;
	// 	cout<<"p_center_of_object.y: "<<p_center_of_object.y<<endl;
	// 	// getchar();
	// 	while(true){
	// 		for(int k=p_center_of_object.x-s_size;k<p_center_of_object.x+s_size;k++){
	// 			for(int l=p_center_of_object.y-s_size;l<p_center_of_object.y+s_size;l++){
	// 				// cout<<"k: "<<k<<endl;
	// 				// cout<<"l: "<<l<<endl;
	// 				// cout<<"p_center_of_object.x: "<<p_center_of_object.x<<endl;
	// 				// cout<<"p_center_of_object.y: "<<p_center_of_object.y<<endl;
	// 				if( (k==p_center_of_object.x-s_size)||(k==p_center_of_object.x+s_size-1)||(l==p_center_of_object.y-s_size)||(l==p_center_of_object.y+s_size-1) ){
	// 					Point t_pp;
	// 					t_pp.x=k;
	// 					t_pp.y=l;
	// 					if((k>=input.cols-s_size*2)||(l>=input.rows-s_size*2)||(k<0)||(l<0)){
	// 						is_tri=true;
	// 						break;
	// 					}
	// 					// circle(temp_output,t_pp,3,Scalar(255,255,255));
	// 					if((int)Mpixel(input,k,l)!=0 ){
	// 						is_tri=true;
	// 					}
	// 				}

	// 			}			
	// 		}
			
	// 		s_size=s_size+append_size;
	// 		cout<<"s_size: "<<s_size<<endl;
	// 		if(is_tri==false){
	// 			break;
	// 		}
	// 		is_tri=false;
	// 		if(s_size>250){
	// 			return s_size*2;
	// 			cout<<"error for size"<<endl;
	// 		}
	// 	}
	// 	return s_size*2;
	// }
	Mat get_blob(){return blob_window;}
	string get_contour_txt(){return contour_txt;}
	Mat get_ROI_and_drawing(){return ROI_and_drawing;}
	Mat get_temp_output_img(){return subtracted_frame;}
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