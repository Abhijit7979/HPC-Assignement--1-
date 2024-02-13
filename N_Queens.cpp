#include <iostream>
#include <vector>
#include <omp.h>

using namespace std;

bool isSafe(int row, int col, const vector<int>& placement) {
    for (int i = 0; i < row; ++i) {
        if (placement[i] == col || abs(i - row) == abs(placement[i] - col)) {
            return false;
        }
    }
    return true;
}

void solveNQueens(int n, int row, vector<int>& placement, int& count) {
    if (row == n) {
        #pragma omp atomic
        ++count; // Increment solution count
        return;
    }

    // Parallelize solving for each column in the current row
    #pragma omp parallel for
    for (int col = 0; col < n; ++col) {
        if (isSafe(row, col, placement)) {
            vector<int> temp_placement = placement; // Create a local copy for each thread
            temp_placement[row] = col;
            solveNQueens(n, row + 1, temp_placement, count);
        }
    }
}

int main() {
    int n;
    int num_threads;

    cout << "Enter the size of the chessboard (n): ";
    cin >> n;

    cout << "Enter the number of threads: ";
    cin >> num_threads;

    if (n <= 0 || num_threads <= 0) {
        cout << "Invalid input. Please enter positive values for n and number of threads.\n";
        return 1;
    }

    vector<int> placement(n, 0);
    int count = 0;

    omp_set_num_threads(num_threads);

    double start_time = omp_get_wtime();

    solveNQueens(n, 0, placement, count);

    double end_time = omp_get_wtime();
    double execution_time = end_time - start_time;

    cout << "Total number of solutions: " << count << endl;
    cout << "Execution time: " << execution_time << " seconds" << endl;

    return 0;
}
