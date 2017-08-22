/*
        Test:   cnn mnist
        Author: Dario Longobardi
        data:   2017
*/

#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <vector>
#include <set>
#include <iterator>
#include <algorithm>

using namespace std;

namespace MNIST
{
	struct DatiImage
	{
		vector<vector<int>> wh;
		int label;
	};
	const int nTesting = 10000;
	const int nTraining = 60000;
	const int width = 28;
	const int height = 28;
	const int n1 = width * height;
	const int n2 = 128; 
	const int n3 = 10;
	const int epochs = 512;
	const double learning_rate = 1e-3;
	const double momentum = 0.9;
	const double epsilon = 1e-3;
	double *w1[n1 + 1], *delta1[n1 + 1], *out1;
	double *w2[n2 + 1], *delta2[n2 + 1], *in2, *out2, *theta2;
	double *in3, *out3, *theta3;
	double expected[n3 + 1];
	int d[width + 1][height + 1];
	ifstream image;
	ifstream label;
	ofstream report;

	/* TRAINING CLASS */
	class MNTrainer
	{
		public:
		void init_array() {
		    for (int i = 1; i <= n1; ++i) {
			w1[i] = new double [n2 + 1];
			delta1[i] = new double [n2 + 1];
		    }
		    out1 = new double [n1 + 1];
		    for (int i = 1; i <= n2; ++i) {
			w2[i] = new double [n3 + 1];
			delta2[i] = new double [n3 + 1];
		    }
		    in2 = new double [n2 + 1];
		    out2 = new double [n2 + 1];
		    theta2 = new double [n2 + 1];
		    in3 = new double [n3 + 1];
		    out3 = new double [n3 + 1];
		    theta3 = new double [n3 + 1];
		    for (int i = 1; i <= n1; ++i) {
			for (int j = 1; j <= n2; ++j) {
			    int sign = rand() % 2;
			    w1[i][j] = (double)(rand() % 6) / 10.0;
			    if (sign == 1) {
				w1[i][j] = - w1[i][j];
			    }
			}
		    }
		    for (int i = 1; i <= n2; ++i) {
			for (int j = 1; j <= n3; ++j) {
			    int sign = rand() % 2;
			    // w2[i][j] = (double)(rand() % 6) / 10.0;
			    w2[i][j] = (double)(rand() % 10 + 1) / (10.0 * n3);
			    if (sign == 1) {
				w2[i][j] = - w2[i][j];
			    }
			}
		    }
		}

		double sigmoid(double x) { return 1.0 / (1.0 + exp(-x)); }

		void perceptron() {
		    for (int i = 1; i <= n2; ++i)
			in2[i] = 0.0;
		    for (int i = 1; i <= n3; ++i) 
			in3[i] = 0.0;
		    for (int i = 1; i <= n1; ++i) {
			for (int j = 1; j <= n2; ++j) {
			    in2[j] += out1[i] * w1[i][j];
			}
		    }
		    for (int i = 1; i <= n2; ++i)
			out2[i] = sigmoid(in2[i]);
		    for (int i = 1; i <= n2; ++i) {
			for (int j = 1; j <= n3; ++j) {
			    in3[j] += out2[i] * w2[i][j];
			}
		    }
		    for (int i = 1; i <= n3; ++i)
			out3[i] = sigmoid(in3[i]);
		}

		double square_error(){
		    double res = 0.0;
		    for (int i = 1; i <= n3; ++i) {
			res += (out3[i] - expected[i]) * (out3[i] - expected[i]);
		    }
		    res *= 0.5;
		    return res;
		}

		void back_propagation() {
		    double sum;
		    for (int i = 1; i <= n3; ++i)
			theta3[i] = out3[i] * (1 - out3[i]) * (expected[i] - out3[i]);
		    for (int i = 1; i <= n2; ++i) {
			sum = 0.0;
			for (int j = 1; j <= n3; ++j) {
			    sum += w2[i][j] * theta3[j];
			}
			theta2[i] = out2[i] * (1 - out2[i]) * sum;
		    }
		    for (int i = 1; i <= n2; ++i) {
			for (int j = 1; j <= n3; ++j) {
			    delta2[i][j] = (learning_rate * theta3[j] * out2[i]) + (momentum * delta2[i][j]);
			    w2[i][j] += delta2[i][j];
			}
		    }
		    for (int i = 1; i <= n1; ++i) {
			for (int j = 1 ; j <= n2 ; j++ ) {
			    delta1[i][j] = (learning_rate * theta2[j] * out1[i]) + (momentum * delta1[i][j]);
			    w1[i][j] += delta1[i][j];
			}
		    }
		}

		int learning_process() {
		    for (int i = 1; i <= n1; ++i) {
			for (int j = 1; j <= n2; ++j) {
					delta1[i][j] = 0.0;
			}
		    }
		    for (int i = 1; i <= n2; ++i) {
			for (int j = 1; j <= n3; ++j) {
					delta2[i][j] = 0.0;
		 	}
		    }
		    for (int i = 1; i <= epochs; ++i) {
			perceptron();
			back_propagation();
			if (square_error() < epsilon) 
				return i;
		    }
		    return epochs;
		}

		DatiImage input() {
	            DatiImage im;
		    char number;
		    for (int j = 1; j <= height; ++j) {
			for (int i = 1; i <= width; ++i) {
			    image.read(&number, sizeof(char));
			    if (number == 0) {
				d[i][j] = 0; 
			    } else {
				d[i][j] = 1;
		            }
			}
		    }			
		    //cout << "Image:" << endl;
		    //cout << "**************************************\n";
		    im.wh.resize(height+1);
		    for (int j = 1; j <= height; ++j) {
			for (int i = 1; i <= width; ++i) {
				//cout << d[i][j];
				im.wh[j].push_back(d[i][j]);
			}
			//cout << endl;
		    }
		    //cout << "**************************************\n";
		    for (int j = 1; j <= height; ++j) {
			for (int i = 1; i <= width; ++i) {
			    	int pos = i + (j - 1) * width;
			    	out1[pos] = d[i][j];
			}
		    }
		    label.read(&number, sizeof(char));
		    for (int i = 1; i <= n3; ++i) 
			expected[i] = 0.0;
		    expected[number + 1] = 1.0;
		    
		    //cout << "Label: " << (int)(number) << endl;
		    im.label=(int)(number);
		    return im;
		}

		void write_matrix(string file_name) {
		    ofstream file(file_name.c_str(), ios::out);
		    for (int i = 1; i <= n1; ++i) {
			for (int j = 1; j <= n2; ++j) {
					file << w1[i][j] << " ";
			}
			file << endl;
		    }
		    for (int i = 1; i <= n2; ++i) {
			for (int j = 1; j <= n3; ++j) {
					file << w2[i][j] << " ";
			}
			file << endl;
		    }
	            file.close();
		}
	};

	/* TESTING CLASS */
	class MNTesting
	{
		public:
		void init_array() {
		    for (int i = 1; i <= n1; ++i) {
			w1[i] = new double [n2 + 1];
		    }
		    out1 = new double [n1 + 1];
		    for (int i = 1; i <= n2; ++i) {
			w2[i] = new double [n3 + 1];
		    }
		    in2 = new double [n2 + 1];
		    out2 = new double [n2 + 1];
		    in3 = new double [n3 + 1];
		    out3 = new double [n3 + 1];
		}

		void load_model(string file_name) {
		    ifstream file(file_name.c_str(), ios::in);
		    for (int i = 1; i <= n1; ++i) {
			for (int j = 1; j <= n2; ++j) {
				file >> w1[i][j];
			}
		    }
		    for (int i = 1; i <= n2; ++i) {
			for (int j = 1; j <= n3; ++j) {
				file >> w2[i][j];
		 	}
		    }
		    file.close();
		}

		double sigmoid(double x) { return 1.0 / (1.0 + exp(-x)); }

		void perceptron() {
		    for (int i = 1; i <= n2; ++i)
			in2[i] = 0.0;
		    for (int i = 1; i <= n3; ++i)
		 	in3[i] = 0.0;
		    for (int i = 1; i <= n1; ++i) {
			for (int j = 1; j <= n2; ++j) {
			    in2[j] += out1[i] * w1[i][j];
			}
		    }
		    for (int i = 1; i <= n2; ++i) 
			out2[i] = sigmoid(in2[i]);
		    for (int i = 1; i <= n2; ++i) {
			for (int j = 1; j <= n3; ++j) {
			    in3[j] += out2[i] * w2[i][j];
			}
		    }
		    for (int i = 1; i <= n3; ++i) {
			out3[i] = sigmoid(in3[i]);
		    }
		}

		double square_error(){
		    double res = 0.0;
		    for (int i = 1; i <= n3; ++i) {
			res += (out3[i] - expected[i]) * (out3[i] - expected[i]);
			}
		    res *= 0.5;
		    return res;
		}

		int input() {
		    char number;
		    for (int j = 1; j <= height; ++j) {
			for (int i = 1; i <= width; ++i) {
			    image.read(&number, sizeof(char));
			    if (number == 0) {
				d[i][j] = 0; 
			    } else {
				d[i][j] = 1;
		            }
			}
		    }

		    for (int j = 1; j <= height; ++j) {
			for (int i = 1; i <= width; ++i) {
			    int pos = i + (j - 1) * width;
			    out1[pos] = d[i][j];
			}
		    }
		    label.read(&number, sizeof(char));
		    for (int i = 1; i <= n3; ++i) 
				expected[i] = 0.0;
		    expected[number + 1] = 1.0;
		    return (int)(number);
		}
	};
};
