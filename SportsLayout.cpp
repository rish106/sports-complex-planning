#include <fstream>
#include <iostream>
#include <bits/stdc++.h>

using namespace std;

#include "SportsLayout.h"

SportsLayout::SportsLayout(string inputfilename) {
    read_input_file(inputfilename);
    best_mapping = new int[l];
}

bool SportsLayout::check_output_format() {
    vector<bool> visited(l, false);
    for (int i = 0; i < z; i++) {
        if (best_mapping[i] >= 1 && best_mapping[i] <= l) {
            if (visited[best_mapping[i] - 1]) {
                cout << "Repeated locations, check format\n";
                return false;
            }
            visited[best_mapping[i] - 1]=true;
        }
        else {
            cout << "Invalid location, check format\n";
            return false;
        }
    }
    return true;

}

long long SportsLayout::cost_fn() {
    long long cost = 0;
    for (int i = 0; i < z; i++) {
        for (int j = 0; j < z; j++) {
            cost += (long long)N[i][j] * (long long)T[best_mapping[i]-1][best_mapping[j]-1];
        }
    }
    return cost;
}

void SportsLayout::read_input_file(string inputfilename)
{
    fstream fin;
    fin.open(inputfilename, ios::in);
    if (!fin) {
        cout << "No such file\n";
        exit(0);
    }
    fin >> time;
    fin >> z;
    fin >> l;

    if (z > l) {
        cout << "Number of zones more than locations, check format of input file\n";
        exit(0);
    }

    int **tempT;
    int **tempN;

    tempT = new int*[l];
    for (int i = 0; i < l; ++i) {
        tempT[i] = new int[l];
    }
    tempN = new int*[z];
    for (int i = 0; i < z; ++i) {
        tempN[i] = new int[z];
    }

    for (int i = 0; i < z; i++) {
        for (int j = 0; j < z; j++) {
            fin >> tempN[i][j];
        }
    }
    for (int i = 0; i < l; i++) {
        for (int j = 0; j < l; j++) {
            fin >> tempT[i][j];
        }
    }

    fin.close();
    T = tempT;
    N = tempN;
}

void SportsLayout::write_to_file(string output_filename){
    // Open the file for writing
    ofstream fout(output_filename);

    // Check if the file is opened successfully
    if (!fout.is_open()) {
        cerr << "Failed to open the file for writing." << endl;
        exit(0);
    }

    for (int i = 0; i < z; i++) {
        fout << best_mapping[i] << " \n"[i == z-1];
    }

    // Close the file
    fout.close();

    cout << "Allocation written to the file successfully." << endl;

}

void SportsLayout::compute_allocation()
{
    int curr_mapping[l];
    obj_matrix = new int*[z];
    for (int i = 0; i < z; i++) {
        obj_matrix[i] = new int[z];
    }

    auto randomise_mapping = [&]() {
        for (int i = 0; i < l; i++) {
            curr_mapping[i] = i+1;
        }
        srand(std::time(0));
        for (int i = l-1; i > 0; i--) {
            int k = rand() % (i + 1);
            swap(curr_mapping[i], curr_mapping[k]);
        }
    };

    auto calculate_objective_matrix = [&]() {
        obj_value = 0;
        for (int i = 0; i < z; i++) {
            for (int j = 0; j < z; j++) {
                obj_matrix[i][j] = N[i][j] * T[curr_mapping[i] - 1][curr_mapping[j] - 1];
                obj_value += obj_matrix[i][j];
            }
        }
    };

    randomise_mapping();
    calculate_objective_matrix();
    for (int i = 0; i < l; i++) {
        best_mapping[i] = curr_mapping[i];
    }
    int best_cost = obj_value;

    auto delta_from_swap = [&](int i, int j) {
        int delta = 0;
        for (int k = 0; k < z; k++) {
            if (k != i && k != j) {
                delta += N[i][k] * T[curr_mapping[j] - 1][curr_mapping[k] - 1] - obj_matrix[i][k];
                delta += N[k][i] * T[curr_mapping[k] - 1][curr_mapping[j] - 1] - obj_matrix[k][i];
            }
        }
        for (int k = 0; j < z && k < z; k++) {
            if (k != i && k != j) {
                delta += N[j][k] * T[curr_mapping[i] - 1][curr_mapping[k] - 1] - obj_matrix[j][k];
                delta += N[k][j] * T[curr_mapping[k] - 1][curr_mapping[i] - 1] - obj_matrix[k][j];
            }
        }
        if (j < z) {
            delta += N[i][j] * T[curr_mapping[j] - 1][curr_mapping[i] - 1] - obj_matrix[i][j];
            delta += N[j][i] * T[curr_mapping[i] - 1][curr_mapping[j] - 1] - obj_matrix[j][i];
        }
        return delta;
    };

    auto update = [&](int i, int j) {
        swap(curr_mapping[i], curr_mapping[j]);
        for (int k = 0; k < z; k++) {
            if (k != i) {
                obj_matrix[i][k] = N[i][k] * T[curr_mapping[i] - 1][curr_mapping[k] - 1];
                obj_matrix[k][i] = N[k][i] * T[curr_mapping[k] - 1][curr_mapping[i] - 1];
            }
        }
        for (int k = 0; j < z && k < z; k++) {
            if (k != j) {
                obj_matrix[j][k] = N[j][k] * T[curr_mapping[j] - 1][curr_mapping[k] - 1];
                obj_matrix[k][j] = N[k][j] * T[curr_mapping[k] - 1][curr_mapping[j] - 1];
            }
        }
        if (obj_value < best_cost) {
            best_cost = obj_value;
            for (int i = 0; i < l; i++) {
                best_mapping[i] = curr_mapping[i];
            }
        }
    };

    auto check_neighbours = [&]() {
        for (int i = 0; i < z; i++) {
            for (int j = i+1; j < l; j++) {
                int delta = delta_from_swap(i, j);
                if (delta < 0) {
                    obj_value += delta;
                    update(i, j);
                    return true;
                }
            }
        }
        return false;
    };

    atomic<bool> stop_processing(false);
    int time_milliseconds = time * 6 * 1000 - 1000;
    thread timer_thread([&]() {
        this_thread::sleep_for(chrono::milliseconds(time_milliseconds));
        stop_processing.store(true);
    });
    auto start_time = chrono::high_resolution_clock::now();
    while (!stop_processing.load()) {
        if (!check_neighbours()) {
            randomise_mapping();
            calculate_objective_matrix();
        }
    }
    timer_thread.join();
    cout << "best_cost: " << best_cost << '\n';
}
