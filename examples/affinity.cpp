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
		cout<< "Error: file data affinity-propagation"<< endl;
		exit(0);
  	}

	float fac = 1;//atof(argv[2]); 
	float x=200,y=120;
        const unsigned char p[3] = { 255,0,255 };
	CImg<unsigned char> img(640,400,1,3);
        img.fill(255,255,255);
        img.draw_text(10,10,"Test affinity-propagation cluster graph",p);

        AffinityPro pro;
        pro.setup(argv[1]);
        pro.update();

	for(unsigned int i = 0; i < pro.getData().id.size(); i++)
	{
	        unsigned char r[3] = { (unsigned char)pro.getData().id[i].second.r,
				       (unsigned char)pro.getData().id[i].second.g,
				       (unsigned char)pro.getData().id[i].second.b };

		if( pro.getData().id[i].first != pro.getData().id[i+1].first )
	                img.draw_circle(x+pro.getData().id[i].second.x*fac,y+pro.getData().id[i].second.y*fac,45,r,0.2);
		
                img.draw_circle(x+pro.getData().id[i].second.x*fac,y+pro.getData().id[i].second.y*fac,2,r);
	}

	img.save("affinity-propagation.jpg");
        img.display("Test");

        return 0;
}
