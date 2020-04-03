#ifndef ML_H
#define ML_H

// System Headers
#include <iostream>
#include <cstdio>
#include <vector>

// Opencv Headers
#include "opencv2/core/core.hpp"
#include "opencv2/ml/ml.hpp"

//namespace
using namespace std;
using namespace cv;
using namespace cv::ml;

// Project Headers
// #include "load_and_save_classifier.h"

// --------------------------------------------------------------------------------
// Image Functions
// --------------------------------------------------------------------------------
Mat test_and_save_classifier(const Ptr<StatModel>& model,
                                     const Mat& data, const Mat& responses,
                                     int ntrain_samples, int rdelta,
                                     const string& filename_to_save);

inline TermCriteria TC(int iters, double eps);

template<typename T>
Ptr<T> load_classifier(const string& filename_to_load);

float Accuracy_Calculation(const Mat& confusion_matrix);
bool ModifyFile(ofstream &file_the_best,float mean, float variance,float sta_dev,int k_fold_value,Mat con_mat[]);

bool Calculate_standard_deviation(float *mean,float *variance,float *sta_dev,float temp_accuracy[],float sum_accuracy,int k_fold_value);

bool Split_train_test_data(Mat *train_data, Mat *test_data, Mat *train_responses, Mat *test_responses,
                        Mat *train_responses_int, Mat *test_responses_int, Mat data, Mat responses,
                        int block,int the_number_of_data, int n_total_samples, int ntest_samples);
bool
build_mlp_classifier(   Mat data,
                        Mat responses,
                        int ntrain_samples,
                        int ntest_samples,
                        const string& filename_to_save,
                        const string& filename_to_load);

bool 
build_boost_classifier( Mat data,
                        Mat responses,
					    int ntrain_samples,
					    int ntest_samples,
						const string& filename_to_save,
						const string& filename_to_load);

bool
build_random_forest_classifier( Mat data,
                                Mat responses,
                                int ntrain_samples,
                                int ntest_samples,
                                const string& filename_to_save,
                                const string& filename_to_load);
// bool
// build_boost_classifier();

//MultiLayer perceptron
// bool
// build_mlp_classifier();

#endif