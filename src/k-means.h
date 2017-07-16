/*
        Test K-Means clustering
        Author: Dario Longobardi
        data:   15 / 7 / 2017

*/

#include <iostream>
#include <string>
#include <fstream>
#include <streambuf>
#include <stdio.h>
#include <math.h>
#include <vector>
//#include "utility.h" 

using namespace std;

struct KMeansData
{
	vector<double> x;
	vector<double> y;
	vector<double> z;
	vector<int> id;
};

class KMeans
{
	public:
	double dmin, dx;
	double sum[3][3];
	vector<vector<double>> mean;
	vector<vector<double>> x;
	int flips;
	int size = 0;
	vector<int> cluster;
	vector<int> count;
	int color;

	KMeansData kdata;

	KMeansData getData()
	{
		return kdata;
	}

	void setup(vector<string> _in,vector<vector<double>> _mean)
	{
		size = _in.size();
		cluster.resize(size);
		count.resize(size);
		x.resize(size,vector<double>(3));
		mean.resize(3,vector<double>(3));

		for(int i = 0; i < size-1; i++)
		{
			vector<string> dat1 = CluserGraphUtility::split(_in[i],";");
			if(dat1.size()>1)
			{
				//cout << dat1.at(0) << dat1.at(1) << dat1.at(2) << endl;
			        x.at(i).at(0) = stof(dat1.at(0).c_str());
			        x.at(i).at(1) = stof(dat1.at(1).c_str());
		        	x.at(i).at(2) = stof(dat1.at(2).c_str());
			}
		}
		mean[0][0] = _mean[0][0]; mean[0][1] = _mean[1][0]; mean[0][2] = _mean[2][0];
		mean[1][0] = _mean[0][1]; mean[1][1] = _mean[1][1]; mean[1][2] = _mean[2][1];
		mean[2][0] = _mean[0][2]; mean[2][1] = _mean[1][2]; mean[2][2] = _mean[2][2];
		
  		for (int i = 0; i<size; i++) cluster.at(i) = 0;
		flips = size;
	}

	void setup(string _file,vector<vector<double>> _mean)
	{
		ifstream t(_file);
		string raw_data((istreambuf_iterator<char>(t)), istreambuf_iterator<char>());
	        vector<string> dat = CluserGraphUtility::split(raw_data,"\n");
		size = dat.size();

		cluster.resize(size);
		count.resize(size);
		x.resize(size,vector<double>(3));
		mean.resize(3,vector<double>(3));

		for(int i = 0; i < size-1; i++)
		{
			vector<string> dat1 = CluserGraphUtility::split(dat[i],";");
			if(dat1.size()>1)
			{
				//cout << dat1.at(0) << dat1.at(1) << dat1.at(2) << endl;
			        x.at(i).at(0) = stof(dat1.at(0).c_str());
			        x.at(i).at(1) = stof(dat1.at(1).c_str());
		        	x.at(i).at(2) = stof(dat1.at(2).c_str());
			}
		}
		mean[0][0] = _mean[0][0]; mean[0][1] = _mean[1][0]; mean[0][2] = _mean[2][0];
		mean[1][0] = _mean[0][1]; mean[1][1] = _mean[1][1]; mean[1][2] = _mean[2][1];
		mean[2][0] = _mean[0][2]; mean[2][1] = _mean[1][2]; mean[2][2] = _mean[2][2];

  		for (int i = 0; i<size; i++) cluster.at(i) = 0;
		flips = size;
	}

	void update()
	{
		while (flips>0) 
		{
			flips = 0;
			for (int j = 0; j < 3; j++) 
			{
      				count.at(j) = 0; 
			        for (int i = 0; i < 3; i++) 
					sum[j][i] = 0.0;
    			}
			
    			for (int i = 0; i < size; i++) 
			{
				dmin = 100000000.0; 
			        color = cluster.at(i);
				double ix,iy,iz;
			        for (int j = 0; j < 3; j++) 
				{
					dx = 0.0;
					for (int k = 0; k < 3; k++) 
					{
						dx +=  (x[i][k] - mean[j][k])*(x[i][k] - mean[j][k]);
					}
					if(j == 0) ix = dx;
					if(j == 1) iy = dx;
					if(j == 2) iz = dx;
	
					//fprintf(stdout, "%5.2f ", dx);
					if (dx < dmin) 
					{
						color = j;
						dmin = dx;
					}
      				}
			        //fprintf(stdout, "  * %d *\n", color);

				kdata.x.push_back( ix );
				kdata.y.push_back( iy );
				kdata.z.push_back( iz );
				kdata.id.push_back( color );

				if (cluster.at(i) != color)
				{
					flips++;
				        cluster.at(i) = color;
      				}
			        count.at(cluster.at(i))++;
			        for (int j = 0; j < 3; j++) 
					sum[cluster.at(i)][j] += x.at(i).at(j);
    			}
 			for (int i = 0; i < 3; i++) 
			{
				//fprintf(stderr,"(");
				for (int j = 0; j < 3; j++) 
				{
					mean[i][j] = sum[i][j]/count.at(i);
					//fprintf(stderr, "%5.2f ", mean[i][j]);
      				}
				//fprintf(stderr,")  ");
    			}
		        //fprintf(stderr, "\n **%d** \n", flips);
		}
		//sort(kdata.id.begin(),kdata.id.end());
	}
};
