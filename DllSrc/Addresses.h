#pragma once
#include <Windows.h>

//805830
const uintptr_t getTaskScheduler_adr = 0xF2DEC0;
const uintptr_t print_adr = 0x424A30; //  0x405230     \xE8\x00\x00\x00\x00\x03\x75\xD0, x????xxx (code pattern)|||(ida pattern) E8 ? ? ? ? 03 75 D0