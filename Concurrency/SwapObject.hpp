//
//  SwapObject.hpp
//  Concurrency
//
//  Created by 张成 on 2016/10/13.
//  Copyright © 2016年 张成. All rights reserved.
//

#ifndef SwapObject_hpp
#define SwapObject_hpp

#include <stdio.h>

#include "common.h"
#include <mutex>




class test_swap
{
public:
    test_swap(int v):x(v){}
    test_swap(test_swap const& b):x(b.x){}
    
    void set(int v)
    {
        x = v;
    }
    
    int get() const{
        return x;
    }
private:
    int x;
};


void swap2(test_swap& l,test_swap& r);



class X
{
private:
    test_swap sb;
    std::mutex m;
    

public:
    X(test_swap const& sd):sb(sd){}
    
    void dump()
    {
        print(sb.get());
    }
    friend void swap(X& lhs,X& rhs)
    {
        if(&lhs == &rhs)
            return;
        
        //此种锁 直接锁死2个mutex 直至交换完成 函数结束
        std::lock(lhs.m, rhs.m);
        std::lock_guard<std::mutex> lock_a(lhs.m, std::adopt_lock);//来自动释放锁
        std::lock_guard<std::mutex> lock_b(rhs.m, std::adopt_lock);//来自动释放锁
        
        swap2(lhs.sb,rhs.sb);
    }
    
    /*
     更灵活的锁
     */
    friend void swapOther(X& lhs,X& rhs)
    {
        if(&lhs == &rhs)
            return;
        
        //此种锁灵活 不直接锁死2个mutex 直至交换完成 可控制lock_a lock_b  的解锁与加锁
        
        std::unique_lock<std::mutex> lock_a(lhs.m, std::defer_lock);//来自动释放锁 不加锁
        std::unique_lock<std::mutex> lock_b(rhs.m, std::defer_lock);//来自动释放锁 不加锁
        
        //现在加锁
        std::lock(lock_a, lock_b);
        
        swap2(lhs.sb,rhs.sb);
    }
    
    
    //可转移的锁
    std::unique_lock<std::mutex> get_lock()
    {

        
        std::unique_lock<std::mutex> lk(m);
        //准备数据 或者一些前置操作
        SPRINTF("操作前数据",sb.get());
        return lk;
    }
    
    void process_data()
    {
        std::unique_lock<std::mutex> lk = get_lock();//随时可已 lock 或 unlock
        
        //改变值
        sb.set(sb.get()+1);
    }
};


class TestSwap{
public:
    TEST_CLASS(TestSwap);
    
    void run();
};


#endif /* SwapObject_hpp */
