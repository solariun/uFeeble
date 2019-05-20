//
//  uFeeble.hpp
//  uFeeble
//
//  Created by GUSTAVO CAMPOS on 08/05/2019.
//  Copyright © 2019 GUSTAVO CAMPOS. All rights reserved.
//

#ifndef uFeeble_hpp
#define uFeeble_hpp

#include <iostream>
#include <stdio.h>
#include <cstdint>


class Observable
{
private:
    uint32_t nLockCount;
    
    friend class Observer;
protected:
public:

    class Observer
    {
    private:
        
        Observable& refObservable;
        Observer (Observable& refObservable);
        
        friend class Observable;
        
    protected:
        
    public:
        Observer () = delete;
        
        //Observer can not be copied
        //but can be assigned by href
        Observer (const Observer&) = delete;
    };
    

    typedef struct
    {
        bool LOCKED : 1;
        bool ERROR : 1;
        bool NOTIFY : 1;
        bool PENDING : 1;
        bool CAN_READ :1;
        bool CAN_WRITE : 1;
    } type;
    
    Observer& getObserver();
    
    //The Observable can not be copied or assigned
    //It will only exist where it was created
    Observable (const Observable&) = delete;
    Observable& operator= (const Observable&) = delete;
};




class uFeeble
{
private:
    
    enum class Signal
    {
        NONE,
        TIMEDOUT,
        LOCKED,
        HALTED,
        RUN,
        NOTIFIELD
    };
    
    
    void Scheduler ();
    
public:
    
    enum class Return
    {
        ERROR = 0,
        OK
    };

    
    class Thread
    {
    private:
        uint64_t nLastActive = 0;
        
        uint32_t nExecutionTime=0;
        uint32_t nTimeSleeping = 0;
        uint32_t nTimeCadency=0;
        uint32_t nID;
        
        Signal   oSignal = Signal::NONE; //starts with Signal::NONE
        uint32_t nSigNote = 0; //Starts with 0
        
        Thread*  pNext;
        
        friend class uFeeble;
    protected:
        virtual void Loop () = 0;
        
        
    public:
        uint64_t getTimeSleeping();
        uint32_t getID();
        uint32_t getSchedulerTime();
        uint32_t getLastExecTime();
        Thread();
        
    };
    
    
    uFeeble ()
    {}
    
    Return Create (Thread& refThread, uint32_t nTimeCadency);
    
    Return Join ();
    
    
    
private:
    Thread* pStart = nullptr;
    uint32_t  nThreadCount = 0;
    
    Thread* nCurrent;
    
protected:
    
};



#endif /* uFeeble_hpp */



