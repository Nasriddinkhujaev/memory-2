

/*
Task 1 Compose a dense square matrix multiplication program in C. Implement matrix
multiplication blocking. Generate two 640x640 matrixes, and do matrix multiplication
with two version of matrix multiplication program, compare their output.
*/

#include <stdio.h> // for printf
#include <stdlib.h> // for malloc and free
#include <time.h> // for clock


#define SIZE 640 // size of the matrix
#define BLOCK_SIZE 64 // block size for blocking. common cache line size is 64 bytes, which can hold 16 integers (4 bytes per integer). By using a block size that fits well within the cache, we can reduce cache misses and improve performance.

// Function to generate a random matrix
void generate_matrix(int matrix[SIZE][SIZE]){
    srand(time(NULL)); // set the seed for random number generation based on the current time
    for (int i = 0; i < SIZE; i++){
        for (int j = 0; j < SIZE; j++){
            // need to generate random numbers each time we run the program
            matrix[i][j] = rand() % 10; // generate a random number between 0 and 9
        }
    }
}
// to call this function:
// int A[SIZE][SIZE], B[SIZE][SIZE], C[SIZE][SIZE];
// generate_matrix(A);

// function to perform standard matrix multiplication 
void naive_matrix_multiply(int A[SIZE][SIZE], int B[SIZE][SIZE], int C[SIZE][SIZE]){
    for (int i = 0; i < SIZE; i++){
        for (int j = 0; j < SIZE; j++){
            C[i][j] = 0;
            for (int k = 0; k < SIZE; k++){
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}
// to call this function:
// standard_matrix_multiply(A, B, C);


// function to perform blocked matrix multiplication
// the idea is to divide the matrices into smaller blocks and perform multiplication on these blocks. This can improve cache performance by keeping the data in the cache during the multiplication process
void blocked_matrix_multiply(int A[SIZE][SIZE], int B[SIZE][SIZE], int C[SIZE][SIZE]) {
    for (int i = 0; i < SIZE; i += BLOCK_SIZE) { // i += block size to move to the next block. instead of processing one element at a time, we process a block of elements that fits well into the cache and helps to reduce cache misses.
        for (int j = 0; j < SIZE; j += BLOCK_SIZE) {
            for (int k = 0; k < SIZE; k += BLOCK_SIZE) {
                // Multiply block A(i, k) with block B(k, j) and store the result in block C(i, j)
                for (int ii = i; ii < i + BLOCK_SIZE && ii < SIZE; ii++) {
                    for (int jj = j; jj < j + BLOCK_SIZE && jj < SIZE; jj++) {
                        int sum = 0; // to store the sum of the products for the current block

                        for (int kk = k; kk < k + BLOCK_SIZE && kk < SIZE; kk++) {
                            sum += A[ii][kk] * B[kk][jj]; // multiply the corresponding elements and store the result in sum
                        }
                        C[ii][jj] += sum; // add the sum to the current value in C[ii][jj]. 
                    }
                }
            }
        }
    }
}

// to call this function:
// blocked_matrix_multiply(A, B, C);

// function to compare the results of the matrix multiplication. compares if they have the same values in the resulting matrix C. If they are the same, it returns 1, otherwise it returns 0.
int compare_matrices(int C1[SIZE][SIZE], int C2[SIZE][SIZE]) {
    for (int i = 0; i < SIZE; i++){
        for (int j = 0; j < SIZE; j++){
            if(C1[i][j] != C2[i][j]){ // check if the values at [i][j] are different in both matrices. if they are different, it returns 0 which means false or not equal
                return 0;
            }
        }
    }
    return 1;// if no missmatch, the return 1 which means true 
}
// to call this function:
// if (compare_matrices(C1, C2)) {
//     printf("The matrices are the same.\n");}

// function to print a matrix. 
void print_matrix(int matrix[SIZE][SIZE]) {
    for (int i = 0; i < 5; i++){
        for (int j = 0; j < 5; j++){
            printf("    %d ", matrix[i][j]);
        }
        printf("\n");
    }
}


// main function to test the matrix multiplication and compare the results
int main()
{
    int A[SIZE][SIZE], B[SIZE][SIZE], C1[SIZE][SIZE], C2[SIZE][SIZE];
    srand(time(NULL)); // set the seed for random number generation based on the current time
    generate_matrix(A);
    generate_matrix(B);

    clock_t start, end;
    double cpu_time_used;

    // Test naive matrix multiplication
    printf("Naive Matrix top left 5X5 corner:\n");
    start = clock();
    naive_matrix_multiply(A, B, C1);

    print_matrix(C1);
    end = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("C1 cpu time used: %f seconds\n", cpu_time_used);

    // Test blocked matrix multiplication
    printf("\nBlocked Matrix top left 5X5 corner:\n");
    start = clock();
    blocked_matrix_multiply(A, B, C2);

    print_matrix(C2);
    end = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("C2 cpu time used: %f seconds\n", cpu_time_used);

    // compare the results of the two matrix multiplications
    if(compare_matrices(C1, C2)) {
        printf("\nOutput Match: Yes\n");
    } else {
        printf("\nOutput Match: No\n");
    }
}

// why every time i run the program, the top left 5X5 corner of the resulting matrix is same? i though it should be different because the input matrices are generated randomly. but it seems that the random number generator is producing the same sequence of numbers each time the program is run. this is because the rand() function uses a seed value to generate random numbers, and if the seed value is not changed, it will produce the same sequence of random numbers each time. To fix this, you can use the srand() function to set a different seed value before calling rand(). For example, you can use srand(time(NULL)) to set the seed based on the current time, which will ensure that you get different random numbers each time you run the program. lets do it 