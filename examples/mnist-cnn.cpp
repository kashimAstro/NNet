/*
        Author: Dario Longobardi
        data:   2017
*/

#include <iostream>
#include "NNet.h"
#include "CImg.h"

using namespace MNIST;
using namespace std;
using namespace cimg_library;

int main(int argc, char **argv)
{
        if(argc < 2)
        {
                cout << "Error: parameter dataset [trainer|testing]"<<endl;
                exit(0);
        }

        string dataset = argv[1];
        string opt     = argv[2];
        string training_image_fn;
        string training_label_fn;

        if(opt == "training")
	{
        	training_image_fn = dataset+"/train-images.idx3-ubyte";
	        training_label_fn = dataset+"/train-labels.idx1-ubyte";
	}
	else if(opt == "testing")
	{
		training_image_fn = dataset+"/t10k-images.idx3-ubyte";
	        training_label_fn = dataset+"/t10k-labels.idx1-ubyte";
	}

        string model_fn = "weights.dat";

        cout << training_image_fn << endl;
        cout << "path: " << argv[1] <<" opt: "<< argv[2] << endl;

        image.open(training_image_fn.c_str(), ios::in | ios::binary);
        label.open(training_label_fn.c_str(), ios::in | ios::binary);

        if(opt == "training")
        {
                cout << "Start Training MNIST dataset"<< endl;
                MNTrainer trainer;
                char number;
                for (int i = 1; i <= 16; ++i)
                        image.read(&number, sizeof(char));
                for (int i = 1; i <= 8; ++i)
                        label.read(&number, sizeof(char));
                trainer.init_array();
                for (int sample = 1; sample <= nTraining; ++sample)
                {
                        DatiImage img = trainer.input();
                        if(img.wh.size()>0)
                        {
                                CImg<unsigned char> c_img(img.wh.size()+1,img.wh[1].size()+1,1,3);
                                c_img.fill(255,255,255);
                                for(int i = 0; i < img.wh.size(); i++)
                                {
                                        for(int k = 0; k < img.wh[i].size(); k++)
                                        {
                                                int c = (img.wh[i][k]==0)?0:255;
                                                unsigned char col[3];
                                                col[0] = c;
                                                col[1] = c;
                                                col[2] = c;
                                                c_img.draw_point(i,k,0,col);
                                        }
                                }
				int nIterations = trainer.learning_process();
                                string str = "image/iterations_"+to_string(nIterations)+"_label_"+to_string(img.label)+"_error_"+to_string(trainer.square_error())+".jpg";
                                cout<<"Iterations:"<<nIterations<<" Label:"<<img.label<<" Sample:"<<sample<<" Error:"<<trainer.square_error()<<endl;
                                if (sample % 100 == 0) {
                                        cout << "Saving weights " << model_fn << " file." << endl;
                                        trainer.write_matrix(model_fn);
                                }
                                c_img.save_jpeg(str.c_str());
                        }
                }
                trainer.write_matrix(model_fn);
        }
        else if(opt == "testing")
        {
                cout << "Start Testing MNIST dataset"<< endl;

                MNTesting testing;
                char number;
                for (int i = 1; i <= 16; ++i)
                        image.read(&number, sizeof(char));
                for (int i = 1; i <= 8; ++i)
                        label.read(&number, sizeof(char));
                testing.init_array();
                testing.load_model(model_fn);
                int nCorrect = 0;
                for (int sample = 1; sample <= nTesting; ++sample) {
                        int label = testing.input();
                        testing.perceptron();
                        int predict = 1;
                        for (int i = 2; i <= n3; ++i) {
                                if (out3[i] > out3[predict]) {
                                        predict = i;
                                }
                        }
                        --predict;
                        double error = testing.square_error();
                        if (label == predict) {
                                ++nCorrect;
                                cout << "classification:Yes label:" << label << 
				" predict:" << predict << " sample:" << sample << " error:"<< error<<endl;
                                CImg<unsigned char> c_img(width,height,1,3);
                                c_img.fill(255,255,255);

                                for (int j = 1; j <= height; ++j) {
                                        for (int i = 1; i <= width; ++i) {
                                                int c = (d[i][j]==0)?0:255;
                                                unsigned char col[3];
                                                col[0] = c;
                                                col[1] = c;
                                                col[2] = c;
                                                c_img.draw_point(i,j,0,col);
                                        }
                                }
				const char b[3] = { 255,60,10 }; 
			        c_img.draw_text(5,5,to_string(label).c_str(),b);
				string str = "image/classify_yes_label_"+to_string(label)+".jpg";
		                c_img.save_jpeg(str.c_str());
                        } else {
                                cout << "classification:No label:" << label << 
				" predict:" << predict << " sample:" << sample << " error:"<< error<<endl;
                        }
                }

                double accuracy = (double)(nCorrect) / nTesting * 100.0;
                cout << "Number of correct samples: " << nCorrect << " / " << nTesting << endl;
                cout << "Accuracy: " << accuracy << endl;
        }
        image.close();
        label.close();
        return 0;
}
