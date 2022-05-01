#pragma once
#define GAMEFLOWMANAGER_STATUS_ADDR 0x00DA57B8
#define UTL_ILIST_CONSTRUCTOR_ADDR 0x005F6A00
#define PLAYER_ILISTABLE_ADDR 0x00DE99DC
#define PLAYER_GETHUD_VTABLE_OFFSET 0x20
#define FENGHUD_ISVISIBLE_VTABLE_OFFSET 0x28

// no cFEng stuff because FEng is used only for HUD in Undercover

#define GAME_HWND_ADDR 0x00DF1DF8

#define GLOBAL_FASTMEM_ADDR 0x00DA5320
#define FASTMEM_ALLOC_ADDR 0x005F6340
#define INIT_LIST_ALLOC_SIZE 0x28

#define WNDPROC_POINTER_ADDR 0x75630B

#define DINPUT_KILL_ADDR 0x0069A5B1

#define DEADZONE_FLOAT_POINTER_ADDR 0x69C8C7
float smallfloat = 0.000001;
#define SMALL_FLOAT_ADDR &smallfloat

#define INPUTMAPPING_CONSTRUCTOR_CALL_ADDR 0x0069D370
#define INPUTDEVICE_FACTORY_INITIALIZER_ADDR 0xBC221D


#define FENGHUD_JOYHANDLER_JMP_FROM 0x005CDA4A
#define FENGHUD_JOYHANDLER_JMP_TO 0x005CD81D

void(*InitProfileSettings)() = (void(*)())0x006774C0;