// Project Headers
// #include "load_and_save_ml.h"
#include "gnuplot.h"

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

extern int run_kuwahara(int argc,char *argv[]);
extern int run_filtering(int argc,char *argv[]);
extern void run_contour(char* argv);
extern bool load_and_save_ml( const string& data_filename,
                      const string& filename_to_save,
                      const string& filename_to_load,
                      float percent_of_division,
                      int ml_technique);

void run_gnuplot(){
    Gnuplot gp;
    ifstream f_plot;
    // f_plot.open ("plot/command.txt");
    f_plot.open ("plot/temp_com5.txt");
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
    string multiple_data_filename[3];
    multiple_data_filename[0]="";
    multiple_data_filename[1]="";
    multiple_data_filename[2]="";

    string multiple_filename_to_save[3];
    multiple_filename_to_save[0] = "";
    multiple_filename_to_save[1] = "";
    multiple_filename_to_save[2] = "";
    bool train_and_test_mode=false;
    bool multiple_mode=false;
    info();
    // getchar();
    int numb_data=0;
    for(int i = 1; i<argc; i++)
    {
        if(strcmp(argv[i],"-data2")==0){ // flag "-data letter_recognition.xml"
        	i++; 
            data_filename = argv[i];
        }else if( strcmp(argv[i],"-save") == 0 ){ // flag "-save filename.xml"
            // i++;
            // filename_to_save = argv[i];
            i++;
            int j=0;
            while(1){
                if(strcmp(argv[i],"-data") == 0){
                    break;
                }
                multiple_filename_to_save[j]=argv[i];
                i++;
                j++;
            }
            i--;
	    cout << "filename to save is "<< filename_to_save << endl;
        }else if( strcmp(argv[i],"-load") == 0){ // flag "-load filename.xml"
            i++;
            filename_to_load = argv[i];
        }else if(strcmp(argv[i],"-contour") == 0){// flag "-contour" train and test
            run_contour(argv[2]);
            exit(1);//terminate program
        }else if(strcmp(argv[i],"-data") == 0){// flag "-multiple filename1.xml filename2.xml filename3.xml" train and test
            multiple_mode=true;
            i++;
            int j=0;
            while(1){
                if(i==argc){
                    break;
                }
                multiple_data_filename[j]=argv[i];
                i++;
                j++;
            }
            numb_data=j;
        }else if(strcmp(argv[i],"-detect") == 0){
            run_kuwahara(argc,argv);
            exit(1);

        }else if(strcmp(argv[i],"-filtering") == 0){
            run_filtering(argc,argv);
            exit(1);
        }

    }
    // #ifndef ADA_BOOST_H
    // build_boost_classifier( data_filename, filename_to_save, filename_to_load,0.5,false, &accuracy,&value );
    if(multiple_mode==true){
        for(int i=0;i<numb_data;i++){
            // cout<<"multiple_data_filename[i]:"<<multiple_data_filename[i]<<endl;
            // cout<<"multiple_filename_to_save[i]:"<<multiple_mode_filename_to_save[i]<<endl;
            load_and_save_ml(multiple_data_filename[i], multiple_filename_to_save[i], filename_to_load,0.85,0);
        }
    }else{
        load_and_save_ml(data_filename, filename_to_save, filename_to_load,0.85,0);    
    }
    
    
    // cout<<"accuracy: "<<accuracy<<endl;
    // cout<<"value: "<<value  <<endl;

    run_gnuplot();

    // build_boost_classifier(data_filename, filename_to_save, filename_to_load, 0.7, true );
    // build_random_forest_classifier( data_filename, filename_to_save, filename_to_load );
}

