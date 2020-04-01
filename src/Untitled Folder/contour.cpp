#include "contour.h" 

template<typename T>
static Ptr<T> load_classifier(const string& filename_to_load){
    // load classifier from the specified file
    Ptr<T> model = StatModel::load<T>( filename_to_load );
    if( model.empty() )
        cout << "Could not read the classifier " << filename_to_load << endl;
    else
        cout << "The classifier " << filename_to_load << " is loaded.\n";

    return model;
}


void EllipticFourierDescriptors(std::vector<Point>& contour, std::vector<float> &CE){
	vector<float>ax,ay,bx,by;
	int m=contour.size();
	int n=n_CE;//number of CEs we are interested in computing
	float t=(2*PI)/m;
	for(int k=0;k<n;k++){
		ax.push_back(0.0);
		ay.push_back(0.0);
		bx.push_back(0.0);
		by.push_back(0.0);
		for(int i=0;i<m;i++){
			ax[k]=ax[k]+contour[i].x*cos((k+1)*t*(i));
			bx[k]=bx[k]+contour[i].x*sin((k+1)*t*(i));
			ay[k]=ay[k]+contour[i].y*cos((k+1)*t*(i));
			by[k]=by[k]+contour[i].y*sin((k+1)*t*(i));
		}
	ax[k]=(ax[k])/m;
	bx[k]=(bx[k])/m;
	ay[k]=(ay[k])/m;
	by[k]=(by[k])/m;
	}
	for(int k=0; k<n; k++){
		CE.push_back(sqrt((ax[k]*ax[k]+ay[k]*ay[k])/(ax[0]*ax[0]+ay[0]*ay[0]))+sqrt((bx[k]*bx[k]+by[k]*by[k])/(bx[0]*bx[0]+by[0]*by[0])));
	}
	for(int count=0; count<n&&count<CE.size(); count++){
		// printf("%d CE %f ax %f ay %f bx %f by %f\n",count,CE[count],ax[count],ay[count],bx[count],by[count]);
	}
}
void SkinTresholding(Mat3b& frame){
	for(int i=0; i<frame.rows; ++i){
		for(int j=0; j<frame.cols; ++j){
			if( (frame(i,j)[0]>=0) && (frame(i,j)[0] < 21) && (frame(i,j)[1]>0) && (frame(i,j)[1]<300) && (frame(i,j)[2]>51) && (frame(i,j)[2]<400) ){
				//Doing nothing
			}else{
				for(int k=0; k<3; ++k){
					frame(i,j)[k] = 0;	
				}	
			}
		}
	}
}

int FindTheLargestContour(std::vector<vector<Point>>contours){
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
void contour(const string& dataset) {

	cout<<"training mode"<<endl;
		
	Mat3b frame;

	int size1=strlen(dataset);
	char name[size1+2];

	strcpy(name,dataset);
	strcat(name,".txt");
	ofstream myfile (name);
	if (myfile.is_open()){
	    
	}else{
		cout << "Unable to open file";
		exit(0);
	}
	int size=strlen(dataset);
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

	for(unsigned int i=0; i<glob_result.gl_pathc; ++i){
	  	cout << glob_result.gl_pathv[i] << endl;
	  	frame=imread(glob_result.gl_pathv[i],1);
		if(!frame.data){printf("Error\n"); exit(0);}
		cvtColor(frame, frame, CV_BGR2HSV);
		// GaussianBlur(frame, frame, Size(7,7), 1, 1);

		//process for make color black for except color skin
		// SkinTresholding(frame);
		
		//BGR change and threshold
		Mat1b frame_gray;
		cvtColor(frame, frame, CV_HSV2BGR);
		cvtColor(frame, frame_gray, CV_BGR2GRAY);


		// medianBlur(frame_gray,frame_gray,7);
		cvtColor(frame, frame, CV_BGR2HSV);


		
		std::vector<vector<Point>>contours;
		findContours(frame_gray,contours,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_NONE);
		Mat drawing=Mat::zeros(frame_gray.size(),CV_8UC3);
		
		int largestcontour=FindTheLargestContour(contours);

		
		Scalar color=CV_RGB(255,0,0);
		drawContours(drawing,contours,largestcontour,color,2,8);

		vector<float>CE;
		EllipticFourierDescriptors(contours[largestcontour],CE);
		myfile << "0,";
		for(int i=1;i<n_CE;i++){
			myfile << CE[i];
			if(i==n_CE-1){
				myfile << "\n";
			}else{
				myfile << ",";
			}
	    	
		}
		

		// Ptr<ANN_MLP> model;
		// model = load_classifier<ANN_MLP>("example.xml");

		// Mat sample1 = (Mat_<float>(1,9) << CE[1], CE[2], CE[3], CE[4], CE[5], CE[6], CE[7], CE[8], CE[9]);// 1
		// float r = model->predict( sample1 );

		// string value="this is gesture: "+to_string((int)r);
		// putText(drawing, value, cvPoint(10,30), FONT_HERSHEY_PLAIN, 2, cvScalar(0,255,255), 2, 8);

		
		
		// cout<<"value of sample:"<<r<<endl;
		namedWindow("contours", 0);
		namedWindow("real_frame", 0);
		// namedWindow("gray_frame", 0);
		imshow("contours" ,drawing);
		imshow("real_frame",frame);
        // imshow("gray_frame" ,gray_frame);
		waitKey(0);
		
	}
    myfile.close();
}
