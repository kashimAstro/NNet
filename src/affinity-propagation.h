/*
        Test K-Means clustering
        Author: Dario Longobardi
        data:   15 / 7 / 2017

*/

#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <algorithm>
#include <map>
#include <cstdlib>

using namespace std;

struct AffPosition
{
	double x;
	double y;
	int r;
	int g;
	int b;
};

struct AffinityProData
{
	vector<pair<int,AffPosition>> id;
	//map<int,AffPosition> id;
};

class AffinityPro
{
	public:
	int N = 0;
	vector<vector<double>> S;
	vector<vector<double>> R;
	vector<vector<double>> A;
	int iter = 230;
	double lambda = 0.9;
	AffinityProData affPro;

	void setup(string _file)
	{
		read(_file);
		srand (time(NULL));
	}

	void update()
	{
		for(int m=0; m<iter; m++) {
			for(int i=0; i<N; i++) {
				for(int k=0; k<N; k++) {
					double max = -1e100;
					for(int kk=0; kk<k; kk++) {
						if(S[i][kk]+A[i][kk]>max) 
							max = S[i][kk]+A[i][kk];
					}
					for(int kk=k+1; kk<N; kk++) {
						if(S[i][kk]+A[i][kk]>max) 
							max = S[i][kk]+A[i][kk];
					}
					R[i][k] = (1-lambda)*(S[i][k] - max) + lambda*R[i][k];
				}
			}
			for(int i=0; i<N; i++) {
				for(int k=0; k<N; k++) {
					if(i==k) {
						double sum = 0.0;
						for(int ii=0; ii<i; ii++) {
							sum += max(0.0, R[ii][k]);
						}
						for(int ii=i+1; ii<N; ii++) {
							sum += max(0.0, R[ii][k]);
						}
						A[i][k] = (1-lambda)*sum + lambda*A[i][k];
					} else {
						double sum = 0.0;
						int maxik = max(i, k);
						int minik = min(i, k);
						for(int ii=0; ii<minik; ii++) {
							sum += max(0.0, R[ii][k]);
						}
						for(int ii=minik+1; ii<maxik; ii++) {
							sum += max(0.0, R[ii][k]);
						}
						for(int ii=maxik+1; ii<N; ii++) {
							sum += max(0.0, R[ii][k]);
						}
						A[i][k] = (1-lambda)*min(0.0, R[k][k]+sum) + lambda*A[i][k];
					}
				}
			}
		}
		
		double E[N][N] = {0};
		vector<int> center;
		for(int i=0; i<N; i++) {
			E[i][i] = R[i][i] + A[i][i];
			if(E[i][i]>0) {
				center.push_back(i);
			}
		}
		int idx[N] = {0};
		for(int i=0; i<N; i++) {
			int idxForI = 0;
			double maxSim = -1e100;
			for(unsigned int j=0; j<center.size(); j++) {
				int c = center[j];
				if (S[i][c]>maxSim) {
					maxSim = S[i][c];
					idxForI = c;
				}
			}
			idx[i] = idxForI;
		}
		sort(idx,idx+N);
		string append; 
		vector<string> str;
		for(int i=0; i<N; i++) 
		{
			if (i > 0 && idx[i]+1 != idx[i-1]+1) 
			{
				str.push_back(append);
				//append = "";
    			}
			append+= to_string(idx[i]+1) +";";
		}		

		for(auto & dd : str)
		{
			vector<string> res = CluserGraphUtility::split(dd,";");
			int px = (rand()%400), py = (rand()%200) , r = (rand()%255), g = (rand()%255), b = (rand()%255);
			for(int k = 0; k < res.size(); k++)
			{
				int id = atoi(res[k].c_str());

				AffPosition pos;
				pos.x     = px+(rand() % 100 + (-100)); 
				pos.y     = py+(rand() % 100 + (-100));
				pos.r     = r;
				pos.g     = g;
				pos.b     = b;
				affPro.id.emplace_back(id,pos);
			}
		}
	}

	AffinityProData getData()
	{
		return affPro;
	}

	private:
	void read(string _file) 
        {
		ifstream t(_file);
                string raw_data((istreambuf_iterator<char>(t)), istreambuf_iterator<char>());
                vector<string> dat = CluserGraphUtility::split(raw_data,"\n");
		N = dat.size();

		S.resize(N,vector<double>(N));
		R.resize(N,vector<double>(N));
		A.resize(N,vector<double>(N));
		vector<vector<double>> vec(N,vector<double>(2));

		for(int i = 0; i < N; i++)
		{
	                vector<string> dat1 = CluserGraphUtility::split(dat[i],";");
			if(dat1.size()>0)
			{
				vec.at(i).at(0) = stof(dat1.at(0));
				vec.at(i).at(1) = stof(dat1.at(1));
			}
		}
		int size = N*(N-1)/2;
		vector<double> tmpS;
		for(int i=0; i<N-1; i++) 
		{
			for(int j=i+1; j<N; j++) 
			{
				S[i][j] = -((vec[i][0]-vec[j][0])*(vec[i][0]-vec[j][0])+(vec[i][1]-vec[j][1])*(vec[i][1]-vec[j][1]));
				S[j][i] = S[i][j];
				tmpS.push_back(S[i][j]); 
			}
		}
		sort(tmpS.begin(), tmpS.end());
		double median = 0;
		if(size%2==0) 
			median = (tmpS[size/2]+tmpS[size/2-1])/2;
		else 
			median = tmpS[size/2];
		for(int i=0; i<N; i++) S[i][i] = median;
	}
};
