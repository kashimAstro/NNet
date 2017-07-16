/*
        Test K-Means clustering
        Author: Dario Longobardi
        data:   15 / 7 / 2017

*/

#include "ClusertGraph.h"

using namespace cimg_library;

int main(int argc, char *argv[])
{
	if(argc < 3)
  	{
		cout<< "Error: file data affinity-propagation and factor"<< endl;
		exit(0);
  	}
	CImg<unsigned char> img(640,400,1,3);
        img.fill(255,255,255);
        const unsigned char p[3] = { 255,0,255 };
        
        img.draw_text(10,10,"Test affinity-propagation cluster graph",p);

        AffinityPro pro;
        pro.setup(argv[1]);
        pro.update();
	float fac = atof(argv[2]); 
	float x=200,y=120;
	int ix = 0, iy = 0;
	if(pro.getData().id.size()>0)
	{
		ix = x+pro.getData().id[0].second.x*fac;
		iy = y+pro.getData().id[0].second.y*fac;
	}
	for(unsigned int i = 0; i < pro.getData().id.size(); i++)
	{
	        unsigned char r[3] = { (unsigned char)pro.getData().id[i].second.r,
				       (unsigned char)pro.getData().id[i].second.g,
				       (unsigned char)pro.getData().id[i].second.b };

                img.draw_circle(x+pro.getData().id[i].second.x*fac,y+pro.getData().id[i].second.y*fac,2,r);
		img.draw_line(ix,iy,x+pro.getData().id[i].second.x*fac,y+pro.getData().id[i].second.y*fac,r,2);
		ix = (x+pro.getData().id[i].second.x*fac);
		iy = (y+pro.getData().id[i].second.y*fac);
	}

	img.save("affinity-propagation.jpg");
        img.display("Test");

        return 0;
}
