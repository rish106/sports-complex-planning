#include <fstream>
#include <iostream>
#include <vector>
#include <chrono>
#include "SportsLayout.h"

using namespace std;

typedef long long ll;

SportsLayout::SportsLayout(string inputfilename) {
    read_input_file(inputfilename);
    best_mapping = new int[z];
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

ll SportsLayout::cost_fn() {
    ll cost = 0;
    for (int i = 0; i < z; i++) {
        for (int j = 0; j < z; j++) {
            cost += N[i][j] * T[best_mapping[i] - 1][best_mapping[j] - 1];
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

    ll **tempT;
    ll **tempN;

    tempT = new ll*[l];
    for (int i = 0; i < l; ++i) {
        tempT[i] = new ll[l];
    }
    tempN = new ll*[z];
    for (int i = 0; i < z; ++i) {
        tempN[i] = new ll[z];
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
    auto start = chrono::high_resolution_clock::now();
    int curr_mapping[l];
    obj_matrix = new ll*[z];
    for (int i = 0; i < z; i++) {
        obj_matrix[i] = new ll[z];
    }
    ll best_cost;
    ll MAX_DURATION_MILLISECONDS = ll(time * 60.0 * 1000.0) - 500;

    auto time_limit_exceeded = [&]() {
        auto curr = chrono::high_resolution_clock::now();
        auto elapsed_milliseconds = chrono::duration_cast<chrono::milliseconds>(curr - start);
        return elapsed_milliseconds.count() >= MAX_DURATION_MILLISECONDS;
    };

    auto randomise_mapping = [&]() {
        for (int i = 0; i < l; i++) {
            if (time_limit_exceeded()) {
                return;
            }
            curr_mapping[i] = i+1;
        }
        srand(std::time(0));
        for (int i = l - 1; i > 0; i--) {
            if (time_limit_exceeded()) {
                return;
            }
            int k = rand() % (i + 1);
            swap(curr_mapping[i], curr_mapping[k]);
        }
    };

    auto calculate_objective_matrix = [&]() {
        obj_value = 0;
        for (int i = 0; i < z; i++) {
            for (int j = 0; j < z; j++) {
                if (time_limit_exceeded()) {
                    return;
                }
                obj_matrix[i][j] = N[i][j] * T[curr_mapping[i] - 1][curr_mapping[j] - 1];
                obj_value += obj_matrix[i][j];
            }
        }
    };

    auto delta_from_swap = [&](int i, int j) {
        ll delta = 0;
        for (int k = 0; k < z; k++) {
            if (time_limit_exceeded()) {
                return 0ll;
            }
            if (k != i && k != j) {
                delta += N[i][k] * T[curr_mapping[j] - 1][curr_mapping[k] - 1] - obj_matrix[i][k];
                delta += N[k][i] * T[curr_mapping[k] - 1][curr_mapping[j] - 1] - obj_matrix[k][i];
            }
        }
        for (int k = 0; j < z && k < z; k++) {
            if (time_limit_exceeded()) {
                return 0ll;
            }
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

    auto update_best_mapping = [&]() {
        for (int i = 0; i < z; i++) {
            best_mapping[i] = curr_mapping[i];
        }
    };

    auto mapping_swap = [&](int i, int j) {
        swap(curr_mapping[i], curr_mapping[j]);
        for (int k = 0; k < z; k++) {
            if (time_limit_exceeded()) {
                return;
            }
            if (k != i) {
                obj_matrix[i][k] = N[i][k] * T[curr_mapping[i] - 1][curr_mapping[k] - 1];
                obj_matrix[k][i] = N[k][i] * T[curr_mapping[k] - 1][curr_mapping[i] - 1];
            }
        }
        if (j < z) {
            for (int k = 0; k < z; k++) {
                if (time_limit_exceeded()) {
                    return;
                }
                if (k != j) {
                    obj_matrix[j][k] = N[j][k] * T[curr_mapping[j] - 1][curr_mapping[k] - 1];
                    obj_matrix[k][j] = N[k][j] * T[curr_mapping[k] - 1][curr_mapping[j] - 1];
                }
            }
        }
        if (obj_value < best_cost) {
            best_cost = obj_value;
            update_best_mapping();
        }
    };

    auto check_neighbours = [&]() {
        for (int i = 0; i < z; i++) {
            for (int j = i+1; j < l; j++) {
                if (time_limit_exceeded()) {
                    return true;
                }
                ll delta = delta_from_swap(i, j);
                if (delta < 0) {
                    obj_value += delta;
                    mapping_swap(i, j);
                    return true;
                }
            }
        }
        return false;
    };

    randomise_mapping();
    calculate_objective_matrix();
    update_best_mapping();
    best_cost = obj_value;

    while (true) {
        if (time_limit_exceeded()) {
            break;
        }
        if (!check_neighbours()) {
            randomise_mapping();
            calculate_objective_matrix();
        }
    }
}
