#include <iostream>
#include <thread>
#include <array>
#include <mutex>

int n = 0;  // 全局变量，10个线程共享
std::mutex mu;  // 声明创建互斥锁

// 注: 频繁lock unlock效率低，打破了线程并发执行(优化方案见mutex_lock_02)
void increase_number() 
{
    for (int i = 0; i < 1000000; i++) 
    {
        mu.lock();      // 上锁
        n++;    
        mu.unlock();    // 解锁
    }
}



int main() 
{
    const int N = 10;
    std::array<std::thread, N> workers;
    
    // 创建N个线程，每个线程执行increase_number任务
    for (int i = 0; i < N; i++) 
    {
        workers[i] = std::thread(increase_number);
    }
    
    // 线程收束
    for (auto& worker : workers) 
    {
        worker.join();
    }
    
    // 打印结果
    std::cout << "最终 n = " << n << std::endl;
    std::cout << "预期 n = " << N * 1000000 << std::endl;
    
    return 0;
}
