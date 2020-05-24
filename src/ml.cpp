// Project Headers
#include "ml.h"
#include "class.h"
bool Split_train_test_data(Mat *train_data, Mat *test_data, Mat *train_responses, Mat *test_responses,
                        Mat *train_responses_int, Mat *test_responses_int, Mat data, Mat responses,
                        int block,int the_number_of_data, int n_total_samples, int ntest_samples)
{
    int i_train=0;
    int i_test=0;

   for(int i=0;i<n_total_samples;i++){
        int cls_label = responses.at<int>(i) - 48;// - 'A'; //change to numerical classes, still they read as chars
        cout << "labels " << cls_label << endl;
        if( (i>=block)&&(i<block+ntest_samples) ){
            test_responses->at<float>(i_test, cls_label) = 1.f;
            test_responses_int->at<float>(i_test,0)=cls_label;
        }else{//test part                
            train_responses->at<float>(i_train, cls_label) = 1.f;
            train_responses_int->at<float>(i_train,0)=cls_label; 
        }
        for(int j=0;j<the_number_of_data;j++){

            if( (i>=block)&&(i<block+ntest_samples) ){
                test_data->at<float>(i_test,j)=data.at<float>(i,j);
            }else{
                train_data->at<float>(i_train,j)=data.at<float>(i,j);
            }
        }
        if( (i>=block)&&(i<block+ntest_samples) ){
            i_test++;
        }else{
            i_train++;
                
        }
    }
    return true;
}

bool ModifyFile(ofstream &file_the_best,float mean, float variance,float sta_dev,int k_fold_value,Mat con_mat[])
{
    char mean_buffer[20],variance_buffer[40],sta_dev_buffer[40],mse_buffer[70];
    sprintf(mean_buffer, "#mean: %f \n", mean);
    sprintf(variance_buffer, "#variance: %f \n", variance);
    sprintf(sta_dev_buffer, "#sta_dev: %f \n", sta_dev);  //header
    sprintf(mse_buffer, "#Mean Square Error: %1.f ± %1.f%% \n", mean*100,sta_dev*100);

    
    if (file_the_best)
    {
        // file_the_best<<"\n\n";    
        file_the_best<<mean_buffer;
        file_the_best<<variance_buffer;
        file_the_best<<sta_dev_buffer;
        file_the_best<<mse_buffer;
        file_the_best<<"\n\n";
        file_the_best<<"#Confusion Matrix\n";
        for(int i=0;i<k_fold_value;i++){
            char buffer[50];
            sprintf(buffer, "#k=%d\n", i);  //header
            file_the_best<<buffer;
            file_the_best<<"#";
            file_the_best<<con_mat[i].at<int>(0,0);
            file_the_best<<", ";
            file_the_best<<con_mat[i].at<int>(0,1);
            file_the_best<<"\n";
            file_the_best<<"#";
            file_the_best<<con_mat[i].at<int>(1,0);
            file_the_best<<", ";
            file_the_best<<con_mat[i].at<int>(1,1);
            file_the_best<<"\n\n";
        }
            
    }
    file_the_best << "-----------------------\n";
    return 0;
}

bool
Calculate_standard_deviation(float *mean,float *variance,float *sta_dev,float temp_accuracy[],float sum_accuracy,int k_fold_value){
    *mean=sum_accuracy/k_fold_value;
    *variance=0;
    for(int i=0;i<k_fold_value;i++){
        *variance=*variance+(temp_accuracy[i]-*mean)*(temp_accuracy[i]-*mean);
        cout<<"(temp_accuracy[i]-*mean)*(temp_accuracy[i]-*mean): "<<(temp_accuracy[i]-*mean)*(temp_accuracy[i]-*mean)<<endl;
    }
    *variance=*variance/k_fold_value;
    *sta_dev=sqrt(*variance);
}
template<typename T>
Ptr<T> load_classifier(const string& filename_to_load)
{
    // load classifier from the specified file
    Ptr<T> model = StatModel::load<T>( filename_to_load );
    if( model.empty() )
        cout << "Could not read the classifier " << filename_to_load << endl;
    else
        cout << "The classifier " << filename_to_load << " is loaded.\n";

    return model;
}

float Accuracy_Calculation(const Mat& confusion_matrix)
{
    // load classifier from the specified file
    float accuracy;
    float total_accurate=confusion_matrix.at<int>(0,0)+confusion_matrix.at<int>(1,1);
    float total_number_of_values=confusion_matrix.at<int>(0,0)+confusion_matrix.at<int>(0,1)+
    confusion_matrix.at<int>(1,0)+confusion_matrix.at<int>(1,1);
    accuracy=total_accurate/total_number_of_values;

    return accuracy;
}



// void test_and_save_classifier(const Ptr<StatModel>& model,
//                                      const Mat& data, const Mat& responses,
//                                      int ntrain_samples, int rdelta,
//                                      const string& filename_to_save, float* accuracy)
Mat test_and_save_classifier(const Ptr<StatModel>& model,
                                     const Mat& data, const Mat& responses,
                                     int ntrain_samples, int rdelta,
                                     const string& filename_to_save )
{
    // int nsamples_all = data.rows;
    int nsamples_all = ntrain_samples;
    double train_hr = 0, test_hr = 0;
    int training_correct_predict=0;
    // compute prediction error on training data
    // for(int i=0; i<nsamples_all; i++){
    Mat confusion_Matrix = Mat::zeros( 2, 2, CV_32S );
    
    // getchar();
    for(int i=0; i<nsamples_all; i++){
        Mat sample = data.row(i);
        // int actual_value=responses.at<int>(i)-48;
        int actual_value=responses.at<float>(i);
        cout << "Actual: " << actual_value << " row " << sample << endl;
        float r = model->predict( sample );
        // cout<<"r: "<<r<<endl;
        // getchar();
        // cout << "Predict:  r = " << round(r) << endl;//rounding in case of random_forest
        if( (int)round(r) == actual_value ){ //prediction is correct
            training_correct_predict++;
        }
        confusion_Matrix.at<int>(actual_value,r)=confusion_Matrix.at<int>(actual_value,r)+1;
        // cout<<"confusion_Matrix: "<<confusion_Matrix<<endl;
        // getchar();


        // cout << "training_correct_predict = " << training_correct_predict << endl;
        // getchar();

        // cout << "Sample: " << responses.at<int>(i) << " row " << data.row(i) << endl;
        // float r = model->predict( sample );
        // cout << "Predict:  r = " << r << endl;
        // if( (int)r == (int)(responses.at<int>(i)) ) //prediction is correct
        //     training_correct_predict++;
   
    // r = std::abs(r + rdelta - responses.at<int>(i)) <= FLT_EPSILON ? 1.f : 0.f;
    
     
        //if( i < ntrain_samples )
        //    train_hr += r;
        //else
        //    test_hr += r;

    }

    //test_hr /= nsamples_all - ntrain_samples;
    //train_hr = ntrain_samples > 0 ? train_hr/ntrain_samples : 1.;
    printf("ntrain_samples %d training_correct_predict %d \n",ntrain_samples, training_correct_predict);
    // *accuracy=training_correct_predict*100.0/ntrain_samples;

    if( filename_to_save.empty() )  {
        printf( "\nTest Recognition rate: training set = %.1f%% \n\n", training_correct_predict*100.0/ntrain_samples);

    }
    // if( filename_to_save.empty() )  printf( "\nTest Recognition rate: training set = %.1f%% \n\n", *accuracy);


    if( !filename_to_save.empty() )
    {
        model->save( filename_to_save );
    }
/*************   Example of how to predict a single sample ************************/   
// Use that for the assignment3, for every frame after computing the features, r is the prediction given the features listed in this format
    //Mat sample = data.row(i);
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
    return confusion_Matrix;
    
/**********************************************************************/
}
inline TermCriteria TC(int iters, double eps)
{
    return TermCriteria(TermCriteria::MAX_ITER + (eps > 0 ? TermCriteria::EPS : 0), iters, eps);
}
bool
build_mlp_classifier(   Mat data,
                        Mat responses,
                        int ntrain_samples,
                        int ntest_samples,
                        const string& filename_to_save,
                        const string& filename_to_load)
{
    int class_count=2;
    // Create or load MLP classifier
    
    int k_fold_value=ntrain_samples/ntest_samples;
    int n_total_samples=ntrain_samples+ntest_samples;
    // cout<<"k_fold_value: "<<k_fold_value<<endl;
    // getchar();

    
    int the_number_of_data=data.cols;//determine how many columns of data
    Mat train_data = Mat::zeros( ntrain_samples, the_number_of_data, CV_32F );
    Mat test_data = Mat::zeros( ntest_samples, the_number_of_data, CV_32F );
    string numb_ce=to_string(the_number_of_data+1);

    // cout<<"train_data:"<<train_data<<endl;
    // cout<<"the_number_of_data:"<<the_number_of_data<<endl;
    // getchar();

    // sprintf(head_buffer, "%s \n", "#index, accuracy");  //header
    // file <<head_buffer;
    int value=0;
    float sum_accuracy=0;
    float temp_accuracy[k_fold_value];
    Mat con_mat[k_fold_value];
    ofstream file ("resource/accuracy"+numb_ce+".txt");
    ofstream file_the_best("resource/standard_deviation"+numb_ce+".txt");
    // ofstream file_data("resource/data"+numb_ce+".txt");

    // char head_buffer1[80];
    // sprintf(head_buffer1, "%s \n", "#index, t_method, a_function, method_param, max_iter, class_count, accuracy");  //header
    // sprintf(head_buffer1, "%s \n", "#k_fold_value, accuracy");  //header
    
    // file_the_best <<"#k_fold_value, index, t_method, a_function, method_param, max_iter, class_count, accuracy\n";
    int i=0;
    double method_param = 0.001;
    int max_iter = 300;
    int max_loop=0;
    while(1){
        if(max_loop==3){break;}
        max_iter=max_iter+200;
        file <<"#k_fold_value, t_method, a_function, method_param, max_iter, class_count, accuracy\n";//header
        value=0;
        while(1){
            if(value==k_fold_value){break;}
            
            char head_buffer[80];
            
            // sprintf(head_buffer, "%s \n", "#index, accuracy");  //header
            // sprintf(head_buffer, "#k_fold_value is %d \n", value);  //header
            // file <<head_buffer;
            
            // file << "#index, accuracy\n";
            int block=ntest_samples*value;
            
            // cout<<"ntest_samples: "<<ntest_samples<<endl;
            // cout<<"ntrain_samples: "<<ntrain_samples<<endl;
            // cout<<"block: "<<block<<endl;
            // cout<<"block+ntest_samples:"<<block+ntest_samples<<endl;
            // cout<<ntrain_samples+ntest_samples<<endl;
            // getchar();

            Mat train_responses = Mat::zeros( ntrain_samples, class_count, CV_32F );
            Mat test_responses = Mat::zeros( ntest_samples, class_count, CV_32F );
            Mat test_responses_int = Mat::zeros( ntest_samples, 1, CV_32F );
            Mat train_responses_int = Mat::zeros( ntrain_samples, 1, CV_32F );

            //1. unroll the responses
            Split_train_test_data(&train_data, &test_data, &train_responses,&test_responses,
                    &train_responses_int, &test_responses_int, data, responses,
                    block, the_number_of_data, n_total_samples, ntest_samples);

            // cout<<"test_data: "<<test_data<<endl;
            // getchar();
            // cout<<"train_data: "<<train_data<<endl;
            // getchar();
            Ptr<ANN_MLP> model;

            if( !filename_to_load.empty() )
            {
                model = load_classifier<ANN_MLP>(filename_to_load);
                if( model.empty() )
                    return false;
                    
                test_and_save_classifier(model, test_data, test_responses_int, ntest_samples, 0, filename_to_save);
                
            }
            else
            {
                // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
                // MLP does not support categorical variables by explicitly.
                // So, instead of the output class label, we will use
                // a binary vector of <class_count> components for training and,
                // therefore, MLP will give us a vector of "probabilities" at the
                // prediction stage
                // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

                // 2. train classifier
                int layer_sz[] = { data.cols, 100, 100, class_count };
                cout <<  " sizeof layer_sz " << sizeof(layer_sz) << " sizeof layer_sz[0]) " << sizeof(layer_sz[0]) << endl;
                int nlayers = (int)(sizeof(layer_sz)/sizeof(layer_sz[0]));
                cout << " nlayers  " << nlayers << endl;
                Mat layer_sizes( 1, nlayers, CV_32S, layer_sz );

                int t_method = ANN_MLP::BACKPROP;
                int a_function = ANN_MLP::SIGMOID_SYM;
                // double method_param = 0.001;
                

                Ptr<TrainData> tdata;
                tdata = TrainData::create(train_data, ROW_SAMPLE, train_responses);    
                
                int i=0;
                int max_loop=4;
                // Neural_Network NN[max_loop];
                Neural_Network NN;
                // String buffer[max_loop];
                // String buffer2[max_loop];
                String buffer;
                String buffer2;

                float the_best_accuracy=-9999;
                int the_i_best_accuracy=0;
                Mat confusion_matrix;


                //training section
                // if(i==max_loop){
                //     break;
                // }
                t_method = ANN_MLP::BACKPROP; // { BACKPROP =0, RPROP = 1, ANNEAL = 2 }
                a_function = ANN_MLP::SIGMOID_SYM; // { IDENTITY = 0, SIGMOID_SYM = 1, GAUSSIAN = 2, RELU = 3, LEAKYRELU = 4 }
                float accuracy;
                // max_iter=max_iter+300;
                
                cout << "iteration ("<<value<<") Training the classifier (may take a few minutes)...\n";
                model = ANN_MLP::create();
                model->setLayerSizes(layer_sizes);
                model->setActivationFunction(a_function, 0, 0);
                // model->setActivationFunction(ANN_MLP::IDENTITY, 0, 0);
                model->setTermCriteria(TC(max_iter,0));
                model->setTrainMethod(t_method, method_param);
                model->train(tdata);
                cout << endl;

                // confusion_matrix=test_and_save_classifier(model, train_data, train_responses_int, ntrain_samples, 0, filename_to_save);
                confusion_matrix=test_and_save_classifier(model, test_data, test_responses_int, ntest_samples, 0, filename_to_save);
                

                accuracy=Accuracy_Calculation(confusion_matrix);
                cout<<"accuracy: "<<accuracy<<endl;
                NN.Reset_Parameters( value, t_method, a_function, method_param, max_iter, class_count, accuracy,confusion_matrix);
                
                // if(the_best_accuracy<accuracy){
                //     the_best_accuracy=accuracy;
                //     the_i_best_accuracy=i;
                // }

                buffer=NN.Return_Parameter();
                buffer2=NN.Return_Parameter();

                // sprintf(buffer[i], "%d, %d, %d, %lf, %d, %d, %f \n", i, t_method, a_function, method_param, max_iter, class_count, accuracy);  //header
                
                file<<buffer;
                
                // i++;
                 

                // cout<<"the best accuracy index: "<<the_i_best_accuracy<<endl;
                // cout<<"the best accuracy: "<<the_best_accuracy<<endl;

                
                // cout<<"the_i_best_accuracy:"<<the_i_best_accuracy<<endl;
                // cout<<"confusion_matrix: "<<confusion_matrix<<endl;

                // file_the_best<<value;
                // file_the_best<<", " ;
                // file_the_best<<buffer2[the_i_best_accuracy];
                // file_the_best<<NN[i].Return_Accuracy2();
                con_mat[value]=confusion_matrix;

                // file_the_best<<"\n\n";
                // file_the_best<<"Confusion Matrix\n";
                // file_the_best<<confusion_matrix;

                temp_accuracy[value]=accuracy;
                value++;
                sum_accuracy=sum_accuracy+accuracy;
                // getchar();

            }
        }
        file << "-----------------------\n";
        float mean, variance, sta_dev;
        Calculate_standard_deviation(&mean,&variance,&sta_dev,temp_accuracy,sum_accuracy,k_fold_value);
        ModifyFile(file_the_best,mean, variance,sta_dev,k_fold_value,con_mat);
        sum_accuracy=0;
        // cout<<"here is the point"<<endl;
        // getchar();
        max_loop++;
    }
    
    

    //print
    // cout<<"mean: "<<mean<<endl;
    // cout<<"variance: "<<variance<<endl;
    // cout<<"standard_deviation: "<<sta_dev<<endl;
    // cout<<"Mean Square Error: "<<mean*100<<"+-"<<sta_dev*100<<"%"<<endl;
    

    if (file.is_open()){
        // file << "-----------------------";
    }else{ 
        cout << "Unable to open file";
    }

    if (file_the_best.is_open()){
        // file_the_best << "-----------------------";
    }else{ 
        cout << "Unable to open file";
    }
    
    
    file.close();
    file_the_best.close();
    return true;
}

bool build_boost_classifier( Mat data,
                                Mat responses,
                                int ntrain_samples,
                                int ntest_samples,
                                const string& filename_to_save,
                                const string& filename_to_load)
{
    float *value;
    float *accuracy;
    Ptr<Boost> model;

    // Test number of samples
    // cout<<"ntrain_samples: "<<ntrain_samples<<endl;
    // cout<<"ntest_samples: "<<ntest_samples<<endl;
    // getchar();

    // Create or load MLP classifier
    Mat train_data = data.rowRange(0, ntrain_samples);
    Mat train_responses = Mat::zeros( ntrain_samples, 1, CV_32S );

    // test_data = data.rowRange(ntrain_samples, ntest_samples+ntrain_samples);
    Mat test_data = data.rowRange(ntrain_samples, ntest_samples+ntrain_samples);
    Mat test_responses = Mat::zeros( ntest_samples, 1, CV_32S );
        
    Mat train_responses_int=responses.rowRange(0, ntrain_samples);
    Mat test_responses_int=responses.rowRange(ntrain_samples, ntest_samples+ntrain_samples);
        

    if( !filename_to_load.empty() ){
        model = load_classifier<Boost>(filename_to_load);
        // model = StatModel::load<Boost>( filename_to_load );
        if( model.empty() ){
             cout << "Could not read the classifier " << filename_to_load << endl;
            return false;
        }else{
            cout << "The classifier " << filename_to_load << " is loaded.\n";
        }
        //ntrain_samples = 0;
            test_and_save_classifier(model, test_data, test_responses_int, ntest_samples, 0, filename_to_save);
            // test_and_save_classifier(model, test_data, test_responses_int, ntest_samples, 0, filename_to_save,accuracy);
        
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
            model = Boost::create();
            model->setBoostType(Boost::GENTLE);  //Gentle 0.5 and true
            model->setWeakCount(51);       //the Gentle best=98;
            model->setWeightTrimRate(0.63);//the Gentle best=0.83;
            model->setMaxDepth(2);         //the Gentle best=2;
            model->setUseSurrogates(false);
            model->setPriors(Mat());
            model->train(tdata);
            cout << endl;

            
            test_and_save_classifier(model, train_data, train_responses_int, ntrain_samples, 0, filename_to_save);
            
            // filename_to_save="";
            // filename_to_load="-load";
            
                
            test_and_save_classifier(model, test_data, test_responses_int, ntest_samples, 0, filename_to_save);
                // test_and_save_classifier(model, test_data, test_responses_int, ntest_samples, 0, filename_to_save,&temp_pre_accuracy);
            

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
    return true;
}

bool
build_random_forest_classifier( Mat data,
                                Mat responses,
                                int ntrain_samples,
                                int ntest_samples,
                                const string& filename_to_save,
                                const string& filename_to_load)
{
    Mat train_responses_int=responses.rowRange(0, ntrain_samples);
    Mat test_responses_int=responses.rowRange(ntrain_samples, ntest_samples+ntrain_samples);
    //mode==0 load and mode==1 save
    Mat train_data = data.rowRange(0, ntrain_samples);
    Mat train_responses = Mat::zeros( ntrain_samples, 1, CV_32F );

    // test_data = data.rowRange(ntrain_samples, ntest_samples+ntrain_samples);
    Mat test_data = data.rowRange(ntrain_samples, ntest_samples+ntrain_samples);
    Mat test_responses = Mat::zeros( ntest_samples, 1, CV_32F );

    Ptr<RTrees> model;
    // Create or load MLP classifier
    if( !filename_to_load.empty() )//load mode
    {
        // model = StatModel::load<RTrees>(filename_to_load);
        model=load_classifier<RTrees>(filename_to_load);
        if( model.empty() ){
             cout << "Could not read the classifier " << filename_to_load << endl;
            return false;
        }else{
            cout << "The classifier " << filename_to_load << " is loaded.\n";
        }
        
        test_and_save_classifier(model, test_data, test_responses_int, ntest_samples, 0, filename_to_save);
        
    }
    else//save mode
    {
        // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        //
        // MLP does not support categorical variables by explicitly.
        // So, instead of the output class label, we will use
        // a binary vector of <class_count> components for training and,
        // therefore, MLP will give us a vector of "probabilities" at the
        // prediction stage
        //
        // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

        

        // 1. unroll the responses
        cout << "Unrolling the responses...\n";
        for(int i=0; i<ntrain_samples; i++){
            int cls_label = responses.at<int>(i) - 48;// - 'A'; //change to numerical classes, still they read as chars
            cout << "labels " << cls_label << endl;
            if(i<ntrain_samples){//train part
                train_responses.at<float>(i, cls_label) = 1.f;
                // train_responses.at<float>(i, cls_label) = 1;
            }else{//test part
                test_responses.at<float>(i-ntrain_samples, cls_label) = 1.f;
                // test_responses.at<float>(i-ntrain_samples, cls_label) = 1;
            }
        }

        // Ptr<TrainData> tdata = TrainData::create(train_data, ROW_SAMPLE, train_responses);
        Ptr<TrainData> tdata;
        
        tdata = TrainData::create(train_data, ROW_SAMPLE, train_responses);    
        
        // cout<<"smaple:"<<<<endl;
        // cout<<"cat count:"<<tdata->getLayout()<<endl;
        // tdata->setTrainTestSplitRatio(0.90, false);
        // Ptr<TrainData> tdata = TrainData::loadFromCSV(train_data, ROW_SAMPLE, train_responses);


        cout << "Training the classifier (may take a few minutes)...\n";
        model = RTrees::create();
        model->setMaxDepth(8);
        model->setMinSampleCount(9);
        model->setRegressionAccuracy(0.01f);
        model->setUseSurrogates(false /* true */);
        model->setMaxCategories(15);
        // model->setCVFolds(0 10); // nonzero causes core dump
        // model->setUse1SERule(true);
        // model->setTruncatePrunedTree(true);
        // model->setLayerSizes(layer_sizes);
        // model->setActivationFunction(ANN_MLP::SIGMOID_SYM, 0, 0);
        // model->setActivationFunction(ANN_MLP::IDENTITY, 0, 0);
        // model->setActiveVarCount(0);
        // model->setCalculateVarImportance(0);
        model->setTermCriteria(TC(300,0));
        // model->setTrainMethod(method, method_param);
        model->train(tdata);
        cout << endl;

        cout<<"ntrain_samples: "<<ntrain_samples<<endl;
        getchar();
        
        test_and_save_classifier(model, test_data, test_responses_int, ntest_samples, 0, filename_to_save);
        
        // filename_to_save="";
        // filename_to_load="-load";
    }
    return true;
}
