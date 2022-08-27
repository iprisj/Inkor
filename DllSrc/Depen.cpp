#include <Windows.h>
#include "Depen.h"

bool RewriteFunc(BYTE* FUNC, BYTE NEW) {
	DWORD oldprotect;
	VirtualProtect(FUNC, 1, PAGE_EXECUTE_READWRITE, &oldprotect);
	*reinterpret_cast<byte*>(FUNC) = NEW;
	VirtualProtect(FUNC, 1, oldprotect, 0);
	return true;
}

bool ReplaceFunc(void* src, void* dst, int len) {
    if (len < 5) return false;
    DWORD oldProtect;
    VirtualProtect(src, len, PAGE_EXECUTE_READWRITE, &oldProtect);
    memset(src, 0x90, len);
    uintptr_t relativeAddy = ((uintptr_t)dst - (uintptr_t)src) - 5;
    *(BYTE*)src = 0xE9;
    *(uintptr_t*)((uintptr_t)src + 1) = relativeAddy;
    DWORD temp;
    VirtualProtect(src, len, oldProtect, &temp);
    return true;
}