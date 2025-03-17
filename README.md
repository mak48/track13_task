# Task report
The task is:
> Implement a function in C++, which for a given array A of size N with double-precision floating-point 
numbers (64 bits per number) and some positive integer number K, performs summation of K randomly 
selected elements from the array A with the uniform probability (probabilities to select i-th element and j-th 
element are equal for any i and j) with replacement (each element can be selected more than once). Try to 
have correct summation result and to have your implementation performance-efficient (assuming that N 
and/or K can be big enough, for example, N >= 50,000,000, K >= 50,000,000).

At first, initiliaze the variables and the array. To fill the array and select random elements I use functions ```srand``` and ```rand```.
```
int N = 50000000;
int K = 50000000;
srand(time(0));
std::vector<double> A(N);
for (int i = 0; i < N; i++)
    A[i] = (double)rand() / RAND_MAX;
```
To make implementation more *performance-efficient* I used multithreading solution.
The program creates several threads
```
std::vector<std::thread> threads;
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
```
And every thread enters the ```random_sum``` function where it calculates a partial sum of random array elements. 
```
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
```

These partial sums are then added together to get the final result
```
double result = std::accumulate(part_sums.begin(), part_sums.end(), 0.0);
```
And then I print the result to console
```
std::cout << "sum of " << K << " random elements: " << std::fixed << result << std::endl;
```
To see how much time was taken by solution, I used ```clock()``` and then calculated the taken time
```
double time_taken = double(end - start) / double(CLOCKS_PER_SEC);
std::cout << "time: " << std::fixed << time_taken << " sec " << std::endl;
```
At first I did the solution without multithreading so we can compare results by time now. 

*before:*

![Снимок экрана 2025-03-18 010655](https://github.com/user-attachments/assets/bf393792-fcc4-4f26-882a-4f7aa65b7aff)

*after:*

![Снимок экрана 2025-03-17 231314](https://github.com/user-attachments/assets/d743dd6e-a797-4d8c-a9b0-d2763cb2e192)

It can be seen that optimizing the solution using multithreading has reduced the execution time in several times.
