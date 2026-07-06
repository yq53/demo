#include <iostream>
#include <thread>
#include <array>
#include <mutex>

std::mutex mu1;     // 锁1
std::mutex mu2;     // 锁2

// 以下线程A、B并发

void threadA()
{
    mu1.lock();     // 1. A获取锁1
    mu2.lock();     // 2. A想获取锁2 --> 锁2被B占有 --> A线程阻塞，等待B解锁2

    mu1.unlock();   
    mu2.unlock();
}

void threadB()
{
    mu2.lock();     // 1. B获取锁2
    mu1.lock();     // 2. B想获取锁1 --> 锁1被A占有 --> B线程阻塞，等待A解锁1

    mu2.unlock();
    mu1.unlock();
}

/*
以上两个线程A和B同时被阻塞，导致死锁deadlock
*/

int main() 
{

    return 0;
}
