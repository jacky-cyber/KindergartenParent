/*
 *  DebugLog.h
 *  Camera
 *
 *  Created by apple on 12-8-29.
 *  Copyright 2012 HuangSiwen. All rights reserved.
 *
 */


#ifndef __Debug_Log_H__
#define __Debug_Log_H__

#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#if DEBUG
#define SHOW_DEBUG_INFO     1
#else
#define SHOW_DEBUG_INFO     0
#endif // DEBUG


#if SHOW_DEBUG_INFO
    #define debugNSLog(arg...)      NSLog(arg)
    #define debugPrintf(arg...)     printf(arg)
//  #define debugPrintf(arg...)     debugPrintf_internal(arg)
#else
    #define debugNSLog(arg...)
    #define debugPrintf(arg...)
#endif


#ifdef __cplusplus
extern "C" {
#endif

    void debugPrintf_internal(const char *format, ...);
    
#ifdef __cplusplus
}
#endif


#endif  // !__Debug_Log_H__
