//
//  threadsafe_stack.cpp
//  Concurrency
//
//  Created by 张成 on 2016/10/11.
//  Copyright © 2016年 张成. All rights reserved.
//

#include "threadsafe_stack.hpp"

#include "common.h"

#include <unistd.h>

void ThreadSafeSimple::run()
{
    
    threadsafe_stack<int> stack;
    for(int i=0;i<100;++i)
    {
        stack.push(i);
    }
    
    //2个线程pop数据
    
    std::thread t1([&stack](){
        while(!stack.empty())
        {
            std::shared_ptr<int> p = stack.pop();
            SPRINTF("T1 - ", *p);
            usleep(1);
        }
    });
    
    std::thread t2([&stack](){
        while(!stack.empty())
        {
            std::shared_ptr<int> p = stack.pop();
            SPRINTF("T2 - ", *p);
            usleep(1);
        }
    });
    
    t1.join();
    t2.join();
    
}
