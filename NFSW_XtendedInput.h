#pragma once
unsigned int MainBase;

#define NO_FENG
#define NO_PROFILE_SETTINGS

#define GAMEFLOWMANAGER_STATUS_ADDR (0x00CFC730 + MainBase)
#define UTL_ILIST_CONSTRUCTOR_ADDR (0x00753940 + MainBase)

#define GAME_HWND_ADDR (0x00D06504 + MainBase)

// memory allocators work differently in World
//#define GLOBAL_FASTMEM_ADDR 0xC8178C + MainBase
//#define FASTMEM_ALLOC_ADDR 0x0060BA70 + MainBase
#define MANAGED_ALLOCATOR_ADDR (0x006D4660 + MainBase)
#define INIT_LIST_ALLOC_SIZE 0x28

#define DINPUT_KILL_ADDR (0x0075C16B + MainBase)

#define DEADZONE_FLOAT_POINTER_ADDR (0x75E26D + MainBase)
float smallfloat = 0.000001;
#define SMALL_FLOAT_ADDR &smallfloat

#define INPUTMAPPING_CONSTRUCTOR_CALL_ADDR (0x0075EB43 + MainBase)
#define INPUTDEVICE_FACTORY_INITIALIZER_ADDR (0xB10F9D + MainBase)

//void(*InitProfileSettings)() = (void(*)())(0x756960 + MainBase);
