#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

void printArray(int count, int *array) {
  printf("[ ");
  for (int i = 0; i < count; i++) {
    printf("%i, ", array[i]);
  }
  printf("]\n");
}

void printMatrix(int count, int array[][count]) {
  printf("[ \n");
  for (int i = 0; i < count; i++) {
    printf("  [ ");
    for (int j=0; j < count; j ++) {
      printf("%i", array[i][j]);
      if (j == count-1) {
        printf(" ");
      } else {
        printf(", ");
      }
    }
    printf("]\n");
  }
  printf("]\n");
}

void printMatrixFile(FILE *fp, int count, int array[][count]) {
  fprintf(fp, "[ \n");
  for (int i = 0; i < count; i++) {
    fprintf(fp, "  [ ");
    for (int j=0; j < count; j ++) {
      fprintf(fp, "%i, ", array[i][j]);
    }
    fprintf(fp, "]\n");
  }
  fprintf(fp, "]\n");
}


struct ranked_point {
  int rank;
  int x;
  int y;
};


int my_solution(int width, int array[][width]) {
  int max_val;

  // struct holds the x/y and rank for the smallest
  // rank for each thread, at the end we compare
  // all the threads and pull the smallest rank
  struct ranked_point rankedA[omp_get_num_threads()];
  struct ranked_point rankedB[omp_get_num_threads()];
  struct ranked_point rankedC[omp_get_num_threads()];
  struct ranked_point rankedD[omp_get_num_threads()];


  // initialize max rank in each struct
  int max_rank = width * width + 1;
  #pragma omp parallel for
  for (int i=0; i < omp_get_num_threads(); i++) {
    rankedA[i].rank = max_rank;
    rankedB[i].rank = max_rank;
    rankedC[i].rank = max_rank;
    rankedD[i].rank = max_rank;
  }

  int rank_a, rank_b, rank_c, rank_d;

  // Iterate through each point to find the closest to each corner
  //
  // We could do this faster if I could find a
  // way to generate an equa-distance algorithm radiating from the each
  // corner in a way that is parallel friendly.
  // At worst, this algorithm looks at every x/y
  // 4 times if there is a 1 and 1 time if there is a 0
  // so it is at worst O(n) squared
  #pragma omp parallel for
  for (int x = 0 ; x < width; x ++) {
    for (int y =0; y < width; y++) {
      if (array[x][y] == 1) {
        rank_a = abs(0 - x)     + abs(0 - y);
        rank_b = abs(0 - x)     + abs(width - y);
        rank_c = abs(width - x) + abs(0 - y);
        rank_d = abs(width - x) + abs(width - y);

        if (rank_a < rankedA[omp_get_thread_num()].rank) {
          rankedA[omp_get_thread_num()].rank = rank_a;
          rankedA[omp_get_thread_num()].x = x;
          rankedA[omp_get_thread_num()].y = y;
        }

        if (rank_b < rankedB[omp_get_thread_num()].rank) {
          rankedB[omp_get_thread_num()].rank = rank_b;
          rankedB[omp_get_thread_num()].x = x;
          rankedB[omp_get_thread_num()].y = y;
        }

        if (rank_c < rankedC[omp_get_thread_num()].rank) {
          rankedC[omp_get_thread_num()].rank = rank_c;
          rankedC[omp_get_thread_num()].x = x;
          rankedC[omp_get_thread_num()].y = y;
        }

        if (rank_d < rankedD[omp_get_thread_num()].rank) {
          rankedD[omp_get_thread_num()].rank = rank_d;
          rankedD[omp_get_thread_num()].x = x;
          rankedD[omp_get_thread_num()].y = y;
        }
      }
    }
  }

  struct ranked_point pointA = rankedA[0];
  struct ranked_point pointB = rankedB[0];
  struct ranked_point pointC = rankedC[0];
  struct ranked_point pointD = rankedD[0];

  // Pull out the actual closest point to each corner
  for (int i = 0; i < omp_get_num_threads(); i++) {
    if (rankedA[i].rank < pointA.rank) {
      pointA = rankedA[i];
    }
    if (rankedB[i].rank < pointB.rank) {
      pointB = rankedB[i];
    }
    if (rankedC[i].rank < pointC.rank) {
      pointC = rankedC[i];
    }
    if (rankedD[i].rank < pointD.rank) {
      pointD = rankedD[i];
    }
  }

  printf("closest points:\n");
  printf("  rank: %i (%i, %i)\n", pointA.rank, pointA.x, pointA.y);
  printf("  rank: %i (%i, %i)\n", pointB.rank, pointB.x, pointB.y);
  printf("  rank: %i (%i, %i)\n", pointC.rank, pointC.x, pointC.y);
  printf("  rank: %i (%i, %i)\n", pointA.rank, pointA.x, pointA.y);

  int next_val;

  // Compare each of the closets points to one another, take
  // the maximum value
  //
  // yah, it's gross, but it's not that many points
  // compare each of the "closest" points to one another,
  // and take the maximum value
  max_val = abs(pointA.x - pointB.x) + abs(pointA.y - pointB.y);
  next_val = abs(pointA.x - pointC.x) + abs(pointA.y - pointC.y);
  if (next_val > max_val)
    max_val = next_val;
  next_val = abs(pointA.x - pointD.x) + abs(pointA.y - pointD.y);
  if (next_val > max_val)
    max_val = next_val;
  next_val = abs(pointB.x - pointC.x) + abs(pointB.y - pointC.y);
  if (next_val > max_val)
    max_val = next_val;
  next_val = abs(pointB.x - pointD.x) + abs(pointB.y - pointD.y);
  if (next_val > max_val)
    max_val = next_val;

  next_val = abs(pointD.x - pointC.x) + abs(pointD.y - pointC.y);
  if (next_val > max_val)
    max_val = next_val;

  return max_val;
}

int main (int argc, char *argv[]){
  int width = -5;
  srand(time(NULL));
  double probability = -1.0;


  for (int i = 0; i < argc; i++) {
    char const *option =  argv[i];
    if (option[0] == '-') {
      switch (option[1])
      {
        case 'p':
          probability = atoi(argv[i+1])/100.0;
          break;
        case 'w':
          width = atoi(argv[i+1]);
          break;
        default:
          printf("flag not recognised %s\n", option);
          break;
      }
    }
    else {
    }
  }

  if (width < 0) {
    width = 5;
  }


  if (probability < 0) {
    probability = 1.0/width;
  }

  int input_array[width][width];
  while (1) {
    // printf("%i\n", );
    #pragma omp parallel for
    for (int i=0; i < width; i++) {
      for (int j=0; j < width; j++) {
        input_array[i][j] = rand() <  probability * ((double)RAND_MAX + 1.0);
      }
    }

    int my_out = my_solution(width, input_array);

    printMatrix(width, input_array);
    printf("Maximum manhattan distance: %i\n", my_out);

    char name[100];
    printf("press enter to continue> ");
    fgets(name,20,stdin);
  }
}