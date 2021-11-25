#include "rainfall_pt.hpp"

// This is a C++ threadpool Library
#include "ctpl_stl.h"

using namespace std;

vector<vector<int>> elevation;
vector<vector<vector<int>>> direction;
vector<vector<float>> absorption;
vector<vector<float>> ground;
vector<vector<float>> flowMatrix;
vector<vector<float>> tempFlowMatrix;
vector<vector<float>> zeroFlowMatrix;
float absorptionRate;
bool wet;
mutex mtx;

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
  int threadsNum = stoi(argv[1]);
  int rainSteps = stoi(argv[2]);
  absorptionRate = stof(argv[3]);
  int N = stoi(argv[4]);
  string elevationFile = argv[5];
  float runtime;
  elevation = vector<vector<int>>(N, vector<int>());
  absorption = vector<vector<float>>(N, vector<float>(N,0));
  ground = vector<vector<float>>(N, vector<float>(N,0));
  flowMatrix = vector<vector<float>>(N, vector<float>(N,0.0));
  tempFlowMatrix = vector<vector<float>>(N, vector<float>(N,0.0));
  zeroFlowMatrix = vector<vector<float>>(N, vector<float>(N,0.0));
  wet = true;

  struct timespec start_time, end_time;
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
  direction = getDirection(elevation);
  ctpl::thread_pool p(threadsNum);
  clock_gettime(CLOCK_MONOTONIC, &start_time);
  
  int totalSteps = simulate(0, rainSteps, p, threadsNum);

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
