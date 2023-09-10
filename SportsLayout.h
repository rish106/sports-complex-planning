#ifndef SPORTSLAYOUT_H
#define	SPORTSLAYOUT_H

#include <fstream>
#include <iostream>
using namespace std;

class SportsLayout {

    private:
    int z,l;
    long long** T;
    long long** N;
    float time;
    int* best_mapping;
    long long obj_value;
    long long** obj_matrix;

    public:
    SportsLayout(string input_filename);

    bool check_output_format();

    long long cost_fn();

    void write_to_file(string output_filename);

    void read_input_file(string input_filename);

    void compute_allocation();
};


#endif
