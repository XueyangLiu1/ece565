#include "rainfall_seq.hpp"

#include <bits/stdc++.h>

#include <iostream>
#include <string>

using namespace std;

double calc_time(struct timespec start, struct timespec end) {
  double start_sec =
    (double)start.tv_sec * 1000000000.0 + (double)start.tv_nsec;
  double end_sec = (double)end.tv_sec * 1000000000.0 + (double)end.tv_nsec;

  if (end_sec < start_sec) {
    return 0;
  } else {
    return end_sec - start_sec;
  }
}

int main(int argc, char **argv) {
    if (argc != 6) {
      cout << "Syntax: ./rainfall_seq <P> <M> <A> <N> <elevation_file>" << endl;
      cout << "P = # of parallel threads to use" << endl;
      cout << "M = # of simulation time steps during which a rain drop will fall on each landcape point" << endl;
      cout << "A = absorption rate" << endl;
      cout << "N = dimension of the landscape(NxN)" << endl;
      cout << "elevation_file = name of input file that specifies the elevation of each point" << endl;
      return EXIT_FAILURE;
    }
    int rainSteps = stoi(argv[2]);
    float absorptionRate = stof(argv[3]);
    int N = stoi(argv[4]);
    string elevationFile = argv[5];
    float runtime;

    vector<vector<int>> elevation(N, vector<int>());
    vector<vector<vector<int>>> direction;
    vector<vector<float>> absorption(N, vector<float>(N,0));
    vector<vector<float>> result(N, vector<float>(N,0));
    struct timespec start_time, end_time;
    int totalSteps = 0;
    fstream input(elevationFile);
    string line;
    int eleValue;

    if (input) {
      int row = 0;

      while (getline(input, line)) {
        stringstream ss(line);
        while (ss >> eleValue) {
            elevation[row].push_back(eleValue);
        }
        ++row;
      }
    } else {
      cout << "No such elevationFile" << endl;
      return EXIT_FAILURE;
    }
    clock_gettime(CLOCK_MONOTONIC, &start_time);
    
    direction = getDirection(elevation);
    bool keepSimulate = true;
    while(keepSimulate){
      if(totalSteps<rainSteps){
        drop(result);
      }
      absorb(result,absorptionRate,absorption);
      keepSimulate = flow(result, direction);
      totalSteps++;
    }

    clock_gettime(CLOCK_MONOTONIC, &end_time);
    float elapsed_ns = calc_time(start_time, end_time);
    cout << "Rainfall simulation took " << totalSteps << " time steps to complete." << endl;
    cout << "Runtime = " << elapsed_ns / 1000000000 << " seconds" << endl;
    cout << endl;
    cout << "The following grid shows the number of raindrops absorbed at each point:" << endl;
    for (auto r : absorption) {
      for (auto c : r) {
          cout << setw(8) << setprecision(6) << c;
      }
      cout << endl;
    }
    return 0;
}
