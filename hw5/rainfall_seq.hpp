#include <vector>

using namespace std;

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

void drop(vector<vector<float> > & ground){
  for(int i = 0;i<ground.size(); ++i){
    for(int j = 0;j<ground[0].size(); ++j){
      ground[i][j] += 1.0;
    }
  }
}

void absorb(vector<vector<float> > & ground, float amount, 
            vector<vector<float> > & absorption){
  for(int i = 0;i<ground.size(); ++i){
    for(int j = 0;j<ground[0].size(); ++j){
      absorption[i][j] += ground[i][j]>=amount ? amount : ground[i][j];
      ground[i][j] -= amount;
    }
  }
}

bool flow(vector<vector<float> > & result, 
          vector<vector<vector<int> > > & direction){
  // return true if there is still water on the groud
  // false if the ground is dry
  bool wet = false;
  int n = result.size();
  vector<vector<float>> flowMatrix(n, vector<float>(n,0.0));
  for(int i = 0; i<n; ++i){
    for(int j = 0; j<n; ++j){
      if(result[i][j]>0){
        wet = true;
        vector<int> modularPos = direction[i][j];
        float dropToFlow = result[i][j]>=1 ? 1 : result[i][j];
        float fractionDrop = dropToFlow / modularPos.size();
        for(int k = 0;k<modularPos.size();++k){
          int currX = modularPos[k]/n;
          int currY = modularPos[k]%n;
          flowMatrix[currX][currY] += fractionDrop;
        }
        flowMatrix[i][j] -= dropToFlow;
      }else{
        result[i][j] = 0;
      }
    }
  }

  for(int i=0;i<n;++i){
    for(int j=0;j<n;++j){
      result[i][j] += flowMatrix[i][j];
    }
  }
  
  return wet;
}