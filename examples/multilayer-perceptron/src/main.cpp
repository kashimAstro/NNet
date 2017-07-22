#include "ofMain.h"
#include "NNet.h"

class ofApp : public ofBaseApp 
{
	public:
	ofImage img;
	int iteration, mode, width, height, twidth, theight;
	bool training;
	MultilayerPerceptron *mlp;
	vector<MultilayerPerceptron::TrainingElement> trainingSet;

	void addPointToTrainingSet(float x_, float y_) 
	{
	        float x = (x_-0.5*width) / (0.5 * width);
        	float y = (y_-0.5*height) / (0.5 * height);
	        vector<float> teInput;
        	teInput.push_back(x);
	        teInput.push_back(y);
	        vector<float> teOutput;
	        teOutput.push_back(0.0);
	        teOutput.push_back(0.0);
	        teOutput.push_back(0.0);
	        teOutput[mode-1] = 1.0f;
	        trainingSet.push_back(MultilayerPerceptron::TrainingElement(teInput, teOutput));
        	mlp->setTrainingSet(trainingSet);
	}

	void setup()
	{
		iteration = 0;
		mode 	  = 1;
		training  = false;
	 	width     = ofGetWidth();
		height    = ofGetHeight();
		twidth    = 64;
		theight   = 48;
		img.allocate(twidth,theight,OF_IMAGE_COLOR);
		img.setColor(ofColor::white);
		img.update();

		mlp = new MultilayerPerceptron(2, 3);
	        mlp->addHiddenLayer(10);
        	mlp->addHiddenLayer(10);
	        mlp->init();
        	mlp->setTrainingSet(trainingSet);
	}

	void create()
	{
		vector<float> testInput;
	        vector<float> testOutput;
	        testInput.push_back(0.0);
	        testInput.push_back(0.0);
	        for(int iy = 0; iy < theight; ++iy) 
		{
               		for(int ix = 0; ix < twidth; ++ix) 
			{
	                        testInput[0] = (2.0f*ix/twidth)-1;
	                        testInput[1] = (2.0f*iy/theight)-1;
	                        testOutput = mlp->classify(testInput);

				img.setColor(ix,iy,
				 	     ofColor( 
						    (128 * testOutput[0]),
		 				    (128 * testOutput[1]),
						    (128 * testOutput[2])
					     )
			        );
				img.update();

               		}
        	}
	}

	void draw()
	{
		ofSetWindowTitle(ofToString(ofGetFrameRate()));
		string msg;
		iteration++;
	        if(training) 
		{
		        float err = mlp->train(0.2f);
			create();
	                msg = "iteration:"+ofToString(iteration)+" error:"+ofToString(err);
               	}
		ofSetColor(255,255,255);
		img.draw(0,0,width,height);
		MultilayerPerceptron::TrainingElement *te;
        	for(int k=0; k<trainingSet.size(); ++k) 
		{
                	te = &(trainingSet[k]);
	                ofSetColor(ofMap(te->out[0],0.,1.,0,255), 
				   ofMap(te->out[1],0.,1.,0,255), 
				   ofMap(te->out[2],0.,1.,0,255));
        	        ofDrawRectangle(320*(1+te->in[0]), 240*(1+te->in[1]),5,5);
        	}
		ofDrawBitmapStringHighlight(msg,20,20);
	}

	void mousePressed(int x, int y, int button)
	{
                addPointToTrainingSet(x, y);	
	}

	void keyPressed(int key)
	{
		if(key == '1') 	mode = 1;
		if(key == '2')	mode = 2;
		if(key == '3')	mode = 3;
		if(key == '4')	training = true;
		if(key == '5')
		{
			training = false;
                        mlp->resetWeights();
                        trainingSet.clear();
                        create();
		}
	}
};

int main()
{
        ofSetupOpenGL(640,480, OF_WINDOW);
        ofRunApp( new ofApp());
}
