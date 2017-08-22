/*
        Author: Dario Longobardi
        data:   2017
*/

#include "NNet.h"
#include <vector>
#include <time.h>

using namespace cimg_library;

int main(int argc, char *argv[])
{

	if(argc < 2)
        {
                cout<< "Error: file data rbm"<< endl;
                exit(0);
        }

        srand (time(NULL));
        CImg<unsigned char> img(640,400,1,3);
        img.fill(255,255,255);
        const unsigned char p[3] = { 255,0,255 };
        const unsigned char b[3] = { 55,10,225 };
        const unsigned char d[3] = { 15,250,15 };
        img.draw_text(10,10,"Test restricted boltzmann machine",b);

	double lrate  = 0.1;
	int tepoch    = 1000;
	int k         = 1;
	int test_num  = 2;
	int nvisible  = 6;
	int nhidden   = 3;

	/* training data */
	vector<vector<int>> train_data;
	vector<string> data = CluserGraphUtility::readfile(argv[1]);
	for(auto & d : data)
	{
	        vector<string> sp = CluserGraphUtility::split(d,",");
		vector<int> t;
	        for(int i = 0; i < sp.size(); i++)
		{
			t.push_back( atoi(sp[i].c_str()) );
		}
		train_data.push_back(t);
	}

	/* RBM */
	RBM rbm;
	rbm.setup(train_data.size(), nvisible, nhidden, NULL, NULL, NULL);

	/* train */
	for(int e=0; e<tepoch; e++) 
	{
		for(int i = 0; i < train_data.size(); i++)
		{
			int * tdat = train_data[i].data();
			rbm.cdivergence(tdat, lrate, k);
		}
	}

	/* test data */
	int tester[test_num][6] = {
	    {1, 1, 0, 0, 0, 0},
	    {0, 0, 0, 1, 1, 0},
	};
	double output[2][6];

	img.draw_line ((640/2), 0, (640/2), 481, b);

	/* test */
	for(int i=0; i<test_num; i++) 
	{
	    rbm.classify(tester[i], output[i]);
	    for(int j=0; j<nvisible; j++) 
	    {
	    	  printf("value=[%i] prob=[%.5f] ", tester[i][j], output[i][j]);
		  if(output[i][j] > 0.50)
		  {
			int x = CluserGraphUtility::random(30,640/2);
			int y = CluserGraphUtility::random(30, 440);
			string str = to_string(tester[i][j])+"  "+to_string(output[i][j]);

		        img.draw_circle(x,y,8,p);
		        img.draw_text(x-4,y-4,str.c_str(),b);
		  }
		  else {
			int x = CluserGraphUtility::random(640/2, 600);
			int y = CluserGraphUtility::random(30, 440);
			string str = to_string(tester[i][j])+"  "+to_string(output[i][j]);

			img.draw_circle(x,y,8,d);
		        img.draw_text(x-4,y-4,str.c_str(),b);
		  }
	    }
	    cout << endl;
	}

        img.save("rbm.jpg");
	img.display("Test");
	/* exit */
	rbm.exit();
  	return 0;
}
