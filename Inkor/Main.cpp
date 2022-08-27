#include <Windows.h>
#include <iostream>
#include <TlHelp32.h>
#include <filesystem>
namespace fs = std::filesystem;

std::string GetCurrentDirectory()
{
    char buffer[MAX_PATH];
    GetModuleFileNameA(NULL, buffer, MAX_PATH);
    std::string::size_type pos = std::string(buffer).find_last_of("\\/");

    return std::string(buffer).substr(0, pos);
}

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
    std::string Path = fs::current_path().string() + "\\DllSrc.dll";
    const char* DllPath = Path.c_str();
    HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS, NULL, GetProcId(ProcName));
    if (hProc && hProc != INVALID_HANDLE_VALUE) {
        LPVOID loc = VirtualAllocEx(hProc, 0, MAX_PATH, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
        WriteProcessMemory(hProc, loc, DllPath, strlen(DllPath) + 1, 0);
        HANDLE hThread = CreateRemoteThread(hProc, 0, 0, (LPTHREAD_START_ROUTINE)LoadLibraryA, loc, 0, 0);
        if (hThread)
        {
            std::cout << "Attached..." << std::endl;
            CloseHandle(hThread);
        }
    }
    std::cin.get();
}