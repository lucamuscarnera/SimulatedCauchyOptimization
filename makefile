y:
	mkdir -p build
	g++ -fopenmp -fconcepts -o build/test1  \
							Tests/travelling_salesman.cpp \
							Serial/Vectorial/VectorialClasses/*.cpp \
							Serial/Vectorial/VectorialClasses/PermutationHelpers/*.cpp 

	g++ -fopenmp -fconcepts -o build/real_test  \
							Tests/real_test.cpp \
							Serial/Vectorial/VectorialClasses/*.cpp \
							Serial/Vectorial/VectorialClasses/PermutationHelpers/*.cpp 	


	g++ -fopenmp -fconcepts -o build/vector_test  \
							Tests/vector_test.cpp \
							Serial/Vectorial/VectorialClasses/*.cpp \
							Serial/Vectorial/VectorialClasses/PermutationHelpers/*.cpp 						