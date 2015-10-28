/*
 *  ofxMemoryUsage.h
 *  FlyCapture
 *
 *  Created by Vincent R. on 07.11.11.
 *  Copyright 2011 www.say-nono.com. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"

#ifdef TARGET_WIN32
	#include "windows.h"
	#include "psapi.h"
	#pragma comment( lib, "psapi.lib" )
#endif

#ifdef TARGET_LINUX
	#include <sys/types.h>
	#include <sys/sysctl.h>
	#include "stdlib.h"
	#include "stdio.h"
	#include "string.h"
	#include "sys/times.h"
	#include "sys/vtimes.h"
#endif

#ifdef TARGET_OSX

	#include <sys/types.h>
	#include <sys/sysctl.h>

	#include <mach/vm_statistics.h>
	#include <mach/mach_types.h> 
	#include <mach/mach_init.h>
	#include <mach/mach_host.h>
	#include <mach/mach.h>

#endif



class ofxMemoryUsage{
	
public:	
	void setup();
	void update();
	void draw();
	
	int getTotalMemory();
	int getUsedMemory();
	int getProcessMemory();
	
private:
	int updateSecs;
	int memory_total;
	int memory_used_total;
	int memory_used_process;		
	
};


