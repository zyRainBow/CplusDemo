#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>

using namespace std;

// 全局变量，表示是否有数据需要处理
bool data_ready = false;

// 互斥锁和条件变量，用于线程间同步
mutex mtx;
condition_variable cv;

// 生产者线程函数，模拟产生数据
void producer() {
    // 模拟产生数据
    this_thread::sleep_for(chrono::seconds(1));
    cout << "Producer: data is ready" << endl;

    // 加锁，修改全局变量
    unique_lock<mutex> lock(mtx);
    data_ready = true;

    // 通知消费者线程
    cv.notify_one();
}

// 消费者线程函数，等待数据并处理
void consumer() {
    // 等待数据
    unique_lock<mutex> lock(mtx);
    cv.wait(lock, []{ return data_ready; });

    // 处理数据
    cout << "Consumer: processing data" << endl;
}

int main() {
    // 创建生产者和消费者线程
    thread t1(producer);
    thread t2(consumer);

    // 等待线程结束
    t1.join();
    t2.join();

    return 0;
}