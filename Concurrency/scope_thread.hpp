//
//  scope_thread.hpp
//  Concurrency
//
//  Created by 张成 on 2016/10/3.
//  Copyright © 2016年 张成. All rights reserved.
//

#ifndef scope_thread_hpp
#define scope_thread_hpp

#include <stdio.h>

#include <thread>
#include <exception>


/**
 独占thread管理 初始化时 使用移动语义构造的
 */
class Scope_thread{
    
public:
    void run();
    
    explicit Scope_thread(std::thread th): t(std::move(th))
    {
        if(!t.joinable())
        {
            throw new std::logic_error("no thread");
        }
    }
    
    
    ~Scope_thread()
    {
        t.join();
    }
    
    Scope_thread(Scope_thread const &) = delete;
    
    Scope_thread& operator= (Scope_thread const &) = delete;
    
private:
    std::thread t;
};


#endif /* scope_thread_hpp */
