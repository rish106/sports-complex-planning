
#ifndef SPORTSLAYOUT_H
#define	SPORTSLAYOUT_H

#include <fstream>
#include <iostream>
#include <bits/stdc++.h>
using namespace std;

class SportsLayout {

    private:
    int z,l;
    int** T;
    int** N;
    int time;
    int* best_mapping;
    int obj_value;
    int** obj_matrix;

    public:
    SportsLayout(string input_filename);

    bool check_output_format();

    // void readOutputFile(string output_filename);
    
    long long cost_fn();

    void write_to_file(string output_filename);

    void read_input_file(string input_filename);

    void compute_allocation();
};


#endif
