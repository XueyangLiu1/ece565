#include <vector>
#include <iostream>
#include <bits/stdc++.h>
#include <string>
// This is a C++ threadpool Library
#include "ctpl_stl.h"
using namespace std;

extern vector<vector<int>> elevation;
extern vector<vector<vector<int>>> direction;
extern vector<vector<float>> absorption;
extern vector<vector<float>> ground;
extern vector<vector<float>> flowMatrix;
extern vector<vector<float>> tempFlowMatrix;
extern vector<vector<float>> zeroFlowMatrix;
extern float absorptionRate;
extern bool wet;
extern mutex mtx;



vector<int> getModularPos(int n, int i, int j, vector<vector<int> > & elevationMatrix){
  int minElev = elevationMatrix[i][j];
  int possibleWay[4][2] = {{-1,0},{1,0},{0,-1},{0,1}};
  for(int k = 0;k<4; ++k){
    int x = possibleWay[k][0]+i;
    int y = possibleWay[k][1]+j;
    if(x>=0 && y>=0 && x<n && y<n){
      int elev = elevationMatrix[x][y];
      minElev = elev < minElev ? elev : minElev;
    }
  }
  vector<int> modularPos;
  if(minElev==elevationMatrix[i][j]){
    modularPos.push_back(i*n+j);
  }else{
    for(int k = 0;k<4; ++k){
      int x = possibleWay[k][0]+i;
      int y = possibleWay[k][1]+j;
      if(x>=0 && y>=0 && x<n && y<n){
        int elev = elevationMatrix[x][y];
        if(elev==minElev){
          int currPosModular = x * n + y;
          modularPos.push_back(currPosModular);
        }
      }
    }
  }
  return modularPos;
}

vector<vector<vector<int> > > getDirection(vector<vector<int> > & elevationMatrix){
  int n = elevationMatrix.size();
  vector<vector<vector<int> > > directionMatrix;
  for(int i = 0; i<n; ++i){
    vector<vector<int> > currDirectionRow;
    for(int j = 0;j<n; ++j){
      vector<int> modulaFlowTo = getModularPos(n,i,j,elevationMatrix);
      currDirectionRow.push_back(modulaFlowTo);
    }
    directionMatrix.push_back(currDirectionRow);
  }
  return directionMatrix;
}

struct Parameters{
  int startRow;
  int rowStep;
  bool rain;
  Parameters(int startRow, int rowStep, bool rain):startRow(startRow),rowStep(rowStep),rain(rain){}
  Parameters(Parameters && rhs){
    this->startRow = rhs.startRow;
    this->rowStep = rhs.rowStep;
    this->rain = rhs.rain;
  }
  Parameters(const Parameters & rhs){
    this->startRow = rhs.startRow;
    this->rowStep = rhs.rowStep;
    this->rain = rhs.rain;
  }
  ~Parameters(){}
};

void doRows(int id, Parameters &ps){
  int startRow = ps.startRow;
  int endRow = startRow + ps.rowStep;
  bool rain = ps.rain;
  int n = ground.size();
  for(int i = startRow;i<endRow;i++){
    for(int j = 0;j<n;j++){
      // add flow from prev round
      ground[i][j] += flowMatrix[i][j];
      // drop if rain
      if(rain){
        ground[i][j] += 1.0;
      }
    }
  }
  for(int i = startRow;i<endRow;i++){
    for(int j = 0;j<n;j++){
      //absorb
      float toAbsorb = ground[i][j]>=absorptionRate ? absorptionRate : ground[i][j];
      absorption[i][j] += toAbsorb;
      ground[i][j] -= toAbsorb;
      if(i==startRow || i==endRow-1){
        mtx.lock();
      }
      if(ground[i][j]>0){
        wet = true;
        vector<int> modularPos = direction[i][j];
        float dropToFlow = ground[i][j]>=1 ? 1 : ground[i][j];
        float fractionDrop = dropToFlow / modularPos.size();
        ground[i][j] -= dropToFlow;
        for(int k = 0;k<modularPos.size();++k){
          int currX = modularPos[k]/n;
          int currY = modularPos[k]%n;
          tempFlowMatrix[currX][currY] += fractionDrop;
        }
      } 
      if(i==startRow || i==endRow-1){
        mtx.unlock();
      }
    }
  }
}

int simulate(int currSteps, int rainSteps, ctpl::thread_pool &p, int threadsNum){

  while(wet){
    bool toRain = currSteps<rainSteps;
    wet = false;
    future<void> results[threadsNum];

    int rowNumForSingleThread = ground.size()/threadsNum;
    rowNumForSingleThread =  max(rowNumForSingleThread,1);
    int cumulativeRows = 0;
    int usedThreadNum = 0;

    while(cumulativeRows<ground.size()){
      results[usedThreadNum] = p.push(doRows,Parameters(cumulativeRows,rowNumForSingleThread,toRain));
      usedThreadNum++;
      cumulativeRows+=rowNumForSingleThread;
    }

    for(int i = 0;i<usedThreadNum;i++){
      results[i].wait();
    }

    flowMatrix = tempFlowMatrix;
    tempFlowMatrix = zeroFlowMatrix;
    currSteps++;
  }
  return currSteps;
}