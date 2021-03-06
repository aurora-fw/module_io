/****************************************************************************
** ┌─┐┬ ┬┬─┐┌─┐┬─┐┌─┐  ┌─┐┬─┐┌─┐┌┬┐┌─┐┬ ┬┌─┐┬─┐┬┌─
** ├─┤│ │├┬┘│ │├┬┘├─┤  ├┤ ├┬┘├─┤│││├┤ ││││ │├┬┘├┴┐
** ┴ ┴└─┘┴└─└─┘┴└─┴ ┴  └  ┴└─┴ ┴┴ ┴└─┘└┴┘└─┘┴└─┴ ┴
** A Powerful General Purpose Framework
** More information in: https://aurora-fw.github.io/
**
** Copyright (C) 2017 Aurora Framework, All rights reserved.
**
** This file is part of the Aurora Framework. This framework is free
** software; you can redistribute it and/or modify it under the terms of
** the GNU Lesser General Public License version 3 as published by the
** Free Software Foundation and appearing in the file LICENSE included in
** the packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
****************************************************************************/

#include <AuroraFW/CoreLib/Target/System.h>

#ifdef AFW_TARGET_PLATFORM_WINDOWS
	#pragma comment(lib, "psapi.lib")
	#include <windows.h>
	#include <psapi.h>
#elif defined(AFW_TARGET_ENVIRONMENT_UNIX) || defined(AFW_TARGET_PLATFORM_APPLE_MAC)
	#include <unistd.h>
	#include <sys/resource.h>

	#if defined(AFW_TARGET_KERNEL_LINUX)
		#include <sys/sysinfo.h>
		#include <stdio.h>
	#elif defined(AFW_TARGET_PLATFORM_APPLE_MAC)
		#include <mach/mach.h>
		#include <sys/types.h>
		#include <sys/sysctl.h>
		#include <sys/vmmeter.h>
	#endif
#endif

#include <AuroraFW/IO/Info/Memory.h>

namespace AuroraFW {
	namespace IO {
		namespace Info {
			// Total virtual memory size in bytes
			size_t getTotalVirtualMemory()
			{
				#ifdef AFW_TARGET_KERNEL_LINUX
				struct sysinfo mem_temp;
				sysinfo (&mem_temp);
				return (mem_temp.totalram + mem_temp.totalswap) * mem_temp.mem_unit;
				#elif defined(AFW_TARGET_PLATFORM_WINDOWS)

				// TODO: Needs to be tested
				MEMORYSTATUSEX mem_temp;
				mem_temp.dwLength = sizeof(MEMORYSTATUSEX);
				GlobalMemoryStatusEx(&mem_temp);
				return mem_temp.ullTotalVirtual;
				#elif defined(AFW_TARGET_PLATFORM_APPLE_MAC)

				// TODO: Needs to be tested
				struct vmtotal mem_temp;
				vmtotal (&mem_temp);
				return mem_temp.t_vm;
				#endif
			}

			// Used virtual memory size in bytes
			size_t getUsedVirtualMemory()
			{
				#ifdef AFW_TARGET_KERNEL_LINUX
				struct sysinfo mem_temp;
				sysinfo (&mem_temp);
				return ((mem_temp.totalram - mem_temp.freeram) + (mem_temp.totalswap - mem_temp.freeswap)) * mem_temp.mem_unit;
				#elif defined(AFW_TARGET_PLATFORM_WINDOWS)
				MEMORYSTATUSEX mem_temp;
				mem_temp.dwLength = sizeof(MEMORYSTATUSEX);
				GlobalMemoryStatusEx(&mem_temp);
				return mem_temp.ullTotalVirtual - mem_temp.ullAvailVirtual;
				#elif defined(AFW_TARGET_PLATFORM_APPLE_MAC)
				struct vmtotal mem_temp;
				vmtotal (&mem_temp);
				return mem_temp.t_vm - mem_temp.t_free;
				#endif
			}

			// Free virtual memory size in bytes
			size_t getFreeVirtualMemory()
			{
				#ifdef AFW_TARGET_KERNEL_LINUX
				struct sysinfo mem_temp;
				sysinfo (&mem_temp);
				return ((mem_temp.totalram + mem_temp.totalswap) - ((mem_temp.totalram - mem_temp.freeram) + (mem_temp.totalswap - mem_temp.freeswap))) * mem_temp.mem_unit;
				#elif defined(AFW_TARGET_PLATFORM_WINDOWS)

				// TODO: Needs to be tested
				MEMORYSTATUSEX mem_temp;
				mem_temp.dwLength = sizeof(MEMORYSTATUSEX);
				GlobalMemoryStatusEx(&mem_temp);
				return mem_temp.ullAvailVirtual;
				#elif defined(AFW_TARGET_PLARFORM_APPLE_MAC)

				// TODO: Needs to be tested
				struct vmtotal mem_temp;
				vmtotal (&mem_temp);
				return mem_temp.t_free;
				#endif
			}

			// Total pysical memory size in bytes
			size_t getTotalPhysicalMemory()
			{
				#ifdef AFW_TARGET_KERNEL_LINUX
				struct sysinfo mem_temp;
				sysinfo (&mem_temp);
				return mem_temp.totalram * mem_temp.mem_unit;
				#elif defined(AFW_TARGET_PLATFORM_WINDOWS)

				// TODO: Needs to be tested
				MEMORYSTATUSEX mem_temp;
				mem_temp.dwLength = sizeof(MEMORYSTATUSEX);
				GlobalMemoryStatusEx(&mem_temp);
				return mem_temp.ullTotalPhys;
				#elif defined(AFW_TARGET_PLATFORM_APPLE_MAC)

				// TODO: Needs to be tested
				int mib[2];
				int64_t physical_memory;
				size_t length;
				// Get the Physical memory size
				mib[0] = CTL_HW;
				mib[1] = HW_MEMSIZE;
				length = sizeof(int64);
				sysctl(mib, 2, &physical_memory, &length, NULL, 0);
				return physical_memory;
				#endif
			}

			// Used pysical memory size in bytes
			size_t getUsedPhysicalMemory()
			{
				#ifdef AFW_TARGET_KERNEL_LINUX
				struct sysinfo mem_temp;
				sysinfo (&mem_temp);
				return (mem_temp.totalram - mem_temp.freeram) * mem_temp.mem_unit;
				#elif defined(AFW_TARGET_PLATFORM_WINDOWS)

				// TODO: Needs to be tested
				MEMORYSTATUSEX mem_temp;
				mem_temp.dwLength = sizeof(MEMORYSTATUSEX);
				GlobalMemoryStatusEx(&mem_temp);
				return mem_temp.ullTotalPhys - mem_temp.ullAvailPhys;
				#elif defined(AFW_TARGET_PLATFORM_APPLE_MAC)

				// TODO: Needs to be tested
				struct vmmeter mem_temp;
				vmmeter (&mem_temp);
				int mib[2];
				int64_t physical_memory;
				size_t length;
				// Get the Physical memory size
				mib[0] = CTL_HW;
				mib[1] = HW_MEMSIZE;
				length = sizeof(int64);
				sysctl(mib, 2, &physical_memory, &length, NULL, 0);
				return physical_memory - mem_temp.v_free_count;
				#endif
			}

			// Free pysical memory size in bytes
			size_t getFreePhysicalMemory()
			{
				#ifdef AFW_TARGET_KERNEL_LINUX
				struct sysinfo mem_temp;
				sysinfo (&mem_temp);
				return mem_temp.freeram * mem_temp.mem_unit;
				#elif defined(AFW_TARGET_PLATFORM_WINDOWS)

				// TODO: Needs to be tested
				MEMORYSTATUSEX mem_temp;
				mem_temp.dwLength = sizeof(MEMORYSTATUSEX);
				GlobalMemoryStatusEx(&mem_temp);
				return mem_temp.ullAvailPhys;
				#elif defined(AFW_TARGET_PLATFORM_APPLE_MAC)

				// TODO: Needs to be tested
				struct vmmeter mem_temp;
				vmmeter (&mem_temp);
				return mem_temp.v_free_count;
				#endif
			}

			size_t getCurrentRSS()
			{
				#ifdef AFW_TARGET_PLATFORM_WINDOWS
					PROCESS_MEMORY_COUNTERS info;
					GetProcessMemoryInfo(GetCurrentProcess(), &info, sizeof(info));
					return info.WorkingSetSize;
				#elif defined(AFW_TARGET_KERNEL_LINUX)
					FILE* fp = fopen("/proc/self/status", "r");
					if(fp == AFW_NULL)
						return AFW_NULLVAL;
					size_t ret;
					if (fscanf(fp, "%*s%lu", &ret) != 1) {
						fclose(fp);
						return AFW_NULLVAL;
					}
					fclose(fp);
					return ret * sysconf(_SC_PAGESIZE);
				#elif defined(AFW_TARGET_PLATFORM_APPLE_MAC)
					struct mach_task_basic_info info;
					mach_msg_type_number_t infoCount = MACH_TASK_BASIC_INFO_COUNT;
					if (task_info(mach_task_self(), MACH_TASK_BASIC_INFO, (task_info_t)&info, &infoCount) != KERN_SUCCESS)
						return AFW_NULLVAL;
					return info.resident_size;
				#else
					return AFW_NULLVAL;
				#endif
			}

			size_t getPeakRSS()
			{
				#ifdef AFW_TARGET_PLATFORM_WINDOWS
					PROCESS_MEMORY_COUNTERS info;
					GetProcessMemoryInfo(GetCurrentProcess(), &info, sizeof(info));
					return (size_t)info.PeakWorkingSetSize;
				#elif defined(AFW_TARGET_ENVIRONMENT_UNIX) || defined(AFW_TARGET_PLATFORM_APPLE_MAC)
					struct rusage ret;
					getrusage(RUSAGE_SELF, &ret);
					#if defined(AFW_TARGET_PLATFORM_APPLE_MAC)
						return ret.ru_maxrss;
					#else
						return ret.ru_maxrss * 1024;
					#endif
				#else
					return AFW_NULLVAL;
				#endif
			}

			SystemMemory getSystemMemory()
			{
				#pragma message ("TODO: Need to be implemented")
			}
		}
	}
}
