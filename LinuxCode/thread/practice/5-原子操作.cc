// 销售火车票
#include <unistd.h>
#include <iostream>
#include <thread>
#include <atomic>

std::atomic<int> ticket(10);

// 每个窗口都执行的售票操作，假设每个窗口每次只卖出 1 张
void SellTicket(const std::string& id);

int main()
{
    // 有 3 个售票窗口在售票
    std::thread t1(SellTicket, "窗口1");
    std::thread t2(SellTicket, "窗口1");
    std::thread t3(SellTicket, "窗口1");
    
    t1.join();
    t2.join();
    t3.join();
    return 0;
}

void SellTicket(const std::string& id)
{
    while (1)
    {
        if (ticket > 0) 
        {
            sleep(1);
            ticket--;
            std::cout << id << "售出 1 张, 剩余 " << ticket << " 张" << std::endl;
        }
        else
        {
            std::cout << "票售罄了" << std::endl;
            break;
        }
    }
}
