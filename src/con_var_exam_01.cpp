#include <algorithm>
#include <chrono>
#include <iostream>
#include <mutex>
#include <string>
#include <syncstream>
#include <thread>
#include <vector>
#include <condition_variable>
#include <deque>

using namespace std::chrono_literals;

// std::string a;
std::deque<std::string> a;
std::mutex mutex_a;
std::condition_variable conVar;


// 发送函数
void t1()
{
    // 发送多个数据
    {
        std::unique_lock<std::mutex> lock(mutex_a);
        a.push_front("111");
        conVar.notify_one();
    }
    std::this_thread::sleep_for(1s);
    {
        std::unique_lock<std::mutex> lock(mutex_a);
        a.push_front("222");
        conVar.notify_one();
    }
    std::this_thread::sleep_for(1s);
    {
        std::unique_lock<std::mutex> lock(mutex_a);
        a.push_front("333");
        conVar.notify_one();
    }
    std::this_thread::sleep_for(1s);
    // 所有消息发送完成
    {
        std::unique_lock<std::mutex> lock(mutex_a);
        a.push_back("EXIT");
        conVar.notify_all();
    }

}

// 接收函数
void t2()
{
    std::unique_lock<std::mutex> lock(mutex_a);
    while (true)
    {
        conVar.wait(lock, []{ return !a.empty();});
        
        if (a.back() == "EXIT") break;  // 没数据了，退出
        
        std::osyncstream(std::cout) << "t2接收消息: " << a.back() << "\n";
        a.pop_back();
    }
    
    
    // conVar.wait_for(lock, 5s, []{ return !a.empty(); });    // 等待5s，超时自动退出
    
    // // 等到deadline，超时自动退出
    // auto deadline = std::chrono::steady_clock::now() + 30s;     // 获取开机时间 + 30s
    // conVar.wait_until(lock, deadline, []{return !a.empty(); });
}

// 接收函数
void t3()
{
    std::unique_lock<std::mutex> lock(mutex_a);
    while (true)
    {
        conVar.wait(lock, []{ return !a.empty();});
        
        if (a.back() == "EXIT") break;  // 没数据了，退出
        
        std::osyncstream(std::cout) << "t3接收消息: " << a.back() << "\n";
        a.pop_back();
    }
    
    
    // conVar.wait_for(lock, 5s, []{ return !a.empty(); });    // 等待5s，超时自动退出
    
    // // 等到deadline，超时自动退出
    // auto deadline = std::chrono::steady_clock::now() + 30s;     // 获取开机时间 + 30s
    // conVar.wait_until(lock, deadline, []{return !a.empty(); });
}

int main()
{
    std::vector<std::jthread> pool;

    pool.emplace_back(std::jthread(t1));
    pool.emplace_back(std::jthread(t2));
    pool.emplace_back(std::jthread(t3));

    return 0;
}