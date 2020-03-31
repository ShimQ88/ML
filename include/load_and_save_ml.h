#ifndef LOAD_AND_SAVE_ML_H
#define LOAD_AND_SAVE_ML_H

// System Headers
#include <iostream>
#include <cstdio>
#include <vector>

// Opencv Headers
#include "opencv2/core/core.hpp"
#include "opencv2/ml/ml.hpp"
#include "ml.h"

//namespace
using namespace std;
using namespace cv;
using namespace cv::ml;

// Project Headers
// #include "load_and_save_classifier.h"

// --------------------------------------------------------------------------------
// Image Functions
// --------------------------------------------------------------------------------
bool
load_and_save_ml( const string& data_filename,
                      const string& filename_to_save,
                      const string& filename_to_load,
                      float percent_of_division,
                      int ml_technique);
bool
read_num_class_data( const string& filename, int var_count,
                     Mat* _data, Mat* _responses );

void help(bool swap_the_role_train_to_test,int ntrain_samples, int ntest_samples, int ml_technique);

#endif // ADA_BOOST_H