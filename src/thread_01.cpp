#include <iostream>
#include <thread>

void greeting(int idx){
    std::cout << "Hello from thread" << idx << "\n" << std::endl;
}

int main()
{
    const int N = 10;
    std::array<std::thread, N> workers;

    // 创建N个线程，每个线程分配执行greeting任务
    for(int i = 0; i < N; i++){
        workers[i] = std::thread(greeting, i);
    }

    // 线程收束
    for(auto& worker:workers){
        worker.join();
    }
}