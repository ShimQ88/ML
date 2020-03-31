// #include "/home/q/Desktop/Master project/ML/include/load_and_save_classifier.h"


// System Headers
#include <iostream>
#include <cstdio>
#include <vector>

// Opencv Headers
#include "opencv2/core/core.hpp"
#include "opencv2/ml/ml.hpp"
// #include "load_and_save_classifier.h"

//namespace
using namespace std;
using namespace cv;
using namespace cv::ml;
// Project Headers
// #include "ada_boost.h"



// // This function reads data and responses from the file <filename>
// bool
// read_num_class_data( const string& filename, int var_count,
//                      Mat* _data, Mat* _responses )
// {
//     const int M = 1024;
//     char buf[M+2];

//     Mat el_ptr(1, var_count, CV_32F);
//     int i;
//     vector<int> responses;

//     _data->release();
//     _responses->release();

//     FILE* f = fopen( filename.c_str(), "rt" );
//     if( !f )
//     {
//         cout << "Could not read the database " << filename << endl;
//         return false;
//     }

//     for(;;)
//     {
//         char* ptr;
//         if( !fgets( buf, M, f ) || !strchr( buf, ',' ) )
//             break;
//         responses.push_back(buf[0]);
//         //char test;
//         //test=buf[0]+65;
//         //responses.push_back(test);
//         cout << "responses " << buf[0] << " " ;;//<<  endl;
//         ptr = buf+2;
//         for( i = 0; i < var_count; i++ )
//         {
//             int n = 0;
//             sscanf( ptr, "%f%n", &el_ptr.at<float>(i), &n );
//             ptr += n + 1;
//         }
//         cout << el_ptr << endl;
//         if( i < var_count )
//             break;
//         _data->push_back(el_ptr);
//     }
//     fclose(f);
//     Mat(responses).copyTo(*_responses);

//     cout << "The database " << filename << " is loaded.\n";

//     return true;
// }

// template<typename T>
// Ptr<T> load_classifier(const string& filename_to_load)
// {
//     // load classifier from the specified file
//     Ptr<T> model = StatModel::load<T>( filename_to_load );
//     if( model.empty() )
//         cout << "Could not read the classifier " << filename_to_load << endl;
//     else
//         cout << "The classifier " << filename_to_load << " is loaded.\n";

//     return model;
// }

// inline TermCriteria TC(int iters, double eps)
// {
//     return TermCriteria(TermCriteria::MAX_ITER + (eps > 0 ? TermCriteria::EPS : 0), iters, eps);
// }

// void test_and_save_classifier(const Ptr<StatModel>& model,
//                                      const Mat& data, const Mat& responses,
//                                      int ntrain_samples, int rdelta,
//                                      const string& filename_to_save, float* accuracy)
// {
//     // int nsamples_all = data.rows;
//     int nsamples_all = ntrain_samples;
//     double train_hr = 0, test_hr = 0;
//     int training_correct_predict=0;
//     // compute prediction error on training data
//     // for(int i=0; i<nsamples_all; i++){
//     for(int i=0; i<nsamples_all; i++){
//         Mat sample = data.row(i);
//         cout << "Sample: " << responses.at<int>(i)-48 << " row " << data.row(i) << endl;
//         float r = model->predict( sample );
//         cout << "Predict:  r = " << round(r) << endl;//rounding in case of random_forest
//         if( (int)round(r) == (int)(responses.at<int>(i)-48) ){ //prediction is correct
//             training_correct_predict++;
//         }
//         // cout << "training_correct_predict = " << training_correct_predict << endl;
//         // getchar();

//         // cout << "Sample: " << responses.at<int>(i) << " row " << data.row(i) << endl;
//         // float r = model->predict( sample );
//         // cout << "Predict:  r = " << r << endl;
//         // if( (int)r == (int)(responses.at<int>(i)) ) //prediction is correct
//         //     training_correct_predict++;
   
//     // r = std::abs(r + rdelta - responses.at<int>(i)) <= FLT_EPSILON ? 1.f : 0.f;
    
     
//         //if( i < ntrain_samples )
//         //    train_hr += r;
//         //else
//         //    test_hr += r;

//     }

//     //test_hr /= nsamples_all - ntrain_samples;
//     //train_hr = ntrain_samples > 0 ? train_hr/ntrain_samples : 1.;
//     printf("ntrain_samples %d training_correct_predict %d \n",ntrain_samples, training_correct_predict);
//     *accuracy=training_correct_predict*100.0/ntrain_samples;

//     // if( filename_to_save.empty() )  printf( "\nTest Recognition rate: training set = %.1f%% \n\n", training_correct_predict*100.0/ntrain_samples);
//     if( filename_to_save.empty() )  printf( "\nTest Recognition rate: training set = %.1f%% \n\n", *accuracy);


//     if( !filename_to_save.empty() )
//     {
//         model->save( filename_to_save );
//     }
// /*************   Example of how to predict a single sample ************************/   
// // Use that for the assignment3, for every frame after computing the features, r is the prediction given the features listed in this format
//     //Mat sample = data.row(i);
//     Mat sample1 = (Mat_<float>(1,9) << 1.52101, 13.64, 4.4899998, 1.1, 71.779999, 0.059999999, 8.75, 0, 0);// 1
//     float r = model->predict( sample1 );
//     cout << "Prediction: " << r << endl;
//     sample1 = (Mat_<float>(1,9) << 1.518, 13.71, 3.9300001, 1.54, 71.809998, 0.54000002, 8.21, 0, 0.15000001);//2
//     r = model->predict( sample1 );
//     cout << "Prediction: " << r << endl;
//     sample1 = (Mat_<float>(1,9) << 1.51694,12.86,3.58,1.31,72.61,0.61,8.79,0,0);//3
//     r = model->predict( sample1 );
//     cout << "Prediction: " << r << endl;
// //    sample1 = (Mat_<float>(1,9) << );//4
// //    r = model->predict( sample1 );
// //    cout << "Prediction: " << r << endl;
//     sample1 = (Mat_<float>(1,9) << 1.5151401, 14.01, 2.6800001, 3.5, 69.889999, 1.6799999, 5.8699999, 2.2, 0);//5
//     r = model->predict( sample1 );
//     cout << "Prediction: " << r << endl;
//     sample1 = (Mat_<float>(1,9) << 1.51852, 14.09, 2.1900001, 1.66, 72.669998, 0, 9.3199997, 0, 0);//6
//     r = model->predict( sample1 );
//     cout << "Prediction: " << r << endl;
//     sample1 = (Mat_<float>(1,9) << 1.51131,13.69,3.2,1.81,72.81,1.76,5.43,1.19,0);//7
//     r = model->predict( sample1 );
//     cout << "Prediction: " << r << endl;
    
// /**********************************************************************/
// }

// void help(bool swap_the_role_train_to_test,int ntrain_samples, int ntest_samples){
//     if(swap_the_role_train_to_test==false){
//         cout<<endl<<endl<<endl<<endl<<endl;
//         cout<<"#########################################################"<<endl;
//         cout<<"######## mode1: train by training sample.        ########"<<endl;
//         cout<<"########       test by testing sample.           ########"<<endl;
//         printf("######## Training the index number from 0 to %d ########\n",ntrain_samples);
//         printf("######## Testing the index number from %d to %d ########\n",ntrain_samples+1, ntrain_samples+ntest_samples);
//         cout<<"#########################################################"<<endl;
//     }else{

//         cout<<endl<<endl<<endl<<endl<<endl;
//         cout<<"#########################################################"<<endl;
//         cout<<"######## mode2: train by testing sample.         ########"<<endl;
//         cout<<"########       test by training sample.          ########"<<endl;
//         printf("######## Training the index number from %d to %d ########\n",ntrain_samples+1, ntrain_samples+ntest_samples);
//         printf("######## Testing the index number from 0 to %d ########\n",ntrain_samples);
//         cout<<"#########################################################"<<endl;
//     }
//     cout<<"Press any key"<<endl;
// }

// #ifndef LOAD_AND_SAVE_CLASSIFIER_H
bool build_boost_classifier( const string& data_filename,
                      const string& filename_to_save,
                      const string& filename_to_load,
                      float percent_of_division,
                      bool swap_the_role_train_to_test,float *accuracy,float *value)
{
    
    const int class_count = 2;//CLASSES
    Mat data;
    Mat responses;
    float accuracy_sample;

    bool ok = read_num_class_data( data_filename, 9, &data, &responses );//third parameter: FEATURES
    if( !ok )
        return ok;

    Ptr<Boost> boost;

    int nsamples_all = data.rows;
    
    /*Division part*/
    int ntrain_samples = (int)(nsamples_all*percent_of_division);//SPLIT
    int ntest_samples = (int)(nsamples_all*(1-percent_of_division));//SPLIT

    // help(swap_the_role_train_to_test,ntrain_samples,ntest_samples);
    getchar();

    // Test number of samples
    // cout<<"ntrain_samples: "<<ntrain_samples<<endl;
    // cout<<"ntest_samples: "<<ntest_samples<<endl;
    // getchar();

    // Create or load MLP classifier

    Mat train_data,train_responses,test_data,test_responses;
    Mat train_responses_int, test_responses_int;

    train_data = data.rowRange(0, ntrain_samples);
    train_responses = Mat::zeros( ntrain_samples, 1, CV_32S );

    // test_data = data.rowRange(ntrain_samples, ntest_samples+ntrain_samples);
    test_data = data.rowRange(ntrain_samples, ntest_samples+ntrain_samples);
    test_responses = Mat::zeros( ntest_samples, 1, CV_32S );
        
    train_responses_int=responses.rowRange(0, ntrain_samples);
    test_responses_int=responses.rowRange(ntrain_samples, ntest_samples+ntrain_samples);
        

    if( !filename_to_load.empty() ){
        // boost = load_classifier<Boost>(filename_to_load);
        boost = StatModel::load<Boost>( filename_to_load );
        if( boost.empty() ){
             cout << "Could not read the classifier " << filename_to_load << endl;
            return false;
        }else{
            cout << "The classifier " << filename_to_load << " is loaded.\n";
        }
        //ntrain_samples = 0;
        if(swap_the_role_train_to_test==true){//trained by test value and test using train value
            test_and_save_classifier(boost, train_data, train_responses_int, ntrain_samples, 0, filename_to_save,accuracy);
        }else{
            test_and_save_classifier(boost, test_data, test_responses_int, ntest_samples, 0, filename_to_save,accuracy);
        }
    }else{
        // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        //
        // MLP does not support categorical variables by explicitly.
        // So, instead of the output class label, we will use
        // a binary vector of <class_count> components for training and,
        // therefore, MLP will give us a vector of "probabilities" at the
        // prediction stage
        //
        // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

        
        //Test debug
        // cout<<"train_data"<<train_data<<endl<<endl;
        // cout<<"responses: "<<responses<<endl;
        // cout<<"test_responses"<<test_responses<<endl;
        // getchar();

        // 1. unroll the responses
        cout << "Unrolling the responses...\n";
        for( int i = 0; i < ntrain_samples+ntest_samples; i++ ){
            int cls_label = responses.at<int>(i) - 48;// - 'A'; //change to numerical classes, still they read as chars
            cout << "labels " << cls_label << endl;
            if(i<ntrain_samples){//train part
                if(cls_label==1){
                    // train_responses.at<float>(i, 0) = 1.f;
                    train_responses.at<int>(i, 0) = 1;
                    // cout<<"ha"<<endl;
                }
                // train_responses.at<float>(i, cls_label) = 1;
            }else{//test part
                if(cls_label==1){
                    // test_responses.at<float>(i-ntrain_samples, 0) = 1.f;
                    test_responses.at<int>(i-ntrain_samples, 0) = 1;
                }
                // test_responses.at<float>(i-ntrain_samples, cls_label) = 1;

            }
            // cout << "train_responses " << train_responses << endl;
        }

        // cout<<"data"<<data<<endl<<endl;
        // getchar();
        // 2. train classifier
#if 1
        // int method = ANN_MLP::BACKPROP;
        // double method_param = 0.001;
        double method_param = 0.0001;
        // int max_iter = 300;
        int max_iter = 3000;
#else
        int method = ANN_MLP::RPROP;
        double method_param = 0.1;
        int max_iter = 1000;
#endif

        Ptr<TrainData> tdata = TrainData::create(train_data, ROW_SAMPLE, train_responses);

        cout << "Training the classifier (may take a few minutes)...\n";
        
        // model->setLayerSizes(layer_sizes);
        // model->setActivationFunction(ANN_MLP::SIGMOID_SYM, 0, 0);
        // // model->setActivationFunction(ANN_MLP::IDENTITY, 0, 0);
        // model->setTermCriteria(TC(max_iter,0));
        // model->setTrainMethod(method, method_param);
        float temp_value;
        float temp_best_accuracy=-9999;
        float temp_pre_accuracy;

        for(int i=0;i<100;i++){
            float temp_float=(float)i/100;
            // cout<<"temp:"<<temp_float<<endl;
            boost = Boost::create();
            boost->setBoostType(Boost::GENTLE);  //Gentle 0.5 and true
            boost->setWeakCount(51);       //the Gentle best=98;
            boost->setWeightTrimRate(0.63);//the Gentle best=0.83;
            boost->setMaxDepth(2);         //the Gentle best=2;
            boost->setUseSurrogates(false);
            boost->setPriors(Mat());
            boost->train(tdata);
            cout << endl;

            if(swap_the_role_train_to_test==true){//trained by test value and test using train value
                test_and_save_classifier(boost, test_data, test_responses_int, ntest_samples, 0, filename_to_save,&temp_pre_accuracy);
            }else{
                test_and_save_classifier(boost, train_data, train_responses_int, ntrain_samples, 0, filename_to_save,&temp_pre_accuracy);
            }
            // filename_to_save="";
            // filename_to_load="-load";
            if(swap_the_role_train_to_test==true){//trained by test value and test using train value
                test_and_save_classifier(boost, train_data, train_responses_int, ntrain_samples, 0, filename_to_save,&temp_pre_accuracy);
            }else{
                
                test_and_save_classifier(boost, test_data, test_responses_int, ntest_samples, 0, filename_to_save,&temp_pre_accuracy);
            }

            if(temp_best_accuracy<temp_pre_accuracy){
                temp_best_accuracy=temp_pre_accuracy;
                temp_value=temp_float;
                // temp_value=i;

                cout<<"accuracy: "<<temp_best_accuracy<<endl;
                cout<<"value: "<<temp_value  <<endl;
                getchar();
            }

        }
        *accuracy=temp_best_accuracy;
        *value=temp_value;
    }

    //test_and_save_classifier(model, data, responses, ntrain_samples, 'A', filename_to_save);
    // test_and_save_classifier(model, data, responses, ntrain_samples, 0, filename_to_save);
    // test_and_save_classifier(model, test_data, test_responses, ntest_samples, 0, filename_to_save);
    // test_and_save_classifier(model, train_data, train_responses2, ntrain_samples, 0, filename_to_save);
    

    return true;
}
// #endif