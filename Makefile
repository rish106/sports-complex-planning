all: main.cpp SportsLayout.cpp
	g++ -o main.out main.cpp SportsLayout.cpp

test: gen_tests.cpp
	g++ -o gen_tests.out gen_tests.cpp
