//
//  main.cpp
//  uFeeble
//
//  Created by GUSTAVO CAMPOS on 08/05/2019.
//  Copyright © 2019 GUSTAVO CAMPOS. All rights reserved.
//

#include <iostream>
#include "uFeeble.hpp"

Observable observable;
Observable::Observer& observer = observable.getObserver ();


class loop1 : public uFeeble::Thread
{
private:
    size_t nValue=0;
    
    
protected:
    
    
   void Loop (int64_t nEventAt) override
    {
        nValue++;
        
        //std::cout << "Loop1: [" << ((size_t) getID()) << "] Value: [" << nValue << "] Slept for: [" << getTimeSleeping() << "] Scheduler@:[" << getSchedulerTime()  << "] << Last ExecTime:[" << getLastExecTime() << "]" << std::endl;
        
        {
            Observable::type tp = Observable::type::NOTIFY;
            observable.notify(tp, (size_t) this);
        }
    }
public:
    
    loop1()
    {
        nValue = 1;
    }
    
    uint32_t getThreadID ()
    {
        return getID();
    }
    
    size_t getValue ()
    {
        return nValue;
    }
};


class Show : public uFeeble::Thread
{
private:
    size_t nSensor[2] = { 0,0 };
    
    
    
protected:
    
    void onNotify (size_t nMessage) override
    {
        //std::cout << __PRETTY_FUNCTION__ << " * NOTIFICATION RECEIVED" << std::endl;
        
        loop1* pLoop = (loop1*) nMessage;
        
        //std::cout << "\t From: [" << pLoop->getID() << "], Value: [" << pLoop->getValue() << "]" << std::endl << std::endl;
        
        nSensor [pLoop->getThreadID () - 1] = pLoop->getValue();
    }
    
    void Loop (int64_t nEventAt) override
    {
        std::cout << ">>>> SHOW <<<<< : Sensors 1:[" << nSensor [0] << "], 2: [" << nSensor [1] <<  "] : Slept for: [" << (getSchedulerTime() + (nEventAt* (-1))) << "] Scheduler@:[" << getSchedulerTime()  << "] << Last ExecTime:[" << getLastExecTime() << "]" << std::endl;
    }
    
public:
    
    Show ()
    {
        
    }
};




int main(int argc, const char * argv[]) {
    // insert code here...
    
    loop1 objLoop1;
    loop1 objLoop2;
    Show  objShow;
    
    std::cout << "uFeeble Size: [" << sizeof (uFeeble) << "]" << std::endl;
    std::cout << "Thread Size: [" << sizeof (uFeeble::Thread) << "]" << std::endl;
    std::cout << "Observable Size: [" << sizeof (Observable) << "]" << std::endl;
    std::cout << "Observer Size: [" << sizeof (Observable::Observer) << "]" << std::endl;
    
    
    uFeeble uKernel;
    
    uKernel.Create (objLoop1, 500);
    uKernel.Create (objLoop2, 2800);
    
    uKernel.Create(objShow, 1000);
    
    observer.Attach(uKernel);
    
    uKernel.Join();
    
    return 0;
}


