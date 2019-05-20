//
//  main.cpp
//  uFeeble
//
//  Created by GUSTAVO CAMPOS on 08/05/2019.
//  Copyright © 2019 GUSTAVO CAMPOS. All rights reserved.
//

#include <iostream>
#include "uFeeble.hpp"


class loop1 : public uFeeble::Thread
{
private:
    size_t nValue=0;
    
protected:
    
   void Loop ()
    {
        nValue++;
        
        std::cout << "Loop1: [" << ((size_t) getID()) << "] Value: [" << nValue << "] Slept for: [" << getTimeSleeping() << "] Scheduler@:[" << getSchedulerTime()  << "] << Last ExecTime:[" << getLastExecTime() << "]" << std::endl;
    }
public:
    
    loop1()
    {
        nValue = 1;
    }
};


int main(int argc, const char * argv[]) {
    // insert code here...
    
    loop1 objLoop1;
    loop1 objLoop2;
    
    uFeeble uKernel;
    
    uKernel.Create (objLoop1, 500);
    uKernel.Create (objLoop2, 2800);
    
    uKernel.Join();
    
    std::cout << "Hello, World!\n";
    return 0;
}


