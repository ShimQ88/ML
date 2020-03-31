// Project Headers
#include "random_forest.h"
// #include "load_and_save_classifier.h"



// This function reads data and responses from the file <filename>
bool
read_num_class_data2( const string& filename, int var_count,
                     Mat* _data, Mat* _responses )
{
    const int M = 1024;
    char buf[M+2];

    Mat el_ptr(1, var_count, CV_32F);
    int i;
    vector<int> responses;

    _data->release();
    _responses->release();

    FILE* f = fopen( filename.c_str(), "rt" );
    if( !f )
    {
        cout << "Could not read the database " << filename << endl;
        return false;
    }

    for(;;)
    {
        char* ptr;
        if( !fgets( buf, M, f ) || !strchr( buf, ',' ) )
            break;
        responses.push_back(buf[0]);
        //char test;
        //test=buf[0]+65;
        //responses.push_back(test);
        cout << "responses " << buf[0] << " " ;;//<<  endl;
        ptr = buf+2;
        for( i = 0; i < var_count; i++ )
        {
            int n = 0;
            sscanf( ptr, "%f%n", &el_ptr.at<float>(i), &n );
            ptr += n + 1;
        }
        cout << el_ptr << endl;
        if( i < var_count )
            break;
        _data->push_back(el_ptr);
    }
    fclose(f);
    Mat(responses).copyTo(*_responses);

    cout << "The database " << filename << " is loaded.\n";

    return true;
}

template<typename T>
Ptr<T> load_classifier2(const string& filename_to_load)
{
    // load classifier from the specified file
    Ptr<T> model = StatModel::load<T>( filename_to_load );
    if( model.empty() )
        cout << "Could not read the classifier " << filename_to_load << endl;
    else
        cout << "The classifier " << filename_to_load << " is loaded.\n";

    return model;
}

inline TermCriteria TC2(int iters, double eps)
{
    return TermCriteria(TermCriteria::MAX_ITER + (eps > 0 ? TermCriteria::EPS : 0), iters, eps);
}

void test_and_save_classifier2(const Ptr<StatModel>& model,
                                     const Mat& data, const Mat& responses,
                                     int ntrain_samples, int rdelta,
                                     const string& filename_to_save)
{
    // int nsamples_all = data.rows;
    int nsamples_all = ntrain_samples;
    double train_hr = 0, test_hr = 0;
    int training_correct_predict=0;
    // compute prediction error on training data
    // for(int i=0; i<nsamples_all; i++){
    for(int i=0; i<nsamples_all; i++){
        Mat sample = data.row(i);
        cout << "Sample: " << responses.at<int>(i)-48 << " row " << data.row(i) << endl;
        float r = model->predict( sample );
        cout << "Predict:  r = " << round(r) << endl;//rounding in case of random_forest
        if( (int)round(r) == (int)(responses.at<int>(i)-48) ){ //prediction is correct
            training_correct_predict++;
        }
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
    if( filename_to_save.empty() )  printf( "\nTest Recognition rate: training set = %.1f%% \n\n", training_correct_predict*100.0/ntrain_samples);


    if( !filename_to_save.empty() )
    {
        model->save( filename_to_save );
    }
/*************   Example of how to predict a single sample ************************/   
// Use that for the assignment3, for every frame after computing the features, r is the prediction given the features listed in this format
    //Mat sample = data.row(i);
    Mat sample1 = (Mat_<float>(1,9) << 1.52101, 13.64, 4.4899998, 1.1, 71.779999, 0.059999999, 8.75, 0, 0);// 1
    float r = model->predict( sample1 );
    cout << "Prediction: " << r << endl;
    sample1 = (Mat_<float>(1,9) << 1.518, 13.71, 3.9300001, 1.54, 71.809998, 0.54000002, 8.21, 0, 0.15000001);//2
    r = model->predict( sample1 );
    cout << "Prediction: " << r << endl;
    sample1 = (Mat_<float>(1,9) << 1.51694,12.86,3.58,1.31,72.61,0.61,8.79,0,0);//3
    r = model->predict( sample1 );
    cout << "Prediction: " << r << endl;
//    sample1 = (Mat_<float>(1,9) << );//4
//    r = model->predict( sample1 );
//    cout << "Prediction: " << r << endl;
    sample1 = (Mat_<float>(1,9) << 1.5151401, 14.01, 2.6800001, 3.5, 69.889999, 1.6799999, 5.8699999, 2.2, 0);//5
    r = model->predict( sample1 );
    cout << "Prediction: " << r << endl;
    sample1 = (Mat_<float>(1,9) << 1.51852, 14.09, 2.1900001, 1.66, 72.669998, 0, 9.3199997, 0, 0);//6
    r = model->predict( sample1 );
    cout << "Prediction: " << r << endl;
    sample1 = (Mat_<float>(1,9) << 1.51131,13.69,3.2,1.81,72.81,1.76,5.43,1.19,0);//7
    r = model->predict( sample1 );
    cout << "Prediction: " << r << endl;
    
/**********************************************************************/
}

void help2(bool swap_the_role_train_to_test,int ntrain_samples, int ntest_samples){
    if(swap_the_role_train_to_test==false){
        cout<<endl<<endl<<endl<<endl<<endl;
        cout<<"#########################################################"<<endl;
        cout<<"######## mode1: train by training sample.        ########"<<endl;
        cout<<"########       test by testing sample.           ########"<<endl;
        printf("######## Training the index number from 0 to %d ########\n",ntrain_samples);
        printf("######## Testing the index number from %d to %d ########\n",ntrain_samples+1, ntrain_samples+ntest_samples);
        cout<<"#########################################################"<<endl;
    }else{

        cout<<endl<<endl<<endl<<endl<<endl;
        cout<<"#########################################################"<<endl;
        cout<<"######## mode2: train by testing sample.         ########"<<endl;
        cout<<"########       test by training sample.          ########"<<endl;
        printf("######## Training the index number from %d to %d ########\n",ntrain_samples+1, ntrain_samples+ntest_samples);
        printf("######## Testing the index number from 0 to %d ########\n",ntrain_samples);
        cout<<"#########################################################"<<endl;
    }
    cout<<"Press any key"<<endl;
}


bool
build_random_forest_classifier( const string& data_filename,
                      const string& filename_to_save,
                      const string& filename_to_load )
{
    const int class_count = 2;//CLASSES
    Mat data;
    Mat responses;

    bool ok = read_num_class_data2( data_filename, 9, &data, &responses );//third parameter: FEATURES
    if( !ok )
        return ok;

    Ptr<RTrees> model;

    int nsamples_all = data.rows;
    int ntrain_samples = (int)(nsamples_all*0.5);//SPLIT

    // Create or load MLP classifier
    if( !filename_to_load.empty() )
    {
        model = load_classifier2<RTrees>(filename_to_load);
        if( model.empty() )
            return false;
        //ntrain_samples = 0;
    }
    else
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

        Mat train_data = data.rowRange(0, ntrain_samples);
        // Mat train_responses = Mat::zeros( ntrain_samples, class_count, CV_32F );
        Mat train_responses = Mat::zeros( ntrain_samples, 1, CV_32F );
        

        // 1. unroll the responses
        cout << "Unrolling the responses...\n";
        for(int i=0; i<ntrain_samples; i++){
            int cls_label = responses.at<int>(i) - 48;// - 'A'; //change to numerical classes, still they read as chars
            cout << "labels " << cls_label << endl;
            if(cls_label==1){
                train_responses.at<float>(i, 0) = 1.f;
            }
            
        }

        cout<<"train_responses:"<<train_responses<<endl;
        getchar();

        // 2. train classifier
        int layer_sz[] = { data.cols, 100, 100, class_count };
    cout <<  " sizeof layer_sz " << sizeof(layer_sz) << " sizeof layer_sz[0]) " << sizeof(layer_sz[0]) << endl;
        int nlayers = (int)(sizeof(layer_sz)/sizeof(layer_sz[0]));
    cout << " nlayers  " << nlayers << endl;
        Mat layer_sizes( 1, nlayers, CV_32S, layer_sz );

#if 1
        // int method = ANN_MLP::BACKPROP;
        // double method_param = 0.001;
        double method_param = 0.01;
        // int max_iter = 300;
        int max_iter = 300;
#else
        int method = ANN_MLP::RPROP;
        double method_param = 0.1;
        int max_iter = 1000;
#endif

        // Ptr<TrainData> tdata = TrainData::create(train_data, ROW_SAMPLE, train_responses);
        Ptr<TrainData> tdata = TrainData::create(train_data, ROW_SAMPLE, train_responses);
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
//         // model->setActivationFunction(ANN_MLP::IDENTITY, 0, 0);
        // model->setActiveVarCount(0);
        // model->setCalculateVarImportance(0);
        model->setTermCriteria(TC2(max_iter,0));
        // model->setTrainMethod(method, method_param);
        model->train(tdata);
        cout << endl;
    }

    // test_and_save_classifier(model, data, responses, ntrain_samples, 'A', filename_to_save);
    test_and_save_classifier2(model, data, responses, ntrain_samples, 0, filename_to_save);
    return true;
}
