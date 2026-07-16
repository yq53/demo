/* 对exam1的封装 */

#include <algorithm>
#include <chrono>
#include <condition_variable>
#include <iostream>
#include <deque>
#include <mutex>
#include <optional>
#include <string>
#include <syncstream>
#include <thread>
#include <vector>

using namespace std::chrono_literals;

template<class T>
class mt_queue
{
public:
    // 推入数据，并通知
    void push(T value)
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_queue.push_front(std::move(value));
        m_cv.notify_one();
    }

    // 陷入阻塞，直到有数据
    T pop()
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_cv.wait(lock, [this]{ return !m_queue.empty(); });
        T value = std::move(m_queue.back());
        m_queue.pop_back();
        return value;
    }

private:
    std::deque<T> m_queue;
    std::mutex m_mutex;
    std::condition_variable m_cv;
};

mt_queue<std::string> a;

void t1()
{
    a.push("111");
    std::this_thread::sleep_for(1s);
    a.push("222");
    std::this_thread::sleep_for(1s);
    a.push("333");
    std::this_thread::sleep_for(1s);
    a.push("EXIT");
}

void t2()
{
    while (true) 
    {
        auto msg = a.pop();
        if (msg == "EXIT") break;
        std::osyncstream(std::cout) << "t2接收消息: " << msg << "\n";
    }
}

int main()
{
    std::vector<std::jthread> pool;

    pool.emplace_back(std::jthread(t1));
    pool.emplace_back(std::jthread(t2));

    return 0;
}












