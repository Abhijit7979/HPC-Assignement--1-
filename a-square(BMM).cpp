#include <iostream>
#include <vector>
#include <ctime>
#include <omp.h>

// Function to perform matrix multiplication
std::vector<std::vector<int>> block_matrix_multiply(const std::vector<std::vector<int>>& A, const std::vector<std::vector<int>>& B, int blockSize) {
    int m = A.size();
    int n = B[0].size();
    int p = B.size();

    std::vector<std::vector<int>> result(m, std::vector<int>(n, 0));

    #pragma omp parallel for collapse(2)
    for (int i = 0; i < m; i += blockSize) {
        for (int j = 0; j < n; j += blockSize) {
            for (int k = 0; k < p; k += blockSize) {
                for (int ii = i; ii < std::min(i + blockSize, m); ++ii) {
                    for (int jj = j; jj < std::min(j + blockSize, n); ++jj) {
                        for (int kk = k; kk < std::min(k + blockSize, p); ++kk) {
                            result[ii][jj] += A[ii][kk] * B[kk][jj];
                        }
                    }
                }
            }
        }
    }

    return result;
}

// Function to generate a sample matrix with random values from {-1, 0, 1}
std::vector<std::vector<int>> generate_sample_matrix(int rows, int cols) {
    std::vector<std::vector<int>> matrix(rows, std::vector<int>(cols));
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            matrix[i][j] = rand() % 3 - 1; // {-1, 0, 1}
        }
    }
    return matrix;
}

int main() {
    // Set the number of threads
    omp_set_num_threads(16); // Change the number of threads as needed

    // Define matrix dimensions
    int rows = 1024;
    int cols = 1024;

    // Generate sample matrix
    std::vector<std::vector<int>> A = generate_sample_matrix(rows, cols);

    // Prompt user for block size
    int blockSize;
    std::cout << "Enter block size (4, 8, 16, 32, or 64): ";
    std::cin >> blockSize;

    // Start timer
    double start_time = omp_get_wtime();

    // Calculate square of sample matrix using block matrix multiplication
    std::vector<std::vector<int>> result = block_matrix_multiply(A, A, blockSize);

    // End timer
    double end_time = omp_get_wtime();

    // Print execution time
    std::cout << "Execution time: " << end_time - start_time << " seconds" << std::endl;

    return 0;
}
