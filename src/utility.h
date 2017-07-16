/*
        Test K-Means clustering
        Author: Dario Longobardi
        data:   15 / 7 / 2017

*/

#include <iostream>
#include <vector>
#include <regex>
#include <cctype>
#include <string>
#include <algorithm>

using namespace std;

#define NOT_USED  0 /* node is currently not used */
#define LEAF_NODE 1 /* node contains a leaf node */
#define A_MERGER  2 /* node contains a merged pair of root clusters */
#define MAX_LABEL_LEN 16

#define AVERAGE_LINKAGE  'a' /* choose average distance */
#define CENTROID_LINKAGE 't' /* choose distance between cluster centroids */
#define COMPLETE_LINKAGE 'c' /* choose maximum distance */
#define SINGLE_LINKAGE   's' /* choose minimum distance */

#define alloc_mem(N, T) (T *) calloc(N, sizeof(T))
#define alloc_fail(M)   fprintf(stderr,"Failed to allocate memory for %s.\n", M)
#define read_fail(M)    fprintf(stderr, "Failed to read %s from file.\n", M)
#define invalid_node(I) fprintf(stderr, "Invalid cluster node at index %d.\n", I)

namespace CluserGraphUtility
{
	vector<string> split(const string& input, const string& _regex) 
	{
	    regex re(_regex);
	    sregex_token_iterator
		first{input.begin(), input.end(), re, -1},
		last;
	    return {first, last};
	}

	inline string trim(const string &s)
	{
	   auto wsfront=find_if_not(s.begin(),s.end(),[](int c){return isspace(c);});
	   auto wsback=find_if_not(s.rbegin(),s.rend(),[](int c){return isspace(c);}).base();
	   return (wsback<=wsfront ? string() : string(wsfront,wsback));
	}

	float map(float value, float inputMin, float inputMax, float outputMin, float outputMax, bool clamp=false) {
		if (fabs(inputMin - inputMax) < FLT_EPSILON)
		{
			return outputMin;
		} 
		else 
		{
			float outVal = ((value - inputMin) / (inputMax - inputMin) * (outputMax - outputMin) + outputMin);
			if( clamp ){
				if(outputMax < outputMin){
					if( outVal < outputMax )outVal = outputMax;
					else if( outVal > outputMin )outVal = outputMin;
				}else{
					if( outVal > outputMax )outVal = outputMax;
					else if( outVal < outputMin )outVal = outputMin;
				}
			}
			return outVal;
		}

	}
};
