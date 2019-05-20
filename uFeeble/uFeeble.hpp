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
    
    enum class Return
    {
        ERROR = 0,
        OK
    };
    
    
    void Scheduler ();
    
public:
    
    class Thread
    {
    private:
        uint64_t nLastActive = 0;
        uint64_t nTimeSleeping = 0;
        
        uint32_t nTimeCadency;
        uint32_t nID;
        
        Signal   oSignal = Signal::NONE; //starts with Signal::NONE
        uint32_t nSigNote = 0; //Starts with 0
        
        Thread*  pNext;
        
        friend class uFeeble;
    protected:
        virtual void Loop () = 0;
        
        
    public:
        uint64_t getTimeSleeping();
        Thread();
        
    };
    
    
    uFeeble ()
    {}
    
    Return Create (Thread& refThread, uint32_t nTimeCadency);
    
    Return Join ();
    
    
    
private:
    Thread* pStart = nullptr;
    size_t  nThreadCount;
    
    Thread* nCurrent;
    
protected:
    
};



#endif /* uFeeble_hpp */



