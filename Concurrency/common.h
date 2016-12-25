//
//  common.h
//  Concurrency
//
//  Created by 张成 on 2016/10/11.
//  Copyright © 2016年 张成. All rights reserved.
//

#ifndef common_h
#define common_h

#include <iostream>
#include <initializer_list>
#include <utility> //std::move
#include <string>
#include <map>

//异常类头文件  为了能够处理所有异常 加载他们
#include <exception> //for class exception and bad_exception
#include <stdexcept> //for most logic and runtime error class
#include <system_error> // for system errors from c++11
#include <new> // for out-of memory exceptions
#include <ios> // for I/O exceptions
#include <future> // for errors with async() and futures from c++11
#include <typeinfo> // for bad_cast and bad_typeid

// for tuple
#include <tuple>
#include <complex> // 复数
#include <sstream>


//
#include <mutex>
#include <stack>
#include <memory>


using namespace std;


template <typename T>
void debug_info(const T& t)
{
    cout << t << endl;
}

#define _DEBUG 1
#ifdef _DEBUG
#define LOG(msg) debug_info(msg)
#else
#define LOG(msg)
#endif


#ifndef PRINT_FUNC
#define PRINT_FUNC
template <typename T>
void print(const T& arg)
{
    cout << arg << endl;
}

template <typename T, typename ... Types>
void print(const T& firstArg, const Types& ... args)
{
    cout << firstArg << endl;
    print(args...);//递归调用 自动生成函数
}

#endif



//自动格式化成一行 打印 辅助输出类
class Sprintf{
    public:
    stringstream stream;
    
    Sprintf()
    {
        
    }
    
    template <typename T>
    void P(const T& arg)
    {
        stream << arg << endl;
        
        cout << stream.str();
        
        stream.clear();
    }
    
    
    template <typename T, typename ... Types>
    void P(const T& firstArg, const Types& ... args)
    {
        stream << firstArg;
        this->P(args ...);
    }
};


//打印多类型值到一行 快捷方法
#define SPRINTF(first, args...) \
{ \
    Sprintf sp;\
    sp.P(first, ##args);\
}


//泛型异常 打印函数
template <typename T>
void processCodeException(const T& e)
{
    using namespace std;
    auto c = e.code();
    cerr << "- category:" << c.category().name() << endl;
    cerr << "- value:" << c.value() << endl;
    cerr << "- msg:" << c.message() << endl;
    cerr << "- def category:" << c.default_error_condition().category().name() << endl;
    cerr << "- def value:" << c.default_error_condition().value() << endl;
    cerr << "- def msg:" << c.default_error_condition().message() << endl;
}

#ifdef MAKE_STR
#error "error for can not define MAKE_STR"
#else
#define MAKE_STR(name)  #name
#endif

//打印类test信息界限
#define CLASS_TRACE_BEGIN(class_name) \
print(MAKE_STR(class test #class_name begin :));

#define CLASS_TRACE_END(class_name) \
print(MAKE_STR(class test #class_name end!), "========================");


//测试类打印快速宏
#define TEST_CLASS(name) \
name(){\
    CLASS_TRACE_BEGIN(name);  \
}\
~name(){\
    CLASS_TRACE_END(name);\
}

//运行类 进行test 宏
#define RUN_CLASS(name) \
{ std::shared_ptr<name> run_##name(std::make_shared<name>()); \
run_##name->run(); }

#endif /* common_h */
