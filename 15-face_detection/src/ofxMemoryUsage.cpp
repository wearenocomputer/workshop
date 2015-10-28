/*
 *  ofxMemoryUsage.cpp
 *  
 *
 *  Created by Vincent R. on 07.11.11.
 *  Copyright 2011 www.say-nono.com. All rights reserved.
 *
 
 
 http://stackoverflow.com/questions/63166/how-to-determine-cpu-and-memory-consumption-from-inside-a-process
 
 
 */

#include "ofxMemoryUsage.h"

void ofxMemoryUsage::setup(){
	update();
	updateSecs = -1;
}

void ofxMemoryUsage::update(){
	
	int secs = (int)(ofGetElapsedTimeMillis()/1000)%60;
	if(updateSecs != secs){
		updateSecs = secs;
		memory_total = getTotalMemory();
		memory_used_total = getUsedMemory();
		memory_used_process = getProcessMemory();		
	}
}


void ofxMemoryUsage::draw(){
	ofPushMatrix();
	ofTranslate(0, ofGetHeight()-70, 0);
	ofEnableAlphaBlending();
	ofSetColor(0xff,0xff,0xff, 0xaa);
	ofRect(0, 0, 180, 70);
	ofDisableAlphaBlending();
	ofSetHexColor(0x000000);
	ofDrawBitmapString("Total: " + ofToString(memory_total), 10,15);
	ofDrawBitmapString("Used:  " + ofToString(memory_used_total), 10,30);
	ofDrawBitmapString("Used by process: " + ofToString(memory_used_process), 10,45);
    ofDrawBitmapString("Framerate: " + ofToString(ofGetFrameRate(),2), 10,60);
    ofSetColor(255,255,255);
	ofPopMatrix();
}


int ofxMemoryUsage::getTotalMemory(){
	
	int mb = 0;
	int gb = 0;
	
#ifdef TARGET_WIN32
    MEMORYSTATUSEX memInfo;
    memInfo.dwLength = sizeof(MEMORYSTATUSEX);
    GlobalMemoryStatusEx(&memInfo);
    DWORDLONG totalVirtualMem = memInfo.ullTotalPageFile;
	DWORDLONG physical_memory = memInfo.ullTotalPhys;
	mb = physical_memory/1048576;
#endif
	
#ifdef TARGET_LINUX
    struct sysinfo memInfo;
    sysinfo (&memInfo);
    long long physical_memory = memInfo.totalram;
    physical_memory *= memInfo.mem_unit;
	mb = physical_memory/1048576;	
#endif
	
#ifdef TARGET_OSX
	int64_t physical_memory;
	int mib[2];
	mib[0] = CTL_HW;
	mib[1] = HW_MEMSIZE;
	size_t length = sizeof(int64_t);
	sysctl(mib, 2, &physical_memory, &length, NULL, 0);
	mb = physical_memory/1048576;
	gb = physical_memory/1073741824;	
#endif
	
	return mb;
}

int ofxMemoryUsage::getUsedMemory(){

	int mb = 0;
	int gb = 0;

#ifdef TARGET_WIN32	
    MEMORYSTATUSEX memInfo;
    memInfo.dwLength = sizeof(MEMORYSTATUSEX);
    GlobalMemoryStatusEx(&memInfo);
    DWORDLONG used_memory = memInfo.ullTotalPhys - memInfo.ullAvailPhys;
	mb = used_memory/1048576;	
#endif
	
#ifdef TARGET_LINUX
    struct sysinfo memInfo;
    sysinfo (&memInfo);
    long long used_memory = memInfo.totalram;
    used_memory *= memInfo.mem_unit;
	mb = used_memory/1048576;
#endif
	
#ifdef TARGET_OSX
	
	vm_size_t page_size;
	mach_port_t mach_port;
	mach_msg_type_number_t count;
	vm_statistics_data_t vm_stats;
	int64_t used_memory = 0;
	int64_t myFreeMemory = 0;
	
	mach_port = mach_host_self();
	count = sizeof(vm_stats) / sizeof(natural_t);
	if (KERN_SUCCESS == host_page_size(mach_port, &page_size) &&
		KERN_SUCCESS == host_statistics(mach_port, HOST_VM_INFO, 
										(host_info_t)&vm_stats, &count))
	{
		myFreeMemory = (int64_t)vm_stats.free_count * (int64_t)page_size;
		
		used_memory = ((int64_t)vm_stats.active_count + 
					   (int64_t)vm_stats.inactive_count + 
					   (int64_t)vm_stats.wire_count) *  (int64_t)page_size;
		mb = used_memory/1048576;
	}

//	int mb = used_memory/1048576;	
//	printf("physical_memory : used : mb : %i\n",mb);
//	mb = myFreeMemory/1048576;
//	printf("physical_memory : free : mb : %i\n",mb);

	
#endif
	
	return mb;
	
}

int ofxMemoryUsage::getProcessMemory(){
	
	int mb = 0;
	int gb = 0;
	
#ifdef TARGET_WIN32
    PROCESS_MEMORY_COUNTERS pmc;
    GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc));
    SIZE_T virtualMemUsedByMe = pmc.PagefileUsage;
    SIZE_T used_memory = pmc.WorkingSetSize;
	mb = used_memory/1048576;	
#endif
	
#ifdef TARGET_LINUX
	FILE* file = fopen("/proc/self/status", "r");
	int result = -1;
	char line[128];
	while (fgets(line, 128, file) != NULL){
		if (strncmp(line, "VmRSS:", 6) == 0) result = parseLine(line);
		break;
	}
	fclose(file);
	mb = result/1024;
#endif
	
#ifdef TARGET_OSX
	struct task_basic_info t_info;
	mach_msg_type_number_t t_info_count = TASK_BASIC_INFO_COUNT;
	
	if (KERN_SUCCESS != task_info(mach_task_self(),
								  TASK_BASIC_INFO, (task_info_t)&t_info, 
								  &t_info_count))
	{
		return -1;
	}
	mb = t_info.resident_size/1048576;
#endif
	
	return mb;
}


