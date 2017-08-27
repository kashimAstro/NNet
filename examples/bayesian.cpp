#include <iostream>
#include <stdlib.h>
#include "NNet.h"

using namespace std;

int main( int argc, char** argv )
{
	if( argc != 4 ) {
		cout << "Error parameter: train, test, cfg";
		exit(0);
	}

	char* train = argv[1];
	char* input = argv[2];
	char* cfg   = argv[3];

	BayesianNetwork bnet(train, input, cfg);

	return 0;
}
