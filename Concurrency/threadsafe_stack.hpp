//
//  threadsafe_stack.hpp
//  Concurrency
//
//  Created by 张成 on 2016/10/11.
//  Copyright © 2016年 张成. All rights reserved.
//

#ifndef threadsafe_stack_hpp
#define threadsafe_stack_hpp

#include "common.h"

#include <stdio.h>

#include <mutex>
#include <stack>
#include <memory>
#include <exception>

struct empty_stack:public std::exception
{
  
    const char* what() const throw()
    {
        return "empty_stack exception";
    }
};

/**
 * 线程安全栈
 */
template <typename T>
class threadsafe_stack{
private:
    std::stack<T> data;
    mutable std::mutex m;
    
public:
    threadsafe_stack(){}
    
    threadsafe_stack(const threadsafe_stack& other)
    {
        std::lock_guard<std::mutex> lock(other.m);
        data = other.data;
    }
    //删除赋值函数
    threadsafe_stack& operator= (const threadsafe_stack& ) = delete;
    
    void push(T new_value)
    {
        std::lock_guard<std::mutex> lock(m);
        data.push(new_value);
    }
    
    std::shared_ptr<T> pop()
    {
        std::lock_guard<std::mutex> lock(m);
        
        if( data.empty())
            throw empty_stack();
        
        std::shared_ptr<T> const res(std::make_shared<T>(data.top()));
        
        data.pop();
        return res;
    }
    
    
    void pop(T& value)
    {
        std::lock_guard<std::mutex> lock(m);
        if( data.empty())
            throw empty_stack();
        
        value = data.top();
        data.pop();
    
    }
    
    
    bool empty() const
    {
        std::lock_guard<std::mutex> lock(m);
        
        return data.empty();
    }
};



class ThreadSafeSimple{
    
public:
    TEST_CLASS(ThreadSafeSimple);
    void run();
};
#endif /* threadsafe_stack_hpp */
