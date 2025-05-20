#include <iostream>
#include <future>
#include <chrono>

int computeSum(int a, int b)
{
  std::this_thread::sleep_for(std::chrono::seconds(2));
  return a + b;
}

int main()
{
  std::future<int> result = std::async(std::launch::async, computeSum, 5, 10);

  std::cout << "Computing in background...\n";

  std::cout << "Result: " << result.get() << "\n";

  return 0;
}
