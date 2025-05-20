#include <iostream>
#include <thread>
#include <vector>
#include <chrono>

void worker(int id)
{
  std::cout << "Thread " << id << " is running...\n";
  for (unsigned long i = 0L; i < 10000000000L; ++i)
  {
    unsigned long x = i;
  }
}

int main()
{
  unsigned int numThreads = std::thread::hardware_concurrency();
  std::cout << "Hardware supports " << numThreads << " concurrent threads.\n";

  std::vector<std::thread> threads;

  auto start = std::chrono::high_resolution_clock::now();

  for (unsigned int i = 0; i < numThreads; ++i)
  {
    threads.emplace_back(worker, i);
  }

  for (auto &t : threads)
  {
    t.join();
  }

  auto end = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
  std::cout << "All threads completed in " << duration.count() << " microseconds.\n";

  return 0;
}
