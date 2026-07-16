#include <iostream>
#include <mutex>
#include <stdexcept>
#include <syncstream>
#include <thread>
#include <vector>

std::string a = "000";
std::mutex mutex_a;

int b = 0;

// lock 与 unlock有很多种方式，以下列出了常见的几种情况

// 1. 手动lock/unlock
// 注意多个线程共享同个锁，之后谁先抢到lock谁先执行
// 最大的问题就是麻烦。如果在lock的中途遇到了异常，则会导致程序阻塞永远不unlock。如果恰好该异常还被捕获，那直接就deadlock了

// void t1()
// {
//     try {
//         mutex_a.lock();
//         a = "111";

//         throw std::runtime_error("似了喵");     // lock中途抛出异常
//         mutex_a.unlock();
//     } catch (...) {  }
// }

// void t2()
// {
//     mutex_a.lock();
//     a = "222";
//     mutex_a.unlock();
// }

// 2. lock_guard自动lock/unlock
// lock_guard本质是个模板类，用的RAII思想让类对象构造时lock，析构时unlock

// void t1()
// {
//     {
//         std::lock_guard<std::mutex> lock(mutex_a);
//         a = "111";
//     }

//     b = 2;
// }

// void t2()
// {
//     std::lock_guard<std::mutex> lock(mutex_a);
//     a = "222";

// }

// 3. unique_lock
// 可以提前解锁

void t1()
{
    std::unique_lock<std::mutex> lock(mutex_a);
    a = "111";

    if (b)
    {
        lock.unlock();
    }

}

void t2()
{
    std::unique_lock<std::mutex> lock(mutex_a);
    a = "222";

}

int main()
{
    std::vector<std::jthread> pool;

    pool.emplace_back(std::jthread(t1));
    pool.emplace_back(std::jthread(t2));

    pool.clear();   // jthread在被清除时自动join

    // for (auto &&t : pool) t.join();

    std::osyncstream(std::cout) << a << "\n";

    return 0;
}
