all:
	mkdir -p bin image
	g++ -Wall -o bin/k-means-example       -Isrc/ examples/k-means.cpp       --std=c++11 -O3 -lm -lpthread -lX11
	g++ -Wall -o bin/affinity-example      -Isrc/ examples/affinity.cpp      --std=c++11 -O3 -lm -lpthread -lX11
	g++ -Wall -o bin/k-means-image-example -Isrc/ examples/k-means-image.cpp --std=c++11 -O3 -lm -lpthread -lX11
	g++ -Wall -o bin/agglomerative         -Isrc/ examples/agglomerative.cpp --std=c++11 -O3 -lm -lpthread -lX11
	g++ -Wall -o bin/mnist-cnn             -Isrc/ examples/mnist-cnn.cpp     --std=c++11 -O3 -lm -lpthread -lX11
	cd examples/multilayer-perceptron/ && make -f Makefile && cd ../../
	cd examples/perceptron/ && make -f Makefile && cd ../../
