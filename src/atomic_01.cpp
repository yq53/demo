#include <iostream>
#include <thread>
#include <array>
#include <mutex>
#include <atomic>

std::atomic<int> n{0};  // 全局变量，10个线程共享

/*
其实原理很简单，因为我们原来使用的是int n，这个n是int类对象
但这里atomic是另一个模板类，那我们此处的n其实就是atomic<int>类对象
所以它的++使用的是atomic类内重载的++，不用eax暂存，直接用CPU里的ALU +1后存回原内存 
*/

/*
内存[n] --> eax --> ALU --> eax --> 内存[n]
         （中转）  （计算）  （中转）

内存[n] --> ALU --> 内存[n]
        （CPU内部特殊通路，无寄存器中转）
*/

// 原子操作的效率往往比锁更高
void increase_number() 
{
    for (int i = 0; i < 1000000; i++) 
    {
        n++;   // 变成原子操作
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
