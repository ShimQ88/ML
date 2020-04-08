#ifndef CLASS_H
#define CLASS_H

// System Headers
#include <iostream>
#include <fstream>
#include <cmath>

// Opencv Headers
#include "opencv2/core/core.hpp"
#include "opencv2/ml/ml.hpp"

//namespace
using namespace std;
using namespace cv;
using namespace cv::ml;
// enum  	ActivationFunctions {
//   IDENTITY = 0,
//   SIGMOID_SYM = 1,
//   GAUSSIAN = 2,
//   RELU = 3,
//   LEAKYRELU = 4
// }
 
// enum  	TrainFlags {
//   UPDATE_WEIGHTS = 1,
//   NO_INPUT_SCALE = 2,
//   NO_OUTPUT_SCALE = 4
// }
 
// enum  	TrainingMethods {
//   BACKPROP =0,
//   RPROP = 1,
//   ANNEAL = 2
// }


class Neural_Network{
private:
	int index;
	int t_method;
	String t_method_name;// { BACKPROP =0, RPROP = 1, ANNEAL = 2 }
	int a_function;
	String a_function_name;// { IDENTITY = 0, SIGMOID_SYM = 1, GAUSSIAN = 2, RELU = 3, LEAKYRELU = 4 }
	double method_param;
	int max_iter;
	int class_count;
	float accuracy;
	Mat confusion_matrix;
public:
	Neural_Network();
	void Reset_Parameters( int p_index,
					int p_t_method,
					int p_a_function,
					double p_method_param,
					int p_max_iter,
					int p_class_count,
					float p_accuracy,
					Mat p_confusion_matrix);
	void Set_Accuracy(float p_accuracy);
	float Get_Accuracy();
	Mat Get_Confusion_Matrix();
	String Return_Parameter();
	String Return_Accuracy();
	String Return_Accuracy2();
};
#endif // CLASS_H

Neural_Network::Neural_Network()//default structure
{
	index=0;
	t_method=ANN_MLP::BACKPROP;
	t_method_name="BACKPROP";

	a_function=ANN_MLP::SIGMOID_SYM;
	a_function_name="SIGMOID_SYM";

	method_param=0.0001;
	max_iter=300;
	class_count=2;
	accuracy=-9999;
	confusion_matrix=Mat::zeros( 2,2,CV_32F );
}
void Neural_Network::Reset_Parameters( int p_index,
								int p_t_method,
								int p_a_function,
								double p_method_param,
								int p_max_iter,
								int p_class_count,
								float p_accuracy,
								Mat p_confusion_matrix){
	
	String p_a_function_name[5];
	p_a_function_name[0]="IDENTITY";
	p_a_function_name[1]="SIGMOID_SYM";
	p_a_function_name[2]="GAUSSIAN";
	p_a_function_name[3]="RELU";
	p_a_function_name[4]="LEAKYRELU";

	String p_t_method_name[3];
	p_t_method_name[0]="BACKPROP";
	p_t_method_name[1]="RPROP";
	p_t_method_name[2]="ANNEAL";

	index=p_index;

	t_method=p_t_method;
	t_method_name=p_t_method_name[t_method];

	a_function=p_a_function;
	a_function_name=p_a_function_name[a_function];

	method_param=p_method_param;
	max_iter=p_max_iter;
	class_count=p_class_count;
	accuracy=p_accuracy;
	confusion_matrix=p_confusion_matrix;
}

void Neural_Network::Set_Accuracy(float p_accuracy){
	accuracy=p_accuracy;
}

float Neural_Network::Get_Accuracy(){
	return accuracy;
}

Mat Neural_Network::Get_Confusion_Matrix(){
	return confusion_matrix;
}

String Neural_Network::Return_Parameter(){
	char buffer[50];
	sprintf(buffer, "%d, %d, %d, %lf, %d, %d, %f \n", index, t_method, a_function, method_param, max_iter, class_count, accuracy);  //header
	return buffer;
}

String Neural_Network::Return_Accuracy(){
	char buffer[50];
	sprintf(buffer, "%d, %f \n", index,accuracy);  //header
	return buffer;
}

String Neural_Network::Return_Accuracy2(){
	char buffer[50];
	sprintf(buffer, "%f \n", accuracy);  //header
	return buffer;
}