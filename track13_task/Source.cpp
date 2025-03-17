#include <iostream>
#include <vector>
#include <numeric>
#include <ctime>
#include <cstdlib>
#include <thread>
#include <mutex>

double random_sum(const std::vector<double>& A, int K, int start_index, int end_index, double& part_sum, std::mutex& mutex) {
    double sum = 0.0;
    int vector_size = A.size();
    for (int i = start_index; i < end_index; ++i) {
        int index = rand() % vector_size;
        sum += A[index];
    }

    std::lock_guard<std::mutex> lock(mutex);
    part_sum += sum;
    return sum;
}

int main() {
    int N = 50000000;
    int K = 50000000;
    int num_threads = std::thread::hardware_concurrency();
    clock_t start, end;
    srand(time(0));
    std::vector<double> A(N);
    for (int i = 0; i < N; i++)
        A[i] = (double)rand() / RAND_MAX;

    int k_thread = K / num_threads;
    int rem = K % num_threads;
    std::vector<std::thread> threads;
    std::vector<double> part_sums(num_threads, 0.0);
    std::mutex mutex;

    start = clock();

    for (int i = 0; i < num_threads; ++i) {
        int start_index = i * k_thread;
        int end_index = start_index + k_thread;
        if (i < rem) {
            start_index += i;
            end_index += (i + 1);
        }
        else {
            start_index += rem;
            end_index += rem;
        }
        threads.emplace_back(random_sum, std::ref(A), K, start_index, end_index, std::ref(part_sums[i]), std::ref(mutex));
    }
    for (auto& thread : threads) {
        thread.join();
    }
    double result = std::accumulate(part_sums.begin(), part_sums.end(), 0.0);

    end = clock();

    std::cout << "sum of " << K << " random elements: " << std::fixed << result << std::endl;
    double time_taken = double(end - start) / double(CLOCKS_PER_SEC);
    std::cout << "time: " << std::fixed << time_taken << " sec " << std::endl;

    return 0;
}