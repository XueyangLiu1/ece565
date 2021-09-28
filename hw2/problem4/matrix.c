#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <stdint.h>

double **matrix_a;
double **matrix_b;
double **matrix_c;
uint64_t matrix_size = 1024;

double calc_time(struct timespec start, struct timespec end) { 
  double start_sec = (double)start.tv_sec*1000000000.0 + (double)start.tv_nsec; 
  double end_sec = (double)end.tv_sec*1000000000.0 + (double)end.tv_nsec; 
  if (end_sec < start_sec) {  
    return 0;   
  } else {   
    return end_sec - start_sec;
  } 
}

void free_matrix(double **matrix){
  for(uint64_t i = 0;i<matrix_size;++i){
    free(matrix[i]);
  }
  free(matrix);
}

void init_matrix(double ***matrix) {
  double **temp = (double**)malloc(matrix_size * sizeof(double*));
  for(uint64_t i = 0;i<matrix_size;++i){
    double *matrix_row = (double*)malloc(matrix_size * sizeof(double));
    for(uint64_t j = 0;j<matrix_size;++j){
      matrix_row[j] = (double)j;
    }
    temp[i] = matrix_row;
  }
  *matrix = temp;
}

void calculate_result(int loop_type){
  if(loop_type==1){
    //i,j,k
    double sum;
    for (uint64_t i=0; i<matrix_size; ++i) {
      for (uint64_t j=0; j<matrix_size; ++j) {
        sum = 0;
        for (uint64_t k=0; k<matrix_size; ++k) {
          sum += matrix_a[i][k] * matrix_b[k][j];
        }
        matrix_c[i][j] = sum;
      }
    }
  }else if(loop_type==2){
    //k,i,j
    double temp;
    for (uint64_t k=0; k<matrix_size; ++k) {
      for (uint64_t i=0; i<matrix_size; ++i) {
        temp = matrix_a[i][k];
        for (uint64_t j=0; j<matrix_size; ++j) {
          matrix_c[i][j] += temp * matrix_b[k][j];
        }
      }
    }
  }else if(loop_type==3){
    //j,k,i
    double temp;
    for (uint64_t j=0; j<matrix_size; ++j) {
      for (uint64_t k=0; k<matrix_size; ++k) {
        temp = matrix_b[k][j];
        for (uint64_t i=0; i<matrix_size; ++i) {
          matrix_c[i][j] += temp * matrix_a[i][k];
        }
      }
    }
  }else if(loop_type==4){
    //i,j,k tiling
    uint64_t block_size = 64;
    for (uint64_t i=0; i<matrix_size; i+=block_size) {
      for (uint64_t j=0; j<matrix_size; j+=block_size) {
        for(uint64_t x = i ; x < i + block_size ; ++x){
          for(uint64_t y = j ; y < j + block_size ; ++y){
            for(uint64_t k = 0 ; k < matrix_size ; k += block_size){
              for(uint64_t z = k ; z < k + block_size ; ++z){
                  matrix_c[x][y] += matrix_a[x][z] * matrix_b[z][y]; 
              }
            }
          }
        }
      }
    }
  }else{
    printf("loop type can only be 1,2,3");
  }
}

int main(int argc, char *argv[]) {
  if(argc!=2){
      return EXIT_FAILURE;
  }
  int loop_type = atoi(argv[1]);

  init_matrix(&matrix_a);
  init_matrix(&matrix_b);
  init_matrix(&matrix_c);

  struct timespec start_time, end_time;
  clock_gettime(CLOCK_MONOTONIC, &start_time);
  calculate_result(loop_type);
  clock_gettime(CLOCK_MONOTONIC, &end_time);

  double elapsed_ns = calc_time(start_time, end_time);
  printf("Time = %fs\n", elapsed_ns / 1000000000);

  free_matrix(matrix_a);
  free_matrix(matrix_b);
  free_matrix(matrix_c);
  return 0;
}