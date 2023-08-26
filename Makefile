all: main.cpp SportsLayout.cpp
	g++ -o main main.cpp SportsLayout.cpp

test: gen_tests.cpp
	g++ -o gen_tests gen_tests.cpp
