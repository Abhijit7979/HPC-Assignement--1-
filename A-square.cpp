#include <iostream>
#include <vector>
#include <ctime>
#include <omp.h>

// Function to perform matrix multiplication
std::vector<std::vector<int>> matrix_multiply(const std::vector<std::vector<int>>& A, const std::vector<std::vector<int>>& B) {
    int m = A.size();
    int n = B[0].size();
    int p = B.size();

    std::vector<std::vector<int>> result(m, std::vector<int>(n, 0));

    #pragma omp parallel for collapse(2)
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            for (int k = 0; k < p; ++k) {
                result[i][j] += A[i][k] * B[k][j];
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

    // Start timer
    double start_time = omp_get_wtime();

    // Calculate square of sample matrix using ordinary matrix multiplication
    std::vector<std::vector<int>> result = matrix_multiply(A, A);

    // End timer
    double end_time = omp_get_wtime();

    // Print execution time
    std::cout << "Execution time: " << end_time - start_time << " seconds" << std::endl;

    return 0;
}
