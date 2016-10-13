//
//  SwapObject.cpp
//  Concurrency
//
//  Created by 张成 on 2016/10/13.
//  Copyright © 2016年 张成. All rights reserved.
//

#include "SwapObject.hpp"

void swap2(test_swap& l,test_swap& r)
{
    int a = l.get();
    l.set(r.get());
    r.set(a);
}

void TestSwap::run()
{
    test_swap test_a(1);
    test_swap test_b(2);
    
    X test_x_a(test_a);
    X test_x_b(test_b);
    
    swap(test_x_a,test_x_b);
    
    test_x_a.dump();
    test_x_b.dump();
    
    test_x_a.process_data();
    test_x_a.dump();
    
    
}
