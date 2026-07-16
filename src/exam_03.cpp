
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
class mt_queue  // multi-threaded多线程
{
public:
    // 推入数据，并通知
    void push(T value)
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_queue.push_front(std::move(value));   // 移动赋值
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

    // 尝试读取数据，不阻塞，如果没有，返回nullopt
    std::optional<T> try_pop()
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        if (m_queue.empty()) 
        {
            return std::nullopt;
        }
        T value = std::move(m_queue.back());
        m_queue.pop_back();
        return value;
    }

    // 尝试取数据，如果没有，等待一段时间，超时返回nullopt
    std::optional<T> try_pop_for(std::chrono::steady_clock::duration timeout)
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        if (!m_cv.wait_for(lock, timeout, [this]{ return !m_queue.empty(); }))
        {
            return std::nullopt;
        }
        T value = std::move(m_queue.back());
        m_queue.pop_back();
        return value;
    }

    // 尝试取数据，如果没有，等待直到时间点，超时返回nullopt
    std::optional<T> try_pop_until(std::chrono::steady_clock::time_point timeout)
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        if (!m_cv.wait_until(lock, timeout, [this]{ return !m_queue.empty(); }))
        {
            return std::nullopt;
        }
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
        if (auto msg = a.try_pop_for(100ms))
        {
            if (msg == "EXIT") break;
            std::osyncstream(std::cout) << "t2接收消息: " << *msg << "\n";
        }
        else
        {
            std::osyncstream(std::cout) << "t2未收到消息" << "\n";
        }
    }
}

int main()
{
    std::vector<std::jthread> pool;

    pool.emplace_back(std::jthread(t1));
    pool.emplace_back(std::jthread(t2));

    return 0;
}


