#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <iostream>
#include <cstdint>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>

#include <linux/sysctl.h>
#include <sys/syscall.h>
#include <sys/sysinfo.h>

long unsigned int cpu_arch = sizeof(char*);
const char* meminfo = "/proc/meminfo";
const char* cpuinfo = "/proc/cpuinfo";
const char* version = "/proc/version";

void banner(void) {
	puts(
		"\n"
		"\n"
		"                                         .__               \n"
		"_____________  ____  _________.__. _____ |__|____    ____   \n"
		"\\____ \\_  __ \\/  _ \\/  ___<   |  |/     \\|  \\__  \\  /    \\  \n"
		"|  |_> >  | \\(  <_> )___ \\ \\___  |  Y Y  \\  |/ __ \\|   |  \\ \n"
		"|   __/|__|   \\____/____  >/ ____|__|_|  /__(____  /___|  / \n"
		"|__|                    \\/ \\/          \\/        \\/     \\/  \n"
		"\n"
		"[*] Lemur - https://github.com/1emvr March, 2023 - GNU GENERAL PUBLIC LICENSE Version 3\n"
	);
}

class SystemInformation 
{
	public:
		long unsigned int Architecture;
		const char* MemoryInfoFile;
		const char* KernelInfoFile;
		const char* CpuInfoFile;
		
		// Constructor & Destructor
		SystemInformation(long unsigned int architecture, const char* meminfo_file, const char* cpuinfo_file, const char* kerninfo_file) 
		{
			Architecture = architecture;
			MemoryInfoFile = meminfo_file;
			CpuInfoFile = cpuinfo_file;
			KernelInfoFile = kerninfo_file;
		}

		// Class methods
		void GetMemoryInfo() 
		{
			double factor = 1024 * 1024;
			
			std::uint64_t value;
			std::string line, label;
	
			auto ifs = std::ifstream{MemoryInfoFile};
			while (std::getline(ifs, line)) 
			{
				std::stringstream StringStream{line};
				StringStream >> label >> value;
	
				if (label == "MemTotal:") { std::cout << "\t[+] Total Memory (GB) " << (value / factor) << "\n"; }
				if (label == "MemAvailable") { std::cout << "\t[+] Memory Available (GB) " << (value / factor) << "\n"; }
			}
		}

		void GetCpuInfo() 
		{
			if (Architecture == 8) { printf("\t[+] 64-bit architecture detected.\n\n"); }
				else { printf("\t[+] 32-bit architecture detected.\n\n"); }

			std::string line, label;
			std::cout << "[+] System Information:" << "\n";

			auto ifs = std::ifstream{CpuInfoFile};
			while (std::getline(ifs, line)) 
			{
				std::stringstream StringStream{line};
				StringStream >> label;

				if (label == "model") { std::cout << line << "\n"; }
				if (label == "cpu") { std::cout << line << "\n"; }
				if (label == "cache") { std::cout << line << "\n"; break; }
			}
			printf("\n");
		}
};

void GetRuntimeInfo() {
	/* 
		This function does not actually utilize the class structure and is just a general function.
		I'm leaving it here because it's simply not a part of SystemInfo class and just uses standard linux struct.b
	
		Credit for this one goes to caiorss - https://gist.github.com/caiorss
	*/
        struct sysinfo info;
        sysinfo(&info);
   		
   		double factor = 1024 * 1024 * 1024;
        std::uint64_t day_to_hours = 24 * 60;

        std::cout << "Runtime Information:\n";
        std::cout << " [*] System uptime since boot (minutes) = " << info.uptime << '\n'
   				  << " [*] System uptime since boot   (hours) = " << info.uptime    / day_to_hours << '\n'
  	  			  << " [*] Total RAM memory (Gb)              = " << info.totalram  / factor << '\n'
	  			  << " [*] Free  RAM memory (Gb)              = " << info.freeram   / factor << '\n'
	  			  << " [*] Total SWAP (Gb)                    = " << info.totalswap / factor << '\n'				  
	  			  << " [*] Free SWAP (Gb)                     = " << info.freeswap  / factor << '\n'				  				  
	  			  << " [*] Number of processes running        = " << info.procs << '\n'
	  			  << '\n';
}

int main(void) {

	banner();

	// Call constructor then methods
	SystemInformation StandardInfo(cpu_arch, meminfo, cpuinfo, version);
	StandardInfo.GetMemoryInfo();
	StandardInfo.GetCpuInfo();
	GetRuntimeInfo();

	std::cout << "Network Information:\n";
	system("ip addr show enp0s3 | egrep 'inet '");
	system("ip addr show enp0s8 | egrep 'inet '");

	puts("\nTry to execute /bin/sh\n");
	bof_function();

	return 0;
}
