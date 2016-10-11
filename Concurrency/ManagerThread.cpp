//
//  ManagerThread.cpp
//  Concurrency
//
//  Created by 张成 on 2016/10/2.
//  Copyright © 2016年 张成. All rights reserved.
//

#include "ManagerThread.hpp"
#include "common.h"

#include <iostream>
#include <sstream>
#include <functional>
#include <vector>
#include <string>
#include <numeric>
#include <algorithm>
using namespace std;

#include "thread_guard.hpp"

#include "scope_thread.hpp"



template <typename Iterator, typename T>
struct accumulate_block{
    
    void operator() (Iterator first, Iterator last, T& result)
    {
        //求和
        result = std::accumulate(first, last, result);
    }
};

template <typename Iterator, typename T >
T parallel_accumlate(Iterator first,Iterator last, T& init)
{
    //距离不能少于1个 0个则退出
    unsigned long const length = std::distance(first,last);
    
    if(!length)
        return init;
    
    //假定 每个线程计算数量为25
    unsigned long const min_per_thread = 25;
    //获取线程最大数量
    unsigned long const max_threads = (length + min_per_thread) / min_per_thread;
    
    //cpu核心数量
    unsigned long const hardward_threads = std::thread::hardware_concurrency();
    
    //硬件核心数 返回失败的话  这里设置最小2个线程
    unsigned long const num_theads =  std::min(hardward_threads !=0 ? hardward_threads : 2, max_threads);
    
    LOG("num_theads");
    LOG(num_theads);
    
    //每个线程处理的数量
    unsigned long const block_size = length / num_theads;
    
    std::vector<T> results(num_theads);
    std::vector<std::thread> threads(num_theads - 1);// 主线程 也算一个
    
    Iterator block_start = first;
    
    for(unsigned long i = 0; i<(num_theads -1) ; ++i)
    {
        Iterator block_end = block_start;
        //使得迭代器增加指定偏移量
        std::advance(block_end, block_size);
        
        //创建线程 加入到vector  函数计算返回值 是results种的引用
        threads[i] = std::thread( accumulate_block<Iterator,T>(), block_start, block_end, std::ref(results[i]) );
        
        
        block_start = block_end;
    }
    
    
    //主线程 最后一个线程 也计算一下自己的任务
    accumulate_block<Iterator, T>()( block_start, last, results[num_theads-1]);
    
    //等待所有线程工作完毕 依次调用每个线程的join函数
    std::for_each( threads.begin(), threads.end(), std::mem_fn(&std::thread::join));
    
    return std::accumulate(results.begin(), results.end(), init);
}

void do_work(unsigned int i)
{

    cout << i << endl;
}

void ManagerThread::run()
{
    std::thread t([](){
        for(int i=0;i<5;i++)
        {
            cout << i << endl;
        }
    });
    
    
    Thread_guard tt(t);
    
    cout << "ManagerThread::run run end" << endl;


    std::thread t1 = std::thread([](){
        cout << "tmp thread" << endl;
    });

    std::thread t2 = std::move(t1);// 临时对象 默认自动移动语义复制
    
    if(t2.joinable())
    {
        t2.join();
    }
    
    
    
    //独占线程
    std::thread t3 = std::thread([](){
        cout << "t3 " << endl;
    });
    
    Scope_thread st(std::move(t3));
    
    
    
    std::vector<std::thread> threads;
    
    for(unsigned int i = 0; i<20;i++)
    {
        threads.push_back(std::thread(do_work,i));
    }
    
    
    // std::mem_fn
    std::for_each(threads.begin(),threads.end(), std::mem_fn(&std::thread::join));
    
    
    /*
    //cpu数量
     
    int cpu = std::thread::hardware_concurrency();
    
    
    ostringstream cpustr;
    
    cpustr << "cpu 核心数量  " << cpu << endl;
    
    cout << cpustr.str();
    */
    
    std::vector<int> numbersVector;
    
    for(int i=0;i<45;++i)
    {
        numbersVector.push_back(i);
    }
    
    
    int result = 0;
    
    //求和计算返回 开多个线程
    result  = parallel_accumlate<std::vector<int>::iterator,int>(numbersVector.begin(),numbersVector.end(), result) ;
    cout << "parallel_accumlate " << result << endl;

    
    std::thread::id thisId = std::this_thread::get_id();
    
    print("this thread id", thisId);

}
