/*
        Test K-Means clustering
        Author: Dario Longobardi
        data:   15 / 7 / 2017

*/

#include "ClusertGraph.h"

using namespace cimg_library;
using namespace CGAgglomerative;

int main(int argc, char *argv[])
{
	if(argc < 3)
  	{
		cout<< "Error: file data agglomerative and factor"<< endl;
		exit(0);
  	}
	CImg<unsigned char> img(640,400,1,3);
        img.fill(255,255,255);
        const unsigned char p[3] = { 255,0,255 };
        const unsigned char b[3] = { 55,10,225 };
        const unsigned char d[3] = { 0,0,0 };
        img.draw_text(10,10,"Test agglomerative cluster graph",p);

	item_t *items = NULL;
	int num_items = process_input(&items, argv[1]);
	set_linkage('s');
	if (num_items) {
		cluster_t *cluster = agglomerate(num_items, items);
		free(items);
		if (cluster) {
			print_cluster(cluster);
			int k = 3;
			get_k_clusters(cluster, k);
			free_cluster(cluster);
			AgglomerativeData data = getData();
			float f = atof(argv[2]);
		        int x = 100;
		        int y = 80;
			for(int i = 0; i < data.x.size(); i++)
			{
                		img.draw_circle(x+data.x[i]*f,y+data.y[i]*f,2,d);
                		img.draw_circle(x+data.x[i]*f,y+data.y[i]*f,40,b,0.1);
				img.draw_text(x+data.x[i]*f,y+data.y[i]*f,data.item[i].c_str(),p);
			}
		}
	}
	img.save("agglomerative.jpg");
	img.display("Test");


        return 0;
}
