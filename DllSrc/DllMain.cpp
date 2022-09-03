#include <Windows.h>
#include <iostream>
#include "Console.h"
#include "Depen.h"
#include "Addresses.h"

Console console;

int main() {

    MessageBoxA(NULL, "prisj", "OKAY", MB_OK);

    console.AllocateConsole();
    console.InitBypasses();
    console.LinkConsole();
    //RewriteFunc((BYTE*)FreeConsole, (BYTE)"0xC3");
    //ReplaceFunc(IsDebuggerPresent, rep, 5);

    const uintptr_t current_module = reinterpret_cast<uintptr_t>(GetModuleHandle(NULL));

    using rbx_print_t = void(__cdecl*)(int method, const char* value);
    rbx_print_t rbx_print = reinterpret_cast<rbx_print_t>(current_module + print_adr);

    using rbx_getTaskScheduler_t = uintptr_t(__cdecl*)();
    rbx_getTaskScheduler_t getTaskScheduler = reinterpret_cast<rbx_getTaskScheduler_t>(current_module + getTaskScheduler_adr);

    const uintptr_t TaskScheduler = getTaskScheduler();


    // ADDRESS INFO
    std::cout << "[ADDR]: Task Scheduler: " << std::hex << TaskScheduler << std::dec << "\n";
    std::cout << "[ADDR]: Console Print: " << std::hex << rbx_print << std::dec << "\n";
    //


    std::string input;
    while (true) {
        std::cin >> input;
        rbx_print(2, input.c_str());
    }
    
}

BOOL APIENTRY DllMain(HMODULE hmodule, DWORD reason, void* reserved) {
    if (reason == DLL_PROCESS_ATTACH) {
        MessageBoxA(NULL, "dll load", "OKAY", MB_OK);
        CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)main, NULL, NULL, NULL);
    }
    return 1;
}