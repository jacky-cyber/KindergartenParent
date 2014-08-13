/*
 *  FpsStat.cpp
 *  Camera
 *
 *  Created by apple on 12-12-27.
 *  Copyright 2012 HuangSiwen. All rights reserved.
 *
 */

#include <time.h>
#include "FpsStat.h"

const int STAT_DURATION = 5;

CFpsStat::CFpsStat() {
    Reset();
}

void CFpsStat::Reset() {
    mFps = 0;
    mFrames = 0;
    mBeginTime = 0;
    mFrameList.clear();    
}

int CFpsStat::GetFps() {
    return mFps;
}

int CFpsStat::AddFrame() {
    if (mBeginTime == 0) {
        mBeginTime = time(NULL);
    }
    
    time_t endTime = time(NULL);    
    if (difftime(endTime, mBeginTime) >= 1) {
        mBeginTime = endTime;
        
        if (mFrameList.size() >= STAT_DURATION) {
            mFrameList.pop_front();
        }
        mFrameList.push_back(mFrames);
        mFrames = 0;
    }
    mFrames++;
    
    int allFrames = 0;
    std::list<int>::iterator iter = mFrameList.begin();
    for (; iter != mFrameList.end(); iter++) {
        allFrames += *iter;
    }
    
    int seconds = mFrameList.size();
    if (seconds == 0) {
        mFps = mFrames;
    } else {
        mFps = allFrames / seconds;
    }
    
    return mFps;
}
