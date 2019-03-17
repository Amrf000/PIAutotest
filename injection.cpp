#include <Windows.h>
#include "Injection.h"
#include <ostream>
#include <iostream>
#include <TlHelp32.h>
#include <tchar.h>
#include <Winternl.h>
#include <fstream>
#include <future>
#include <filesystem>

#define RTN_OK 0
#define RTN_USAGE 1
#define RTN_ERROR 13

#define DEBUG
using namespace std;

VOID DbgPrint(char *msg)
{
   printf("%s\n",msg);
}

BOOL Dll_Injection(TCHAR *dll_name, TCHAR processname[])
{
    TCHAR lpdllpath[MAX_PATH];
    GetFullPathName(dll_name, MAX_PATH, lpdllpath, nullptr);

    /* Snapshot of processes */
    DWORD processId{};
    DbgPrint(const_cast<char*>("[+] creating process snapshot"));
    auto hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot == INVALID_HANDLE_VALUE)
    {
        DbgPrint(const_cast<char*>("[!] failed to create process snapshot"));
        return FALSE;
    }
    DbgPrint(const_cast<char*>("[+] Created process snapshot\n\n"));
    PROCESSENTRY32 pe{}; /* Describes an entry from a list of the processes residing
                         in the system address space when a snapshot was taken.
                         The size of the structure, in bytes. Before calling the
                         Process32First function, set this member to sizeof(PROCESSENTRY32).
                         If you do not initialize dwSize, Process32First fails. (msdn) */

    pe.dwSize = sizeof PROCESSENTRY32;
        /* MSDN:
        The size of the structure, in bytes. Before calling the Process32First
        function, set this member to sizeof(PROCESSENTRY32).
        If you do not initialize dwSize, Process32First fails.*/


    /* get first PID */
    DbgPrint(const_cast<char*>("[+] Starting process search"));
    BOOL isProcessFound = FALSE;
    if (Process32First(hSnapshot, &pe) == FALSE)
    {
        CloseHandle(hSnapshot);
        DbgPrint(const_cast<char*>("[!] unable to take first process snapshot"));
        return FALSE;
    }

    if (_wcsicmp(pe.szExeFile, processname) == 0) // if pe.szExeFile and Processname are the same
    {
        CloseHandle(hSnapshot);
        processId = pe.th32ProcessID;
        isProcessFound = TRUE;
        #ifdef DEBUG
        cout << "[+] Got PID: " << processId << endl;
        #endif
    }

    /* End get first PID */

    /* Get the rest and process like the first */
    while (Process32Next(hSnapshot, &pe))
    {
        if (_wcsicmp(pe.szExeFile, processname) == 0)
        {
            DbgPrint(const_cast<char*>("[+] Closing handle to process snapshot"));
            CloseHandle(hSnapshot);
            processId = pe.th32ProcessID;
            DbgPrint(const_cast<char*>("[+] Found Process"));
            #ifdef DEBUG
            cout << "[+] Got PID: " << processId << endl;
            #endif
            break;
        }
    }
    DbgPrint(const_cast<char*>("[+] Done with process search\n\n"));
    //Check if process was found
    if (isProcessFound)
    {
        DbgPrint(const_cast<char*>("[!] failed to find process"));
        return FALSE;
    }


    /* this portion get it and puts it in the memory of the remote process */
    // get size of the dll's path
    auto size = wcslen(lpdllpath) * sizeof(TCHAR);

    // open selected process
    DbgPrint(const_cast<char*>("[+] Opening Process"));
    auto hVictimProcess = OpenProcess(PROCESS_ALL_ACCESS, 0, processId);
    if (hVictimProcess == NULL) // check if process open failed
    {
        DbgPrint(const_cast<char*>("[!]Failed to open process"));
        return FALSE;
    }
    DbgPrint(const_cast<char*>("[+] Open'd Process\n\n"));
    DbgPrint(const_cast<char*>("[+] Allocating some memory in the remote process"));
    // allocate memory in the remote process
    auto pNameInVictimProcess = VirtualAllocEx(hVictimProcess,
                                                    nullptr,
                                                        size,
                        MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if (pNameInVictimProcess == NULL) //Check if allocation failed
    {
        DbgPrint(const_cast<char*>("[!] allocation of memory failed"));
        return FALSE;
    }
    DbgPrint(const_cast<char*>("[+] allocated memory\n\n"));
    // write the DLL to memory
    DbgPrint(const_cast<char*>("[+] Writing to remote process mem"));
    auto bStatus = WriteProcessMemory(hVictimProcess,
                                pNameInVictimProcess,
                                           lpdllpath,
                                                size,
                                                nullptr);
    if (bStatus == 0)
    {
        DbgPrint(const_cast<char*>("[!] failed to write memory to the process"));
        return FALSE;
    }

    DbgPrint(const_cast<char*>("[+] Wrote remote process memory\n\n"));

    // gets a handle for kernel32dll's LoadLibrary call
    DbgPrint(const_cast<char*>("[+] Getting handle for kernel32"));
    auto hKernel32 = GetModuleHandle(L"kernel32.dll");
    if (hKernel32 == NULL)
    {
        DbgPrint(const_cast<char*>("[!] Unable to find Kernel32 in process"));
        return FALSE;
    }
    DbgPrint(const_cast<char*>("[+] Got kernel32 handle"));
    DbgPrint(const_cast<char*>("[+] Getting loadLibraryW handle"));
    auto LoadLibraryAddress = GetProcAddress(hKernel32, "LoadLibraryW");
    if (LoadLibraryAddress == NULL)
    {
        DbgPrint(const_cast<char*>("[-] Unable to find LoadLibraryW"));
        DbgPrint(const_cast<char*>("[-] Trying LoadLibraryA"));
        if ((LoadLibraryAddress = GetProcAddress(hKernel32, "LoadLibraryA")) == NULL)
        {
            DbgPrint(const_cast<char*>("[!] LoadLibraryA failed as well."));
            return FALSE;
        }
    }
    DbgPrint(const_cast<char*>("[+] Got loadLibrary handle\n\n"));

    // Using the above objects execute the DLL in the remote process
    DbgPrint(const_cast<char*>("[+] starting new thread to execute injected dll"));
    auto hThreadId = CreateRemoteThread(hVictimProcess,
        nullptr,
        0,
        reinterpret_cast<LPTHREAD_START_ROUTINE>(LoadLibraryAddress),
        pNameInVictimProcess,
        NULL,
        nullptr);
    if (hThreadId == NULL)
    {
       // printf("%s\n",GetLastError());
        DbgPrint(const_cast<char*>("[!] failed to create remote process"));
        return FALSE;
    }
    DbgPrint(const_cast<char*>("[+] started new thread\n\n"));

    /*if (bStatus == NULL)
        return FALSE;
        NOT NEEDED ANYMORE*/
    DbgPrint(const_cast<char*>("[+] waiting for thread to execute"));
    WaitForSingleObject(hThreadId, INFINITE);
    DbgPrint(const_cast<char*>("[+] Done!!!! Closing handle\n"));

    CloseHandle(hVictimProcess);
    DbgPrint(const_cast<char*>("[+] Closed process handle"));
    VirtualFreeEx(hVictimProcess, pNameInVictimProcess, size, MEM_RELEASE);

    DbgPrint(const_cast<char*>("Injected Successfully"));
    return TRUE;
}

BOOL HookInjection(TCHAR target[], TCHAR *dll_name)
{
    // https://msdn.microsoft.com/en-us/library/windows/desktop/ms644990(v=vs.85).aspx
    // SetWindowsHookEx can be used to inject a DLL into another process. A 32 - bit DLL cannot be injected into a 64 - bit process,
    // and a 64 - bit DLL cannot be injected into a 32 - bit process.If an application requires the use of hooks in other processes,
    // it is required that a 32 - bit application call SetWindowsHookEx to inject a 32 - bit DLL into 32 - bit processes,
    // and a 64 - bit application call SetWindowsHookEx to inject a 64 - bit DLL into 64 - bit processes.The 32 - bit and 64 - bit DLLs must have different names.
    DbgPrint(const_cast<char*>("[ ] loading module in local process"));
    auto hdll = LoadLibrary(dll_name);
    DbgPrint(const_cast<char*>("[+] loaded dll\n"));

    typedef LRESULT(WINAPI * MyProc)(int code, WPARAM wp, LPARAM lp); // export from calc_dll.dll

    auto mp = MyProc(GetProcAddress(hdll, "MyProc"));
    //auto mp = MyProc(GetProcAddress(hdll, "StartW"));


    auto pStartupInfo = new STARTUPINFO();
    auto pProcessInfo = new PROCESS_INFORMATION();
    DbgPrint(const_cast<char*>("[ ] creating process to hook"));
    CreateProcess(target,
        nullptr,
        nullptr,
        nullptr,
        FALSE,
        NORMAL_PRIORITY_CLASS,
        nullptr,
        nullptr,
        pStartupInfo,
        pProcessInfo);

    if (!pProcessInfo->hProcess)
    {
        DbgPrint(const_cast<char*>("[-] failed to create process"));
        return FALSE;
    }
    DbgPrint(const_cast<char*>("[+] Created hook process\n"));

    DbgPrint(const_cast<char*>("[ ] creating process hook"));
    auto hProc = SetWindowsHookEx(WH_CBT,	// Installs a hook procedure that receives notifications useful to a CBT application
        mp,									// my proc symbol taken from the dll
        hdll,								// dll containing my proc
        pProcessInfo->dwThreadId);			// dword to the thread (something something windows store) RTFM
    if (!hProc)
    {
        DbgPrint(const_cast<char*>("[-] failed to hook process"));
        printf("SetWindowsHookEx error=%d\n",GetLastError());
        return FALSE;
    }
    DbgPrint(const_cast<char*>("[+] hook injected"));
    UnhookWindowsHookEx(hProc);

    return TRUE;
}
