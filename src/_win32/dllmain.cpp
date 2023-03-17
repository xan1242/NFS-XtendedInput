/*
// clang-format off
// 
//    MIT License
//    Need for Speed (Black Box, MW & newer) - Xtended Input plugin
//    Bringing native XInput to NFS
//    
//    Copyright (c) 2022–2023 Lovro Plese (Xan/Tenjoin)
//    Copyright (c) 2023 Berkay Yigit <mail@berkay.link>
//
// clang-format on
*/

#include "stdafx.h"
#include "../Main.hpp"

BOOL APIENTRY DllMain(HMODULE, DWORD reason, LPVOID) {
  if (reason == DLL_PROCESS_ATTACH) {
#ifdef GAME_WORLD
    uintptr_t base = (uintptr_t)GetModuleHandleA(NULL);
    MainBase       = base - 0x400000;
#endif

    Init();
  }
  return TRUE;
}
