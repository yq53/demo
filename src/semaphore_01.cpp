#include <type_traits>
#include <vector>
#include <array>
#include <iostream>
#include <sstream>
#include <syncstream>
#include <mutex>
#include <semaphore>
#include <thread>
#include <chrono>
#include "random_range.h"


template <std::size_t N = 10>
class CarPark
{
public:
    CarPark() : sem(N) {};

    // 进入停车场
    int enter()
    {
        std::osyncstream(std::cout) << "[" << std::this_thread::get_id() << "]准备进入停车场\n";
        sem.acquire();  // 获取许可
        std::lock_guard<std::mutex> lock(mtx);  // 同步线程，处理多辆车进入情况
        std::osyncstream(std::cout) << "[" << std::this_thread::get_id() << "]\t进入停车场\n";
        
        // 寻找空车位
        for (int i = 0; i < N; i++) // 遍历N个车位
        {
            // 如果第i个车位为空
            if (spaces[i].empty())
            {
                // 停入该空车位
                std::stringstream ss;
                ss << std::this_thread::get_id();
                spaces[i] = ss.str();   // 填入车牌号(线程id)表示有车了
                std::osyncstream(std::cout) << "[" << std::this_thread::get_id() << "]\t\t找到#" << i << "号车位\n";
                return i;
            }
        }
        throw(std::runtime_error("程序异常到达此处！"));
    }

    // 随机等待一段时间
    void do_something()
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(rn.generate())); 
    }

    // 离开停车场
    void leave(int idx)
    {
        std::osyncstream(std::cout) << "[" << std::this_thread::get_id() << "]\t\t\t离开#" << idx << "号车位\n";
        {
            std::lock_guard<std::mutex> lock(mtx);
            spaces[idx].clear();    // 清空
        }
        sem.release();  // 更新信号量
    }

    // 合并上述三个函数(用作线程函数)
    void enter_and_leave()
    {
        int spaceId = enter();
        do_something();
        leave(spaceId);
    }

private:
    std::array<std::string, N> spaces;  // N个停车位(储存线程id)
    std::counting_semaphore<N> sem;     // 空车位信号量
    std::mutex mtx;                     // 互斥量
    Random rn{500, 5000};      // 产生 500 到 5000 之间随机数
};

int main(void)
{
    const int NSpaces = 10;     // 车位数
    const int NCars   = 100;    // 车数

    CarPark<NSpaces> park;      // 停车场对象

    std::vector<std::thread> cars;  
    for (int i = 0; i < NCars; i++){
        cars.emplace_back(
            [&park](){park.enter_and_leave();});
    }

    for (auto &t : cars)
        t.join();
}

/* 
1. template <std::size_t N = 10>

- N为**编译期**常量整数

2. std::size_t

- 无符号>=0，整数，用于数组大小、索引、sizeof返回值

3. std::counting_semaphore<N>

- N是计数器允许最大值(上限)

4. std::osyncstream(std::cout)

- c++ 20**同步输出流** / std::cout是指定输出到哪

5. std::lock_guard<std::mutex> lock(mtx);

- RAII自动锁，构造lock，析构unlock

6. emplace_back

- 原地构造，不经过临时对象
*/