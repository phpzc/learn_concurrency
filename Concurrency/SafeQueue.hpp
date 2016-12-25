//
//  SafeQueue.hpp
//  Concurrency
//
//  Created by 张成 on 2016/11/18.
//  Copyright © 2016年 张成. All rights reserved.
//

#ifndef SafeQueue_hpp
#define SafeQueue_hpp

#include <stdio.h>

#include "common.h"
#include <queue>

class SafeQueue
{
public:
    TEST_CLASS(SafeQueue);
    void run();
};

template<class T>
class SafeQueueData
{
public:
    SafeQueueData(){}
    SafeQueueData(const SafeQueueData& sqd)
    {
        std::lock_guard<std::mutex> lock(mutex);
        queue = sqd.queue;
        
    }

    
protected:
    mutable std::mutex mutex;
    std::queue<T> queue;
    std::condition_variable condtion;
};

#endif /* SafeQueue_hpp */
