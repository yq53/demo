/*
*  循环缓冲区
* 实现读写线程示例，其中一个线程写入共享缓冲区，另一个线程从中读取
* 该版本使用信号量来确保读取器只读取有效数据，而写入器不会覆盖未读取的数据。
*
*/
#include <mutex>
#include <thread>
#include <iostream>
#include <string.h>
#include <syncstream>
#include <array>
#include "random_range.h"
#include <semaphore>

template <size_t BuffSize = 6>
class RWBuffer
{
public:
    RWBuffer() { buff.fill(' '); };

    // 准备数据
    char prepareData()
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(rgen()));     //模拟生成数据消耗的时间
        return chargen();
    }

    // 处理数据
    void processData(char ch)   
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(rgen2()));    //模拟处理数据消耗的时间
    }

    // 打印当前buffer中的内容
    void printBuffer(std::osyncstream& out)
    {
        //std::osyncstream out(std::cout);
        out << "BUFFER: |";
        // 遍历打印每个字符
        for (auto c : buff)
        {
            out << c << "|";
        }
        out << std::endl;
    }

    // 写入buffer
    void writeToBuffer(size_t iterations)
    {
        std::osyncstream(std::cout) << "写线程就绪.\n";

        // 遍历iterations次buffer
        for (size_t i = 0; i < iterations * buff.size(); i++)
        {
            char ch = prepareData();    // 获取随机字符
            sem_writer.acquire();       // 可写空位-1
    
            buff[i % buff.size()] = ch; // 保证不越界

            std::osyncstream out(std::cout) ;
            out << "写线程: 写入字符 '"<< ch << "'.\t\t";
            printBuffer(out);   // 打印写入后结果

            // Signal that there is content to read
            sem_reader.release();   // 可读数+1
        }
    }

    // 读buffer
    void readFromBuffer(size_t iterations)
    {
        std::osyncstream(std::cout) << "读线程就绪.\n";
        // 遍历iterations次
        for (size_t i = 0; i < iterations * buff.size(); i++)
        {
            sem_reader.acquire();       // 可读数-1
            auto idx = i % buff.size(); 

            char ch = buff[idx];    // 获取字符
            processData(ch);         
            buff[idx] = ' ';        // 填充空位

            std::osyncstream out(std::cout);
            out << "读线程: 读取数据 '" << ch << "'.\t\t";
            printBuffer(out);
            sem_writer.release();   // 可写空位+1
        }
    }

private:
    // 随机数模拟读写操作消耗的时间
    Random rgen{500, 1000};
    Random rgen2{800, 1500};
    // 生成随机字符
    Random chargen{'A', 'Z'};
    std::array<char, BuffSize> buff;    // 用于存储字符
    std::counting_semaphore<BuffSize> sem_reader{0};        // 可读取字符数
    std::counting_semaphore<BuffSize> sem_writer{BuffSize}; // 空位信号量
};

// 读/写循环次数.
static const unsigned int kNumIterations = 3;
int main(int argc, const char *argv[])
{
    // Create an empty buffer
    RWBuffer rwbuffer;
    std::thread writer([&rwbuffer](){
        rwbuffer.writeToBuffer(kNumIterations);
    });

    std::thread reader([&rwbuffer](){
        rwbuffer.readFromBuffer(kNumIterations);
    });

    writer.join();
    reader.join();
    return 0;
}