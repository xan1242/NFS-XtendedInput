/*
// clang-format off
// 
//    MIT License
//    Need for Speed (Black Box, MW & newer) - Xtended Input plugin
//    Bringing native XInput to NFS
//    
//    Copyright (c) 2022�2023 Lovro Plese (Xan/Tenjoin)
//    Copyright (c) 2023 Berkay Yigit <mail@berkay.link>
//
// clang-format on
*/

#ifndef XTENDEDINPUT_PCH_H
#define XTENDEDINPUT_PCH_H
#pragma once

#include "macros.h"
MSVC_WARNINGS_OFF

// Win32 targeting
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#define _USE_MATH_DEFINES
#include <WinSDKVer.h>
#define _WIN32_WINNT 0x0A00  // _WIN32_WINNT_WINTHRESHOLD
#include <SDKDDKVer.h>
// Win32
#include <Windows.h>

MSVC_WARNINGS_RESTORE

inline float clampf(const float& val, const float& min, const float& max) { return val < min ? min : val > max ? max : val; }

#endif