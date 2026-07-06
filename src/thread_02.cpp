#include <iostream>
#include <thread>
#include <array>

int n = 0;  // 全局变量，10个线程共享

void increase_number() {
    for (int i = 0; i < 1000000; i++) {
        n++; 
    }
}

/*
n++: 将内存写入CPU寄存器eax ---> CPU计算(+1) ---> 写回内存

*/

int main() {
    const int N = 10;
    std::array<std::thread, N> workers;
    
    // 创建N个线程，每个线程执行increase_number任务
    for (int i = 0; i < N; i++) {
        workers[i] = std::thread(increase_number);
    }
    
    // 线程收束
    for (auto& worker : workers) {
        worker.join();
    }
    
    // 打印结果
    std::cout << "最终 n = " << n << std::endl;
    std::cout << "预期 n = " << N * 1000000 << std::endl;
    
    return 0;
}
