/*
// clang-format off
//
//    PepegaScript (for Pepega Mod)
//    Copyright (C) 2023 Berkay Yigit <mail@berkay.link>
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU Affero General Public License as published
//    by the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
//    GNU Affero General Public License for more details.
//
//    You should have received a copy of the GNU Affero General Public License
//    along with this program. If not, see <https://www.gnu.org/licenses/>.
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