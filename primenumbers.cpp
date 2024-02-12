#include <iostream>
#include <fstream>
#include <omp.h>

#define START (1LL << 40)  // 2^40
#define END (1LL << 41)    // 2^41

bool is_prime(long long n) {
    if (n <= 1) return false;
    if (n <= 3) return true;
    if (n % 2 == 0 || n % 3 == 0) return false;
    for (long long i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0)
            return false;
    }
    return true;
}

int main() {
    std::ofstream outfile("prime_numbers.txt");
    if (!outfile.is_open()) {
        std::cerr << "Failed to open file for writing prime numbers." << std::endl;
        return 1;
    }

    std::cout << "Prime numbers between 2^40 and 2^41:\n";
    
    // Set the number of threads to use
    omp_set_num_threads(16); // Set to the desired number of threads
    
    double start_time = omp_get_wtime(); // Record the start time

    #pragma omp parallel for
    for (long long i = START; i <= END; ++i) {
        if (is_prime(i)) {
            #pragma omp critical
            {
                outfile << i << " ";
            }
        }
    }

    double end_time = omp_get_wtime(); // Record the end time
    double execution_time = end_time - start_time; // Calculate execution time

    outfile.close();

    std::cout << "\nExecution time: " << execution_time << " seconds\n";
    
    return 0;
}
