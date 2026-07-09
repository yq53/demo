#include <iostream>
#include <sstream>
#include <string>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>

using namespace std::chrono_literals;

class Channel
{
public:
    // 接收线程调用，读取数据
    void getData()
    {
        auto tid = std::this_thread::get_id();
        std::unique_lock<std::mutex> lck(mutex_);   // 获取mutex
        std::cout << "接收方[" << tid << "]等待数据;" << std::endl;
        condVar.wait(lck);  // 等待通知
        std::cout << "接收方[" << tid << "]得到数据;" << sharedData << std::endl;
        sharedData.clear();     // 清除sharedData
    }

    // 发送线程调用，写入数据
    void setData()
    {
        // 准备共享数据
        static int id = 1;
        std::stringstream ss;
        ss << "Hello #" << id;
        {
            std::unique_lock<std::mutex> lck(mutex_);   // 获取mutex
            sharedData = ss.str();  // 将准备好的共享数据，赋值给sharedData
            std::cout << "\n发送方: 第" << id << "条数据写入完毕;" << std::endl;
            id++;
        }
        // 通知接收线程
        condVar.notify_one();
        std::this_thread::sleep_for(1ms);
    }

private:
    std::mutex mutex_;                  // 互斥量
    std::condition_variable condVar;    // 条件变量
    std::string sharedData;             // 共享数据
};

int main()
{
    Channel channel;

    // 发送线程
    std::thread write_th([&channel](){
        channel.setData();
    });

    // 接收线程
    std::thread read_th([&channel](){
        channel.getData();
    });

    read_th.join();
    write_th.join();

    return 0;
}