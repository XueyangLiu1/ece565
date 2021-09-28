#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <stdint.h>

int num_elements;
int num_traversals;
uint64_t *array;

double calc_time(struct timespec start, struct timespec end) { 
  double start_sec = (double)start.tv_sec*1000000000.0 + (double)start.tv_nsec; 
  double end_sec = (double)end.tv_sec*1000000000.0 + (double)end.tv_nsec; 
  if (end_sec < start_sec) {  
    return 0;   
  } else {   
    return end_sec - start_sec;
  } 
}

void init_array() {
  uint64_t tmp;
  for(int i = 0;i<num_elements;i++){
    array[i] = -i;
  }
}

int main(int argc, char *argv[]) {
  if(argc!=3){
    return EXIT_FAILURE;
  }
  struct timespec start_time, end_time;

  num_elements = atoi(argv[1]);
  num_traversals = atoi(argv[2]);
  array = (uint64_t*)malloc(num_elements * sizeof(uint64_t));

  init_array();
  
  clock_gettime(CLOCK_MONOTONIC, &start_time);
  for(int i = 0;i<num_traversals;i++){
    for (uint64_t j = 0; j < num_elements; j++) {
      array[j] = j;
    }
  }
  clock_gettime(CLOCK_MONOTONIC, &end_time);

  double elapsed_ns = calc_time(start_time, end_time);
  printf("Time = %f\n", elapsed_ns);
  printf("Bandwidth = %f GB/s\n", ((uint64_t)num_elements * num_traversals * 8) / elapsed_ns);

  free(array);
  
  return 0;
}