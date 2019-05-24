//
//  uFeeble.cpp
//  uFeeble
//
//  Created by GUSTAVO CAMPOS on 08/05/2019.
//  Copyright © 2019 GUSTAVO CAMPOS. All rights reserved.
//

#include "uFeeble.hpp"
#include <sys/time.h>
#include <unistd.h>

#define MIN(a,b) (((a)<(b))?(a):(b))

uFeeble::Thread::Thread()
{
}

uint32_t uFeeble::Thread::getID()
{
    return nID;
}

uint32_t uFeeble::Thread::getSchedulerTime()
{
    return nTimeCadency;
}

uint32_t uFeeble::Thread::getLastExecTime()
{
    return nExecutionTime;
}








uFeeble::Return uFeeble::Create(uFeeble::Thread &refThread, uint32_t nTimeCadency)
{
    if (nThreadCount == UINT32_MAX) return Return::ERROR;
    
    refThread.pNext = nullptr;
    
    refThread.nTimeCadency = nTimeCadency;
    
    if (pStart == nullptr)
    {
        pStart = &refThread;
    }
    else
    {
        Thread* pOffset = pStart;
        
        while (pOffset->pNext != nullptr) pOffset = pOffset->pNext;
        
        pOffset->pNext = &refThread;
    }
    
    nThreadCount++;
    
    refThread.nID = nThreadCount;
    
    refThread.nLastActive = getMileSeconds();
    
    std::cout << "[ADDING]\t(" << (nThreadCount-1) << ") - Memory: [" << ((size_t) &refThread) << "]" << std::endl;
    
    return Return::OK;
}

inline uint64_t uFeeble::getMileSeconds()
{
    struct timeval tp;
    gettimeofday(&tp, NULL);
    
    return tp.tv_sec * 1000 + tp.tv_usec / 1000; //get current timestamp in milliseconds
}


void uFeeble::Scheduler()
{
    if (pStart == nullptr) return;
    
    uint64_t nTimeNow;
    int64_t nTimeFactor;
    int64_t nNextEvent;
    int64_t nEventCalc;
    
    Thread* pOffSet;
    
    pOffSet = pStart;
    
    
    //Initializing timers
    do
    {
        pOffSet->nLastActive = getMileSeconds();
    } while ((pOffSet = pOffSet->pNext) !=  nullptr);
    
    
    //Start Scheduler procedures
    //the small amount of time will be 1ms.
    while ((pOffSet = pStart))
    {
        nTimeNow = getMileSeconds();
        nNextEvent=nTimeNow;
        
        //This will initiate all thread counters
        if (pOffSet->nLastActive == 0)
        {
            pOffSet->nLastActive = nTimeNow;
        }
        
        do
        {
            nEventCalc = (int64_t)((int64_t) pOffSet->nTimeCadency - (nTimeNow - (int64_t) pOffSet->nLastActive));
            
            if (nEventCalc < 1)
            {
                //std::cout << "Delay: (" << nNextEvent << ")" << std::endl;
                
                pOffSet->nLastActive = getMileSeconds();
                pOffSet->Loop(nEventCalc);
                pOffSet->nExecutionTime = (uint32_t) (getMileSeconds() - pOffSet->nLastActive);
                
                nNextEvent = pOffSet->nTimeCadency - (pOffSet->nExecutionTime % pOffSet->nTimeCadency);
                
            }
            
            
            nNextEvent = (int64_t) MIN ((int64_t) nNextEvent, (int64_t) nEventCalc);
            
            
            //std::cout << __PRETTY_FUNCTION__ << ": [" << ((size_t) pOffSet) << "] expires at: [" << pOffSet->nTimeCadency << "].[" << (nTimeNow - pOffSet->nLastActive) << "] nNextEvent: [" << nNextEvent << "]- Calc: [" << (int64_t)((int64_t) pOffSet->nTimeCadency - (nTimeNow - (int64_t) pOffSet->nLastActive)) << "] EventCalc: [" << nEventCalc << "]" << std::endl;
            
        } while ((pOffSet = pOffSet->pNext) !=  nullptr);
        
        
        nTimeFactor = (getMileSeconds() - nTimeNow)+5;
        nTimeFactor = nTimeFactor < 0 ? 0 : nTimeFactor;
        
        //std::cout << "restarting.... factor (" << nTimeFactor << ")" << std::endl << std::endl;
        
        if (nNextEvent > nTimeFactor && nNextEvent != nTimeNow)
        {
            //std::cout << "Sleeping..." << getMileSeconds() <<  std::endl;
            usleep ((useconds_t) ((nNextEvent - nTimeFactor) * 1000));
            //std::cout << "Waking up..." << getMileSeconds() <<  std::endl << std::endl;
        }
            
    }
}

uFeeble::Return uFeeble::Join()
{
    Scheduler();
    
    return Return::ERROR;
}





Observable::Observer::Observer(Observable &refObservable) : refObservable(refObservable)
{;}


void Observable::Observer::Attach(uFeeble &refuFeeble)
{
    puFeeble = &refuFeeble;
}


void Observable::Observer::Dettach()
{
    puFeeble = nullptr;
}




Observable::Observer &Observable::getObserver()
{
    return *pObserver;
}


Observable::Observable()
{
    pObserver = new Observer (*this);
}


Observable::~Observable()
{
    if (pObserver != nullptr) delete pObserver;
}


void Observable::notify(Observable::type& typeAction, size_t nMessage)
{
    
    if (pObserver->puFeeble == nullptr)
    {
        switch (typeAction)
        {
        case type::ERROR:
                pObserver->action.type.ERROR = 1;
            break;
                
        case type::NOTIFY:
                pObserver->action.type.NOTIFY=1;
            break;
                
        case type::NONE:
                 pObserver->action.nValue = 0;
            break;
        }
    
        pObserver->nMessage = nMessage;
    }
    else if (typeAction != type::NONE)
    {
        uFeeble::Thread* pThOffset = pObserver->puFeeble->pStart;
        
        if (pThOffset != nullptr) do
        {
            if (pThOffset->__boolInternal == false) switch (typeAction)
            {
                case type::ERROR:
                    pThOffset->onError (nMessage);
                    break;
                    
                case type::NOTIFY:
                    pThOffset->onNotify (nMessage);
                    break;
                    
                default:
                    break;
            }
        } while ((pThOffset = pThOffset->pNext) != nullptr);
        
        pObserver->action.nValue = 0;
    }
}



void ObservableCalls::onError(size_t nMessage)
{__boolInternal = true;}

void ObservableCalls::onNotify(size_t nMessage)
{__boolInternal = true;}
