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

class ObservableCalls
{
protected:
    virtual void onLock();
    virtual void onError();
    virtual void onNotify();
    virtual void onPending();
    virtual void onCanRead();
    virtual void onCanWrite();
    
    friend class Observable;
};


class Observable 
{
private:
    uint32_t nLocks=0;
    
    friend class Observer;
    
protected:
public:

    Observable();
    ~Observable();

    enum class type
    {
        NONE = 0,
        LOCKED=1,
        ERROR,
        NOTIFY,
        PENDING,
        CANREAD,
        CANWRITE
    };

    class Observer : protected ObservableCalls
    {
    private:
        
        Observable& refObservable;
        Observer (Observable& refObservable);
    
        type notify = type::NONE;
        size_t nMessage  = 0;
        
        friend class Observable;
        
    protected:
        
    public:
        Observer () = delete;
        
        //Observer can not be copied
        //but can be assigned by href
        Observer (const Observer&) = delete;
    };
    
    
    Observer& getObserver ();
    
    //The Observable can not be copied or assigned
    //It will only exist where it was created
    Observable (const Observable&) = delete;
    Observable& operator= (const Observable&) = delete;
    
    
    //Set Operations
    
    void lock ();
    void unlock ();
    
    void notify (type& typeAction, size_t nMessage);
    
private:
    Observer* pObserver;
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



