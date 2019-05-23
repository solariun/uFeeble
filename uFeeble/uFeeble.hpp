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

class uFeeble;


class ObservableCalls
{
private:
    bool __boolInternal=false;
    
    friend class Observable;
    
protected:
    virtual void onError(size_t nMessage);
    virtual void onNotify(size_t nMessage);

    friend class Observable;
};


class Observable 
{
private:
    friend class Observer;
    
protected:
public:

    Observable();
    ~Observable();

    enum class type : uint8_t
    {
        NONE = 0,
        ERROR,
        NOTIFY,
    };

    class Observer : protected ObservableCalls
    {
    private:
        
        Observable& refObservable;
        Observer (Observable& refObservable);
        
        uFeeble* puFeeble = nullptr;
        
        uint32_t nLocks=0;

        
        size_t nMessage  = 0;
        
        friend class Observable;
        
        union type
        {
            struct
            {
                bool ERROR : 1;
                bool NOTIFY : 1;
            } type;
            
            uint8_t nValue = 0;
        } action;
        
    protected:
        
    public:
        
        //Observer can not be copied
        //but can be assigned by href
        Observer (const Observer&) = delete;
        Observer () = delete;
        
        
        void Attach(uFeeble& refuFeeble);
        void Dettach();
    };
    
    
    Observer& getObserver ();
    
    //The Observable can not be copied or assigned
    //It will only exist where it was created
    Observable (const Observable&) = delete;
    Observable& operator= (const Observable&) = delete;
    
    
    //Set Operations
    
    void lock ();
    void unlock ();
    
    void notify (type& typeAction, size_t nMessage = 0);
    
private:
    Observer* pObserver;
    
};




class uFeeble
{
private:
    
    void Scheduler ();
    
    friend class Observable;
    
public:
    
    enum class Return : uint8_t
    {
        ERROR = 0,
        OK
    };

    
    class Thread :  protected ObservableCalls
    {
    private:
        uint64_t nLastActive = 0;
        
        uint32_t nExecutionTime=0;
        uint32_t nTimeSleeping = 0;
        uint32_t nTimeCadency=0;
        uint32_t nID;
        
        
        Thread*  pNext;
        
        friend class uFeeble;
        friend class Observable;
        
        
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



