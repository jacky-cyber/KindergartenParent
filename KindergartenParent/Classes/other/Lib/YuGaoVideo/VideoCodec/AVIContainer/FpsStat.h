/*
 *  FpsStat.h
 *  Camera
 *
 *  Created by apple on 12-12-27.
 *  Copyright 2012 HuangSiwen. All rights reserved.
 *
 */

#include <list>

class CFpsStat {
    
public:
    CFpsStat();
    void Reset();
    int GetFps();
    int AddFrame();
    
private:
    int mFps;
    int mFrames;
    time_t mBeginTime;
    std::list<int> mFrameList;
};