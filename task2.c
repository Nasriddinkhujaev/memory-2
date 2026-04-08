/*
Task 2 Generate random dense square matrices, with size of [20x20, 40x40, 80x80,
160x160, 320x320, 640x640, 1280x1280]. Record and plot elapsed times with different
Block size. Which block size is the fastest? Show your answer in the report
*/

#include<stdio.h>
#include <stdlib.h>
#include <time.h>

#define NUM_SIZES 7 // number of different matrix sizes to test
#define NUM_BLOCKS 6 // number of different block sizes to test

int sizes[NUM_SIZES] = {20, 40, 80, 160, 320, 640, 1280}; // different matrix sizes to test
int block_sizes[NUM_BLOCKS] = {8, 16, 32, 64, 128, 256}; // different block sizes to test

// 1D indexing
#define A(i, j, n) A[(i) * (n) + (j)] // basically in simple words A(i, j, n) is a macro that converts 2D indices (i, j) into a 1D index for accessing elements in a 1D array A.
#define B(i, j, n) B[(i) * (n) + (j)]
#define C(i, j, n) C[(i) * (n) + (j)]

// Blocked matrix multiplication
void blocked_matmul(int *A, int *B, int *C, int N, int BS)
{
    for (int i = 0; i < N; i++) // initialize the resulting matrix C to zero before performing the multiplication
    {
        for (int j = 0; j < N; j++)
        {
            C(i, j, N) = 0;
        }
    }

    for (int ii = 0; ii < N; ii += BS) // ii += block size to move to the next block. 
    {
        for (int jj = 0; jj < N; jj += BS) // jj += block size to move to the next block
        {
            for (int kk = 0; kk < N; kk += BS) // kk += block size to move to the next block
            {
                for (int i = ii; i < ii + BS && i < N; i++) // i < ii + BS && i < N to ensure that we do not go out of bounds when processing the last block
                {
                    for (int j = jj; j < jj + BS && j < N; j++) 
                    {
                        int sum = C(i, j, N); // to store the sum of the products for the current block
                        for (int k = kk; k < kk + BS && k < N; k++)
                        {
                            sum += A(i, k, N) * B(k, j, N);
                        }
                        C(i, j, N) = sum;
                    }
                }
            }
        }
    }
}

// Generate random matrix
void generate_matrix(int *M, int N)
{
    for (int i = 0; i < N * N; i++)
    {
        M[i] = rand() % 10;
    }
}

int main()
{
    srand(time(NULL));

    printf("N\tBS=8\tBS=16\tBS=32\tBS=64\tBS=128\tBS=256\tBestBS\tBestTime(s)\n");  // print the header for the output table

    for (int s = 0; s < NUM_SIZES; s++) // loop through each matrix size to test the performance of different block sizes
    {
        int N = sizes[s]; // get the current matrix size from the sizes array

        // Allocate matrices
        int *A = malloc(N * N * sizeof(int)); // allocate memory for matrix A, B and C. we use malloc to dynamically allocate memory for the matrices based on the current size N. each matrix is a 1D array of size N*N to store the elements of the matrix in a contiguous block of memory.
        int *B = malloc(N * N * sizeof(int));
        int *C = malloc(N * N * sizeof(int));

        generate_matrix(A, N);
        generate_matrix(B, N);

        printf("%d\t", N); // print the current matrix size 

        double best_time = 1e9; // 1e9 is a very large number to initialize the best time. 
        int best_bs = -1; // initialize best block size to -1 to indicate that we have not found a valid block size yet.

        for (int b = 0; b < NUM_BLOCKS; b++) // loop through each block size to test the performance of the blocked matrix multiplication for the current matrix size N
        {
            int BS = block_sizes[b]; // get the current block size from the block_size array

            if (BS > N) // if the block size is larger than the matrix size, we cannot perform blocked matrix multiplication. in this case, we print "N/A"
            {
                printf("N/A\t");
                continue;
            }

            clock_t start = clock(); 

            blocked_matmul(A, B, C, N, BS);

            clock_t end = clock();

            double time = (double)(end - start) / CLOCKS_PER_SEC;

            printf("%.5f\t", time);

            if (time < best_time)
            {
                best_time = time;
                best_bs = BS;
            }
        }

        printf("%d\t%.5f\n", best_bs, best_time);

        free(A);
        free(B);
        free(C);
    }

    return 0;
}

/* example output:
N        BS=8        BS=16       BS=32       BS=64       BS=128      BS=256      BestBS       BestTime(s)
20       0.000046     0.000041     0.000038     0.000039     0.000036     0.000036     128          0.000036
40       0.000270     0.000179     0.000179     0.000176     0.000279     0.000292     64           0.000176
80       0.001764     0.001409     0.001654     0.001878     0.001682     0.001540     16           0.001409
160      0.013046     0.011353     0.010750     0.010971     0.010565     0.010404     256          0.010404
320      0.098416     0.089935     0.085778     0.087318     0.085559     0.085567     128          0.085559
640      0.823804     0.718660     0.689838     0.697262     0.682030     0.700673     128          0.682030
1280     6.395969     5.795705     5.556359     5.627483     6.139690     6.390465     32           5.556359
*/