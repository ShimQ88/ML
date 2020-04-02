// Project Headers
#include "load_and_save_ml.h"
#include "gnuplot.h"
#include "contour.h"
// #include "class.h"

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

void Gnuplot_Generate(){
    Gnuplot gp;
    ifstream f_plot;
    f_plot.open ("plot/command.testxt");
    // f_plot.open ("plot/RMSE.txt");
    string line;
    if(f_plot.is_open())
    {
        while (getline(f_plot,line))
        {
          cout << line << '\n';
          // cout<<line<<endl;
          gp(line);
          // getchar();
        }
        f_plot.close();
    }
}

void info(){
    cout<<"hello this is the program for machine learning"<<endl;
    cout<<"choose which technique you want to test for it"<<endl;
    cout<<"0 = neural_network"<<endl;
    cout<<"1 = ada_boost"<<endl;
    cout<<"2 = random_forest"<<endl;
}

int main(int argc, char *argv[]){
    // cout<<"hell"<<endl;
    float accuracy;
    float value;
	string filename_to_save = "";
    string filename_to_load = "";
    string data_filename = "letter-recognition.data";
    bool train_and_test_mode=false;
    info();
    getchar();
    for(int i = 1; i<argc; i++)
    {
        if(strcmp(argv[i],"-data")==0){ // flag "-data letter_recognition.xml"
        	i++; 
            data_filename = argv[i];
        }else if( strcmp(argv[i],"-save") == 0 ){ // flag "-save filename.xml"
            i++;
            filename_to_save = argv[i];
	    cout << "filename to save is "<< filename_to_save << endl;
        }else if( strcmp(argv[i],"-load") == 0){ // flag "-load filename.xml"
            i++;
            filename_to_load = argv[i];
        }else if(strcmp(argv[i],"-tt") == 0){// flag "-tt filename.xml" train and test
            i++;
            filename_to_save ="-save";
            train_and_test_mode=true;
        }else if(strcmp(argv[i],"-contour") == 0){// flag "-tt filename.xml" train and test
            contour(argv[2]);
            exit(1);//terminate program
        }
    }
    // #ifndef ADA_BOOST_H
    // build_boost_classifier( data_filename, filename_to_save, filename_to_load,0.5,false, &accuracy,&value );
    load_and_save_ml(data_filename, filename_to_save, filename_to_load,0.95,0);
    if(train_and_test_mode==true){ 
        filename_to_save="";
        filename_to_load="-load";
        // build_boost_classifier( data_filename, filename_to_save, filename_to_load,0.8,true, &accuracy,&value );
        load_and_save_ml(data_filename, filename_to_save, filename_to_load,0.95,0);
    }
    // cout<<"accuracy: "<<accuracy<<endl;
    // cout<<"value: "<<value  <<endl;

    Gnuplot_Generate();

    // build_boost_classifier(data_filename, filename_to_save, filename_to_load, 0.7, true );
    // build_random_forest_classifier( data_filename, filename_to_save, filename_to_load );
}

