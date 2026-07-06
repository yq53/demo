#include <iostream>
#include <thread>
#include <array>
#include <mutex>

int n = 0;  // 全局变量，10个线程共享
std::mutex mu;  // 声明创建互斥锁

/*  
定义临时变量避免了：
1. 多线程对同一全局变量执行操作，导致多线程混乱。临时变量间相互独立
2. 仅最后一步操作需要lock/unlock，累加temp过程多线程并发，效率更高
*/
void increase_number() 
{
    int temp = 0;
    for (int i = 0; i < 1000000; i++) 
    {
        temp++;    
    }

    mu.lock();
    n += temp;
    mu.unlock();
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
