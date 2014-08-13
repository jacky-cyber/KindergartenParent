/*
 *  SafeLock.h
 *  Camera
 *
 *  Created by apple on 12-3-30.
 *  Copyright 2012 HuangSiwen. All rights reserved.
 *
 */


#if !defined(_SAFE_LOCK_H_)
#define _SAFE_LOCK_H_

#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

class CriticalSection
{
public:
	CriticalSection(void)
	{ pthread_mutex_init(&mutex,NULL); }
	~CriticalSection(void)
	{ pthread_mutex_destroy(&mutex); }
	void Lock(void)
	{ pthread_mutex_lock(&mutex); }
	void Unlock(void)
	{ pthread_mutex_unlock(&mutex); }
private:
	pthread_mutex_t mutex;
};

class CSafeLock
{
public:
    CSafeLock(CriticalSection* pcs) : pcs_(pcs)
    {
        // assert(NULL!=pcs_);
        pcs_->Lock();
    }
    
    ~CSafeLock(void)
    {
        pcs_->Unlock();
    }
private:
    CriticalSection* pcs_;
};

#endif // _SAFE_LOCK_H_
