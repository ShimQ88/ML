#include "kuwahara.h"
#include "contour.h"

const int max_value_H = 360/2;
const int max_value = 255;
const String window_capture_name = "Video Capture";
const String window_detection_name = "Object Detection";
int low_H = 0, low_S = 0, low_V = 0;
int high_H = max_value_H, high_S = max_value, high_V = max_value;

static void on_low_H_thresh_trackbar(int, void *)
{
    low_H = min(high_H-1, low_H);
    setTrackbarPos("Low H", window_detection_name, low_H);
}
static void on_high_H_thresh_trackbar(int, void *)
{
    high_H = max(high_H, low_H+1);
    setTrackbarPos("High H", window_detection_name, high_H);
}
static void on_low_S_thresh_trackbar(int, void *)
{
    low_S = min(high_S-1, low_S);
    setTrackbarPos("Low S", window_detection_name, low_S);
}
static void on_high_S_thresh_trackbar(int, void *)
{
    high_S = max(high_S, low_S+1);
    setTrackbarPos("High S", window_detection_name, high_S);
}
static void on_low_V_thresh_trackbar(int, void *)
{
    low_V = min(high_V-1, low_V);
    setTrackbarPos("Low V", window_detection_name, low_V);
}
static void on_high_V_thresh_trackbar(int, void *)
{
    high_V = max(high_V, low_V+1);
    setTrackbarPos("High V", window_detection_name, high_V);
}

int threshold_value = 0;
int threshold_type = 3;
// int const max_value = 255;
int const max_type = 4;
int const max_binary_value = 255;
Mat src, src_gray, dst;
const char* window_name = "Threshold Demo";
const char* trackbar_type = "Type: \n 0: Binary \n 1: Binary Inverted \n 2: Truncate \n 3: To Zero \n 4: To Zero Inverted";
const char* trackbar_value = "Value";


static void Threshold_Demo( int, void* )
{
    /* 0: Binary
     1: Binary Inverted
     2: Threshold Truncated
     3: Threshold to Zero
     4: Threshold to Zero Inverted
    */
    threshold( src_gray, dst, threshold_value, max_binary_value, threshold_type );
    imshow( window_name, dst );
}

Mat Cropping_ROI(Mat imput_image,Point center_of_object, int kernel_size){
	Point temp_p;
	temp_p.x=center_of_object.x-(kernel_size/2);
	temp_p.y=center_of_object.y-(kernel_size/2);
	

	if(temp_p.x<0){
		temp_p.x=0;
	}

	if(temp_p.y<0){
		temp_p.y=0;	
	}

	if(temp_p.x+kernel_size>imput_image.cols){
		temp_p.x=imput_image.cols-kernel_size;
	}

	if(temp_p.y+kernel_size>imput_image.rows){
		temp_p.y=imput_image.rows-kernel_size;	
	}
	// cout<<"temp_p.x:"<<temp_p.x<<endl;
	// cout<<"temp_p.y:"<<temp_p.y<<endl;


	Mat ROI=imput_image(Rect(temp_p.x,temp_p.y,kernel_size,kernel_size)).clone();
	// Mat ROI=imput_image(Rect(abs(temp_p.x),abs(temp_p.y),abs(temp_p.y-center_of_object.y),abs(temp_p.y-center_of_object.y))).clone();
	return ROI;
}



bool check_numb_in_a_row(string prev_name, string cur_name){
	string str_prev_name=prev_name.substr(prev_name.size()-8,4);
	string str_cur_name=cur_name.substr(cur_name.size()-8,4);
	int prev_img_numb=stoi(str_prev_name);
	int cur_img_numb=stoi(str_cur_name);
	int diff_numb=cur_img_numb-prev_img_numb;
	
	if(diff_numb==1){
		return true;
	}else{
		return false;
	}

}



void thresholding_image(Mat image, int value, bool inverted, int window_size){
	if(inverted==true){
		for(int x=(window_size/2); x<image.cols-(window_size/2);x++){
			for(int y=(window_size/2); y<image.rows-(window_size/2);y++){
				if((Mpixel(image,x,y)>=value)){
					Mpixel(image,x,y)=255;
				}else{
					Mpixel(image,x,y)=0;
				}
			}
		}	
	}else{
		for(int x=(window_size/2); x<image.cols-(window_size/2);x++){
			for(int y=(window_size/2); y<image.rows-(window_size/2);y++){				
				if(Mpixel(image,x,y)>=value){
					Mpixel(image,x,y)=0;
				}else{
					Mpixel(image,x,y)=255;
				}
			}
		}
	}	
}


float binary_histogram(Mat image, int window_size, double histogram[]){
	for(int i=0;i<256;i++){
		histogram[i]=0;
	}

	// for(int x=( (window_size/2)+(image.cols*2/5) ); x<image.cols-( (window_size/2) +(image.cols*2/5));x++){
	// 	for(int y=((window_size/2) +(image.rows*2/5)); y<image.rows-( (window_size/2) +(image.rows*2/5) );y++){
	for(int x=(window_size/2); x<image.cols-(window_size/2);x++){
		for(int y=(window_size/2); y<image.rows-(window_size/2);y++){	
			int hist_val=(int)Mpixel(image,x,y);
			histogram[hist_val]++;
			// Point temp_p;
			// temp_p.x=x;
			// temp_p.y=y;
			// circle(image,temp_p,1,Scalar(255,255,255));
		}
	}	
	float total_sum=0;
	float total=0;
	int biggest=-9999;
	int big_i=-1;
	for(int i=0;i<100;i++){
		// if(i>0 && i<51){
			if(biggest<histogram[i]){
				biggest=histogram[i];
				big_i=i;
			}
		// }
		total_sum=total_sum+(histogram[i]*i);
		total=total+histogram[i];
		// cout<<i<<": "<<histogram[i]<<endl;
		// cout<<"total_sum: "<<total_sum<<endl;
	}
	// total_sum=total_sum/256;
	// return big_i;
	return total_sum/total;
}

Point draw_rect_box(Mat input_image, Point* p1, Point* p2, int loop_number){
	int big_blob_i = -1;
	int big_blob_len = -9999;
	for(int i=0;i<loop_number;i++){
		if((p1[i].x!=9999)&&(p1[i].y!=9999)&&(p2[i].x!=-9999)&&(p2[i].y!=-9999)){
			int temp_value = abs( (p2[i].x-p1[i].x)*(p2[i].y-p1[i].y) );
			if(big_blob_len < temp_value){
				big_blob_len= temp_value;
				big_blob_i = i;
			}
		}
	}
	Rect rec(p1[big_blob_i].x-2, p1[big_blob_i].y-2, p2[big_blob_i].x-p1[big_blob_i].x+2, p2[big_blob_i].y-p1[big_blob_i].y+2);
	
	Point center_of_rect = (rec.br() + rec.tl())*0.5;

    // circle(input_image,center_of_rect,3,Scalar(0,0,255));
	// rectangle(input_image,rec,Scalar(0,255,0),2);

	return center_of_rect;
}



int blob(Mat image2, Mat image3,Point *p1_rec, Point *p2_rec){

	int counterColour=0;

	vector< tuple<int, int> > SET[image2.cols];
	
	int A[image2.cols][image2.rows];

	
	int s1,s2;

	for(int x=0; x<image2.cols;x++){
		for(int y=0; y<image2.rows;y++){
			A[x][y]=-1;
		}
	}
	// cout<<"P1"<<endl;
	int i=0;
	int index=0, counter=-1;
	bool is_s1_in=false;
	// for(int y=1; y<image2.rows;y++){
	for(int y=image2.rows/50; y<image2.rows;y++){//Ignoring the numbers from top section
		for(int x=1; x<image2.cols;x++){
			// cout<<"counter: "<<counter<<endl;
			if(Mpixel(image2,x,y)!=0){
				if((Mpixel(image2,x-1,y)!=0)||(Mpixel(image2,x,y-1)!=0)){
					s1=A[x-1][y];
					s2=A[x][y-1];
					if(s1!=-1){
						SET[s1].push_back(make_tuple(x,y));
						
						A[x][y]=s1;
						is_s1_in=true;
					}
					if(s2!=-1){
						SET[s2].push_back(make_tuple(x,y));
						A[x][y]=s2;
						is_s1_in=false;

					}
					if((s1!=s2)&&(s1!=-1)&&(s2!=-1)){
						SET[s1].insert(SET[s1].end(),SET[s2].begin(),SET[s2].end());						
						for (int j = 0; j < SET[s2].size(); j++ ){
							A[get<0>(SET[s2][j])][get<1>(SET[s2][j])]=s1;
						}				
						SET[s2].clear();
					}
					is_s1_in=false;

				}else{
					counter++;
					SET[counter].push_back(make_tuple(x,y));
					A[x][y]=counter;
				}

			}
			if(counter>150){return -100;}//This is using for avoiding segmental fault.
		}


	}

	// cout<<"P2"<<endl;
	//colour counting
	// for(int i=0;i<image2.cols;i++){
	// 	colour_count(SET[i]);
	// }

	//put random colour to image3
	
	counter++;
	
	

	// Point p1_rec[counter];
	// Point p2_rec[counter];

	// p1_rec= new Point[counter];
	// p2_rec= new Point[counter];

	for(int i=0;i<counter;i++){
		p1_rec[i].x=9999;
		p1_rec[i].y=9999;

		p2_rec[i].x=-9999;
		p2_rec[i].y=-9999;
	}
	
	// cout<<"P3"<<endl;

	int rand_numb1 = rand() % 100; 
	int rand_numb2 = rand() % 100; 
	int rand_numb3 = rand() % 100;

	for(int y=image2.rows/50; y<image2.rows;y++){//Ignoring the numbers from top section
		for(int x=0; x<image3.cols;x++){
			if(y==0){
				if(SET[x].empty()==true){

				}else{
					counterColour++;
				}
				// colour_count(SET[x]);
			}
			if(A[x][y]==-1){
				pixelB(image3,x,y)=0;
				pixelG(image3,x,y)=0;
				pixelR(image3,x,y)=0;	
			}
			if(A[x][y]!=-1){	
				pixelB(image3,x,y)=(A[x][y]+1)*rand_numb1;
				pixelG(image3,x,y)=(A[x][y]+1)*rand_numb2;
				pixelR(image3,x,y)=(A[x][y]+1)*rand_numb3;

				if(x < p1_rec[ (A[x][y]) ].x){
					p1_rec[ (A[x][y]) ].x=x;
				}

				if(y < p1_rec[ (A[x][y]) ].y){
					p1_rec[ (A[x][y]) ].y=y;
				}


				if(x > p2_rec[ (A[x][y]) ].x){
					p2_rec[ (A[x][y]) ].x=x;
				}

				if(y > p2_rec[ (A[x][y]) ].y){
					p2_rec[ (A[x][y]) ].y=y;
				}

			}
		}
	}
	draw_rect_box(image3, p1_rec, p2_rec, counter);
	// cout<<"P4"<<endl;
	// for(int i=0;i<counter;i++){
	// 	if((p1_rec[i].x!=9999)&&(p1_rec[i].y!=9999)&&(p2_rec[i].x!=-9999)&&(p2_rec[i].y!=-9999)){
	// 		Rect rec(p1_rec[i].x, p1_rec[i].y, p2_rec[i].x-p1_rec[i].x+1, p2_rec[i].y-p1_rec[i].y+1);
	// 		rectangle(image3,rec,Scalar(0,255,0),1);
	// 	}
	// }
	// getchar();
	// for(int i=0;i<20;i++){
	// 	cout<<p1_rec->x<<endl;
	// }


	// return counterColour;
	return counter;
}

void Grey_to_Color(Mat3b source_image, Mat filtered_image,Mat3b output_image){
	for(int x=0; x<source_image.cols;x++){
		for(int y=0; y<source_image.rows;y++){
			if(Mpixel(filtered_image,x,y)!=0){//when pixel is not zero
				pixelB(output_image,x,y)=pixelB(source_image,x,y);
				pixelG(output_image,x,y)=pixelG(source_image,x,y);
				pixelR(output_image,x,y)=pixelR(source_image,x,y);
			}
		}	
	}
}
void Determining_ROI_Size(Mat source_image, int *small_x, int *small_y,int *large_x,int *large_y){
	//This is function for finding coordinates (p1 and p2) of rectangle before cropping Region of Interest area

	// * p1(small_x,small_y)
	// @ p2(large_x,large_y)
	//  |*| | |        
	//	| | | |
	//	| | |@|


	*small_x=9999;
	*small_y=9999;
	*large_x=-9999;
	*large_y=-9999;
	for(int x=0; x<source_image.cols;x++){
		for(int y=0; y<source_image.rows;y++){
			if(Mpixel(source_image,x,y)!=0){//when pixel is not zero
				if(x>*large_x){
					*large_x=x;
				}
				if(y>*large_y){
					*large_y=y;
				}
				if(x<*small_x){
					*small_x=x;
				}
				if(y<*small_y){
					*small_y=y;
				}
			}
		}	
	}
	if((*small_x==9999)||(*small_y==9999)||(*large_x==-9999)||(*large_y==-9999)){
		*small_x=0;
		*small_y=0;
		*large_x=0;
		*large_y=0;
	}
}


void Image_stitching(Mat source_image,Mat filtered_image,Mat output_image){
	
	//vertical way
	int sm[source_image.cols];
	int lar[source_image.cols];
	for(int x=0; x<source_image.cols;x++){
		sm[x]=9999;
		lar[x]=-9999;
		for(int y=0; y<source_image.rows;y++){
			if(Mpixel(filtered_image,x,y)<35){//thresholding
											  //this is better to do seperate but I put on here for performancing
				Mpixel(filtered_image,x,y)=0;
			}

			if((int)Mpixel(filtered_image,x,y)!=0){//The process for detecting the beginning point of pixel and the last point of pixel of each column. 
				if(y<sm[x]){
					sm[x]=y;
				}
				if(y>lar[x]){
					lar[x]=y;
				}
			}
			if(x!=0){
				if( (y>=sm[x-1])&&(y<=lar[x-1])){//The vertical way copy all previous line pixels from the point of the first detected pixcel to the last.  
					Mpixel(output_image,x-1,y)=Mpixel(source_image,x-1,y);
				}
			}
			if( (x==source_image.cols-1)&&(y==source_image.rows-1) ){//The same process of upper process but the last one is exception.
				for(int a=sm[x]; a<lar[x];a++){
					Mpixel(output_image,x,a)=Mpixel(source_image,x,a);	
				}						
			}

		}
	}
	//horizontal way
	int sm2[source_image.rows];
	int lar2[source_image.rows];
	for(int x=0; x<source_image.rows;x++){
		sm2[x]=9999;
		lar2[x]=-9999;
		for(int y=0; y<source_image.cols;y++){
			if((int)Mpixel(output_image,y,x)!=0){//The process for detecting the beginning point of pixel and the last point of pixel of each row.
				if(y<sm2[x]){
					sm2[x]=y;
				}
				if(y>lar2[x]){
					lar2[x]=y;
				}
			}
			if(x!=0){
				if( (y>sm2[x-1])&&(y<lar2[x-1])  ){//The horizontal way copy all previous line pixels from the point of the first detected pixcel to the last.  
					Mpixel(output_image,y,x-1)=Mpixel(source_image,y,x-1);	
				}							
			}
			if( (x==source_image.rows-1)&&(y==source_image.cols-1) ){//the case of last
				for(int a=sm2[x]; a<lar2[x];a++){
					Mpixel(output_image,a,x)=Mpixel(source_image,a,x);	
				}						
			}
		}
	}
}
void median_filter(Mat image1,Mat image2,int window_size){
    //picture elements(pels)
    int function_size_input=window_size;//this is for window size

    int picture_x_size=image1.cols;
    int picture_y_size=image1.rows;
    int mdn;//median value in a window
    int ltmdn=0;// number of pels having gray levels less than mdn in a window
    int window_x_size=function_size_input;
    int window_y_size=function_size_input;
    //int hist[window_x_size*window_y_size];
    int index_for_array=0;
    int count_of_less_than_median=0;
    int median_value;
    int left_column[window_y_size];
    int right_column[window_y_size];
    // int left_column[window_y_size-1];
    // int right_column[window_y_size-1];
    int hist_index=0;
    int g1;

    int th=(window_x_size*window_y_size/2);
    
    // for(int i=0;i<picture_y_size;i++){
    //      Mpixel(image2,637,i)=255;
    //  }
    
    for(int i=(window_x_size/2);i<picture_x_size-(window_x_size/2);i++){
        
        int hist[256];
        for(int q=0;q<256;q++){
            hist[q]=0;
        }

        int index_for_hist=0;
        int y_sizeof=i+window_y_size;
        int x_sizeof=(window_x_size/2)+window_x_size;
        for(int a=i;a<y_sizeof;a++){
            for(int b=(window_x_size/2);b<x_sizeof;b++){
                index_for_hist=(int)Mpixel(image1,a,b);
                hist[index_for_hist]=hist[index_for_hist]+1;
            }
        }


        int counter_for_find_median=(window_x_size*window_y_size/2)+1;
        int counter_for_find_less_than_median=0;

        for(int z=0;z<256;z++){
            if(hist[z]!=0){ 
                counter_for_find_median=counter_for_find_median-hist[z];
                if(counter_for_find_median<=0){
                    median_value=z;
                    mdn=median_value;
                    break;
                }else{
                    counter_for_find_less_than_median
                    =counter_for_find_less_than_median+hist[z];
                }
            }       
        }

        ltmdn=counter_for_find_less_than_median;

        //Mpixel(image2,i,(window_y_size/2))=mdn;

        for(int j=(window_y_size/2)+1;j<picture_y_size-(window_y_size/2);j++){//j indicates picture column number

    
            int index_for_left_column=0;
            int index_for_right_column=0;

            for(int l=i;l<i+window_x_size;l++){
                left_column[index_for_left_column]=(int)Mpixel(image1,l,j);
                index_for_left_column++;

                right_column[index_for_right_column]=(int)Mpixel(image1,l,j+window_x_size-1);               
                index_for_right_column++;   
            }
            
                    
            

            for(int k=0;k<window_y_size;k++){
                g1=left_column[k];
                hist[g1]=hist[g1]-1;
                if(g1<mdn){
                    ltmdn=ltmdn-1;
                }
                g1=right_column[k];
                hist[g1]=hist[g1]+1;
                if(g1<mdn){
                    ltmdn=ltmdn+1;
                }

                if(ltmdn>th){
                    while(true){
                        mdn=mdn-1;
                        ltmdn=ltmdn-hist[mdn];
                        if(ltmdn<=th){
                            break;
                        }
                    }
                }else{
                    while(ltmdn+hist[mdn]<=th){
                        ltmdn=ltmdn+hist[mdn];
                        mdn=mdn+1;
                    }
                }
            }
            Mpixel(image2,i,j)=mdn; //determine pixel
        }
    }

    for(int j=0;j<window_size/2+1;j++){ 
        for(int i=0;i<picture_y_size;i++){
            Mpixel(image2,j,i)=0;
            Mpixel(image2,picture_x_size-1-j,i)=0;
        }
    }
    for(int j=0;j<window_size/2+1;j++){ 
        for(int i=window_size/2;i<picture_x_size-(window_size/2);i++){
            Mpixel(image2,i,j)=0;
            Mpixel(image2,i,picture_y_size-1-j)=0;
        }
    }

}

int FindTheLargestContour2(std::vector<vector<Point>>contours){
    int largestcontour=0;
    long int largestsize=0;
    for(int i=0;i<contours.size();i++){
        if(largestsize<contours[i].size()){
            largestsize=contours[i].size();
            largestcontour=i;
        }
    }
    return largestcontour;
}


void Integral_test(Mat image1, double **integral_image, double **squared_integral_image){
//This is the function for printing out 1/20 size of original metrices values and summed-area table values.
//Checking whether properly summed or not. 	

	/*
		1/20 size of original metrices values
	*/
	cout<<"The First image"<<endl;
	for(int i=0;i<image1.cols;i++){
		cout<<"[ ";
		for(int j=0;j<image1.rows;j++){
			cout<<(int)Mpixel(image1,i,j)<<" ";
		}
		cout<<" ]"<<endl;
	}
	///////////////////////////////////////////

	/*
		summed-area table values
	*/
	cout<<"The Second image"<<endl;
	for(int i=0;i<image1.cols;i++){
		cout<<"[ ";
		for(int j=0;j<image1.rows;j++){
			cout<<integral_image[i][j]<<" ";
		}
		cout<<" ]"<<endl;
	}


	cout<<"The squared image"<<endl;
	for(int i=0;i<image1.cols/20;i++){
		cout<<"[ ";
		for(int j=0;j<image1.rows/20;j++){
			cout<<squared_integral_image[i][j]<<" ";
		}
		cout<<" ]"<<endl;
	}
	//////////////////////////////////////////
}

void Integral_Gray_Initialize(Mat image1, double **integral_image,double **squared_integral_image){
	//This is the function for creating summed-area table in gray image.
	for(int i=0;i<image1.cols+1;i++){
		for(int j=0;j<image1.rows+1;j++){
			if((i==0)||(j==0)){//The value of the first row and column is zero. 
				integral_image[i][j]=0;
				squared_integral_image[i][j]=0;
			}else{
				integral_image[i][j]=integral_image[i-1][j]+integral_image[i][j-1]-integral_image[i-1][j-1]+(int)Mpixel(image1,i-1,j-1);
				squared_integral_image[i][j]=squared_integral_image[i-1][j]+squared_integral_image[i][j-1]-squared_integral_image[i-1][j-1]
				+(int)Mpixel(image1,i-1,j-1)*(int)Mpixel(image1,i-1,j-1);
			}
			
		}
	}
}



void Integral_Color_Initialize(Mat3b image1, double **integral_image_B, double **integral_image_G, double **integral_image_R, double **squared_integral_image_B, double **squared_integral_image_G, double **squared_integral_image_R){
	//This is the function for creating summed-area table in color image.
	for(int i=0;i<image1.cols;i++){
		for(int j=0;j<image1.rows;j++){
			if((i==0)||(j==0)){//The value of the first row and column is zero.
				integral_image_B[i][j]=0;
				integral_image_G[i][j]=0;
				integral_image_R[i][j]=0;

				squared_integral_image_B[i][j]=0;
				squared_integral_image_G[i][j]=0;
				squared_integral_image_R[i][j]=0;
			}else{
				integral_image_B[i][j]=integral_image_B[i-1][j]+integral_image_B[i][j-1]-integral_image_B[i-1][j-1]+(int)pixelB(image1,i-1,j-1);
				integral_image_G[i][j]=integral_image_G[i-1][j]+integral_image_G[i][j-1]-integral_image_G[i-1][j-1]+(int)pixelG(image1,i-1,j-1);
				integral_image_R[i][j]=integral_image_R[i-1][j]+integral_image_R[i][j-1]-integral_image_R[i-1][j-1]+(int)pixelR(image1,i-1,j-1);

				squared_integral_image_B[i][j]=squared_integral_image_B[i-1][j]+squared_integral_image_B[i][j-1]-squared_integral_image_B[i-1][j-1]+(int)pixelB(image1,i-1,j-1)*(int)pixelB(image1,i-1,j-1);
				squared_integral_image_G[i][j]=squared_integral_image_G[i-1][j]+squared_integral_image_G[i][j-1]-squared_integral_image_G[i-1][j-1]+(int)pixelG(image1,i-1,j-1)*(int)pixelG(image1,i-1,j-1);
				squared_integral_image_R[i][j]=squared_integral_image_R[i-1][j]+squared_integral_image_R[i][j-1]-squared_integral_image_R[i-1][j-1]+(int)pixelR(image1,i-1,j-1)*(int)pixelR(image1,i-1,j-1);
			}
			
		}

	}
}


void Filter_Gray_Integral(Mat image1, Mat image2, double** integral_image,double** squared_integral_image, int window_size){
	//This is function for applying the Kuwahara filter to image2, it must require summed-area table.

	//Mat image1 is the source image with gray scale. 
	//Mat image2 is the final output which is applied to Kuwahara filter in gray scale image.
	//double** integral_image is 2-dimensional array which stores the summed-area table.
	//int window_size is the size of window it is big more blurly when the value is high.

	int picture_x_size=image1.cols;//the x-axis length of source image.
	int picture_y_size=image1.rows;//the y-axis length of source image.
	int window_x_size=window_size;//the x-axis length of window.
	int window_y_size=window_size;//the y-axis length of window.


	/* The function of Kuhawara filter*/
			
	// mk=1(n+1)×(n+1)×∑(x,y)∈θkφ(f(x,y)

	// k∈{0,1,2,3},fis the source image function,
	// f(x,y)is the value of the pixel at coordinates(x,y),
	// φis a function calculating the value of a particular pixel,
	// 1/(n+1)×(n+1)is the number of pixels in the current area,
	// n is the value obtained directly from the filter windowsize.

	// reference by 
	// https://link-springer-com.ezproxy.massey.ac.nz/content/pdf/10.1007%2Fs11760-015-0791-3.pdf
	// page 665
	/**********************************/

	for(int i=(window_x_size/2);i<picture_x_size-(window_x_size/2+1);i++){
		for(int j=(window_y_size/2);j<picture_y_size-(window_y_size/2+1);j++){
			
			int f[4];
			int s_f[4];
			int result[4];
			int small_window_size=(window_x_size/2)*(window_y_size/2);

			int i1=i;
			int j1=j;

			int i2=i+(window_x_size/2);
			int j2=j;

			int i3=i;
			int j3=j+(window_y_size/2);

			int i4=i+(window_x_size/2);
			int j4=j+(window_y_size/2);

			int mean_f[4];

			



			// f[0]=integral_image[i1][j1]-integral_image[i1-(window_x_size/2)][j1]
			// -integral_image[i1][j1-(window_y_size/2)]+integral_image[i1-(window_x_size/2)][j1-(window_y_size/2)];


			f[0]=integral_image[i1-1][j1]-integral_image[i1][j1-1]
			-integral_image[i1-1][j1-1]+(int)Mpixel(image1,i1-1,j1-1);


			// s_f[0]=squared_integral_image[i1][j1]-squared_integral_image[i1-(window_x_size/2)][j1]
			// -squared_integral_image[i1][j1-(window_y_size/2)]+squared_integral_image[i1-(window_x_size/2)][j1-(window_y_size/2)];


			cout<<"s_f: "<<s_f[0]<<endl;
			cout<<"f[0]: "<<f[0]<<endl;
			// f[0]=f[0]/small_window_size;
			// cout<<"s_f: "<<s_f[0]<<endl;
			// cout<<"(f[0]*f[0])/small_window_size): "<<((f[0]*f[0])/small_window_size)<<endl;
			
			// result[0]=(f[0]*f[0]-((f[0]*f[0])/small_window_size))/small_window_size;

			result[0]=(s_f[0]-((f[0]*f[0])/small_window_size));
			// result[0]=sqrt(result[0]);


			f[1]=integral_image[i2][j2]-integral_image[i2-(window_x_size/2)][j2]
			-integral_image[i2][j2-(window_y_size/2)]+integral_image[i2-(window_x_size/2)][j2-(window_y_size/2)];

			s_f[1]=squared_integral_image[i2][j2]-squared_integral_image[i2-(window_x_size/2)][j2]
			-squared_integral_image[i2][j2-(window_y_size/2)]+squared_integral_image[i2-(window_x_size/2)][j2-(window_y_size/2)];
			// f[1]=f[1]/small_window_size;
			


			cout<<"s_f: "<<s_f[1]<<endl;
			cout<<"f[1]: "<<f[1]<<endl;
			// result[1]=(f[1]*f[1]-((f[1]*f[1])/small_window_size))/small_window_size;
			result[1]=(s_f[1]-((f[1]*f[1])/small_window_size));
			// result[1]=sqrt(result[1]);


			f[2]=integral_image[i3][j3]-integral_image[i3-(window_x_size/2)][j3]
			-integral_image[i3][j3-(window_y_size/2)]+integral_image[i3-(window_x_size/2)][j3-(window_y_size/2)];

			s_f[2]=squared_integral_image[i3][j3]-squared_integral_image[i3-(window_x_size/2)][j3]
			-squared_integral_image[i3][j3-(window_y_size/2)]+squared_integral_image[i3-(window_x_size/2)][j3-(window_y_size/2)];
			// f[2]=f[2]/small_window_size;


			cout<<"s_f: "<<s_f[2]<<endl;
			cout<<"f[2]: "<<f[2]<<endl;
			
			// result[2]=(f[2]*f[2]-((f[2]*f[2])/small_window_size))/small_window_size;
			result[2]=(s_f[2]-((f[2]*f[2])/small_window_size));
			// result[2]=sqrt(result[2]);


			f[3]=integral_image[i4][j4]-integral_image[i4-(window_x_size/2)][j4]
			-integral_image[i4][j4-(window_y_size/2)]+integral_image[i4-(window_x_size/2)][j4-(window_y_size/2)];

			s_f[3]=squared_integral_image[i4][j4]-squared_integral_image[i4-(window_x_size/2)][j4]
			-squared_integral_image[i4][j4-(window_y_size/2)]+squared_integral_image[i4-(window_x_size/2)][j4-(window_y_size/2)];
			// f[3]=f[3]/small_window_size;


			cout<<"s_f: "<<s_f[3]<<endl;
			cout<<"f[3]: "<<f[3]<<endl;

			
			// result[3]=(f[3]*f[3]-((f[3]*f[3])/small_window_size))/small_window_size;
			result[3]=(s_f[3]-((f[3]*f[3])/small_window_size));
			// result[3]=sqrt(result[3]);
			
			/*
				The process finding the smallest value between f[0] and f[3].
			*/
			int final=9999;
			for(int l=0;l<4;l++){
				cout<<"result: "<<result[l]<<endl;
				// if(final>f[l]){
				// 	final=f[l];
				// }
				if(final>result[l]){
					final=result[l];
				}
			}
			cout<<"final: "<<final<<endl;
			getchar();

			Mpixel(image2,i,j)=final;
			//////////////////////////// 

		}
	}

}

void Filter_Gray_Integral2(Mat image1, Mat image2, double** integral_image,double** squared_integral_image, int window_size){
	//This is function for applying the Kuwahara filter to image2, it must require summed-area table.

	//Mat image1 is the source image with gray scale. 
	//Mat image2 is the final output which is applied to Kuwahara filter in gray scale image.
	//double** integral_image is 2-dimensional array which stores the summed-area table.
	//int window_size is the size of window it is big more blurly when the value is high.

	int picture_x_size=image1.cols;//the x-axis length of source image.
	int picture_y_size=image1.rows;//the y-axis length of source image.
	int window_x_size=window_size;//the x-axis length of window.
	int window_y_size=window_size;//the y-axis length of window.


	/* The function of Kuhawara filter*/
			
	// mk=1(n+1)×(n+1)×∑(x,y)∈θkφ(f(x,y)

	// k∈{0,1,2,3},fis the source image function,
	// f(x,y)is the value of the pixel at coordinates(x,y),
	// φis a function calculating the value of a particular pixel,
	// 1/(n+1)×(n+1)is the number of pixels in the current area,
	// n is the value obtained directly from the filter windowsize.

	// reference by 
	// https://link-springer-com.ezproxy.massey.ac.nz/content/pdf/10.1007%2Fs11760-015-0791-3.pdf
	// page 665
	/**********************************/

	for(int i=(window_x_size/2);i<picture_x_size-(window_x_size/2+1);i++){
		for(int j=(window_y_size/2);j<picture_y_size-(window_y_size/2+1);j++){
			
			int f[4];
			int s_f[4];
			int result[4];
			int small_window_size=(window_x_size/2+1)*(window_y_size/2+1);

			int i0=i;
			int j0=j;

			int i1=i+(window_x_size/2);
			int j1=j;

			int i2=i;
			int j2=j+(window_y_size/2);

			int i3=i+(window_x_size/2);
			int j3=j+(window_y_size/2);

			int mean_f[4];

			// cout<<"small_window_size: "<<small_window_size<<endl;




			// f[0]=integral_image[i1][j1]-integral_image[i1-(window_x_size/2)][j1]
			// -integral_image[i1][j1-(window_y_size/2)]+integral_image[i1-(window_x_size/2)][j1-(window_y_size/2)];


			f[0]=integral_image[i0+(window_x_size/2)][j0+(window_y_size/2)]-integral_image[i0-1][j0+(window_y_size/2)]
			-integral_image[i0+(window_y_size/2)][j0-1]+integral_image[i0-1][j0-1];

			s_f[0]=squared_integral_image[i0+(window_x_size/2)][j0+(window_y_size/2)]-squared_integral_image[i0-1][j0+(window_y_size/2)]
			-squared_integral_image[i0+(window_y_size/2)][j0-1]+squared_integral_image[i0-1][j0-1];

			// cout<<"integral_image[i1+(window_x_size/2)][j1+(window_y_size/2)]:"<<integral_image[i1+(window_x_size/2)][j1+(window_y_size/2)]<<endl;
			// cout<<"-integral_image[i1][j1+(window_y_size/2)]:"<<integral_image[i1][j1+(window_y_size/2)]<<endl;
			// cout<<"-integral_image[i1+(window_y_size/2)][j1]:"<<integral_image[i1+(window_y_size/2)][j1]<<endl;
			// cout<<"integral_image[i1][j1]:"<<integral_image[i1][j1]<<endl;

			result[0]=(s_f[0]-((f[0]*f[0])/small_window_size))/small_window_size;

			// cout<<"s_f[0]: "<<s_f[0]<<endl;
			// cout<<"f[0]: "<<	f[0]<<endl;


			f[1]=integral_image[i1+(window_x_size/2)][j1+(window_y_size/2)]-integral_image[i1-1][j1+(window_y_size/2)]
			-integral_image[i1+(window_y_size/2)][j1-1]+integral_image[i1-1][j1-1];

			s_f[1]=squared_integral_image[i1+(window_x_size/2)][j1+(window_y_size/2)]-squared_integral_image[i1-1][j1+(window_y_size/2)]
			-squared_integral_image[i1+(window_y_size/2)][j1-1]+squared_integral_image[i1-1][j1-1];

			result[1]=(s_f[1]-((f[1]*f[1])/small_window_size))/small_window_size;


			f[2]=integral_image[i2+(window_x_size/2)][j2+(window_y_size/2)]-integral_image[i2-1][j2+(window_y_size/2)]
			-integral_image[i2+(window_y_size/2)][j2-1]+integral_image[i2-1][j2-1];

			s_f[2]=squared_integral_image[i2+(window_x_size/2)][j2+(window_y_size/2)]-squared_integral_image[i2-1][j2+(window_y_size/2)]
			-squared_integral_image[i2+(window_y_size/2)][j2-1]+squared_integral_image[i2-1][j2-1];

			result[2]=(s_f[2]-((f[2]*f[2])/small_window_size))/small_window_size;


			f[3]=integral_image[i3+(window_x_size/2)][j3+(window_y_size/2)]-integral_image[i3-1][j3+(window_y_size/2)]
			-integral_image[i3+(window_y_size/2)][j3-1]+integral_image[i3-1][j3-1];

			s_f[3]=squared_integral_image[i3+(window_x_size/2)][j3+(window_y_size/2)]-squared_integral_image[i3-1][j3+(window_y_size/2)]
			-squared_integral_image[i3+(window_y_size/2)][j3-1]+squared_integral_image[i3-1][j3-1];

			result[3]=(s_f[3]-((f[3]*f[3])/small_window_size))/small_window_size;

			// cout<<"s_f[3]: "<<s_f[3]<<endl;
			// cout<<"f[3]: "<<	f[3]<<endl;
			// cout<<"small_window_size: "<<small_window_size<<endl; 
			// cout<<"result[3]: "<<result[3]<<endl;			
			// cout<<"f[3]*f[3]: "<<f[3]*f[3]<<endl;
			// cout<<"f[3]*f[3])/small_window_size: "<<(f[3]*f[3])/small_window_size<<endl;
			/*
				The process finding the smallest value between f[0] and f[3].
			*/
			int final=9999;
			for(int l=0;l<4;l++){
				// cout<<"result: "<<result[l]<<endl;
				// if(final>f[l]){
				// 	final=f[l];
				// }
				if(final>result[l]){
					final=result[l];
				}
			}
			cout<<"final: "<<final<<endl;
			
			// getchar();

			Mpixel(image2,i,j)=final;
			//////////////////////////// 

		}
	}

}

void Kuwahara_Filter_Gray_With_Sum_Table(Mat source_image, Mat output_image, double** integral_image, double** squared_integral_image, int window_size){
	//This is function for applying the Kuwahara filter to output_image, it must require summed-area table.

	//Mat source_image is the source image with gray scale. 
	//Mat output_image is the final output which is applied to Kuwahara filter in gray scale image.
	//double** integral_image is 2-dimensional array which stores the summed-area table.
	//int window_size is the size of window it is big more blurly when the value is high.

	int picture_x_size=source_image.cols;//the x-axis length of source image.
	int picture_y_size=source_image.rows;//the y-axis length of source image.
	

	for(int i=0;i<=source_image.cols-(window_size);i++){
		for(int j=0;j<=source_image.rows-(window_size);j++){
			double f[4];
			double s_f[4];
			double result[4];
			int small_window_size=(window_size/2+1)*(window_size/2+1);

			int i_col[4];
			int i_row[4];

			i_col[0]=i;
			i_row[0]=j;

			i_col[1]=i+(window_size/2);
			i_row[1]=j;

			i_col[2]=i;
			i_row[2]=j+(window_size/2);

			i_col[3]=i+(window_size/2);
			i_row[3]=j+(window_size/2);

			double mean_fa[4];

			
			for(int a=0;a<4;a++){
				f[a]=integral_image[i_col[a]+(window_size/2)+1][i_row[a]+(window_size/2)+1]-integral_image[i_col[a]][i_row[a]+(window_size/2)+1]
				-integral_image[i_col[a]+(window_size/2)+1][i_row[a]]+integral_image[i_col[a]][i_row[a]];

				s_f[a]=squared_integral_image[i_col[a]+(window_size/2)+1][i_row[a]+(window_size/2)+1]-squared_integral_image[i_col[a]][i_row[a]+(window_size/2)+1]
				-squared_integral_image[i_col[a]+(window_size/2)+1][i_row[a]]+squared_integral_image[i_col[a]][i_row[a]];

				mean_fa[a]=f[a]/small_window_size;
				// cout<<"mean_fa["<<a<<"]: "<<mean_fa[a]<<endl;
			}
			for(int a=0;a<4;a++){
				result[a]=(s_f[a]-((f[a]*f[a])/small_window_size))/small_window_size;
			}


			double final=9999;
			int min_index=0;
			for(int l=0;l<4;l++){
				if(final>result[l]){
					final=result[l];
					min_index=l;
				}
			}
			Mpixel(output_image,i+window_size/2,j+window_size/2)=(int)mean_fa[min_index];
		}
	}

}

void Kuwahara_Filter_Color_With_Sum_Table(Mat3b source_image, Mat3b output_image, double **integral_image_B, double **integral_image_G, double **integral_image_R, double **squared_integral_image_B, double **squared_integral_image_G, double **squared_integral_image_R, int window_size){
	//This is function for applying the Kuwahara filter to output_image, it must require summed-area table.

	//Mat source_image is the source image with gray scale. 
	//Mat output_image is the final output which is applied to Kuwahara filter in gray scale image.
	//double** integral_image is 2-dimensional array which stores the summed-area table.
	//int window_size is the size of window it is big more blurly when the value is high.

	int picture_x_size=source_image.cols;//the x-axis length of source image.
	int picture_y_size=source_image.rows;//the y-axis length of source image.
	

	for(int i=0;i<=source_image.cols-(window_size);i++){
		for(int j=0;j<=source_image.rows-(window_size);j++){
			double f_B[4];
			double f_G[4];
			double f_R[4];
			double s_f_B[4];
			double s_f_G[4];
			double s_f_R[4];
			double result_B[4];
			double result_G[4];
			double result_R[4];
			int small_window_size=(window_size/2+1)*(window_size/2+1);

			int i_col[4];
			int i_row[4];

			i_col[0]=i;
			i_row[0]=j;

			i_col[1]=i+(window_size/2);
			i_row[1]=j;

			i_col[2]=i;
			i_row[2]=j+(window_size/2);

			i_col[3]=i+(window_size/2);
			i_row[3]=j+(window_size/2);

			double mean_fa_B[4];
			double mean_fa_G[4];
			double mean_fa_R[4];

			for(int a=0;a<4;a++){
				f_B[a]=integral_image_B[i_col[a]+(window_size/2)+1][i_row[a]+(window_size/2)+1]-integral_image_B[i_col[a]][i_row[a]+(window_size/2)+1]
				-integral_image_B[i_col[a]+(window_size/2)+1][i_row[a]]+integral_image_B[i_col[a]][i_row[a]];

				s_f_B[a]=squared_integral_image_B[i_col[a]+(window_size/2)+1][i_row[a]+(window_size/2)+1]-squared_integral_image_B[i_col[a]][i_row[a]+(window_size/2)+1]
				-squared_integral_image_B[i_col[a]+(window_size/2)+1][i_row[a]]+squared_integral_image_B[i_col[a]][i_row[a]];

				mean_fa_B[a]=f_B[a]/small_window_size;
				// cout<<"mean_fa_B["<<a<<"]: "<<mean_fa_B[a]<<endl;
			}

			for(int a=0;a<4;a++){
				f_G[a]=integral_image_G[i_col[a]+(window_size/2)+1][i_row[a]+(window_size/2)+1]-integral_image_G[i_col[a]][i_row[a]+(window_size/2)+1]
				-integral_image_G[i_col[a]+(window_size/2)+1][i_row[a]]+integral_image_G[i_col[a]][i_row[a]];

				s_f_G[a]=squared_integral_image_G[i_col[a]+(window_size/2)+1][i_row[a]+(window_size/2)+1]-squared_integral_image_G[i_col[a]][i_row[a]+(window_size/2)+1]
				-squared_integral_image_G[i_col[a]+(window_size/2)+1][i_row[a]]+squared_integral_image_G[i_col[a]][i_row[a]];

				mean_fa_G[a]=f_G[a]/small_window_size;
				// cout<<"mean_fa_G["<<a<<"]: "<<mean_fa_G[a]<<endl;
			}
			
			for(int a=0;a<4;a++){
				f_R[a]=integral_image_R[i_col[a]+(window_size/2)+1][i_row[a]+(window_size/2)+1]-integral_image_R[i_col[a]][i_row[a]+(window_size/2)+1]
				-integral_image_R[i_col[a]+(window_size/2)+1][i_row[a]]+integral_image_R[i_col[a]][i_row[a]];

				s_f_R[a]=squared_integral_image_R[i_col[a]+(window_size/2)+1][i_row[a]+(window_size/2)+1]-squared_integral_image_R[i_col[a]][i_row[a]+(window_size/2)+1]
				-squared_integral_image_R[i_col[a]+(window_size/2)+1][i_row[a]]+squared_integral_image_R[i_col[a]][i_row[a]];

				mean_fa_R[a]=f_R[a]/small_window_size;
				// cout<<"mean_fa_R["<<a<<"]: "<<mean_fa_R[a]<<endl;
			}

			for(int a=0;a<4;a++){
				result_B[a]=(s_f_B[a]-((f_B[a]*f_B[a])/small_window_size))/small_window_size;
			}

			for(int a=0;a<4;a++){
				result_G[a]=(s_f_G[a]-((f_G[a]*f_G[a])/small_window_size))/small_window_size;
			}

			for(int a=0;a<4;a++){
				result_R[a]=(s_f_R[a]-((f_R[a]*f_R[a])/small_window_size))/small_window_size;
			}



			double final=9999;
			int min_index=0;
			for(int l=0;l<4;l++){
				if(final>result_B[l]){
					final=result_B[l];
					min_index=l;
				}
			}

			pixelB(output_image,i+window_size/2,j+window_size/2)=(int)mean_fa_B[min_index];

			final=9999;
			min_index=0;
			for(int l=0;l<4;l++){
				if(final>result_G[l]){
					final=result_G[l];
					min_index=l;
				}
			}

			pixelG(output_image,i+window_size/2,j+window_size/2)=(int)mean_fa_G[min_index];

			final=9999;
			min_index=0;

			for(int l=0;l<4;l++){
				if(final>result_R[l]){
					final=result_R[l];
					min_index=l;
				}
			}

			pixelR(output_image,i+window_size/2,j+window_size/2)=(int)mean_fa_R[min_index];
		}
	}

}




void Filter_Gray_Integral4(Mat image1, Mat image2, double** integral_image,double** squared_integral_image, int window_size){
	//This is function for applying the Kuwahara filter to image2, it must require summed-area table.

	//Mat image1 is the source image with gray scale. 
	//Mat image2 is the final output which is applied to Kuwahara filter in gray scale image.
	//double** integral_image is 2-dimensional array which stores the summed-area table.
	//int window_size is the size of window it is big more blurly when the value is high.

	int picture_x_size=image1.cols;//the x-axis length of source image.
	int picture_y_size=image1.rows;//the y-axis length of source image.
	int window_x_size=window_size;//the x-axis length of window.
	int window_y_size=window_size;//the y-axis length of window.


	for(int i=(window_x_size/2);i<picture_x_size-(window_x_size/2+1);i++){
		for(int j=(window_y_size/2);j<picture_y_size-(window_y_size/2+1);j++){
			float f[4];
			float s_f[4];
			float result[4];
			int small_window_size=(window_x_size/2+1)*(window_y_size/2+1);

			int i0=i;
			int j0=j;

			int i1=i+(window_x_size/2);
			int j1=j;

			int i2=i;
			int j2=j+(window_y_size/2);

			int i3=i+(window_x_size/2);
			int j3=j+(window_y_size/2);

			float mean_f[4];

			cout<<"small_window_size: "<<small_window_size<<endl;

			mean_f[0]=0;
			for(int l=i-(window_x_size/2);l<=i;l++){
				for(int k=j-(window_y_size/2);k<=j;k++){
					mean_f[0]=mean_f[0]+(float)Mpixel(image1,l,k);
				}
			}
			mean_f[0]=mean_f[0]/small_window_size;

			cout<<"mean_f[0]: "<<mean_f[0]<<endl;
			
			//the section of f[1]
			//  | |*|*|        
			//	| |*|*|
			//	| | | |
			mean_f[1]=0;
			for(int l=i;l<=i+(window_x_size/2);l++){
				for(int k=j-(window_y_size/2);k<=j;k++){
					mean_f[1]=mean_f[1]+(float)Mpixel(image1,l,k);
				}
			}
			mean_f[1]=mean_f[1]/small_window_size;

			cout<<"mean_f[1]: "<<mean_f[1]<<endl;

			//the section of f[2]
			//  | | | |        
			//	|*|*| |
			//	|*|*| |
			mean_f[2]=0;
			for(int l=i-(window_x_size/2);l<=i;l++){
				for(int k=j;k<=j+(window_y_size/2);k++){
					mean_f[2]=mean_f[2]+(float)Mpixel(image1,l,k);
				}
			}
			mean_f[2]=mean_f[2]/small_window_size;


			//the section of f[3]
			//  | | | |        
			//	| |*|*|
			//	| |*|*|
			mean_f[3]=0;
			for(int l=i;l<=i+(window_x_size/2);l++){
				for(int k=j;k<=j+(window_x_size/2);k++){
					mean_f[3]=mean_f[3]+(float)Mpixel(image1,l,k);
				}
			}

			mean_f[3]=mean_f[3]/small_window_size;





			//the section of f[0]
			//  |*|*| |        
			//	|*|*| |
			//	| | | |

			float temp=0;
			result[0]=0;


			for(int l=i-(window_x_size/2);l<=i;l++){
				for(int k=j-(window_y_size/2);k<=j;k++){
					temp=mean_f[0]-(float)Mpixel(image1,l,k);
					temp=temp*temp;
					result[0]=result[0]+temp;
				}
			}

			result[0]=result[0]/small_window_size;


			
			//the section of f[1]
			//  | |*|*|        
			//	| |*|*|
			//	| | | |
			result[1]=0;
			for(int l=i;l<=i+(window_x_size/2);l++){
				for(int k=j-(window_y_size/2);k<=j;k++){
					temp=mean_f[1]-(float)Mpixel(image1,l,k);
					temp=temp*temp;
					result[1]=result[1]+temp;
				}
			}
			result[1]=result[1]/small_window_size;


			//the section of f[2]
			//  | | | |        
			//	|*|*| |
			//	|*|*| |
			result[2]=0;
			for(int l=i-(window_x_size/2);l<=i;l++){
				for(int k=j;k<=j+(window_y_size/2);k++){
					temp=mean_f[2]-(float)Mpixel(image1,l,k);
					temp=temp*temp;
					result[2]=result[2]+temp;
				}
			}
			result[2]=result[2]/small_window_size;


			//the section of f[3]
			//  | | | |        
			//	| |*|*|
			//	| |*|*|
			result[3]=0;
			for(int l=i;l<=i+(window_x_size/2);l++){
				for(int k=j;k<=j+(window_x_size/2);k++){
					temp=mean_f[3]-(float)Mpixel(image1,l,k);
					temp=temp*temp;
					// cout<<"temp: "<<temp<<endl;
					result[3]=result[3]+temp;
				}
			}
			// getchar();

			result[3]=result[3]/small_window_size;












			// cout<<"s_f[3]: "<<s_f[3]<<endl;
			// cout<<"f[3]: "<<	f[3]<<endl;
			// cout<<"small_window_size: "<<small_window_size<<endl; 
			cout<<"result[0]: "<<result[0]<<endl;			
			cout<<"result[1]: "<<result[1]<<endl;			
			cout<<"result[2]: "<<result[2]<<endl;			
			cout<<"result[3]: "<<result[3]<<endl;
			cout<<"i: "<<i<<endl;
			cout<<"j: "<<j<<endl;			
			// cout<<"f[3]*f[3]: "<<f[3]*f[3]<<endl;
			// cout<<"f[3]*f[3])/small_window_size: "<<(f[3]*f[3])/small_window_size<<endl;
			/*
				The process finding the smallest value between f[0] and f[3].
			*/
			float final=9999;
			for(int l=0;l<4;l++){
				// cout<<"result: "<<result[l]<<endl;
				// if(final>f[l]){
				// 	final=f[l];
				// }
				if(final>result[l]){
					final=result[l];
				}
			}

			for(int l=0;l<4;l++){
				if(final==result[l]){
					Mpixel(image2,i,j)=mean_f[l];
					break;
				}
			}
			// cout<<"final: "<<final<<endl;
			// if(final>255){
			// 	// getchar();
			// 	Mpixel(image2,i,j)=(int)Mpixel(image1,i,j);
			// }else{
			// Mpixel(image2,i,j)=(int)final;	
			// }

			
			//////////////////////////// 

		}
	}

}

void Kuwahara_Filter_Gray_Without_Sum_Table(Mat source_image, Mat output_image, int window_size){
	//This is function for applying the Kuwahara filter to output_image, it must require summed-area table.

	//Mat source_image is the source image with gray scale. 
	//Mat output_image is the final output which is applied to Kuwahara filter in gray scale image.
	//double** integral_image is 2-dimensional array which stores the summed-area table.
	//int window_size is the size of window it is big more blurly when the value is high.

	int picture_x_size=source_image.cols;//the x-axis length of source image.
	int picture_y_size=source_image.rows;//the y-axis length of source image.


	
	for(int i=0;i<=source_image.cols-(window_size);i++){
		for(int j=0;j<=source_image.rows-(window_size);j++){
			double f[4];
			double s_f[4];
			double result[4];
			int small_window_size=(window_size/2+1)*(window_size/2+1);

			int i_col[4];
			int i_row[4];

			i_col[0]=i;
			i_row[0]=j;

			i_col[1]=i+(window_size/2);
			i_row[1]=j;

			i_col[2]=i;
			i_row[2]=j+(window_size/2);

			i_col[3]=i+(window_size/2);
			i_row[3]=j+(window_size/2);

			double mean_f[4];

			// cout<<"small_window_size: "<<small_window_size<<endl;

			mean_f[0]=0;
			for(int l=i;l<=i+(window_size/2);l++){
				for(int k=j;k<=j+(window_size/2);k++){
					mean_f[0]=mean_f[0]+(float)Mpixel(source_image,l,k);
				}
			}
			mean_f[0]=mean_f[0]/small_window_size;

			// cout<<"mean_f[0]: "<<mean_f[0]<<endl;
			
			//the section of f[1]
			//  | |*|*|        
			//	| |*|*|
			//	| | | |
			mean_f[1]=0;
			for(int l=i+(window_size/2);l<i+window_size;l++){
				for(int k=j;k<=j+(window_size/2);k++){
					mean_f[1]=mean_f[1]+(float)Mpixel(source_image,l,k);
				}
			}
			mean_f[1]=mean_f[1]/small_window_size;

			// cout<<"mean_f[1]: "<<mean_f[1]<<endl;

			//the section of f[2]
			//  | | | |        
			//	|*|*| |
			//	|*|*| |

			mean_f[2]=0;
			for(int l=i;l<=i+(window_size/2);l++){
				for(int k=j+(window_size/2);k<j+window_size;k++){
					mean_f[2]=mean_f[2]+(float)Mpixel(source_image,l,k);
				}
			}
			mean_f[2]=mean_f[2]/small_window_size;
			// cout<<"mean_f[2]: "<<mean_f[2]<<endl;


			//the section of f[3]
			//  | | | |        
			//	| |*|*|
			//	| |*|*|

			mean_f[3]=0;
			for(int l=i+(window_size/2);l<i+window_size;l++){
				for(int k=j+(window_size/2);k<j+window_size;k++){
					mean_f[3]=mean_f[3]+(float)Mpixel(source_image,l,k);
				}
			}

			mean_f[3]=mean_f[3]/small_window_size;
			// cout<<"mean_f[3]: "<<mean_f[3]<<endl;
			
			

			double temp=0;
			result[0]=0;
			for(int l=i;l<=i+(window_size/2);l++){
				for(int k=j;k<=j+(window_size/2);k++){
					temp=mean_f[0]-(double)Mpixel(source_image,l,k);
					temp=temp*temp;
					result[0]=result[0]+temp;
				}
			}

			result[0]=result[0]/small_window_size;


			
			//the section of f[1]
			//  | |*|*|        
			//	| |*|*|
			//	| | | |
			result[1]=0;
			for(int l=i+(window_size/2);l<i+window_size;l++){
				for(int k=j;k<=j+(window_size/2);k++){
					temp=mean_f[1]-(double)Mpixel(source_image,l,k);
					temp=temp*temp;
					result[1]=result[1]+temp;
				}
			}
			result[1]=result[1]/small_window_size;


			//the section of f[2]
			//  | | | |        
			//	|*|*| |
			//	|*|*| |
			result[2]=0;
			for(int l=i;l<=i+(window_size/2);l++){
				for(int k=j+(window_size/2);k<j+window_size;k++){
					temp=mean_f[2]-(double)Mpixel(source_image,l,k);
					temp=temp*temp;
					result[2]=result[2]+temp;
				}
			}
			result[2]=result[2]/small_window_size;


			//the section of f[3]
			//  | | | |        
			//	| |*|*|
			//	| |*|*|
			result[3]=0;
			for(int l=i+(window_size/2);l<i+window_size;l++){
				for(int k=j+(window_size/2);k<j+window_size;k++){
					temp=mean_f[3]-(double)Mpixel(source_image,l,k);
					temp=temp*temp;
					// cout<<"temp: "<<temp<<endl;
					result[3]=result[3]+temp;
				}
			}
			// getchar();

			result[3]=result[3]/small_window_size;

			double final=9999;
			int min_index=0;
			for(int l=0;l<4;l++){
				if(final>result[l]){
					final=result[l];
					min_index=l;
				}
			}
			Mpixel(output_image,i+window_size/2,j+window_size/2)=(int)mean_f[min_index];
		}
	}

}



void Filter_Gray(Mat image1, Mat image2, int window_size){
	// This is function for applying the Kuwahara filter to gray-image2 (without summed-table).

	// Mat image1 is the source image with Gray scale. 
	// Mat image2 is the final output which is applied to Kuwahara filter in Gray image.

	// int window_size is the size of window it is big more blurly when the value is high.


	int picture_x_size=image1.cols;//the x-axis length of source image.
	int picture_y_size=image1.rows;//the y-axis length of source image.
	int window_x_size=window_size;//the x-axis length of window. 
	int window_y_size=window_size;//the y-axis length of window.

	for(int i=(window_x_size/2);i<picture_x_size-(window_x_size/2);i++){
		for(int j=(window_y_size/2);j<picture_y_size-(window_y_size/2);j++){
			int value[4];
			int f[4];
			int small_window_size=((window_x_size/2)+1)*((window_y_size/2)+1);

			//	This is 3*3 window
			//  | | | |        
			//	| | | |
			//	| | | |



			//the section of f[0]
			//  |*|*| |        
			//	|*|*| |
			//	| | | |

			value[0]=0;
			for(int l=i-(window_x_size/2);l<=i;l++){
				for(int k=j-(window_y_size/2);k<=j;k++){
					value[0]=value[0]+(int)Mpixel(image1,l,k);
				}
			}

			f[0]=value[0]/small_window_size;


			//the section of f[1]
			//  | |*|*|        
			//	| |*|*|
			//	| | | |

			value[1]=0;
			for(int l=i;l<=i+(window_x_size/2);l++){
				for(int k=j-(window_y_size/2);k<=j;k++){
					value[1]=value[1]+(int)Mpixel(image1,l,k);
				}
			}

			f[1]=value[1]/small_window_size;


			//the section of f[2]
			//  | | | |        
			//	|*|*| |
			//	|*|*| |

			value[2]=0;
			for(int l=i-(window_x_size/2);l<=i;l++){
				for(int k=j;k<=j+(window_y_size/2);k++){
					value[2]=value[2]+(int)Mpixel(image1,l,k);
				}
			}

			f[2]=value[2]/small_window_size;


			//the section of f[3]
			//  | | | |        
			//	| |*|*|
			//	| |*|*|

			value[3]=0;
			for(int l=i;l<=i+(window_x_size/2);l++){
				for(int k=j;k<=j+(window_x_size/2);k++){
					value[3]=value[3]+(int)Mpixel(image1,l,k);
				}
			}

			f[3]=value[3]/small_window_size;

			
			/*
				The process finding the smallest value between f[0] and f[3].
			*/
			int final=9999;
			for(int l=0;l<4;l++){
				if(final>f[l]){
					final=f[l];
				}
			}

			Mpixel(image2,i,j)=final;
			/////////////////////////

		}
	}

}


int run_kuwahara(int argc,char *argv[]){
	/*The First image*/
	double fps=0.0;	
	if(argc==3){

		int size=strlen(argv[2]);
		cout<<"size:"<<size<<endl;
		char path[size+2];

		strcpy(path,argv[2]);
		strcat(path,"/*");
		cout<<"path:"<<path<<endl;
		

		// string path = "argv[2]";
		// cout<<"path:"<<path<<endl;
		glob_t glob_result;
		glob(path,GLOB_TILDE,NULL,&glob_result);
		// glob(argv[2],GLOB_TILDE,NULL,&glob_result);
		int key=0;
		for(unsigned int i=0; i<glob_result.gl_pathc-2; ++i){
		// for(unsigned int i=0; i<1; ++i){
		  	cout << glob_result.gl_pathv[i] << endl;
		  	cout << glob_result.gl_pathv[i+1] << endl;
		  	cout << glob_result.gl_pathv[i+2] << endl;
		  	bool is_the_end_numb_in_a_row1=check_numb_in_a_row(glob_result.gl_pathv[i],glob_result.gl_pathv[i+1]);
		  	bool is_the_end_numb_in_a_row2=check_numb_in_a_row(glob_result.gl_pathv[i+1],glob_result.gl_pathv[i+2]);
		  	
		  	if((is_the_end_numb_in_a_row1==false)||(is_the_end_numb_in_a_row2==false)){
		  		cout<<"The file not in a row"<<endl;
		  		continue;
		  	}
		  	
		  	





		  	/*The First image process*/
		  	Mat3b image1;

	        Mat gray_image1;
		  	image1=imread(glob_result.gl_pathv[i],1);
		   	resize(image1, image1, cv::Size(), 0.5, 0.5);
		   	if(!image1.data){printf("Could not open the file\n"); exit(0);}
			cvtColor(image1,gray_image1, COLOR_BGR2GRAY);//color image1 to gray scale
			

			// exit(0);
			
		   	/*****************/

		   	/*The Second image process*/
	   		Mat3b image2;
	   		Mat gray_image2;
	   		image2=imread(glob_result.gl_pathv[i+1],1);	
	   		resize(image2, image2, cv::Size(), 0.5, 0.5);
	   		if(!image2.data){printf("Could not open the file\n"); exit(0);}
	   		cvtColor(image2,gray_image2, COLOR_BGR2GRAY);//color image2 to gray scale




	   		/*The Third image process*/
	   		Mat3b image3;
	   		Mat gray_image3;
	   	// 	if(is_the_end_numb_in_a_row==true){
		  	// 	image3=imread(glob_result.gl_pathv[i+1],1);	
		  	// }else{
		  	// 	image3=imread(glob_result.gl_pathv[i],1);
		  	// }
		  	image3=imread(glob_result.gl_pathv[i+2],1);	
		  	resize(image3, image3, cv::Size(), 0.5, 0.5);
		   	if(!image3.data){printf("Could not open the file\n"); exit(0);}
			cvtColor(image3, gray_image3, COLOR_BGR2GRAY);//color image1 to gray scale


			Kuhawara sample1(image1);
			Kuhawara sample2(image2);
			Kuhawara sample3(image3);
		  	// Kuhawara sample2;
		  	// Kuhawara sample3;


			// sample1.main(image1);
			// sample[0].main(image1);
			// sample[1].main(image2);
			// sample[2].main(image3);
			// sample2.main(image2);
			// sample3.main(image3);
			// Mat t=sample[0].get_kuhawara_img();
			cout<<"hello"<<endl;

			Kuhawara_ROI ku(sample1,sample2,sample3);


			

			imshow("kuhawara_sample1", ku.get_blob());
			imshow("get_thresholded_img", ku.get_thresholded_img());
			key=waitKey(0);
			continue;


	   		//////////////////////////////////////
    		//Kuwahara filter using Summed-table//
    		//////////////////////////////////////

			/*Memory Allocation*/
			double** integral_image1=new double*[gray_image1.cols+1];
			double** squared_integral_image1=new double*[gray_image1.cols+1];

			double** integral_image2=new double*[gray_image2.cols+1];
			double** squared_integral_image2=new double*[gray_image2.cols+1];

			double** integral_image3=new double*[gray_image3.cols+1];
			double** squared_integral_image3=new double*[gray_image3.cols+1];

			for(int i = 0; i < gray_image2.cols+1; ++i){
				integral_image1[i] = new double[gray_image1.rows+1];
				squared_integral_image1[i] = new double[gray_image1.rows+1];

				integral_image2[i] = new double[gray_image2.rows+1];
				squared_integral_image2[i] = new double[gray_image2.rows+1];

				integral_image3[i] = new double[gray_image3.rows+1];
				squared_integral_image3[i] = new double[gray_image3.rows+1];
			}
    		/*********************/	

			Mat filtered_image1, filtered_image2, filtered_image3;

			filtered_image1=Mat::zeros(gray_image1.size(),IMREAD_GRAYSCALE);//initialize the value of output metrices to zero
			filtered_image2=Mat::zeros(gray_image2.size(),IMREAD_GRAYSCALE);//initialize the value of output metrices to zero
			filtered_image3=Mat::zeros(gray_image3.size(),IMREAD_GRAYSCALE);//initialize the value of output metrices to zero

			Integral_Gray_Initialize(gray_image1,integral_image1,squared_integral_image1);//create summed-table to integral_image array.
    		Kuwahara_Filter_Gray_With_Sum_Table(gray_image1,filtered_image1,integral_image1,squared_integral_image1,3);//Applying kuwahara filter to output using integral_image.

    		Integral_Gray_Initialize(gray_image2,integral_image2,squared_integral_image2);//create summed-table to integral_image array.
    		Kuwahara_Filter_Gray_With_Sum_Table(gray_image2,filtered_image2,integral_image2,squared_integral_image2,3);//Applying kuwahara filter to output using integral_image.
    		
			Integral_Gray_Initialize(gray_image3, integral_image3, squared_integral_image3);//create summed-table to integral_image array.
    		Kuwahara_Filter_Gray_With_Sum_Table(gray_image3, filtered_image3, integral_image3, squared_integral_image3, 3);//Applying kuwahara filter to output using integral_image.

			/*Memory deallocation*/
			for(int i = 0; i < gray_image1.cols+1; ++i) {
				delete [] integral_image1[i];
				delete [] squared_integral_image1[i];

				delete [] integral_image2[i];
				delete [] squared_integral_image2[i];

				delete [] integral_image3[i];
				delete [] squared_integral_image3[i];
			}
			delete [] integral_image1;
			delete [] squared_integral_image1;
			delete [] integral_image2;
			delete [] squared_integral_image2;
			delete [] integral_image3;
			delete [] squared_integral_image3;
			/***************/
			/*subtraction process between The first image and the second image*/

			Mat output1;
			output1=filtered_image1-filtered_image2;	
			// if(is_the_end_numb_in_a_row==true){
		 //  		output1=filtered_image1-filtered_image2;
		 //  	}else{
		 //  		output1=filtered_image2-filtered_image1;
		 //  	}
		  	// Mat temp_output1=filtered_image1-filtered_image2;
		  	// Mat temp_output2=filtered_image2-filtered_image1;

		  	Mat temp_output1=filtered_image1-filtered_image2;
		  	Mat temp_output2=filtered_image3-filtered_image2;

			// median_filter(output1,output1,5);
			// median_filter(temp_output1,temp_output1,15);
			// median_filter(temp_output2,temp_output2,15);

			/***Cropping Object by outline of object***/
			Mat temp_window=Mat::zeros(image2.size(),IMREAD_GRAYSCALE);//gray case
			// Image_stitching(gray_image2,output,temp_window);


			Mat blob_window=Mat::zeros(image2.size(),CV_8UC3);

			// thresholding_image(temp_output1, 10,true,0);
			// thresholding_image(temp_output2, 10,true,0);

			Mat temp_output=temp_output1+temp_output2;
			thresholding_image(temp_output, 30,true,0);

			// imshow("temp_output1", temp_output1);
			// imshow("temp_output2", temp_output2);
			

			median_filter(temp_output,temp_output,11);
			// thresholding_image(temp_output1, 10,true,0);
			// thresholding_image(temp_output2, 10,true,0);

			// imshow("temp_output1", temp_output1);
			// imshow("temp_output2", temp_output2);
			// key=waitKey(0);
			// continue;

			Point *p1,*p2;
			p1=new Point[200];//approx numb
			p2=new Point[200];//approx numb

			// Point p1[200],p2[200];

			// p1=new Point[200];
			// p2=new Point[200];
			// int count_numb=blob(output1,blob_window,p1,p2);

			int count_numb=blob(temp_output,blob_window,p1,p2);

			if(count_numb==-100){
				cout<<"Skip: too much blob"<<endl;
				continue;
			}//segmental fault
			// int count_numb=blob(output2,blob_window,p1,p2);
			
			
			Point center_of_object=draw_rect_box(image2, p1, p2, 200);

			Mat ROI=Cropping_ROI(image2,center_of_object,200);
			Mat ROI_thresholding=Cropping_ROI(temp_output,center_of_object,200);

			// Mat ROI_2=Cropping_ROI(image2,center_of_object2,200);

			Mat ROI_gray,ROI_2_gray;
			cvtColor(ROI, ROI_gray, CV_BGR2GRAY);
			// Canny(ROI_gray, ROI_gray, 50, 200, 3);

			std::vector<vector<Point>>contours;
			// findContours(ROI_gray,contours,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_NONE);
			// cout<<"p2"<<endl;
			findContours(ROI_thresholding,contours,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_NONE);
			// cout<<"p3"<<endl;
			Point ROI_mid_p;

			ROI_mid_p.x=ROI_gray.rows/2;
			ROI_mid_p.y=ROI_gray.cols/2;

			int object_i= Find_The_Object_Contour(contours,ROI_mid_p);
			// cout<<"p4"<<endl;
			// cout<<"1"<<endl;
			Mat drawing=Mat::zeros(ROI_gray.size(),CV_8UC3);
			// cout<<"object_i:"<<object_i<<endl;

			// cout<<"2"<<endl;
			// int largestcontour=FindTheLargestContour(contours);

			// for(int i=0;i<contours[largestcontour].size();i++){
			// 	cout<<"contours(x,y) ("<<i<<"): "<<contours[largestcontour][i].x<<", "<<contours[largestcontour][i].y<<endl;
				
			// }
			// cout<<"3"<<endl;
			Scalar color=CV_RGB(255,0,0);
			drawContours(ROI,contours,object_i,color,2,8);

			// Point *rp1,*rp2;
			// rp1=new Point[200];//approx numb
			// rp2=new Point[200];//approx numb

			// // Point p1[200],p2[200];

			// // p1=new Point[200];
			// // p2=new Point[200];
			// // int count_numb=blob(output1,blob_window,p1,p2);
			// Mat ROI_blob_window;
			// int count_numb1=blob(ROI_gray,ROI_blob_window,rp1,rp2);

			// if(count_numb1==-100){
			// 	cout<<"Skip: too much blob"<<endl;
			// 	continue;
			// }//segmental fault
			// Point center_of_object_ROI=draw_rect_box(ROI, rp1, rp2, 20);

			// median_filter(ROI_gray,ROI_gray,7);
			// cvtColor(ROI_2, ROI_2_gray, CV_BGR2GRAY);

			// double** ROI_integral_image=new double*[ROI_gray.cols+1];
			// double** ROI_squared_integral_image=new double*[ROI_gray.cols+1];

			// for(int i = 0; i < ROI_gray.cols+1; ++i){
			// 	ROI_integral_image[i] = new double[ROI_gray.rows+1];
			// 	ROI_squared_integral_image[i] = new double[ROI_gray.rows+1];
			// }

			// Mat kuwahara_ROI;

			// kuwahara_ROI=Mat::zeros(ROI_gray.size(),IMREAD_GRAYSCALE);//initialize the value of output metrices to zero

			// Integral_Gray_Initialize(ROI_gray,ROI_integral_image,ROI_squared_integral_image);//create summed-table to integral_image array.
   //  		Kuwahara_Filter_Gray_With_Sum_Table(ROI_gray,kuwahara_ROI,ROI_integral_image,ROI_squared_integral_image,5);//Applying kuwahara filter to output using integral_image.

			// /*Memory deallocation*/
			// for(int i = 0; i < ROI_gray.cols+1; ++i) {
			// 	delete [] ROI_integral_image[i];
			// 	delete [] ROI_squared_integral_image[i];
			// }
			// delete [] ROI_integral_image;
			// delete [] ROI_squared_integral_image;
			// while(true){

			// namedWindow(window_capture_name);
   //  		namedWindow(window_detection_name);
	  //   	// Trackbars to set thresholds for HSV values
	  //   	createTrackbar("Low H", window_detection_name, &low_H, max_value_H, on_low_H_thresh_trackbar);
	  //   	createTrackbar("High H", window_detection_name, &high_H, max_value_H, on_high_H_thresh_trackbar);
	  //   	createTrackbar("Low S", window_detection_name, &low_S, max_value, on_low_S_thresh_trackbar);
	  //   	createTrackbar("High S", window_detection_name, &high_S, max_value, on_high_S_thresh_trackbar);
	  //   	createTrackbar("Low V", window_detection_name, &low_V, max_value, on_low_V_thresh_trackbar);
	  //   	createTrackbar("High V", window_detection_name, &high_V, max_value, on_high_V_thresh_trackbar);

			// Mat ROI_hsv;
			// Mat frame, frame_HSV, frame_threshold;
			// cvtColor(ROI, ROI_hsv, CV_BGR2HSV);
			// frame_HSV=ROI_hsv;
			// inRange(frame_HSV, Scalar(low_H, low_S, low_V), Scalar(high_H, high_S, high_V), frame_threshold);
			// // imshow(window_capture_name, frame);
   //      	imshow(window_detection_name, frame_threshold);
   //      	key=waitKey(1);
   //      	}


			// src_gray=ROI_gray;
			// src=image2;


			// namedWindow( window_name, WINDOW_AUTOSIZE ); // Create a window to display results
		 //    createTrackbar( trackbar_type,
		 //                    window_name, &threshold_type,
		 //                    max_type, Threshold_Demo ); // Create a Trackbar to choose type of Threshold
		 //    createTrackbar( trackbar_value,
		 //                    window_name, &threshold_value,
		 //                    max_value, Threshold_Demo ); // Create a Trackbar to choose Threshold value
		 //    Threshold_Demo( 0, 0 ); // Call the function to initialize
			// cout<<"p1"<<endl;
			// Mat test;
			// // test=ROI_2_gray-kuwahara_ROI;
			// test=ROI_2_gray-ROI_gray;
			// int window_size=0;
			// for(int x=(window_size/2); x<test.cols-(window_size/2);x++){
			// 	for(int y=(window_size/2); y<test.rows-(window_size/2);y++){
			// 		if(Mpixel(test,x,y)>130){
			// 			Mpixel(test,x,y)=255;
			// 		}else{
			// 			Mpixel(test,x,y)=0;
			// 		}
			// 	}
			// }	
			// thresholding_image(test, 40,false,7);
			// imshow("test",test);

			// Mat kuwahara_ROI_th;
			// kuwahara_ROI_th=kuwahara_ROI.clone();
			// double histogram[256];
			// int val=binary_histogram(kuwahara_ROI_th,105, histogram);
			// // if(val>100){
			// // 	val=val-30;	
			// // }else if(val<31){
			// // 	val=val+30;
			// // }
			// // }else if(val>=100&&val<150){
			// // 	val=val-80;
			// // }
			// // else if(val>=150&&val<250){
			// // 	val=val-100;
			// // }
			// // if(val>49){
			// // 	val=val-30;
			// // }
			// cout<<"val: "<<val<<endl;
			// // thresholding_image(kuwahara_ROI_th, val,false,5);
			// Mat final=test+kuwahara_ROI_th;
			    // Edge detection
    		// Canny(kuwahara_ROI, kuwahara_ROI, 50, 200, 3);
			

			// imshow("kuwahara_ROI_th", kuwahara_ROI_th);
			imshow("ROI_gray", ROI_gray);
			imshow("ROI", ROI);
			imshow("drawing", drawing);
			// imshow("ROI_2", ROI_2);
			// imshow("kuwahara_ROI", kuwahara_ROI);
			// imshow("temp_output1", temp_output1);
			// imshow("temp_output2", temp_output2);
			imshow("temp_output", temp_output);
			key=waitKey(0);
			continue;


			// int val=binary_histogram(kuwahara_ROI_th,131, histogram);
			// if(val>100){
			// 	val=val-30;	
			// }else if(val<31){
			// 	val=val+30;
			// }
			// }else if(val>=100&&val<150){
			// 	val=val-80;
			// }
			// else if(val>=150&&val<250){
			// 	val=val-100;
			// }

			
			// cout<<"val:"<<val<<endl;
			// val=200;

			// thresholding_image(kuwahara_ROI_th, val,false,3);
			// if(val<150){
				// thresholding_image(kuwahara_ROI_th, val,false,3);	
			// }else{
				// thresholding_image(kuwahara_ROI_th, val,true,3);	
			// }
			
			// median_filter(kuwahara_ROI_th,kuwahara_ROI_th,3);

	// 		std::vector<vector<Point>>contours;
	// 		// findContours(ROI_gray,contours,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_NONE);
	// 		// cout<<"p2"<<endl;
	// 		findContours(final,contours,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_NONE);
	// 		// cout<<"p3"<<endl;
	// 		Point ROI_mid_p;

	// 		ROI_mid_p.x=ROI_gray.rows/2;
	// 		ROI_mid_p.y=ROI_gray.cols/2;

	// 		int object_i= Find_The_Object_Contour(contours,ROI_mid_p);
	// 		// cout<<"p4"<<endl;
	// 		// cout<<"1"<<endl;
	// 		Mat drawing=Mat::zeros(ROI.size(),CV_8UC3);
	// 		// cout<<"object_i:"<<object_i<<endl;

	// 		// cout<<"2"<<endl;
	// 		// int largestcontour=FindTheLargestContour(contours);

	// 		// for(int i=0;i<contours[largestcontour].size();i++){
	// 		// 	cout<<"contours(x,y) ("<<i<<"): "<<contours[largestcontour][i].x<<", "<<contours[largestcontour][i].y<<endl;
				
	// 		// }
	// 		// cout<<"3"<<endl;
	// 		Scalar color=CV_RGB(255,0,0);
	// 		drawContours(drawing,contours,object_i,color,2,8);
	// 		cout<<"object_i: "<<object_i<<endl;
	// 		if(object_i!=-1){
	// 			cout<<"contour size: "<<contours[object_i].size()<<endl;
	// 		}
			

	// 		imshow("ROI_gray", ROI_gray);
	// 		imshow("final", final);
	// 		imshow("kuwahara_ROI_th", kuwahara_ROI_th);
	// 		imshow("kuwahara_ROI", kuwahara_ROI);
	// 		imshow("drawing", drawing);
			
	// 		// cout<<"p5"<<endl;


	// 		string saving_directory="ROI_images/";
	// 		string s = glob_result.gl_pathv[i];
	// 		string delimiter = ".";
	// 		string token = s.substr(0, s.find(delimiter)); 

	// 		string prev_name=glob_result.gl_pathv[i];
	// 		string str_prev_name=prev_name.substr(prev_name.size()-8,4);

	// 		string path_ROI=saving_directory+str_prev_name+"_ROI.jpg";
	// 		string path_contour=saving_directory+str_prev_name+"_contour.jpg";


			

	// 		// string str_cur_name=cur_name.substr(cur_name.size()-8,4);
	// // int prev_img_numb=stoi(str_prev_name);
	// // int cur_img_numb=stoi(str_cur_name);
	// // int diff_numb=cur_img_numb-prev_img_numb;
	// 		cout<<"path_ROI: "<<path_ROI<<endl; 
	// 		imwrite( path_ROI, ROI );
	// 		imwrite( path_contour, drawing );
	// 		key=waitKey(0);

			




	// 		continue;
	// 		// cout<<"4"<<endl;
	// 		// vector<float>CE;
	// 		// EllipticFourierDescriptors(contours[largestcontour],CE);








			
		    
	// 		// thresholding_image(gray_image2, 120);
			
	// 		// draw_rect_box(image2, p1, p2, 200);
			
	// 		cout<<"count_numb:"<<count_numb<<endl;
			

	// 		// imshow("output", output);
	// 		imshow("ROI", ROI);
	// 		imshow("gray_image1", gray_image2);
	// 		cout<<"1"<<endl;
	// 		imshow("output1", output1);
	// 		cout<<"2"<<endl;
	// 		imshow("blob_window", blob_window);
	// 		cout<<"3"<<endl;
	// 		key=waitKey(0);
	// 		// waitKey(1);
	// 		// cout<<"1"<<endl;
	// 		if(key==113 || key==27) exit(1);//either esc or 'q'
	// 		cout<<"loop Done"<<endl;

			
			delete p1;
			delete p2;
		}


		exit(1);//point to exit

		///////////////////////////////////

  //       Mat3b image1;
  //       Mat gray_image1;
	   	
	 //   	image1=imread(argv[2],1);
	 //   	if(!image1.data){printf("Could not open the file\n"); exit(0);}
		// cvtColor(image1,gray_image1, COLOR_BGR2GRAY);//color image1 to gray scale
	 //   	/*****************/

	 //   	/*The Second image process*/
  //  		Mat3b image2;
  //  		Mat gray_image2;
  //  		Mat output;
  //  		Mat3b final_output;
  //  		image2=imread(argv[3],1);
  //  		if(!image2.data){printf("Could not open the file\n"); exit(0);}
  //  		cvtColor(image2,gray_image2, COLOR_BGR2GRAY);//color image2 to gray scale
		
		
		// output=Mat::zeros(gray_image2.size(),IMREAD_GRAYSCALE);//initialize the value of output metrices to zero
		// final_output=Mat::zeros(gray_image2.size(),CV_8UC3);//initialize the value of final_output metrices to zero

		// gray_image2=Mat::ones(gray_image2.size(),IMREAD_GRAYSCALE);//initialize the value of output metrices to zero
		// Mat gray_image3=Mat::ones(gray_image2.size(),IMREAD_GRAYSCALE);//initialize the value of output metrices to zero
		// gray_image2=gray_image2+gray_image3;




		// // //////////////////////////////////////
		// // //Kuwahara filter using Summed-table//
		// // //////////////////////////////////////

		// // /*Memory Allocation*/
		// // double** integral_image=new double*[gray_image2.cols+1];
		// // double** squared_integral_image=new double*[gray_image2.cols+1];
		// // for(int i = 0; i < gray_image2.cols+1; ++i){
		// // 	integral_image[i] = new double[gray_image2.rows+1];
		// // 	squared_integral_image[i] = new double[gray_image2.rows+1];
		// // }
		// // /*********************/

		// // Integral_Gray_Initialize(gray_image2,integral_image,squared_integral_image);//create summed-table to integral_image array.
		// // Integral_test(gray_image2,integral_image,squared_integral_image);//create summed-table to integral_image array.
		
		// // Kuwahara_Filter_Gray_Without_Sum_Table(gray_image2,output,7);//Applying kuwahara filter to output using integral_image.
		
		// // /*Memory deallocation*/
		// // for(int i = 0; i < gray_image1.cols+1; ++i) {
		// // 	delete [] integral_image[i];
		// // 	delete [] squared_integral_image[i];
		// // }
		// // delete [] integral_image;
		// // delete [] squared_integral_image;
		// // /***************/
		
		// // ///////////////////////////////////////


		


		// //////////////////////////////////////
		// //Kuwahara filter using Summed-table with colour//
		// //////////////////////////////////////

		// /*Memory Allocation*/
		// double** integral_image_R=new double*[gray_image2.cols+1];
		// double** squared_integral_image_R=new double*[gray_image2.cols+1];
		// double** integral_image_G=new double*[gray_image2.cols+1];
		// double** squared_integral_image_G=new double*[gray_image2.cols+1];
		// double** integral_image_B=new double*[gray_image2.cols+1];
		// double** squared_integral_image_B=new double*[gray_image2.cols+1];

		// for(int i = 0; i < gray_image2.cols+1; ++i){
		// 	integral_image_R[i] = new double[gray_image2.rows+1];
		// 	squared_integral_image_R[i] = new double[gray_image2.rows+1];
		// 	integral_image_G[i] = new double[gray_image2.rows+1];
		// 	squared_integral_image_G[i] = new double[gray_image2.rows+1];
		// 	integral_image_B[i] = new double[gray_image2.rows+1];
		// 	squared_integral_image_B[i] = new double[gray_image2.rows+1];
		// }
		// /*********************/
		// Mat3b color_output;
		// color_output=Mat::zeros(gray_image2.size(),CV_8UC3);//initialize the value of final_output metrices to zero
		// Integral_Color_Initialize(image2,integral_image_B,integral_image_G,integral_image_R,squared_integral_image_B,squared_integral_image_G,squared_integral_image_R);//create summed-table to integral_image array.
		// Kuwahara_Filter_Color_With_Sum_Table(image2,color_output,integral_image_B,integral_image_G,integral_image_R,squared_integral_image_B,squared_integral_image_G,squared_integral_image_R,23);//Applying kuwahara filter to output using integral_image.
		


		// // color_output
		// Mat3b temp_output=Mat::zeros(image1.size(),CV_8UC3);//initialize the value of final_output metrices to zero
		// for(int i = 0; i < color_output.cols; ++i){
		// 	for(int j = 0; j < color_output.rows; ++j){
		// 		// if(Mpixel(filtered_image,x,y)!=0){//when pixel is not zero
		// 			pixelB(temp_output,i,j)=pixelB(image1,i,j);
		// 			pixelG(temp_output,i,j)=pixelG(image1,i,j);
		// 			pixelR(temp_output,i,j)=pixelR(image1,i,j);
		// 		// }

		// 	}
		// }

		// // Integral_Color_Initialize(image2,integral_image_R,integral_image_B,integral_image_G,squared_integral_image_R,squared_integral_image_B,squared_integral_image_G);//create summed-table to integral_image array.
		// // Kuwahara_Filter_Color_With_Sum_Table(image2,color_output,integral_image_R,integral_image_B,integral_image_G,squared_integral_image_R,squared_integral_image_B,squared_integral_image_G,7);//Applying kuwahara filter to output using integral_image.
		// /*Memory deallocation*/
		// for(int i = 0; i < gray_image1.cols+1; ++i) {
		// 	delete [] integral_image_R[i];
		// 	delete [] squared_integral_image_R[i];
		// 	delete [] integral_image_G[i];
		// 	delete [] squared_integral_image_G[i];
		// 	delete [] integral_image_B[i];
		// 	delete [] squared_integral_image_B[i];
		// }
		// delete [] integral_image_R;
		// delete [] squared_integral_image_R;
		// delete [] integral_image_G;
		// delete [] squared_integral_image_G;
		// delete [] integral_image_B;
		// delete [] squared_integral_image_B;
		// /***************/
		
		// ///////////////////////////////////////

		// imwrite("temp_output.jpg", temp_output);

		// ////////////////////////////////////////
		// //Kuwahara filter without Summed-table//
		// ////////////////////////////////////////
		
		// // Filter_Gray(gray_image2,output,15);
		
		// ///////////////////////////////////////
		// namedWindow("image2",WINDOW_NORMAL);
		// namedWindow("color_output",WINDOW_NORMAL);
		// resizeWindow("color_output", 1200,1200);
		// resizeWindow("image2", 1200,1200);
		// imshow("color_output" ,color_output);
		// imshow("image2" ,image2);
		// // imwrite( "lee_eun.jpg", color_output );
		// waitKey(0);
		// exit(0);

		// /*subtraction process between The first image and the second image*/
		// for(int x=0; x<gray_image2.cols;x++){
		// 	for(int y=0; y<gray_image2.rows;y++){
		// 		// Mpixel(output,x,y)=(float)Mpixel(gray_image1,x,y)-(float)Mpixel(gray_image2,x,y);
		// 		Mpixel(output,x,y)=(float)Mpixel(gray_image2,x,y)-(float)Mpixel(gray_image1,x,y);
		// 		// cout<<"x:"<<x<<" y: "<<y<<endl;

		// 		if(Mpixel(output,x,y)<100){//thresholding
		// 			Mpixel(output,x,y)=0;
		// 		}
		// 	}
		// }


		// /*****************************************************************/

		// /*Load the subtracted area to final_output from source*/
		// for(int x=0; x<gray_image2.cols;x++){
		// 	for(int y=0; y<gray_image2.rows;y++){
		// 		if((int)Mpixel(output,x,y)!=0){
		// 			pixelB(final_output,x,y)=pixelB(image2,x,y);
		// 			pixelG(final_output,x,y)=pixelG(image2,x,y);
		// 			pixelR(final_output,x,y)=pixelR(image2,x,y);
		// 		}

		// 	}
		// }


		// // Scalar color=CV_RGB(255,0,0);
		// // std::vector<vector<Point>>contours;
		// // findContours(output,contours,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_NONE);

        
  // //       int largestcontour=FindTheLargestContour(contours);
  // //       Mat drawing=Mat::zeros(output.size(),CV_8UC3);
		// // drawContours(drawing,contours,largestcontour,color,2,8);
		// /*******************************************************/

  //   	/*final_output show*/
  //   	imshow("final_output" ,final_output);
  //   	// imshow("filtered image" ,output);
  //   	// imshow("gray_image2" ,gray_image2);
  //   	// imshow("drawing" ,drawing);



  //   	waitKey(0);
    	/*************/

	}else{//dynamic mode with camera
		/*Information*/
		cout<<"dynamic mode"<<endl;
		// const string about =
	 //        "This is Mastewr Project in Massey University.\n"
	 //        "The example file can be downloaded from:\n"
	 //        "https://github.com/ShimQ88/Kuwahara-filter";
	 //    const string keys =
	 //        "{ h help |      | print this help message }"
	 //        "{ @image |<none>| path to image file }";
  //       CommandLineParser parser(argc, argv, keys);
	 //    parser.about(about);
	 //    if (parser.has("help"))
	 //    {
	 //        parser.printMessage();
	 //        return 0;
	 //    }
  //       string filename = parser.get<string>("@image");
	 //    if (!parser.check())
	 //    {
	 //        parser.printErrors();
	 //        return 0;
	 //    }

		/*Camera setup*/
		VideoCapture cap;
	  	cap.open(0);
	 	if (!cap.isOpened()){
	        cout << "Failed to open camera" << endl;
	        return 0;
	    }

	    cout << "Opened camera" << endl;
	    int cap_width=640;
	    int cap_height=480;
	   	cap.set(CAP_PROP_FRAME_WIDTH, cap_width);
        cap.set(CAP_PROP_FRAME_HEIGHT, cap_height);
        /**************/

        /*capture*/
     //    const string about =
	    //     "This sample demonstrates the camshift algorithm.\n"
	    //     "The example file can be downloaded from:\n"
	    //     "  https://www.bogotobogo.com/python/OpenCV_Python/images/mean_shift_tracking/slow_traffic_small.mp4";
	    // const string keys =
	    //     "{ h help |      | print this help message }"
	    //     "{ @image |<none>| path to image file }";
     //    CommandLineParser parser(argc, argv, keys);
	    // parser.about(about);
	    // if (parser.has("help"))
	    // {
	    //     parser.printMessage();
	    //     return 0;
	    // }
     //    string filename = parser.get<string>("@image");
	    // if (!parser.check())
	    // {
	    //     parser.printErrors();
	    //     return 0;
	    // }
	    // VideoCapture cap(filename);
	    // if (!cap.isOpened()){
	    //     //error in opening the video input
	    //     cerr << "Unable to open file!" << endl;
	    //     return 0;
	    // }

        /*The First image*/
        Mat3b image1;
        Mat gray_image1;
	   	int key=0;
	   	cap >> image1;
	   	if(!image1.data){printf("Could not open the file\n"); exit(0);}
	   	cvtColor(image1,gray_image1, COLOR_BGR2GRAY);//copy camera color image to gray scale
	   	/*****************/

	   	//////////////////////////////////////
		//Kuwahara filter using Summed-table//
		//////////////////////////////////////
    	
    	

		

	   	/*The Second image process*/
	   	int j=0;
	   	while (1){
	   		system_clock::time_point start = system_clock::now();//start clock
	   		
	   		Mat image2;
	   		Mat gray_image2;
	   		Mat output;
	   		Mat output1;
	   		Mat output2;
	   		Mat3b final_output;
	   		cap >> image2;
			
			cvtColor(image2,gray_image2, COLOR_BGR2GRAY);//copy camera color image to gray scale
			
			if(!image2.data){printf("Could not open the file\n"); exit(0);}
			output=Mat::zeros(gray_image2.size(),IMREAD_GRAYSCALE);//initialize the value of output metrices to zero
			output1=Mat::zeros(gray_image1.size(),IMREAD_GRAYSCALE);//initialize the value of output metrices to zero
			output2=Mat::zeros(gray_image2.size(),IMREAD_GRAYSCALE);//initialize the value of output metrices to zero
    		final_output=Mat::zeros(gray_image2.size(),CV_8UC3);//initialize the value of final_output metrices to zero

    		//////////////////////////////////////
    		//Kuwahara filter using Summed-table//
    		//////////////////////////////////////

			/*Memory Allocation*/
			double** integral_image1=new double*[gray_image1.cols+1];
			double** squared_integral_image1=new double*[gray_image1.cols+1];

			double** integral_image2=new double*[gray_image2.cols+1];
			double** squared_integral_image2=new double*[gray_image2.cols+1];
			for(int i = 0; i < gray_image2.cols+1; ++i){
				integral_image1[i] = new double[gray_image1.rows+1];
				squared_integral_image1[i] = new double[gray_image1.rows+1];

				integral_image2[i] = new double[gray_image2.rows+1];
				squared_integral_image2[i] = new double[gray_image2.rows+1];
			}
    		/*********************/

    		Integral_Gray_Initialize(gray_image2,integral_image2,squared_integral_image2);//create summed-table to integral_image array.
    		Kuwahara_Filter_Gray_With_Sum_Table(gray_image2,output2,integral_image2,squared_integral_image2,11);//Applying kuwahara filter to output using integral_image.
    		Integral_Gray_Initialize(gray_image1,integral_image1,squared_integral_image1);//create summed-table to integral_image array.
    		Kuwahara_Filter_Gray_With_Sum_Table(gray_image1,output1,integral_image1,squared_integral_image1,11);//Applying kuwahara filter to output using integral_image.
		
			/*Memory deallocation*/
			for(int i = 0; i < gray_image1.cols+1; ++i) {
				delete [] integral_image1[i];
				delete [] squared_integral_image1[i];

				delete [] integral_image2[i];
				delete [] squared_integral_image2[i];
			}
			delete [] integral_image1;
			delete [] squared_integral_image1;
			delete [] integral_image2;
			delete [] squared_integral_image2;
			/***************/
			
			///////////////////////////////////////



			////////////////////////////////////////
    		//Kuwahara filter without Summed-table//
    		////////////////////////////////////////
    		
    		// Filter_Gray(gray_image2,output,5);

    		
    		///////////////////////////////////////
    		

			/*subtraction process between The first image and the second image*/
			output=gray_image1-gray_image2;
			/********************************/
			
			/***Cropping Object by outline of object***/
			Mat temp_window=Mat::zeros(image2.size(),IMREAD_GRAYSCALE);//gray case
			Image_stitching(gray_image2,output,temp_window);

			

			/******************************************/

			/***ROI Section***/
			int small_x,small_y,large_x,large_y;//size of
			Determining_ROI_Size(temp_window,&small_x,&small_y,&large_x,&large_y);
			// median_filter(temp_window,temp_window,15);
			Rect window(small_x, small_y, large_x-small_x, large_y-small_y); // simply hardcoded the values
			rectangle(final_output, window, Scalar(0, 255, 255), 2);
				

			int x_value=(large_x-small_x-small_x);
			int y_value=(large_y-small_y-small_y);
			int trigger=x_value+y_value;
			
			/*Debugging for values*/
			// cout<<"1.small_x:"<<small_x<<endl;
			// cout<<"1.large_x:"<<large_x<<endl;
			// cout<<"2.small_y:"<<small_y<<endl;
			// cout<<"2.large_y:"<<large_y<<endl;
			/**********************/

			/*Mean shift part*/
			if( trigger>200 ){//trigger to work meanshift.(question1)
				cout<<"mean shift working"<<endl;
				namedWindow(window_detection_name);
				namedWindow(window_capture_name);
				createTrackbar("Low H", window_detection_name, &low_H, max_value_H, on_low_H_thresh_trackbar);
			    createTrackbar("High H", window_detection_name, &high_H, max_value_H, on_high_H_thresh_trackbar);
			    createTrackbar("Low S", window_detection_name, &low_S, max_value, on_low_S_thresh_trackbar);
			    createTrackbar("High S", window_detection_name, &high_S, max_value, on_high_S_thresh_trackbar);
			    createTrackbar("Low V", window_detection_name, &low_V, max_value, on_low_V_thresh_trackbar);
			    createTrackbar("High V", window_detection_name, &high_V, max_value, on_high_V_thresh_trackbar);
   				Mat frame, frame_HSV, frame_threshold;
				// cout<<"1.small_x:"<<small_x<<endl;
				// cout<<"1.large_x:"<<large_x<<endl;
				// cout<<"2.small_y"<<small_y<<endl;
				// cout<<"2.large_y"<<large_y<<endl;
				Rect track_window(small_x, small_y, large_x-small_x, large_y-small_y);
				// rectangle(image2,track_window,Scalar(0,255,0),3);
				float range_1[] = {0, 90};
				// float range_2[] = {0, 90};
				// float range_3[] = {0, 90};
			    const float* range[] = {range_1};
			    Mat roi_hist;
			    Mat roi, mask;

			    Mat hsv;
			    cvtColor(image2, frame_HSV, COLOR_BGR2HSV);
			    cvtColor(image2, hsv, COLOR_BGR2HSV);
			    // inRange(frame_HSV, Scalar(low_H, low_S, low_V), Scalar(high_H, high_S, high_V), frame_threshold);


				Mat mask1, mask2;
			    // inRange(hsv, Scalar(161, 155, 84), Scalar(179, 255, 255), mask1);
			    // inRange(hsv, Scalar(179, 255, 255), Scalar(180, 255, 255), mask2);
			    // inRange(hsv, Scalar(94, 80, 2), Scalar(126, 255, 255), mask);
			    // mask = mask1 + mask2;
				// mask=zeros(image2.size(),IMREAD_GRAYSCALE);

			    // Mat mask;
			    roi = image2(track_window);
			    // roi = image2(window);
			    int histSize[] = {180};
			    int channels[] = {0};
			    // calcHist(&image2, 1, channels, mask, roi_hist, 1, histSize, range);
			    calcHist(&hsv, 1, channels, mask, roi_hist, 1, histSize, range);
			    normalize(roi_hist, roi_hist, 0, 255, NORM_MINMAX);
			    TermCriteria term_crit(TermCriteria::EPS | TermCriteria::COUNT, 100, 1);
				int k=0;
				while(1){
					k++;
					system_clock::time_point start = system_clock::now();//start clock
					Mat hsv1, dst;
			        cap >> image2;
			        if (image2.empty())
			            break;
			        cvtColor(image2, hsv, COLOR_BGR2HSV);
			        // calcBackProject(&image2, 1, channels, roi_hist, dst, range);
			        calcBackProject(&hsv, 1, channels, roi_hist, dst, range);
			        
			        // apply camshift to get the new location
			        RotatedRect rot_rect = CamShift(dst, track_window, term_crit);
			        
			        // Draw it on image
			        Point2f points[4];
			        rot_rect.points(points);
			        int loop_breaker=0;

			        //find smallest point 
			        Point2f pt_small,pt_large;
			        pt_small.x=9999;
			        pt_small.y=9999;
			        pt_large.x=-9999;
			        pt_large.y=-9999;

			        for (int i = 0; i < 4; i++){
			            // line(image2, points[i], points[(i+1)%4], 255, 2);
			            line(hsv, points[i], points[(i+1)%4], 255, 2);
			            if((points[i].x<0)||(points[i].x>640)||(points[i].y<0)||(points[i].y>480)){
			            	loop_breaker++;
			            }
			            cout<<"P"<<i<<": x,y: "<<points[i].x<<", "<<points[i].y<<endl;

			            if(points[i].x<pt_small.x){
			            	pt_small.x=points[i].x;
			            }

			            if(points[i].y<pt_small.y){
			            	pt_small.y=points[i].y;
			            }

			            if(points[i].x>pt_large.x){
			            	pt_large.x=points[i].x;
			            }

			            if(points[i].y>pt_large.y){
			            	pt_large.y=points[i].y;
			            }

			        }
			        // Rect t_rec(points[3].x,points[3].y,points[1].x-points[3].x,points[1].y-points[3].y);
			        Rect t_rec(pt_small.x, pt_small.y, pt_large.x-pt_small.x, pt_large.y-pt_small.y);
			        
			        // rectangle(image2,t_rec,Scalar(0,255,0),3);
			        rectangle(hsv,t_rec,Scalar(0,255,0),3);

			        cout<<"small"<<": x,y: "<<pt_small.x<<", "<<pt_small.y<<endl;
			        cout<<"large"<<": x,y: "<<pt_large.x<<", "<<pt_large.y<<endl;
			        
			        Point center_of_rect = (t_rec.br() + t_rec.tl())*0.5;

			        

    				// circle(image2,center_of_rect,3,Scalar(0,0,255));
    				circle(hsv,center_of_rect,3,Scalar(0,0,255));

			        // if(loop_breaker>=){//breaking rule (question2)
			        // 	break;
			        // }
			     
			        string directory_name="captured_image/"+to_string(k)+".jpg";
			        string file_name="captured_image/"+to_string(k)+".txt";

			        float file_width=abs(points[2].x-points[0].x)/cap_width;
			        float file_height=abs(points[2].y-points[0].y)/cap_height;
			        float file_x=center_of_rect.x/cap_width;
			        float file_y=center_of_rect.y/cap_height;
			        string file_content="0, "+to_string(file_x)+", "
			        +to_string(file_y)+", "+to_string(file_width)+", "
			        +to_string(file_height)+'\n';

			        ofstream myfile (file_name);
					if (myfile.is_open())
					{
						myfile << file_content;
					    myfile.close();
					}

			        imshow("object following window", image2);
			        // imshow("object following window", hsv1);
			        imshow("hsv", hsv);
			        // imshow("mask2", mask2);
			        // imshow("mask1", mask1);
			        // imshow("mask", mask);
			        imshow("dst of object", dst);
			        // cvtColor(image2, frame_HSV, COLOR_BGR2HSV);
			        // inRange(frame_HSV, Scalar(low_H, low_S, low_V), Scalar(high_H, high_S, high_V), frame_threshold);
			        // imshow(window_detection_name, frame_threshold);
			        // imshow("hellow world", frame_HSV);
			        // imwrite(directory_name, image2);//write
			        // imshow("roi", roi);
			        
			        key=waitKey(1);
			        system_clock::time_point end = system_clock::now();
			       	double seconds = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
			     	fps = 1000000/seconds;
			     	cout << "frames " << fps << " seconds " << seconds << endl;
				}
			}
			/**********************/

			Grey_to_Color(image2,temp_window,final_output);
	    	// imshow("temp_window" ,temp_window);
	    	imshow("final_output" ,final_output);
	    	// imshow("Gray_output" ,output);

	    	// imshow("image1" ,output1);
	    	// imshow("image2" ,output2);
	    	/*************/
	    	
	    	/*program termination*/
	    	key=waitKey(1);
	       	if(key==113 || key==27) return 0;//either esc or 'q'
	       	/****************/

	       	/*Caculate performance of program*/
	       	system_clock::time_point end = system_clock::now();
	       	double seconds = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
	     	fps = 1000000/seconds;
	     	cout << "frames " << fps << " seconds " << seconds << endl;
	     	/*********************************/
	     	
	     	/***Delay for image1***/
	     	// j++;
	     	// if(j==3){
	     	// 	image1=image2;
	     	// 	cvtColor(image1,gray_image1, CV_BGR2GRAY);//copy camera color image to gray scale
	     	// 	j=0;
	     	// }
	     	/**********************/
	     	image1=image2;
	     	cvtColor(image1,gray_image1, COLOR_BGR2GRAY);//copy camera color image to gray scale
	    }
	    /************/
	}
}

