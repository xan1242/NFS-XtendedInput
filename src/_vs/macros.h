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

#ifndef XTENDEDINPUT_MACROS_H
#define XTENDEDINPUT_MACROS_H
#pragma once

#if defined(_MSC_VER)

#define MSVC_WARNINGS_OFF                      \
  __pragma(warning(push, 0)) __pragma(warning( \
      disable : 4244 4265 4267 4350 4472 4509 4548 4623 4710 4985 6001 6011 6255 6285 6387 6031 6320 6258 4755 4625 4626 4702 26812 26439 26451 26495 26819 26498))
#define MSVC_WARNINGS_RESTORE __pragma(warning(pop))

#define MSVC_RUNTIME_CHECKS_OFF     __pragma(runtime_checks("", off)) __pragma(check_stack(off)) __pragma(strict_gs_check(push, off))
#define MSVC_RUNTIME_CHECKS_RESTORE __pragma(runtime_checks("", restore)) __pragma(check_stack()) __pragma(strict_gs_check(pop))

#else  // defined(_MSC_VER)

#define MSVC_WARNINGS_OFF
#define MSVC_WARNINGS_RESTORE
#define MSVC_RUNTIME_CHECKS_OFF
#define MSVC_RUNTIME_CHECKS_RESTORE

#endif  // defined(_MSC_VER)

#endif