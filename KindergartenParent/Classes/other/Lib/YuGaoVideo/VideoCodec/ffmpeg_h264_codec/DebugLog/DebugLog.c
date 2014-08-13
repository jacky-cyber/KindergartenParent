/*
 *  DebugLog.c
 *  Camera
 *
 *  Created by apple on 12-8-29.
 *  Copyright 2012 HuangSiwen. All rights reserved.
 *
 */

#include "DebugLog.h"

void debugPrintf_internal(const char *format, ...)
{
    va_list vlist;
    va_start(vlist, format);
    vprintf(format, vlist);
    va_end(vlist);
}
