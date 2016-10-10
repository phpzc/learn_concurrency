//
//  thread_guard.hpp
//  Concurrency
//
//  Created by 张成 on 2016/10/3.
//  Copyright © 2016年 张成. All rights reserved.
//

#ifndef thread_guard_hpp
#define thread_guard_hpp

#include <thread>
#include <iostream>
using namespace std;


/**
 
 管理线程 在对象销毁时 如果thread 可连接  等待线程结束   此对象 不独占thread
 */
class Thread_guard
{
private:
    std::thread& t;
    
public:
    Thread_guard(std::thread& _t):t(_t)
    {
        
    }
    
    
    ~Thread_guard()
    {
        if( t.joinable())
        {
            cout << "Thread_guard destruct" << endl;
            t.join();
        }
    }
    
    //删除多余的不可使用的函数
    
    Thread_guard& operator= (Thread_guard const&) =delete;
    
    Thread_guard(Thread_guard const&) =delete;
};

#endif /* thread_guard_hpp */
