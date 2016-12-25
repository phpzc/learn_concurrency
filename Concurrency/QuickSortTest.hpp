//
//  QuickSortTest.hpp
//  Concurrency
//
//  Created by 张成 on 2016/12/25.
//  Copyright © 2016年 张成. All rights reserved.
//

#ifndef QuickSortTest_hpp
#define QuickSortTest_hpp

#include "common.h"
#include <future>
#include <thread>
#include <list>
#include <utility>

//函数式编程的快速的排序实现
template<typename T>
std::list<T> sequential_quick_sort(std::list<T> input)
{
    if(input.empty()){
        return input;
    }
    
    std::list<T> result;
    result.splice(result.begin(),input,input.begin());//列表的首个元素
    
    T const& pivot = *result.begin();
    
    auto divide_point = std::partition(input.begin(), input.end(), [&](T const& t){ return t< pivot;} ); // 排序 比pivot的 移位到前面去   divide_point 第一个不小于pivot的值得 迭代器
    
    std::list<T> lower_part;
    lower_part.splice(lower_part.end(), input, input.begin(), divide_point);// 这里是 从input里面拆出 小于中间值的部分的列表
    
    auto new_lower( sequential_quick_sort( std::move(lower_part)) );//排序前端
    
    auto new_higher( sequential_quick_sort( std::move(input)) );//排序后端
    
    result.splice( result.end(), new_higher);//插入到result的结束位置 并从new_higher删除
    result.splice( result.begin(), new_lower);//插入到result的开始位置 并从new_lower删除
    
    return result;
    
    
}


//FP风格并行快速排序 利用
template<typename T>
std::list<T> parallel_quick_sort(std::list<T> input)
{
    if(input.empty()){
        return input;
    }
    
    std::list<T> result;
    
    result.splice(result.begin(), input, input.begin());
    
    T const& pivot = *result.begin();
    
    //排序
    auto divide_point = std::partition(input.begin(),input.end(), [&](const T& t){ return t < pivot;});
    
    std::list<T> lower_part;
    lower_part.splice(lower_part.end(), input, input.begin(), divide_point); //分离比中间值小的
    
    
    std::future<std::list<T> > new_lower( std::async( &parallel_quick_sort<T>, std::move(lower_part)) );
    
    auto new_higher( parallel_quick_sort(std::move(input)) );
    
    result.splice( result.end(), new_higher);
    result.splice( result.begin(), new_lower.get() );
    
    return result;
}


//自己封装future任务到packaged_task
template<typename F, typename A>
std::future<typename std::result_of< F(A&&)>::type >
    spawnn_task(F&& f, A&& a)
{
    typedef typename std::result_of<F(A&&)>::type result_type;
    
    std::packaged_task< result_type(A&&) > task(std::move(f));
    
    std::future<result_type> res(task.get_future());
    
    std::thread t(std::move(task), std::move(a));
    t.detach();
    return res;
}

#endif /* QuickSortTest_hpp */
