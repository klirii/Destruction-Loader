#pragma once

#define DISABLE_OUTPUT
#define DLL_VIMEWORLD_ATTACH 0x888

#include <Windows.h>
#include <iostream>
#include <fstream>
#include <TlHelp32.h>
#include <stdio.h>
#include <string>

#pragma warning(disable:6387)

using f_LoadLibraryA = HINSTANCE(WINAPI*)(const char* lpLibFilename);
using f_GetProcAddress = FARPROC(WINAPI*)(HMODULE hModule, LPCSTR lpProcName);
using f_DLL_ENTRY_POINT = BOOL(WINAPI*)(void* hDll, DWORD dwReason, void* pReserved);

#if defined(DISABLE_OUTPUT)
#define ILog(data, ...)
#else
#define ILog(text, ...) printf(text, __VA_ARGS__);
#endif

#ifdef _WIN64
using f_RtlAddFunctionTable = BOOL(WINAPIV*)(PRUNTIME_FUNCTION FunctionTable, DWORD EntryCount, DWORD64 BaseAddress);
#endif

#ifdef _WIN64
#define CURRENT_ARCH IMAGE_FILE_MACHINE_AMD64
#else
#define CURRENT_ARCH IMAGE_FILE_MACHINE_I386
#endif

struct MANUAL_MAPPING_DATA
{
	f_LoadLibraryA pLoadLibraryA;
	f_GetProcAddress pGetProcAddress;
#ifdef _WIN64
	f_RtlAddFunctionTable pRtlAddFunctionTable;
#endif
	BYTE* pbase;
	HINSTANCE hMod;
	DWORD fdwReasonParam;
	LPVOID reservedParam;
	BOOL SEHSupport;
};

void __stdcall Shellcode(MANUAL_MAPPING_DATA* pData);
HANDLE GetProcessHandleFromHwnd(HWND hwnd);

bool __forceinline Inject(DWORD pID, const char* path) {
    HANDLE proc_handle;
    LPVOID RemoteString;
    LPCVOID LoadLibAddy;

    if (pID == 0) return false;
    proc_handle = OpenProcess(PROCESS_ALL_ACCESS, false, pID);
    if (proc_handle == 0) return false;
    
    LoadLibAddy = GetProcAddress(GetModuleHandleA("kernel32.dll"), "LoadLibraryA");
    RemoteString = VirtualAllocEx(proc_handle, NULL, strlen(path), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    WriteProcessMemory(proc_handle, RemoteString, path, strlen(path), NULL);

    CreateRemoteThread(proc_handle, NULL, NULL, (LPTHREAD_START_ROUTINE)LoadLibAddy, RemoteString, NULL, NULL);
    CloseHandle(proc_handle);
    return true;
}

//Note: Exception support only x64 with build params /EHa or /EHc
bool __forceinline ManualMapDll(HANDLE hProc, BYTE* pSrcData, SIZE_T FileSize, bool ClearHeader = true, bool ClearNonNeededSections = true, bool AdjustProtections = true, bool SEHExceptionSupport = true, DWORD fdwReason = DLL_PROCESS_ATTACH, LPVOID lpReserved = 0) {
    IMAGE_NT_HEADERS* pOldNtHeader = nullptr;
    IMAGE_OPTIONAL_HEADER* pOldOptHeader = nullptr;
    IMAGE_FILE_HEADER* pOldFileHeader = nullptr;
    BYTE* pTargetBase = nullptr;

    if (reinterpret_cast<IMAGE_DOS_HEADER*>(pSrcData)->e_magic != 0x5A4D) { //"MZ"

        ILog("Invalid file\n");
        CloseHandle(hProc);
        return false;
    }

    pOldNtHeader = reinterpret_cast<IMAGE_NT_HEADERS*>(pSrcData + reinterpret_cast<IMAGE_DOS_HEADER*>(pSrcData)->e_lfanew);
    pOldOptHeader = &pOldNtHeader->OptionalHeader;
    pOldFileHeader = &pOldNtHeader->FileHeader;

    if (pOldFileHeader->Machine != CURRENT_ARCH) {
        ILog("Invalid platform\n");
        CloseHandle(hProc);
        return false;
    }

    ILog("File ok\n");

    pTargetBase = reinterpret_cast<BYTE*>(VirtualAllocEx(hProc, nullptr, pOldOptHeader->SizeOfImage, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE));
    if (!pTargetBase) {
        ILog("Target process memory allocation failed (ex) 0x%X\n", GetLastError());
        CloseHandle(hProc);
        return false;
    }

    DWORD oldp = 0;
    VirtualProtectEx(hProc, pTargetBase, pOldOptHeader->SizeOfImage, PAGE_EXECUTE_READWRITE, &oldp);

    MANUAL_MAPPING_DATA data{ 0 };
    data.pLoadLibraryA = LoadLibraryA;
    data.pGetProcAddress = GetProcAddress;
#ifdef _WIN64
    data.pRtlAddFunctionTable = (f_RtlAddFunctionTable)RtlAddFunctionTable;
#else 
    SEHExceptionSupport = false;
#endif
    data.pbase = pTargetBase;
    data.fdwReasonParam = fdwReason;
    data.reservedParam = lpReserved;
    data.SEHSupport = SEHExceptionSupport;


    //File header
    if (!WriteProcessMemory(hProc, pTargetBase, pSrcData, 0x1000, nullptr)) { //only first 0x1000 bytes for the header
        ILog("Can't write file header 0x%X\n", GetLastError());
        VirtualFreeEx(hProc, pTargetBase, 0, MEM_RELEASE);
        CloseHandle(hProc);
        return false;
    }

    IMAGE_SECTION_HEADER* pSectionHeader = IMAGE_FIRST_SECTION(pOldNtHeader);
    for (UINT i = 0; i != pOldFileHeader->NumberOfSections; ++i, ++pSectionHeader) {
        if (pSectionHeader->SizeOfRawData) {
            if (!WriteProcessMemory(hProc, pTargetBase + pSectionHeader->VirtualAddress, pSrcData + pSectionHeader->PointerToRawData, pSectionHeader->SizeOfRawData, nullptr)) {
                ILog("Can't map sections: 0x%x\n", GetLastError());
                VirtualFreeEx(hProc, pTargetBase, 0, MEM_RELEASE);
                CloseHandle(hProc);
                return false;
            }
        }
    }

    //Mapping params
    BYTE* MappingDataAlloc = reinterpret_cast<BYTE*>(VirtualAllocEx(hProc, nullptr, sizeof(MANUAL_MAPPING_DATA), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE));
    if (!MappingDataAlloc) {
        ILog("Target process mapping allocation failed (ex) 0x%X\n", GetLastError());
        VirtualFreeEx(hProc, pTargetBase, 0, MEM_RELEASE);
        CloseHandle(hProc);
        return false;
    }

    if (!WriteProcessMemory(hProc, MappingDataAlloc, &data, sizeof(MANUAL_MAPPING_DATA), nullptr)) {
        ILog("Can't write mapping 0x%X\n", GetLastError());
        VirtualFreeEx(hProc, pTargetBase, 0, MEM_RELEASE);
        VirtualFreeEx(hProc, MappingDataAlloc, 0, MEM_RELEASE);
        CloseHandle(hProc);
        return false;
    }

    //Shell code
    void* pShellcode = VirtualAllocEx(hProc, nullptr, 0x1000, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    if (!pShellcode) {
        ILog("Memory shellcode allocation failed (ex) 0x%X\n", GetLastError());
        VirtualFreeEx(hProc, pTargetBase, 0, MEM_RELEASE);
        VirtualFreeEx(hProc, MappingDataAlloc, 0, MEM_RELEASE);
        CloseHandle(hProc);
        return false;
    }

    if (!WriteProcessMemory(hProc, pShellcode, Shellcode, 0x1000, nullptr)) {
        ILog("Can't write shellcode 0x%X\n", GetLastError());
        VirtualFreeEx(hProc, pTargetBase, 0, MEM_RELEASE);
        VirtualFreeEx(hProc, MappingDataAlloc, 0, MEM_RELEASE);
        VirtualFreeEx(hProc, pShellcode, 0, MEM_RELEASE);
        CloseHandle(hProc);
        return false;
    }

    ILog("Mapped DLL at %p\n", pTargetBase);
    ILog("Mapping info at %p\n", MappingDataAlloc);
    ILog("Shell code at %p\n", pShellcode);

    ILog("Data allocated\n");

#ifdef _DEBUG
    ILog("My shellcode pointer %p\n", Shellcode);
    ILog("Target point %p\n", pShellcode);
    system("pause");
#endif

    HANDLE hThread = CreateRemoteThread(hProc, nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(pShellcode), MappingDataAlloc, 0, nullptr);
    if (!hThread) {
        ILog("Thread creation failed 0x%X\n", GetLastError());
        VirtualFreeEx(hProc, pTargetBase, 0, MEM_RELEASE);
        VirtualFreeEx(hProc, MappingDataAlloc, 0, MEM_RELEASE);
        VirtualFreeEx(hProc, pShellcode, 0, MEM_RELEASE);
        CloseHandle(hProc);
        return false;
    }
    CloseHandle(hThread);

    ILog("Thread created at: %p, waiting for return...\n", pShellcode);

    HINSTANCE hCheck = NULL;
    while (!hCheck) {
        DWORD exitcode = 0;
        GetExitCodeProcess(hProc, &exitcode);
        if (exitcode != STILL_ACTIVE) {
            ILog("Process crashed, exit code: %d\n", exitcode);
            CloseHandle(hProc);
            return false;
        }

        MANUAL_MAPPING_DATA data_checked{ 0 };
        ReadProcessMemory(hProc, MappingDataAlloc, &data_checked, sizeof(data_checked), nullptr);
        hCheck = data_checked.hMod;

        if (hCheck == (HINSTANCE)0x404040) {
            ILog("Wrong mapping ptr\n");
            VirtualFreeEx(hProc, pTargetBase, 0, MEM_RELEASE);
            VirtualFreeEx(hProc, MappingDataAlloc, 0, MEM_RELEASE);
            VirtualFreeEx(hProc, pShellcode, 0, MEM_RELEASE);
            CloseHandle(hProc);
            return false;
        }
        else if (hCheck == (HINSTANCE)0x505050) {
            ILog("WARNING: Exception support failed!\n");
        }

        Sleep(10);
    }

    BYTE* emptyBuffer = (BYTE*)malloc(1024 * 1024 * 20);
    if (emptyBuffer == nullptr) {
        ILog("Unable to allocate memory\n");
        CloseHandle(hProc);
        return false;
    }
    memset(emptyBuffer, 0, 1024 * 1024 * 20);

    //CLEAR PE HEAD
    if (ClearHeader) {
        if (!WriteProcessMemory(hProc, pTargetBase, emptyBuffer, 0x1000, nullptr)) {
            ILog("WARNING!: Can't clear HEADER\n");
        }
    }
    //END CLEAR PE HEAD


    if (ClearNonNeededSections) {
        pSectionHeader = IMAGE_FIRST_SECTION(pOldNtHeader);
        for (UINT i = 0; i != pOldFileHeader->NumberOfSections; ++i, ++pSectionHeader) {
            if (pSectionHeader->Misc.VirtualSize) {
                if ((SEHExceptionSupport ? 0 : strcmp((char*)pSectionHeader->Name, ".pdata") == 0) ||
                    strcmp((char*)pSectionHeader->Name, ".rsrc") == 0 ||
                    strcmp((char*)pSectionHeader->Name, ".reloc") == 0) {
                    ILog("Processing %s removal\n", pSectionHeader->Name);
                    if (!WriteProcessMemory(hProc, pTargetBase + pSectionHeader->VirtualAddress, emptyBuffer, pSectionHeader->Misc.VirtualSize, nullptr)) {
                        ILog("Can't clear section %s: 0x%x\n", pSectionHeader->Name, GetLastError());
                    }
                }
            }
        }
    }

    if (AdjustProtections) {
        pSectionHeader = IMAGE_FIRST_SECTION(pOldNtHeader);
        for (UINT i = 0; i != pOldFileHeader->NumberOfSections; ++i, ++pSectionHeader) {
            if (pSectionHeader->Misc.VirtualSize) {
                DWORD old = 0;
                DWORD newP = PAGE_READONLY;

                if ((pSectionHeader->Characteristics & IMAGE_SCN_MEM_WRITE) > 0) {
                    newP = PAGE_READWRITE;
                }
                else if ((pSectionHeader->Characteristics & IMAGE_SCN_MEM_EXECUTE) > 0) {
                    newP = PAGE_EXECUTE_READ;
                }
                if (VirtualProtectEx(hProc, pTargetBase + pSectionHeader->VirtualAddress, pSectionHeader->Misc.VirtualSize, newP, &old)) {
                    ILog("section %s set as %lX\n", (char*)pSectionHeader->Name, newP);
                }
                else {
                    ILog("FAIL: section %s not set as %lX\n", (char*)pSectionHeader->Name, newP);
                }
            }
        }
        DWORD old = 0;
        VirtualProtectEx(hProc, pTargetBase, IMAGE_FIRST_SECTION(pOldNtHeader)->VirtualAddress, PAGE_READONLY, &old);
    }

    if (!WriteProcessMemory(hProc, pShellcode, emptyBuffer, 0x1000, nullptr)) {
        ILog("WARNING: Can't clear shellcode\n");
    }
    if (!VirtualFreeEx(hProc, pShellcode, 0, MEM_RELEASE)) {
        ILog("WARNING: can't release shell code memory\n");
    }
    if (!VirtualFreeEx(hProc, MappingDataAlloc, 0, MEM_RELEASE)) {
        ILog("WARNING: can't release mapping data memory\n");
    }

    CloseHandle(hProc);
    return true;
}
