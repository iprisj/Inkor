#include <Windows.h>
#include <iostream>
#include <TlHelp32.h>

DWORD GetProcId(const char* procName)
{
    DWORD pID{};
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
    if (hSnapshot != INVALID_HANDLE_VALUE) {
        PROCESSENTRY32 pe;
        pe.dwSize = sizeof(pe);

        if (Process32First(hSnapshot, &pe)) {
            do {
                if (!strcmp(pe.szExeFile, procName))
                {
                    pID = pe.th32ProcessID;
                    break;
                }
            } while (Process32Next(hSnapshot, &pe));
        }
    }
    CloseHandle(hSnapshot);
    std::cout << pID << "\n";
    return pID;
}

bool RewriteFunc(LPVOID FUNC, LPVOID NEW) {
    DWORD oldprotect;
    VirtualProtect(FUNC, 1, PAGE_EXECUTE_READWRITE, &oldprotect);
    *reinterpret_cast<LPVOID*>(FUNC) = NEW;
    VirtualProtect(FUNC, 1, oldprotect, 0);
    return true;
}


int Reply() {
    return 5;
}

int HIjack() {
    return 2;
}






int main() {
    std::cout << "Starting..." << std::endl;
    const char* ProcName = "RobloxPlayerBeta.exe";
    const char* dllPath = "D:\\Documents\\Server\\GitRepos\\Inkor\\bin\Win32\\Debug\\DllSrc.dll";

    HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS, NULL, GetProcId(ProcName));
    if (hProc && hProc != INVALID_HANDLE_VALUE) {
        void* loc = VirtualAllocEx(hProc, 0, MAX_PATH, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
        WriteProcessMemory(hProc, loc, dllPath, strlen(dllPath) + 1, 0);
        HANDLE hThread = CreateRemoteThread(hProc, 0, 0, (LPTHREAD_START_ROUTINE)LoadLibraryA, loc, 0, 0);
        if (hThread)
        {
            std::cout << "Attached..." << std::endl;
            CloseHandle(hThread);
        }
    }
    std::cin.get();
}