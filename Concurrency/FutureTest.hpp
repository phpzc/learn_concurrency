//
//  FutureTest.hpp
//  Concurrency
//
//  Created by 张成 on 2016/11/22.
//  Copyright © 2016年 张成. All rights reserved.
//

#ifndef FutureTest_hpp
#define FutureTest_hpp

#include <stdio.h>
#include "common.h"
#include <unistd.h>
#include <future>
#include <thread>

#include "QuickSortTest.hpp"


class FutureTest{
public:
    static void callfunc()
    {
        print(12345);
    }
public:
    TEST_CLASS(FutureTest);
    
    
    std::mutex mutex1;
    std::deque< std::packaged_task<void()> > tasks;
    
    int count(int a,int b) {
    
        print("count------run");
        return a+b;
    
    };
    
    void run()
    {
        //future
        auto  future = std::async(std::launch::async ,&FutureTest::count, this, 1,2);
        
        print( future.get());
        
        
        //packaged_task
        /*
        auto run2 = std::async(std::launch::async ,&FutureTest::producter, this);
        
        while(true){
            
            
            std::packaged_task< void() > task;//不可复制的 只可移动
            {
                print("waiting for task");
                
                //取锁这步 可能需要等待
                
                std::lock_guard<std::mutex> lock(mutex1); //锁自动释放
                if(tasks.empty())
                    continue;
                
                task = std::move(tasks.front());
                tasks.pop_front();
            }
            task();
        }
        */
        
        //test exception
        //auto  future2 = std::async(std::launch::deferred ,&FutureTest::callfunc_with_exception, this, -2);
        //print( future2.get());//触发异常
        
        
        
        //promise
        /*
        std::promise<int> prom;
        std::future<int> prom_future = prom.get_future();
        thread t(&FutureTest::callfunc_promise,this,std::ref(prom_future) );
        
        //prom.set_value(100);
        prom.set_exception( std::make_exception_ptr(std::logic_error("logic_error")));
        
        t.join();
        */
        
        
        
        
        //shared_future
        
        
        try {
            auto result = std::async(std::launch::async, &FutureTest::future_shared_num,this).share();
        
            auto result1 = std::async(std::launch::async, &FutureTest::use_share_future, this,'.', result);
            
            auto result2 = std::async(std::launch::async, &FutureTest::use_share_future,this, '+', result);
            auto result3 = std::async(std::launch::async, &FutureTest::use_share_future, this,'*', result);
            
            print("begin get");
            result1.get();
            result2.get();
            result3.get();
            
            
        }catch (const std::exception& e) {
            std::cout << "EXCEPTION: " << e.what() << std::endl;
        }
        
         
        
        
        //时钟
        auto tt = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        
        struct tm* ptm = localtime(&tt);
        char date[60] = {0};
        sprintf(date, "现在时间 %d-%02d-%02d %02d:%02d:%02d", (int)ptm->tm_year + 1900,(int)ptm->tm_mon + 1,(int)ptm->tm_mday, (int)ptm->tm_hour,(int)ptm->tm_min,(int)ptm->tm_sec);
        
        print( date );
        
        std::this_thread::sleep_for(std::chrono::seconds(1));
        print("std::chrono::seconds sleep end");
        
        auto f_wait = std::async(std::launch::async, &FutureTest::count,this, 1,9);
        
        if( f_wait.wait_for(std::chrono::seconds(2))  == std::future_status::ready){
            auto val = f_wait.get();
            print(val);
        }
        
        //std::this_thread::sleep_for(std::chrono::minutes(1));
        //print("sleep end");
        
        
        //测试时间
        auto time_var_1 = std::chrono::system_clock::now();
        
        //匀速状态判断
        auto is_steady = std::chrono::system_clock::is_steady;
        print("是否是匀速的std::chrono::system_clock",is_steady);
        //匀速时钟
        //std::chrono::steady_clock;
        
        //时间段
        auto time_var_2 = std::chrono::duration<int , std::ratio<60,1> >(1);
        /*
         
         typedef duration<long long,         nano> nanoseconds;纳秒
         typedef duration<long long,        micro> microseconds;微秒
         typedef duration<long long,        milli> milliseconds;毫秒
         typedef duration<long long              > seconds;秒
         typedef duration<     long, ratio<  60> > minutes;分钟
         typedef duration<     long, ratio<3600> > hours;小时
         
         时间段可以 四则运算  时间段内部使用匀速时钟
         */
        
        //时间点   时间纪元与时间段 的结合   典型的时间纪元未19700101 00:00:00
        
        
        //接受超时的函数
        
        //线程的睡眠
        //std::this_thread::sleep_for();
        //std::this_thread::sleep_util();
    
        //互斥元的超时
        //std::timed_mutex m;
        //m.try_lock_for();
        //m.try_lock_until();
        
        //std::recursive_timed_mutex
        
        //std::timed_mutex m;
        //std::recursive_timed_mutex m2;
        
        
        //函数式的编程 快速排序  不使用future 递归的排序
        
        //数据源
        std::list<int> list_int_source;
        for(int i = 10;i>0;i--){
            list_int_source.push_back(i);
        }
        
        auto result_list = sequential_quick_sort(list_int_source);
    
        for(auto val : result_list)
        {
            print("val---",val);
        }
    
    }
    
    
    
    void callfunc_promise(std::future<int>& fu)
    {
        try{
        
            int param = fu.get();//阻塞 直到 prom 设置了值 或者 异常
        
            print("get value from promise",param);
            
        }catch(const std::exception& e){
            
            print("get exception");
        }
    }
    
    void producter()
    {
        for(int i=0;i<5;i++){
        
        std::packaged_task< void() > task(&FutureTest::callfunc); //可放入不同的执行函数
        std::future<void> fu = task.get_future();
        std::lock_guard<std::mutex> lock(mutex1);
        tasks.push_back( std::move(task));
        
        sleep(5);
            
        }
    }
    
    
    //异常存储在future中
    int callfunc_with_exception(int a)
    {
        print("callfunc_with_exception");
        if(a<0)
        {
            throw std::out_of_range("a<0");
        }
       
        return 1;
    }
    
    
    //----for shared_future
    int future_shared_num()
    {
        int f;
        print("input num");
        std::cin >> f;
        if(!std::cin)
        {
            throw std::runtime_error("runtime error");
        }
        
        return f;
    }
    
    //--use shared_future
    void use_share_future(char c,const std::shared_future<int> &f)
    {
        try {
            int number = f.get();
            for (int i = 0; i < number; ++i) {
                std::this_thread::sleep_for(std::chrono::duration<int>(1));
                std::cout.put(c).flush();
            }
        }catch (const std::exception& e) {
            //processCodeException<std::exception>(e);
            std::cerr << "exception in thread" << std::this_thread::get_id() << ": " << e.what() << std::endl;
        }
    }
};

#endif /* FutureTest_hpp */
