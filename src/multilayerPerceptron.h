#include <iostream>
#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <ctime>
#include <algorithm>
#include <cmath>

using namespace std;

class MultilayerPerceptron 
{
	struct WeightMatrix {
		int inputDim;
		int outputDim;		
		std::vector<float> w;

		WeightMatrix(int inputDim_, int outputDim_, float initialWeightScale_) {
			w.clear();
			inputDim = inputDim_;
			outputDim = outputDim_;
			for (int k=0; k<inputDim*outputDim; ++k) {
				w.push_back( 2*initialWeightScale_* (rand() / double(RAND_MAX)) - initialWeightScale_ );
			}
		}
	};

	struct Layer {
		int dim;
		std::vector<float> in;
		std::vector<float> out;
		std::vector<float> err;
		
		Layer(int dim_) {
			dim = dim_;
			for (int k=0; k<dim; ++k) {
				in.push_back(0);
				out.push_back(0);
				err.push_back(0);
			}
		}
	};

	public:
		int H;
		int inputDimension;
		int outputDimension;

		struct TrainingElement {
			std::vector<float> in;
			std::vector<float> out;		
			
			TrainingElement(std::vector<float> in_, std::vector<float> out_) {
				in = in_;
				out = out_;
			}	
		};

		MultilayerPerceptron(int inputDimension_, int outputDimension_) {
			inputDimension = inputDimension_;
			outputDimension = outputDimension_;
			
			layers.push_back(Layer(inputDimension));
			H = 1;
		}

		std::vector<WeightMatrix> weights;
		std::vector<Layer> layers;
		std::vector<TrainingElement> trainingSet;

		~MultilayerPerceptron() {

		}

		void addHiddenLayer(int dimension_) {
			layers.push_back(Layer(dimension_));
			H++;
		}

		void init() {
			layers.push_back(Layer(outputDimension));
			H++;
			
			resetWeights();
				
			WeightMatrix *weightMatrix;
			for(int h=0; h<H-1; ++h) {
				weightMatrix = &(weights[h]);
			}
				
		}


		void resetWeights() {
			weights.clear();
			int h;
			int dim0, dim1;
			for(h=0; h<H-1; ++h) {
				dim0 = layers[h].dim;
				dim1 = layers[h+1].dim;
				weights.push_back(WeightMatrix(dim0, dim1, 1.0f));
			}	
		}


		void calcLayerInput(int h_) {
			if(h_>0 && h_<H) {
				WeightMatrix *w = &(weights[h_-1]);
				int i,j;
				for(i=0; i<layers[h_].dim; ++i) {
					layers[h_].in[i] = 0;
					for(j=0; j<layers[h_-1].dim; ++j) {
						layers[h_].in[i] += layers[h_-1].out[j] * w->w[i*w->inputDim+j];
					}
				}
			}
		}


		void calcLayerOutput(int h_) {
			for(int i=0; i<layers[h_].dim; ++i) {
				layers[h_].out[i] = psi(layers[h_].in[i]);
			}
		}


		vector<float> classify(vector<float> x_) {
			int h;
			int i;
			if(x_.size() == inputDimension) {
				for(i=0; i<inputDimension; ++i) {
					layers[0].out[i] = x_[i];
				}
				for(h=1; h<H; ++h) {
					calcLayerInput(h);
					calcLayerOutput(h);
				}
				return layers[H-1].out;
			}
			return x_;
		}


		void calcLayerError(int h_) {
			int i,j;
			WeightMatrix *w = &(weights[h_]);
			for(i=0; i<layers[h_].dim; ++i) {
				float sum = 0;
				for(j=0; j<layers[h_+1].dim; ++j) {
					sum += w->w[j * w->inputDim + i] * layers[h_+1].err[j];
				}
				layers[h_].err[i] = dpsidx(layers[h_].in[i]) * sum;
			}
		}


		void updateWeights(int h_, float eta_) {
			WeightMatrix *w = &(weights[h_-1]);
			int i,j;
			float dw;
			for(i=0; i<w->outputDim; ++i) {
				for(j=0; j<w->inputDim; ++j) {
					dw = eta_ * ( layers[h_].err[i] * layers[h_-1].out[j]);
					w->w[i * w->inputDim + j] += dw;
				}
			}
		}

				
		float psi(float x_) {
			float a = 0.5f;
			return 1.0f / (1+exp(-a*x_));
		}

		float dpsidx(float x_) {
			return psi(x_) * (1-psi(x_));	
		}

		void setTrainingSet(vector<TrainingElement> trainingSet_) {
			trainingSet = trainingSet_;
		}

		float train(float eta_) {
			
			float trainingSetError = 0;
			int t, i, h;
			TrainingElement *te;
			for(t=0; t<trainingSet.size(); ++t) {
				te = &(trainingSet[t]);
				vector<float> x = te->in;
				vector<float> y_desired = te->out;
				vector<float> y_actual = classify(x);
				float err = 0;
				for(i=0; i<y_actual.size(); ++i) {
					err += pow(y_desired[i] - y_actual[i], 2);
				}
				trainingSetError += err*err;
				for(i=0; i<layers[H-1].dim; ++i) {
					layers[H-1].err[i] = y_desired[i] - y_actual[i];
				}
				for(h=H-2; h>=0; h--) {
					calcLayerError(h);
				}
				for(h=1; h<H; ++h) {
					updateWeights(h, eta_);
				}
			}
			return sqrt(trainingSetError); 
		}
};
