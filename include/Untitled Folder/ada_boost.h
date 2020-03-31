#ifndef ADA_BOOST_H
#define ADA_BOOST_H

// System Headers
#include <iostream>
#include <cstdio>
#include <vector>

// Opencv Headers
#include "opencv2/core/core.hpp"
#include "opencv2/ml/ml.hpp"
#include "load_and_save_classifier.h"

//namespace
using namespace std;
using namespace cv;
using namespace cv::ml;

// Project Headers
// #include "load_and_save_classifier.h"

// --------------------------------------------------------------------------------
// Image Functions
// --------------------------------------------------------------------------------
// bool read_num_class_data( const string& filename, int var_count, Mat* _data, Mat* _responses );

// template<typename T> 
// Ptr<T> load_classifier(const string& filename_to_load);

// inline TermCriteria TC(int iters, double eps);

// void test_and_save_classifier(const Ptr<StatModel>& model, const Mat& data, const Mat& responses,int ntrain_samples, int rdelta,const string& filename_to_save,float* accuracy);

// void help(bool swap_the_role_train_to_test,int ntrain_samples, int ntest_samples);

bool 
build_boost_classifier( const string& data_filename,
                      const string& filename_to_save,
                      const string& filename_to_load,
                      float percent_of_division,
                      bool swap_the_role_train_to_test,float *accuracy,float *value);

#endif // ADA_BOOST_H