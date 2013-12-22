// run.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Windows.h"
#include "winuser.h"
#include <Mmsystem.h>
#include "TlHelp32.h"
#include<fstream>
#include<iostream>

using namespace std;

#include "pdh.h"  
#pragma comment(lib, "Pdh.lib") //显式连接Pdh.lib  

bool bBack=true;
int WAITE_TIME=600000;
int CHECK_TIME=3000;
int CPUUSEAGE=20;

bool inactive()
{
	LASTINPUTINFO lpi;
	DWORD dwTime = 0;
	lpi.cbSize = sizeof(lpi);
	GetLastInputInfo(&lpi);
	dwTime = ::GetTickCount()-lpi.dwTime;
	if(dwTime > WAITE_TIME)
		return true;
	else
		return false;
}

bool creatProcess(PROCESS_INFORMATION *pi)
{
	char chPath[301];

	::GetCurrentDirectory(300,(LPTSTR)chPath);//得到当前目录

	char path[200]= "//ToRun.cmd";

	strcat(chPath,path);
	STARTUPINFO si;
	
	ZeroMemory( pi, sizeof(*pi) );
	ZeroMemory( &si, sizeof(si) );

	si.cb = sizeof(si);

	// Start the child process
	if(CreateProcess(chPath, "", NULL, NULL, FALSE, 0, NULL, NULL, &si, pi))
	{
		printf("Created!!!\n");
		return true;
	}
	else
	{
		printf("Created fail!!!\n");
		return false;
	}
}

bool __fastcall KillProcessTree(DWORD myprocID)
{

	PROCESSENTRY32 pe;

	memset(&pe, 0, sizeof(PROCESSENTRY32));
	pe.dwSize = sizeof(PROCESSENTRY32);

	HANDLE hSnap = :: CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if (::Process32First(hSnap, &pe))
	{
		BOOL bContinue = TRUE;

		// kill child processes
		while (bContinue)
		{
			// only kill child processes
			if (pe.th32ParentProcessID == myprocID)
			{
				HANDLE hChildProc = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, pe.th32ProcessID);

				if (hChildProc)
				{
					::TerminateProcess(hChildProc, 1);
					::CloseHandle(hChildProc);
				}               
			}

			bContinue = ::Process32Next(hSnap, &pe);
		}

		// kill the main process
		HANDLE hProc = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, myprocID);

		if (hProc)
		{
			::TerminateProcess(hProc, 1);
			::CloseHandle(hProc);
		}       
	}
	return true;
}

void runBack()
{
	HWND hwnd;
	if(hwnd=::FindWindow("ConsoleWindowClass",NULL)) //
	{
		::ShowWindow(hwnd,SW_HIDE); //后台运行
	}
}

void readConfig()
{
	ifstream inf;

    inf.open("Config.txt");
	if(inf)
	{
		inf >> bBack;
		inf >> CHECK_TIME;
		inf >> WAITE_TIME;
		inf >> CPUUSEAGE;
	}
   

	inf.close();
}

int main(int argc, char* argv[])
{
	bool bCreated = false;
	bool bInactive = false;
	PROCESS_INFORMATION pi;
	
	
	readConfig();
	if(bBack)
	{
		runBack();
	}

	PDH_STATUS status = -1;  
	HQUERY hQuery = NULL;                       //计数器对象句柄  
	HCOUNTER hCounter = NULL;                   //计数器句柄  
	PDH_FMT_COUNTERVALUE DisplayValue;          //计数器值  
	DWORD dwCounterType = 0;                    //计数器类型  
	status = PdhOpenQuery(NULL, 0, &hQuery);    //打开计数器对象

	if (ERROR_SUCCESS == status)  
	{  
	  //PdhAddCounter()函数添加计数器，参数中Processor代表计数器对象，(0)表示第1个处理器计数实例。
	  //如果计算机上有两个处理器，则第2个处理器计数实例用(1)表示  
	  //% Processor Time表示计数器（取得处理器占用时间）,注意%后面有个空格  
	  status = PdhAddCounter(hQuery, "\\Processor(0)\\% Processor Time", 0, &hCounter);

	  if (ERROR_SUCCESS == status)  
	  {  
		  // PdhCollectQueryData()函数收集数据需两次调用, PdhCollectQueryDataEx只需一次
		  printf("CPU占用 首次收集数据  \n");  
		  PdhCollectQueryData(hQuery);        //首次收集数据  
	  }
	}


    while(1)
    {
        Sleep(CHECK_TIME);

		PdhCollectQueryData(hQuery);    //再次收集数据

		status = PdhGetFormattedCounterValue(hCounter,
					PDH_FMT_DOUBLE,   
					&dwCounterType, 
					&DisplayValue);

		//取得收集的数据值
		//printf("CPU占用 %.2f\n", DisplayValue.doubleValue);  

		bInactive = inactive();
		if(false == bCreated)
		{
			if(bInactive && DisplayValue.doubleValue < CPUUSEAGE)
			{
				bCreated = creatProcess(&pi);
			}
		}
		else
		{
			if(!bInactive)
			{
				
				if(KillProcessTree(pi.dwProcessId))
				{
					CloseHandle(pi.hThread);
					CloseHandle(pi.hProcess);
					printf("Stoped!!!\n");
					bCreated = false;
				}
			}
		}

    }
	
	PdhCloseQuery(hQuery);                  //关闭计数器对象  
	return 0;
}
