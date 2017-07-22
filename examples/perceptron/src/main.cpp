#include "ofMain.h"
#include "NNet.h"

class ofApp : public ofBaseApp 
{
	public:
	Perceptron *ptron;
	vector<PTrainer*> training;
	int count = 0;
	float xmin = -400;
	float ymin = -100;
	float xmax =  400;
	float ymax =  100;

	float fun(float x) { return 0.4*x+1; }

	void setup()
	{
		training.resize(2000);
		ptron = new Perceptron(3, 0.00001);
  		for (int i = 0; i < training.size(); i++) 
		{
			float x = ofRandom(xmin, xmax);
			float y = ofRandom(ymin, ymax);
			int answer = 1;
			if (y < fun(x)) answer = -1;
			training[i] = new PTrainer(x, y, answer);
  		}
	}

	void draw()
	{
		ofBackgroundGradient(ofColor::white,ofColor::black);
		ofTranslate(ofGetWidth()/2,ofGetHeight()/2);

		float x1 = xmin;
		float y1 = fun(x1);
		float x2 = xmax;
		float y2 = fun(x2);
		ofSetColor(ofColor::red);
		ofDrawLine(x1,y1,x2,y2);

                vector<float> weights = ptron->getWeights();
		x1 = xmin;
		y1 = (-weights[2] - weights[0]*x1)/weights[1];
		x2 = xmax;
		y2 = (-weights[2] - weights[0]*x2)/weights[1];
		ofSetColor(ofColor::blue);
		ofDrawLine(x1,y1,x2,y2);
		ptron->train(training[count]->inputs, training[count]->answer);
		count = (count + 1) % training.size();
		for (int i = 0; i < count; i++) 
		{
			ofSetColor(ofColor::green);
		    	int guess = ptron->fforward(training[i]->inputs);
		    	if (guess > 0) ofSetColor(ofColor::red);
		    	ofDrawEllipse(training[i]->inputs[0], training[i]->inputs[1], 8, 8);
		}
	}
};

int main()
{
        ofSetupOpenGL(640,260, OF_WINDOW);
        ofRunApp( new ofApp());
}
