// optimized version of matrix column normalization
#include "colnorm.h"

////////////////////////////////////////////////////////////////////////////////
// REQUIRED: Paste a copy of your sumdiag_benchmark from an ODD grace
// node below.
//
// -------REPLACE WITH YOUR RUN + TABLE --------
// 
// grace3>> ./colnorm_benchmark 
// ==== Matrix Column Normalization Benchmark Version 1 ====
// ------ Tuned for ODD grace.umd.edu machines -----
// Running with REPEATS: 2 and WARMUP: 1
// Running with 4 sizes (max 6001) and 4 thread_counts (max 4)
//   ROWS   COLS   BASE  T   OPTM SPDUP POINT TOTAL 
//   1111   2223  0.029  1  0.000  0.00  0.00  0.00 
//                       2  0.000  0.00  0.00  0.00 
//                       3  0.000  0.00  0.00  0.00 
//                       4  0.000  0.00  0.00  0.00 
//   2049   4098  0.202  1  0.000  0.00  0.00  0.00 
//                       2  0.000  0.00  0.00  0.00 
//                       3  0.000  0.00  0.00  0.00 
//                       4  0.000  0.00  0.00  0.00 
//   4099   8197  2.566  1  0.000  0.00  0.00  0.00 
//                       2  0.000  0.00  0.00  0.00 
//                       3  0.000  0.00  0.00  0.00 
//                       4  0.000  0.00  0.00  0.00 
//   6001  12003  5.801  1  0.000  0.00  0.00  0.00 
//                       2  0.000  0.00  0.00  0.00 
//                       3  0.000  0.00  0.00  0.00 
//                       4  0.000  0.00  0.00  0.00 
// RAW POINTS: 0.00
// TOTAL POINTS: 0 / 35
// -------REPLACE WITH YOUR RUN + TABLE --------


// You can write several different versions of your optimized function
// in this file and call one of them in the last function.

int cn_verA(matrix_t mat, vector_t avg, vector_t std, int thread_count) {
  for(int j=0; j<mat.cols; j++){             // for each column in matrix

    double sum_j = 0.0;                      // PASS 1: Compute column average
    for(int i=0; i<mat.rows; i++){                 
      sum_j += MGET(mat,i,j);
    }
    double avg_j = sum_j / mat.rows;
    VSET(avg,j,avg_j);
    sum_j = 0.0;

    for(int i=0; i<mat.rows; i++){           // PASS 2: Compute standard deviation
      double diff = MGET(mat,i,j) - avg_j;
      sum_j += diff*diff;
    };
    double std_j = sqrt(sum_j / mat.rows);
    VSET(std,j,std_j);

    for(int i=0; i<mat.rows; i++){           // PASS 3: Normalize matrix column
      double mij = MGET(mat,i,j);
      mij = (mij - avg_j) / std_j;
      MSET(mat,i,j,mij);
    }
  }
  return 0;
}

int cn_verB(matrix_t mat, vector_t avg, vector_t std, int thread_count) {
  // OPTIMIZED CODE HERE
  return 0;
}


int colnorm_OPTM(matrix_t mat, vector_t avg, vector_t std, int thread_count){
  // call your preferred version of the function
  return cn_verA(mat, avg, std, thread_count);
}

////////////////////////////////////////////////////////////////////////////////
// REQUIRED: DON'T FORGET TO PASTE YOUR TIMING RESULTS FOR
// sumdiag_benchmark FROM AN ODD GRACE NODE AT THE TOP OF THIS FILE
////////////////////////////////////////////////////////////////////////////////
