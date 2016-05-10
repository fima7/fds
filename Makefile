all: array-test array-test-debug

#all: array-test.cpp array.hpp array-alt.hpp point.hpp
#	g++ -std=c++11 -Wall array-test.cpp -o array-test 
#debug: array-test.cpp array.h array-alt.hpp point.h
#	g++ -std=c++11 -Wall -g array-test.cpp -o array-test

# -DGLIBCXX_FORCE_NEW

array-test: array-test.o huge.o
	g++ -DGLIBCXX_FORCE_NEW -std=c++11 -Wall array-test.o huge.o -o array-test	
array-test-debug: array-test-debug.o huge-debug.o
	g++ -DGLIBCXX_FORCE_NEW -std=c++11 -Wall -g array-test-debug.o huge-debug.o -o array-test-debug	

array-test.o: array-test.cpp array.hpp
	g++ -DGLIBCXX_FORCE_NEW -std=c++11 -Wall -c array-test.cpp -o array-test.o	

huge.o: huge.cpp
	g++ -DGLIBCXX_FORCE_NEW -std=c++11 -Wall -c huge.cpp -o huge.o 

array-test-debug.o: array-test.cpp
	g++ -DGLIBCXX_FORCE_NEW -std=c++11 -Wall -g -c array-test.cpp -o array-test-debug.o	

huge-debug.o: huge.cpp
	g++ -DGLIBCXX_FORCE_NEW -std=c++11 -Wall -g -c huge.cpp -o huge-debug.o 

clean:
	rm -f *.o array-test	
