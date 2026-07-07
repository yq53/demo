#include <cstdio>
#include <iostream>
#include <thread>

void greeting(int idx)
{
    // std::cout << "Hello from thread" << idx << "\n" << std::endl;

    printf("Hellp from thread %d\n", idx);  // 线程安全
}

int main()
{
    const int N = 10;
    std::array<std::thread, N> workers;

    // 创建N个线程，每个线程分配执行greeting任务
    for(int i = 0; i < N; i++)
    {
        workers[i] = std::thread(greeting, i);  // 这里是移动拷贝，std::thread把 = 重载了
    }

    // 线程收束
    for(auto& worker:workers)
    {
        worker.join();
    }
}