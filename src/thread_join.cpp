#include <iostream>
#include <string>
#include <thread>

void hello(const std::string& name)
{
    std::cout << "Hello, " << name << ".[Thread ID = " << std::this_thread::get_id() << "]\n" << std::endl;
}

int main()
{
    std::thread mythread(hello, std::string("World"));

    // 该程序会最终出现错误，因为main线程和mythread线程异步进行，main线程会直接执行return而不等mythread执行完hello
    // 所以我们需要利用mythread.join()函数强行阻塞main线程等待mythread线程执行完成后再继续执行

    // 注释掉，看报错
    if (mythread.joinable())    // 保证mythread是“joinable”的(持有handle)
    {
        mythread.join();    
    }
    
    return 0;
}