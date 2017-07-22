/*
	Test K-Means clustering
	Author: Dario Longobardi
	data:   15 / 7 / 2017

*/
#include "NNet.h"

using namespace cimg_library;

int main(int argc, char *argv[])
{
	if(argc < 3)
	{
		cout<< "Error: ./bin/k-means-image image factor"<< endl;
		exit(0);
	}

	CImg<unsigned char> img(argv[1]);
	CImgDisplay disp_img(img, argv[1]);

	CImg<unsigned char> output(640,400,1,3);
	const unsigned char p[3] = { 255,0,255 };
        const unsigned char b[3] = { 55,10,225 };
        const unsigned char d[3] = { 15,250,15 };

        output.fill(255,255,255);
	output.draw_text(10,10,"Test k-means-image cluster graph",b);

	int w = img.width();
	int h = img.height();
	vector<string> color;
	for(int j = 0; j < w; j++)
	{
		for(int k = 0; k < h; k++)
		{
			int r = img(j, k, 0);
			int g = img(j, k, 1);
			int b = img(j, k, 2);
			string out = to_string(              CluserGraphUtility::map(r,0,255,0.0,1.0))+
					       ";"+to_string(CluserGraphUtility::map(g,0,255,0.0,1.0))+
					       ";"+to_string(CluserGraphUtility::map(b,0,255,0.0,1.0))+"\n";
			cout << out ;
			color.push_back(out);
		}
	}

	KMeans k;
	vector<vector<double>> med;
	med.resize(3,vector<double>(3));
	med[0][0]=1.0; med[1][0]=0.0; med[2][0]=0.5;
	med[0][1]=0.5; med[1][1]=0.5; med[2][1]=1.0;
	med[0][2]=0.0; med[1][2]=1.0; med[2][2]=0.0;

	k.setup(color,med);
	k.update();

	float f = atof(argv[2]);
	int x = 100;
	int y = 80;
	for( unsigned int i = 0; i < k.getData().id.size(); i++ )
	{
		if( k.getData().id[i] == 0 ) output.draw_circle(x+k.getData().x[i]*f,y+k.getData().y[i]*f,2,p);
		if( k.getData().id[i] == 1 ) output.draw_circle(x+k.getData().x[i]*f,y+k.getData().y[i]*f,2,b);
		if( k.getData().id[i] == 2 ) output.draw_circle(x+k.getData().x[i]*f,y+k.getData().y[i]*f,2,d);
	}
	
	output.save("k-means-image.jpg");
	CImgDisplay disp_output(output);

	while(true) 
	{
     		CImgDisplay::wait(disp_img,disp_output);
	}
}
