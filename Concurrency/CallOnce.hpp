//
//  CallOnce.hpp
//  Concurrency
//
//  Created by 张成 on 2016/10/13.
//  Copyright © 2016年 张成. All rights reserved.
//

#ifndef CallOnce_hpp
#define CallOnce_hpp

#include <stdio.h>
#include "common.h"

class CallOnce{
public:
    TEST_CLASS(CallOnce);
    void run();
    
    void oldFunc();
    void newFunc();
    void newFuncCompare();
    
    void newFunc2();
    void newFunc2Compare();
    
    void connect();
    void connect2();
    
private:
    std::mutex m;
    
    int i = 0;
    
    std::once_flag flag;

};
#endif /* CallOnce_hpp */
