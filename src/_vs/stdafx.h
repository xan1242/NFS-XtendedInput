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
#define _WIN32_WINNT 0x0601  // _WIN32_WINNT_WIN7
#include <SDKDDKVer.h>
// Win32
#include <Windows.h>

MSVC_WARNINGS_RESTORE

#endif