#include <functional>
#include <thread>
#include <iostream>

// 1. 普通函数
void myfunc(double param1, double param2, double& result)
{
    result = param1 + param2;
}

// 2. 仿函数(函数对象)
struct myfunctor
{
    // 重载 () 符号
    void operator()(double param1, double param2)
    {
        result = param1 + param2;
    }

    double result = 0;
};

// 3. 类非静态成员函数
class MyClass
{
public:
    void sum(double param1, double param2)
    {
        result = param1 + param2;
    }
    
    double result = 0;
};

int main()
{
    double a = 1.1;
    double b = 3.2;
    double result = 0;

    std::thread thread1(myfunc, a, b, std::ref(result));    // result接收返回值

    myfunctor f;
    std::thread thread2(f, a, b);   // 接收不到返回值

    MyClass obj;
    // std::thread thread3(MyClass::sum, &obj, a, b);  // (成员函数, 类对象地址, 参数列表)，会报错，一般使用Lambda辅助
    std::thread thread3([&obj, a, b](){
        obj.sum(a, b);
    });

    double result2;
    std::thread thread4([&result2](double a, double b){ 
        result2 = a + b;
    }, a, b);                                       // lambda

    // 线程合并
    thread1.join();
    thread2.join();
    thread3.join();
    thread4.join();

    // 输出查看结果
    std::cout << "thread1 result: " << result << std::endl;
    std::cout << "thread2 result: " << f.result << std::endl;       // 线程会拷贝副本然后执行，所以拷贝过去的f不会对main中f有任何影响
    std::cout << "thread3 result: " << obj.result << std::endl;
    std::cout << "thread4 result: " << result2 << std::endl;


    return 0;
}