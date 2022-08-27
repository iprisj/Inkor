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
    const uintptr_t rbx_print_address = current_module + print_adr; //  0x405230     \xE8\x00\x00\x00\x00\x03\x75\xD0, x????xxx (code pattern)|||(ida pattern) E8 ? ? ? ? 03 75 D0
    using rbx_print_t = void(__cdecl*)(int method, const char* value);
    rbx_print_t rbx_print = reinterpret_cast<rbx_print_t>(rbx_print_address);
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