//
//  CallOnce.cpp
//  Concurrency
//
//  Created by 张成 on 2016/10/13.
//  Copyright © 2016年 张成. All rights reserved.
//

#include "CallOnce.hpp"

#include "common.h"
#include <vector>

void CallOnce::run()
{
    
    //这样的看出不可控的结果
    /*
    std::vector<std::thread> threadVec;
    
    for(int i = 0;i<20;++i)
    {
        if(i < 10){
            threadVec.push_back(std::thread(&CallOnce::newFuncCompare,this));
        }else
            threadVec.push_back(std::thread(&CallOnce::newFunc,this));
    }
    
    std::for_each(threadVec.begin(), threadVec.end(), std::mem_fn(&std::thread::join));
    */
    
    
    //
    std::vector<std::thread> threadVec;
    
    for(int i = 0;i<20;++i)
    {
        if(i > 10){
            threadVec.push_back(std::thread(&CallOnce::newFunc2Compare,this));
        }else
            threadVec.push_back(std::thread(&CallOnce::newFunc2,this));
    }
    
    std::for_each(threadVec.begin(), threadVec.end(), std::mem_fn(&std::thread::join));
    
}

void CallOnce::connect()
{
    i = 100;
    print("connect");
}
void CallOnce::connect2()
{
    i = 200;
    print("connect2");
}

void CallOnce::oldFunc()
{
    //如果i=0 我们就要设置他+100
    //目的 要只能设置1次
    //方式一  加锁
    std::unique_lock<std::mutex> lock(m);
    if( i == 0)
    {
    
        connect();
    }
    lock.unlock();
    
    //在操作i前需要 connect 初始化下 假定有这个需求   在用锁的这种情况下  每次都需要加锁 较
    print(i);
}

void CallOnce::newFunc()
{
    //二次检查
    if(i == 0)
    {
        //这一步之间 可能已经发生了 初始化调用 connect
        std::lock_guard<std::mutex> lk(m);
        if( i == 0)
        {
            
            connect();
        }
    }
    //这里往下的代码是不确定结果的 多线程情况下
    
    SPRINTF("new func ",i);
}

void CallOnce::newFuncCompare()
{
    //二次检查
    if(i == 0)
    {
        //这一步之间 可能已经发生了 初始化调用 connect
        std::lock_guard<std::mutex> lk(m);
        if( i == 0)
        {
            
            connect();
            
            i += 100;
        }
    }
    //这里往下的代码是不确定结果的 多线程情况下
    
    SPRINTF("newFuncCompare ",i);
}



void CallOnce::newFunc2()
{
    std::call_once(flag, &CallOnce::connect, this);
    
    SPRINTF("newFunc2 ",i);
}

void CallOnce::newFunc2Compare()
{
    std::call_once(flag, &CallOnce::connect2, this);
    
    SPRINTF("newFunc2Compare ",i);
}
