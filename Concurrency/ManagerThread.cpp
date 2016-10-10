//
//  ManagerThread.cpp
//  Concurrency
//
//  Created by 张成 on 2016/10/2.
//  Copyright © 2016年 张成. All rights reserved.
//

#include "ManagerThread.hpp"

#include <iostream>
#include <functional>
#include <vector>
#include <string>
using namespace std;

#include "thread_guard.hpp"

#include "scope_thread.hpp"


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
    
}

