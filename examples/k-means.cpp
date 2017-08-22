/*
        Author: Dario Longobardi
        data:   2017
*/
#include "NNet.h"

using namespace cimg_library;

int main(int argc, char *argv[])
{
	if(argc < 2)
	{
		cout<< "Error: file data (k-means)"<< endl;
		exit(0);
	}

	/* set image graph */
	CImg<unsigned char> img(640,400,1,3);
	img.fill(255,255,255);
	const unsigned char p[3] = { 255,0,255 };
	const unsigned char b[3] = { 55,10,225 };
	const unsigned char d[3] = { 15,250,15 };
	img.draw_text(10,10,"Test k-means cluster graph",p);

	/* medium point */
	vector<vector<double>> med;
        med.resize(3,vector<double>(3));
	med[0][0]=-5.0; med[1][0]=-1.0; med[2][0]=-3.5;
        med[0][1]=0.0;  med[1][1]=-2.5; med[2][1]=3.0;
        med[0][2]=1.0;  med[1][2]=5.0;  med[2][2]=0.8;

	/* k-means algorithm */
	KMeans k;
	k.setup(argv[1],med); /* file data and medium point */
	k.update();

	/* draw point graph */
	float f = 0.6;
	int x = 100;
	int y = 80;
	for( unsigned int i = 0; i < k.getData().id.size(); i++ )
	{
		if( k.getData().id[i] == 0 ) img.draw_circle(x+k.getData().x[i]*f,y+k.getData().y[i]*f,2,p);
		if( k.getData().id[i] == 1 ) img.draw_circle(x+k.getData().x[i]*f,y+k.getData().y[i]*f,2,b);
		if( k.getData().id[i] == 2 ) img.draw_circle(x+k.getData().x[i]*f,y+k.getData().y[i]*f,2,d);
	}
	
 	/* save and display */
	img.save("k-means.jpg");
	img.display("Test");
}
