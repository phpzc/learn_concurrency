//
//  main.cpp
//  Concurrency
//
//  Created by 张成 on 2016/10/1.
//  Copyright © 2016年 张成. All rights reserved.
//

#include <iostream>

#include "ManagerThread.hpp"

int main(int argc, const char * argv[]) {
    // insert code here...
    
    ManagerThread mt;
    mt.run();

    return 0;
}
