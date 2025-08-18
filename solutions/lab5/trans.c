/* 
 * trans.c - Matrix transpose B = A^T
 *
 * Each transpose function must have a prototype of the form:
 * void trans(int M, int N, int A[N][M], int B[M][N]);
 *
 * A transpose function is evaluated by counting the number of misses
 * on a 1KB direct mapped cache with a block size of 32 bytes.
 */ 
#include <stdio.h>
#include "cachelab.h"

int is_transpose(int M, int N, int A[N][M], int B[M][N]);
void transpose_submit1(int M, int N, int A[N][M], int B[M][N]);
void transpose_submit2(int M, int N, int A[N][M], int B[M][N]);
/* 
 * transpose_submit - This is the solution transpose function that you
 *     will be graded on for Part B of the assignment. Do not change
 *     the description string "Transpose submission", as the driver
 *     searches for that string to identify the transpose function to
 *     be graded. 
 */
char transpose_submit_desc[] = "Transpose submission";
void transpose_submit(int M, int N, int A[N][M], int B[M][N])
{
    if (M == 32) {
        const int BLOCK_SIZE = 8;
        int i, j;
        int t1, t2, t3, t4, t5, t6, t7, t8;
        for (i = 0; i < N; i += BLOCK_SIZE) {
            for (j = 0; j < M; j += BLOCK_SIZE) {
                for (int ii = i; ii < i + BLOCK_SIZE && ii < N; ++ii) {
                    t1 = A[ii][j];
                    t2 = A[ii][j + 1];
                    t3 = A[ii][j + 2];
                    t4 = A[ii][j + 3];
                    t5 = A[ii][j + 4];
                    t6 = A[ii][j + 5];
                    t7 = A[ii][j + 6];
                    t8 = A[ii][j + 7];
                    B[j][ii] = t1;
                    B[j + 1][ii] = t2;
                    B[j + 2][ii] = t3;
                    B[j + 3][ii] = t4;
                    B[j + 4][ii] = t5;
                    B[j + 5][ii] = t6;
                    B[j + 6][ii] = t7;
                    B[j + 7][ii] = t8;
                }
                
            }
        }    
    } else if (M == 61) {
        transpose_submit1(M, N, A, B);
    } else if (M == 64) {
        transpose_submit2(M, N, A, B); 
    }

}
// 61 x 67
void transpose_submit1(int M, int N, int A[N][M], int B[M][N])
{
    const int BLOCK_SIZE = 16;
    int i, j;
    int tmp;
    for (i = 0; i < N; i += BLOCK_SIZE) {
        for (j = 0; j < M; j += BLOCK_SIZE) {
            for (int ii = i; ii < i + BLOCK_SIZE && ii < N; ++ii) {
                for (int jj = j; jj < j + BLOCK_SIZE && jj < M; ++jj) {
                    tmp = A[ii][jj];
                    B[jj][ii] = tmp;
                }
            }
            
        }
    }    
}
// 64 x 64
void transpose_submit2(int M, int N, int A[N][M], int B[M][N])
{
    const int SMALL_BLOCK = 4;
    int i, j, ii, jj;
    int t1, t2, t3, t4, t5, t6, t7, t8;
    for (i = 0; i < N; i += SMALL_BLOCK * 2) {
        for (j = 0; j < M; j += SMALL_BLOCK * 2) {
            // 1, 2
            for (ii = i; ii < i + SMALL_BLOCK && ii < N; ++ii) {
                t1 = A[ii][j];
                t2 = A[ii][j + 1];
                t3 = A[ii][j + 2];
                t4 = A[ii][j + 3];

                t5 = A[ii][j + 4];
                t6 = A[ii][j + 5];
                t7 = A[ii][j + 6];
                t8 = A[ii][j + 7];

                B[j][ii] = t1;
                B[j + 1][ii] = t2;
                B[j + 2][ii] = t3;  
                B[j + 3][ii] = t4;

                B[j][ii + SMALL_BLOCK] = t5;
                B[j + 1][ii + SMALL_BLOCK] = t6;
                B[j + 2][ii + SMALL_BLOCK] = t7;
                B[j + 3][ii + SMALL_BLOCK] = t8;
            }
            // 3
            for (jj = j; jj < j + SMALL_BLOCK && jj < M; ++jj) {
                // B[j][ii + SMALL_BLOCK] <-> B[j + SMALL_BLOCK][ii]
                // B[j][ii + SMALL_BLOCK + 1] <-> B[j + SMALL_BLOCK][ii + 1]
                // B[j][ii + SMALL_BLOCK + 2] <-> B[j + SMALL_BLOCK][ii + 2]
                // B[j][ii + SMALL_BLOCK + 3] <-> B[j + SMALL_BLOCK][ii + 3]
                t1 = B[jj][i + SMALL_BLOCK];
                t2 = B[jj][i + SMALL_BLOCK + 1];
                t3 = B[jj][i + SMALL_BLOCK + 2];
                t4 = B[jj][i + SMALL_BLOCK + 3];

                t5 = A[i + SMALL_BLOCK][jj];
                t6 = A[i + SMALL_BLOCK + 1][jj];
                t7 = A[i + SMALL_BLOCK + 2][jj];
                t8 = A[i + SMALL_BLOCK + 3][jj];

                B[jj][i + SMALL_BLOCK] = t5;
                B[jj][i + SMALL_BLOCK + 1] = t6;
                B[jj][i + SMALL_BLOCK + 2] = t7;
                B[jj][i + SMALL_BLOCK + 3] = t8;

                B[jj + SMALL_BLOCK][i] = t1;
                B[jj + SMALL_BLOCK][i + 1] = t2;
                B[jj + SMALL_BLOCK][i + 2] = t3;
                B[jj + SMALL_BLOCK][i + 3] = t4;
            }
            // 4
            for (ii = i + SMALL_BLOCK; ii < i + SMALL_BLOCK * 2 && ii < N; ++ii) {
                t1 = A[ii][j + SMALL_BLOCK];
                t2 = A[ii][j + SMALL_BLOCK + 1];
                t3 = A[ii][j + SMALL_BLOCK + 2];
                t4 = A[ii][j + SMALL_BLOCK + 3];
                B[j + SMALL_BLOCK][ii] = t1;
                B[j + SMALL_BLOCK + 1][ii] = t2;
                B[j + SMALL_BLOCK + 2][ii] = t3;
                B[j + SMALL_BLOCK + 3][ii] = t4;
            }
        }
    }    
}
/* 
 * You can define additional transpose functions below. We've defined
 * a simple one below to help you get started. 
 */ 

/* 
 * trans - A simple baseline transpose function, not optimized for the cache.
 */
char trans_desc[] = "Simple row-wise scan transpose";
void trans(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, tmp;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            tmp = A[i][j];
            B[j][i] = tmp;
        }
    }    

}

/*
 * registerFunctions - This function registers your transpose
 *     functions with the driver.  At runtime, the driver will
 *     evaluate each of the registered functions and summarize their
 *     performance. This is a handy way to experiment with different
 *     transpose strategies.
 */
void registerFunctions()
{
    /* Register your solution function */
    registerTransFunction(transpose_submit, transpose_submit_desc); 
    /* Register any additional transpose functions */
    registerTransFunction(trans, trans_desc); 

}

/* 
 * is_transpose - This helper function checks if B is the transpose of
 *     A. You can check the correctness of your transpose by calling
 *     it before returning from the transpose function.
 */
int is_transpose(int M, int N, int A[N][M], int B[M][N])
{
    int i, j;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; ++j) {
            if (A[i][j] != B[j][i]) {
                return 0;
            }
        }
    }
    return 1;
}

