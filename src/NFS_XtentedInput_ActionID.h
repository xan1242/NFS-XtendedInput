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

#pragma once
#include <string.h>

unsigned int sub_5CC090(char* a1, int a2, unsigned int a3) {
  unsigned int v4;   // eax
  unsigned int v5;   // ecx
  unsigned int v6;   // ebp
  int          v7;   // edi
  unsigned int v9;   // edx
  unsigned int v10;  // ecx
  unsigned int v11;  // esi
  int          v12;  // eax
  unsigned int v13;  // ecx
  unsigned int v14;  // esi
  int          v15;  // eax
  unsigned int v16;  // ecx
  unsigned int v17;  // esi
  unsigned int v18;  // esi
  int          v19;  // edi
  unsigned int v20;  // ecx
  unsigned int v21;  // esi
  int          v22;  // edi
  unsigned int v23;  // ecx
  unsigned int v24;  // esi
  int          v25;  // edi

  v4 = a3;
  v5 = 0x9E3779B9;
  v6 = a3;
  v7 = 0x9E3779B9;
  if (a3 >= 0xC) {
    v9 = a3 / 0xC;
    do {
      v10 = *((int*)a1 + 1) + v5;
      v11 = *((int*)a1 + 2) + a2;
      v12 = (v11 >> 13) ^ (v7 + *(int*)a1 - v11 - v10);
      v13 = (v12 << 8) ^ (v10 - v11 - v12);
      v14 = (v13 >> 13) ^ (v11 - v13 - v12);
      v15 = (v14 >> 12) ^ (v12 - v14 - v13);
      v16 = (v15 << 16) ^ (v13 - v14 - v15);
      v17 = (v16 >> 5) ^ (v14 - v16 - v15);
      v7  = (v17 >> 3) ^ (v15 - v17 - v16);
      v5  = (v7 << 10) ^ (v16 - v17 - v7);
      a2  = (v5 >> 15) ^ (v17 - v5 - v7);
      a1 += 12;
      v6 -= 12;
      --v9;
    } while (v9);
    v4 = a3;
  }
  v18 = v4 + a2;
  switch (v6) {
    case 11:
      v18 += a1[10] << 24;
    case 10:
      v18 += a1[9] << 16;
    case 9:
      v18 += a1[8] << 8;
    case 8:
      v5 += a1[7] << 24;
    case 7:
      v5 += a1[6] << 16;
    case 6:
      v5 += a1[5] << 8;
    case 5:
      v5 += a1[4];
    case 4:
      v7 += a1[3] << 24;
    case 3:
      v7 += a1[2] << 16;
    case 2:
      v7 += a1[1] << 8;
    case 1:
      v7 += *a1;
      break;
    default:
      break;
  }

  v19 = (v18 >> 13) ^ (v7 - v18 - v5);
  v20 = (v19 << 8) ^ (v5 - v18 - v19);
  v21 = (v20 >> 13) ^ (v18 - v20 - v19);
  v22 = (v21 >> 12) ^ (v19 - v21 - v20);
  v23 = (v22 << 16) ^ (v20 - v21 - v22);
  v24 = (v23 >> 5) ^ (v21 - v23 - v22);
  v25 = (v24 >> 3) ^ (v22 - v24 - v23);
  return (((v25 << 10) ^ (v23 - v24 - v25)) >> 15) ^ (v24 - ((v25 << 10) ^ (v23 - v24 - v25)) - v25);
}

unsigned int stringhash32(char* a1) {
  unsigned int result;  // eax

  if (a1 && *a1)
    result = sub_5CC090(a1, 0xABCDEF00, strlen(a1));
  else
    result = 0;
  return result;
}

#ifdef GAME_MW
enum ActionID {
  NULL_ACTION                   = 0,
  GAMEACTION_STEERLEFT          = 1,
  GAMEACTION_STEERRIGHT         = 2,
  GAMEACTION_TURNLEFT           = 3,
  GAMEACTION_TURNRIGHT          = 4,
  GAMEACTION_GAS                = 5,
  GAMEACTION_BRAKE              = 6,
  GAMEACTION_HANDBRAKE          = 7,
  GAMEACTION_GAMEBREAKER        = 8,
  GAMEACTION_SHIFTUP            = 9,
  GAMEACTION_SHIFTDOWN          = 10,
  GAMEACTION_NOS                = 11,
  GAMEACTION_RESET              = 12,
  GAMEACTION_FORWARD            = 13,
  GAMEACTION_BACK               = 14,
  GAMEACTION_JUMP               = 15,
  GAMEACTION_DEBUGHUMAN1        = 16,
  GAMEACTION_DEBUGHUMAN2        = 17,
  GAMEACTION_DEBUGHUMAN3        = 18,
  GAMEACTION_DEBUGHUMAN4        = 19,
  CAMERAACTION_CHANGE           = 20,
  CAMERAACTION_DEBUG            = 21,
  CAMERAACTION_ENABLE_ICE       = 22,
  CAMERAACTION_LOOKBACK         = 23,
  CAMERAACTION_PULLBACK         = 24,
  FRONTENDACTION_UP             = 25,
  FRONTENDACTION_DOWN           = 26,
  FRONTENDACTION_LEFT           = 27,
  FRONTENDACTION_RIGHT          = 28,
  FRONTENDACTION_RUP            = 29,
  FRONTENDACTION_RDOWN          = 30,
  FRONTENDACTION_RLEFT          = 31,
  FRONTENDACTION_RRIGHT         = 32,
  FRONTENDACTION_ACCEPT         = 33,
  FRONTENDACTION_CANCEL         = 34,
  FRONTENDACTION_CANCEL_ALT     = 35,
  FRONTENDACTION_START          = 36,
  FRONTENDACTION_BUTTON0        = 37,
  FRONTENDACTION_BUTTON1        = 38,
  FRONTENDACTION_BUTTON2        = 39,
  FRONTENDACTION_BUTTON3        = 40,
  FRONTENDACTION_BUTTON4        = 41,
  FRONTENDACTION_BUTTON5        = 42,
  FRONTENDACTION_LTRIGGER       = 43,
  FRONTENDACTION_RTRIGGER       = 44,
  HUDACTION_PAUSEREQUEST        = 45,
  HUDACTION_ENGAGE_EVENT        = 46,
  HUDACTION_PAD_LEFT            = 47,
  HUDACTION_PAD_DOWN            = 48,
  HUDACTION_PAD_RIGHT           = 49,
  HUDACTION_SKIPNIS             = 50,
  HUDACTION_NEXTSONG            = 51,
  VOIPACTION_PUSHTOTALK         = 52,
  DEBUGACTION_MOVE_UP           = 53,
  DEBUGACTION_MOVE_DOWN         = 54,
  DEBUGACTION_MOVE_LEFT         = 55,
  DEBUGACTION_MOVE_RIGHT        = 56,
  DEBUGACTION_MOVE_FORWARD      = 57,
  DEBUGACTION_MOVE_BACK         = 58,
  DEBUGACTION_MOVE_D_LEFT       = 59,
  DEBUGACTION_MOVE_D_RIGHT      = 60,
  DEBUGACTION_MOVE_D_FORWARD    = 61,
  DEBUGACTION_MOVE_D_BACK       = 62,
  DEBUGACTION_LOOK_UP           = 63,
  DEBUGACTION_LOOK_DOWN         = 64,
  DEBUGACTION_LOOK_LEFT         = 65,
  DEBUGACTION_LOOK_RIGHT        = 66,
  DEBUGACTION_LOOK_D_UP         = 67,
  DEBUGACTION_LOOK_D_DOWN       = 68,
  DEBUGACTION_LOOK_D_LEFT       = 69,
  DEBUGACTION_LOOK_D_RIGHT      = 70,
  DEBUGACTION_TURBO             = 71,
  DEBUGACTION_SUPER_TURBO       = 72,
  DEBUGACTION_DROPCAR           = 73,
  DEBUGACTION_TOGGLEAI          = 74,
  DEBUGACTION_TOGGLESIMSTEP     = 75,
  DEBUGACTION_SIMSTEP           = 76,
  DEBUGACTION_SCREENSHOT        = 77,
  DEBUGACTION_STOPRECORDPATH    = 78,
  DEBUGACTION_TOGGLECARCOLOUR   = 79,
  DEBUGACTION_TOGGLEDEMOCAMERAS = 80,
  ICEACTION_GRAB_LEFT           = 81,
  ICEACTION_GRAB_RIGHT          = 82,
  ICEACTION_LEFT                = 83,
  ICEACTION_RIGHT               = 84,
  ICEACTION_FAST_LEFT           = 85,
  ICEACTION_FAST_RIGHT          = 86,
  ICEACTION_UP                  = 87,
  ICEACTION_DOWN                = 88,
  ICEACTION_HELP                = 89,
  ICEACTION_UNDO                = 90,
  ICEACTION_PLAY                = 91,
  ICEACTION_HIDE                = 92,
  ICEACTION_SCREENSHOT          = 93,
  ICEACTION_SCREENSHOT_STREAM   = 94,
  ICEACTION_INSERT              = 95,
  ICEACTION_CUT                 = 96,
  ICEACTION_COPY                = 97,
  ICEACTION_PASTE               = 98,
  ICEACTION_SELECT              = 99,
  ICEACTION_CANCEL              = 100,
  ICEACTION_ALT_1               = 101,
  ICEACTION_CLIP_IN             = 102,
  ICEACTION_CLIP_OUT            = 103,
  ICEACTION_SIMSPEED_INC        = 104,
  ICEACTION_SIMSPEED_DEC        = 105,
  ICEACTION_DUTCH_LEFT          = 106,
  ICEACTION_DUTCH_RIGHT         = 107,
  ICEACTION_ZOOM_IN             = 108,
  ICEACTION_ZOOM_OUT            = 109,
  ICEACTION_LETTERBOX_IN        = 110,
  ICEACTION_LETTERBOX_OUT       = 111,
  ICEACTION_BUBBLE_IN           = 112,
  ICEACTION_BUBBLE_OUT          = 113,
  ICEACTION_BUBBLE_UP           = 114,
  ICEACTION_BUBBLE_DOWN         = 115,
  ICEACTION_BUBBLE_TILT_UP      = 116,
  ICEACTION_BUBBLE_TILT_DOWN    = 117,
  ICEACTION_BUBBLE_SPIN_LEFT    = 118,
  ICEACTION_BUBBLE_SPIN_RIGHT   = 119,
  ICEACTION_BUBBLE_LEFT         = 120,
  ICEACTION_BUBBLE_RIGHT        = 121,
  ICEACTION_BUBBLE_BACK         = 122,
  ICEACTION_BUBBLE_FORTH        = 123,
  ICEACTION_SHAKE_MAG_DEC       = 124,
  ICEACTION_SHAKE_MAG_INC       = 125,
  ICEACTION_SHAKE_FRQ_DEC       = 126,
  ICEACTION_SHAKE_FRQ_INC       = 127,
  ICEACTION_SHUTTLE_LEFT        = 128,
  ICEACTION_SHUTTLE_RIGHT       = 129,
  ICEACTION_SHUTTLE_UP          = 130,
  ICEACTION_SHUTTLE_DOWN        = 131,
  ICEACTION_SHUTTLE_SPEED_DEC   = 132,
  ICEACTION_SHUTTLE_SPEED_INC   = 133,
  ACTION_EXITAPPLICATION        = 134,
  ACTION_PLUGGED                = 135,
  ACTION_UNPLUGGED              = 136,
  ACTION_FLUSH                  = 137,
  MAX_ACTIONID                  = 138,
};

char* ActionIDStr[139] = {"NULL_ACTION",
                          "GAMEACTION_STEERLEFT",
                          "GAMEACTION_STEERRIGHT",
                          "GAMEACTION_TURNLEFT",
                          "GAMEACTION_TURNRIGHT",
                          "GAMEACTION_GAS",
                          "GAMEACTION_BRAKE",
                          "GAMEACTION_HANDBRAKE",
                          "GAMEACTION_GAMEBREAKER",
                          "GAMEACTION_SHIFTUP",
                          "GAMEACTION_SHIFTDOWN",
                          "GAMEACTION_NOS",
                          "GAMEACTION_RESET",
                          "GAMEACTION_FORWARD",
                          "GAMEACTION_BACK",
                          "GAMEACTION_JUMP",
                          "GAMEACTION_DEBUGHUMAN1",
                          "GAMEACTION_DEBUGHUMAN2",
                          "GAMEACTION_DEBUGHUMAN3",
                          "GAMEACTION_DEBUGHUMAN4",
                          "CAMERAACTION_CHANGE",
                          "CAMERAACTION_DEBUG",
                          "CAMERAACTION_ENABLE_ICE",
                          "CAMERAACTION_LOOKBACK",
                          "CAMERAACTION_PULLBACK",
                          "FRONTENDACTION_UP",
                          "FRONTENDACTION_DOWN",
                          "FRONTENDACTION_LEFT",
                          "FRONTENDACTION_RIGHT",
                          "FRONTENDACTION_RUP",
                          "FRONTENDACTION_RDOWN",
                          "FRONTENDACTION_RLEFT",
                          "FRONTENDACTION_RRIGHT",
                          "FRONTENDACTION_ACCEPT",
                          "FRONTENDACTION_CANCEL",
                          "FRONTENDACTION_CANCEL_ALT",
                          "FRONTENDACTION_START",
                          "FRONTENDACTION_BUTTON0",
                          "FRONTENDACTION_BUTTON1",
                          "FRONTENDACTION_BUTTON2",
                          "FRONTENDACTION_BUTTON3",
                          "FRONTENDACTION_BUTTON4",
                          "FRONTENDACTION_BUTTON5",
                          "FRONTENDACTION_LTRIGGER",
                          "FRONTENDACTION_RTRIGGER",
                          "HUDACTION_PAUSEREQUEST",
                          "HUDACTION_ENGAGE_EVENT",
                          "HUDACTION_PAD_LEFT",
                          "HUDACTION_PAD_DOWN",
                          "HUDACTION_PAD_RIGHT",
                          "HUDACTION_SKIPNIS",
                          "HUDACTION_NEXTSONG",
                          "VOIPACTION_PUSHTOTALK",
                          "DEBUGACTION_MOVE_UP",
                          "DEBUGACTION_MOVE_DOWN",
                          "DEBUGACTION_MOVE_LEFT",
                          "DEBUGACTION_MOVE_RIGHT",
                          "DEBUGACTION_MOVE_FORWARD",
                          "DEBUGACTION_MOVE_BACK",
                          "DEBUGACTION_MOVE_D_LEFT",
                          "DEBUGACTION_MOVE_D_RIGHT",
                          "DEBUGACTION_MOVE_D_FORWARD",
                          "DEBUGACTION_MOVE_D_BACK",
                          "DEBUGACTION_LOOK_UP",
                          "DEBUGACTION_LOOK_DOWN",
                          "DEBUGACTION_LOOK_LEFT",
                          "DEBUGACTION_LOOK_RIGHT",
                          "DEBUGACTION_LOOK_D_UP",
                          "DEBUGACTION_LOOK_D_DOWN",
                          "DEBUGACTION_LOOK_D_LEFT",
                          "DEBUGACTION_LOOK_D_RIGHT",
                          "DEBUGACTION_TURBO",
                          "DEBUGACTION_SUPER_TURBO",
                          "DEBUGACTION_DROPCAR",
                          "DEBUGACTION_TOGGLEAI",
                          "DEBUGACTION_TOGGLESIMSTEP",
                          "DEBUGACTION_SIMSTEP",
                          "DEBUGACTION_SCREENSHOT",
                          "DEBUGACTION_STOPRECORDPATH",
                          "DEBUGACTION_TOGGLECARCOLOUR",
                          "DEBUGACTION_TOGGLEDEMOCAMERAS",
                          "ICEACTION_GRAB_LEFT",
                          "ICEACTION_GRAB_RIGHT",
                          "ICEACTION_LEFT",
                          "ICEACTION_RIGHT",
                          "ICEACTION_FAST_LEFT",
                          "ICEACTION_FAST_RIGHT",
                          "ICEACTION_UP",
                          "ICEACTION_DOWN",
                          "ICEACTION_HELP",
                          "ICEACTION_UNDO",
                          "ICEACTION_PLAY",
                          "ICEACTION_HIDE",
                          "ICEACTION_SCREENSHOT",
                          "ICEACTION_SCREENSHOT_STREAM",
                          "ICEACTION_INSERT",
                          "ICEACTION_CUT",
                          "ICEACTION_COPY",
                          "ICEACTION_PASTE",
                          "ICEACTION_SELECT",
                          "ICEACTION_CANCEL",
                          "ICEACTION_ALT_1",
                          "ICEACTION_CLIP_IN",
                          "ICEACTION_CLIP_OUT",
                          "ICEACTION_SIMSPEED_INC",
                          "ICEACTION_SIMSPEED_DEC",
                          "ICEACTION_DUTCH_LEFT",
                          "ICEACTION_DUTCH_RIGHT",
                          "ICEACTION_ZOOM_IN",
                          "ICEACTION_ZOOM_OUT",
                          "ICEACTION_LETTERBOX_IN",
                          "ICEACTION_LETTERBOX_OUT",
                          "ICEACTION_BUBBLE_IN",
                          "ICEACTION_BUBBLE_OUT",
                          "ICEACTION_BUBBLE_UP",
                          "ICEACTION_BUBBLE_DOWN",
                          "ICEACTION_BUBBLE_TILT_UP",
                          "ICEACTION_BUBBLE_TILT_DOWN",
                          "ICEACTION_BUBBLE_SPIN_LEFT",
                          "ICEACTION_BUBBLE_SPIN_RIGHT",
                          "ICEACTION_BUBBLE_LEFT",
                          "ICEACTION_BUBBLE_RIGHT",
                          "ICEACTION_BUBBLE_BACK",
                          "ICEACTION_BUBBLE_FORTH",
                          "ICEACTION_SHAKE_MAG_DEC",
                          "ICEACTION_SHAKE_MAG_INC",
                          "ICEACTION_SHAKE_FRQ_DEC",
                          "ICEACTION_SHAKE_FRQ_INC",
                          "ICEACTION_SHUTTLE_LEFT",
                          "ICEACTION_SHUTTLE_RIGHT",
                          "ICEACTION_SHUTTLE_UP",
                          "ICEACTION_SHUTTLE_DOWN",
                          "ICEACTION_SHUTTLE_SPEED_DEC",
                          "ICEACTION_SHUTTLE_SPEED_INC",
                          "ACTION_EXITAPPLICATION",
                          "ACTION_PLUGGED",
                          "ACTION_UNPLUGGED",
                          "ACTION_FLUSH",
                          "MAX_ACTIONID"};

#endif

#ifdef GAME_CARBON
enum ActionID {
  NULL_ACTION                   = 0,
  GAMEACTION_STEERLEFT          = 1,
  GAMEACTION_STEERRIGHT         = 2,
  GAMEACTION_OVERSTEER          = 3,
  GAMEACTION_OVERSTEERLEFT      = 4,
  GAMEACTION_OVERSTEERRIGHT     = 5,
  GAMEACTION_TURNLEFT           = 6,
  GAMEACTION_TURNRIGHT          = 7,
  GAMEACTION_GAS                = 8,
  GAMEACTION_BRAKE              = 9,
  GAMEACTION_HANDBRAKE          = 10,
  GAMEACTION_GAMEBREAKER        = 11,
  GAMEACTION_SHIFTUP            = 12,
  GAMEACTION_SHIFTDOWN          = 13,
  GAMEACTION_SHIFTFIRST         = 14,
  GAMEACTION_SHIFTSECOND        = 15,
  GAMEACTION_SHIFTTHIRD         = 16,
  GAMEACTION_SHIFTFOURTH        = 17,
  GAMEACTION_SHIFTFIFTH         = 18,
  GAMEACTION_SHIFTSIXTH         = 19,
  GAMEACTION_SHIFTNEUTRAL       = 20,
  GAMEACTION_SHIFTREVERSE       = 21,
  GAMEACTION_NOS                = 22,
  GAMEACTION_RESET              = 23,
  GAMEACTION_CREWAGGRESSIVE     = 24,
  GAMEACTION_CREWDEFENSIVE      = 25,
  GAMEACTION_CREWDEFAULT        = 26,
  GAMEACTION_CREWSPEED          = 27,
  GAMEACTION_FORWARD            = 28,
  GAMEACTION_BACK               = 29,
  GAMEACTION_JUMP               = 30,
  GAMEACTION_DEBUGHUMAN1        = 31,
  GAMEACTION_DEBUGHUMAN2        = 32,
  GAMEACTION_DEBUGHUMAN3        = 33,
  GAMEACTION_DEBUGHUMAN4        = 34,
  GAMEACTION_CLUTCH             = 35,
  CAMERAACTION_CHANGE           = 36,
  CAMERAACTION_DEBUG            = 37,
  CAMERAACTION_ENABLE_ICE       = 38,
  CAMERAACTION_LOOKBACK         = 39,
  CAMERAACTION_PULLBACK         = 40,
  FRONTENDACTION_UP             = 41,
  FRONTENDACTION_DOWN           = 42,
  FRONTENDACTION_LEFT           = 43,
  FRONTENDACTION_RIGHT          = 44,
  FRONTENDACTION_RUP            = 45,
  FRONTENDACTION_RDOWN          = 46,
  FRONTENDACTION_RLEFT          = 47,
  FRONTENDACTION_RRIGHT         = 48,
  FRONTENDACTION_ACCEPT         = 49,
  FRONTENDACTION_CANCEL         = 50,
  FRONTENDACTION_CANCEL_ALT     = 51,
  FRONTENDACTION_START          = 52,
  FRONTENDACTION_BUTTON0        = 53,
  FRONTENDACTION_BUTTON1        = 54,
  FRONTENDACTION_BUTTON2        = 55,
  FRONTENDACTION_BUTTON3        = 56,
  FRONTENDACTION_BUTTON4        = 57,
  FRONTENDACTION_BUTTON5        = 58,
  FRONTENDACTION_LTRIGGER       = 59,
  FRONTENDACTION_RTRIGGER       = 60,
  HUDACTION_PAUSEREQUEST        = 61,
  HUDACTION_ENGAGE_EVENT        = 62,
  HUDACTION_PAD_LEFT            = 63,
  HUDACTION_PAD_DOWN            = 64,
  HUDACTION_PAD_RIGHT           = 65,
  HUDACTION_SKIPNIS             = 66,
  HUDACTION_NEXTSONG            = 67,
  HUDACTION_VOIPACTIVATION      = 68,
  HUDACTION_VOIPDEACTIVATION    = 69,
  VOIPACTION_PUSHTOTALK         = 70,
  DEBUGACTION_MOVE_UP           = 71,
  DEBUGACTION_MOVE_DOWN         = 72,
  DEBUGACTION_MOVE_LEFT         = 73,
  DEBUGACTION_MOVE_RIGHT        = 74,
  DEBUGACTION_MOVE_FORWARD      = 75,
  DEBUGACTION_MOVE_BACK         = 76,
  DEBUGACTION_MOVE_D_LEFT       = 77,
  DEBUGACTION_MOVE_D_RIGHT      = 78,
  DEBUGACTION_MOVE_D_FORWARD    = 79,
  DEBUGACTION_MOVE_D_BACK       = 80,
  DEBUGACTION_LOOK_UP           = 81,
  DEBUGACTION_LOOK_DOWN         = 82,
  DEBUGACTION_LOOK_LEFT         = 83,
  DEBUGACTION_LOOK_RIGHT        = 84,
  DEBUGACTION_LOOK_D_UP         = 85,
  DEBUGACTION_LOOK_D_DOWN       = 86,
  DEBUGACTION_LOOK_D_LEFT       = 87,
  DEBUGACTION_LOOK_D_RIGHT      = 88,
  DEBUGACTION_TURBO             = 89,
  DEBUGACTION_SUPER_TURBO       = 90,
  DEBUGACTION_DROPCAR           = 91,
  DEBUGACTION_TOGGLEAI          = 92,
  DEBUGACTION_TOGGLESIMSTEP     = 93,
  DEBUGACTION_SIMSTEP           = 94,
  DEBUGACTION_SCREENSHOT        = 95,
  DEBUGACTION_STOPRECORDPATH    = 96,
  DEBUGACTION_TOGGLECARCOLOUR   = 97,
  DEBUGACTION_TOGGLEDEMOCAMERAS = 98,
  ACTION_EXITAPPLICATION        = 136,
  ACTION_PLUGGED                = 137,
  ACTION_UNPLUGGED              = 138,
  ACTION_FLUSH                  = 139,
  MAX_ACTIONID
};

char* ActionIDStr[MAX_ACTIONID] = {"NULL_ACTION",
                                   "GAMEACTION_STEERLEFT",
                                   "GAMEACTION_STEERRIGHT",
                                   "GAMEACTION_OVERSTEER",
                                   "GAMEACTION_OVERSTEERLEFT",
                                   "GAMEACTION_OVERSTEERRIGHT",
                                   "GAMEACTION_TURNLEFT",
                                   "GAMEACTION_TURNRIGHT",
                                   "GAMEACTION_GAS",
                                   "GAMEACTION_BRAKE",
                                   "GAMEACTION_HANDBRAKE",
                                   "GAMEACTION_GAMEBREAKER",
                                   "GAMEACTION_SHIFTUP",
                                   "GAMEACTION_SHIFTDOWN",
                                   "GAMEACTION_SHIFTFIRST",
                                   "GAMEACTION_SHIFTSECOND",
                                   "GAMEACTION_SHIFTTHIRD",
                                   "GAMEACTION_SHIFTFOURTH",
                                   "GAMEACTION_SHIFTFIFTH",
                                   "GAMEACTION_SHIFTSIXTH",
                                   "GAMEACTION_SHIFTNEUTRAL",
                                   "GAMEACTION_SHIFTREVERSE",
                                   "GAMEACTION_NOS",
                                   "GAMEACTION_RESET",
                                   "GAMEACTION_CREWAGGRESSIVE",
                                   "GAMEACTION_CREWDEFENSIVE",
                                   "GAMEACTION_CREWDEFAULT",
                                   "GAMEACTION_CREWSPEED",
                                   "GAMEACTION_FORWARD",
                                   "GAMEACTION_BACK",
                                   "GAMEACTION_JUMP",
                                   "GAMEACTION_DEBUGHUMAN1",
                                   "GAMEACTION_DEBUGHUMAN2",
                                   "GAMEACTION_DEBUGHUMAN3",
                                   "GAMEACTION_DEBUGHUMAN4",
                                   "GAMEACTION_CLUTCH",
                                   "CAMERAACTION_CHANGE",
                                   "CAMERAACTION_DEBUG",
                                   "CAMERAACTION_ENABLE_ICE",
                                   "CAMERAACTION_LOOKBACK",
                                   "CAMERAACTION_PULLBACK",
                                   "FRONTENDACTION_UP",
                                   "FRONTENDACTION_DOWN",
                                   "FRONTENDACTION_LEFT",
                                   "FRONTENDACTION_RIGHT",
                                   "FRONTENDACTION_RUP",
                                   "FRONTENDACTION_RDOWN",
                                   "FRONTENDACTION_RLEFT",
                                   "FRONTENDACTION_RRIGHT",
                                   "FRONTENDACTION_ACCEPT",
                                   "FRONTENDACTION_CANCEL",
                                   "FRONTENDACTION_CANCEL_ALT",
                                   "FRONTENDACTION_START",
                                   "FRONTENDACTION_BUTTON0",
                                   "FRONTENDACTION_BUTTON1",
                                   "FRONTENDACTION_BUTTON2",
                                   "FRONTENDACTION_BUTTON3",
                                   "FRONTENDACTION_BUTTON4",
                                   "FRONTENDACTION_BUTTON5",
                                   "FRONTENDACTION_LTRIGGER",
                                   "FRONTENDACTION_RTRIGGER",
                                   "HUDACTION_PAUSEREQUEST",
                                   "HUDACTION_ENGAGE_EVENT",
                                   "HUDACTION_PAD_LEFT",
                                   "HUDACTION_PAD_DOWN",
                                   "HUDACTION_PAD_RIGHT",
                                   "HUDACTION_SKIPNIS",
                                   "HUDACTION_NEXTSONG",
                                   "HUDACTION_VOIPACTIVATION",
                                   "HUDACTION_VOIPDEACTIVATION",
                                   "VOIPACTION_PUSHTOTALK",
                                   "DEBUGACTION_MOVE_UP",
                                   "DEBUGACTION_MOVE_DOWN",
                                   "DEBUGACTION_MOVE_LEFT",
                                   "DEBUGACTION_MOVE_RIGHT",
                                   "DEBUGACTION_MOVE_FORWARD",
                                   "DEBUGACTION_MOVE_BACK",
                                   "DEBUGACTION_MOVE_D_LEFT",
                                   "DEBUGACTION_MOVE_D_RIGHT",
                                   "DEBUGACTION_MOVE_D_FORWARD",
                                   "DEBUGACTION_MOVE_D_BACK",
                                   "DEBUGACTION_LOOK_UP",
                                   "DEBUGACTION_LOOK_DOWN",
                                   "DEBUGACTION_LOOK_LEFT",
                                   "DEBUGACTION_LOOK_RIGHT",
                                   "DEBUGACTION_LOOK_D_UP",
                                   "DEBUGACTION_LOOK_D_DOWN",
                                   "DEBUGACTION_LOOK_D_LEFT",
                                   "DEBUGACTION_LOOK_D_RIGHT",
                                   "DEBUGACTION_TURBO",
                                   "DEBUGACTION_SUPER_TURBO",
                                   "DEBUGACTION_DROPCAR",
                                   "DEBUGACTION_TOGGLEAI",
                                   "DEBUGACTION_TOGGLESIMSTEP",
                                   "DEBUGACTION_SIMSTEP",
                                   "DEBUGACTION_SCREENSHOT",
                                   "DEBUGACTION_STOPRECORDPATH",
                                   "DEBUGACTION_TOGGLECARCOLOUR",
                                   "DEBUGACTION_TOGGLEDEMOCAMERAS",
                                   "unknown",
                                   "unknown",
                                   "unknown",
                                   "unknown",
                                   "unknown",
                                   "unknown",
                                   "unknown",
                                   "unknown",
                                   "unknown",
                                   "unknown",
                                   "unknown",
                                   "unknown",
                                   "unknown",
                                   "unknown",
                                   "unknown",
                                   "unknown",
                                   "unknown",
                                   "unknown",
                                   "unknown",
                                   "unknown",
                                   "unknown",
                                   "unknown",
                                   "unknown",
                                   "unknown",
                                   "unknown",
                                   "unknown",
                                   "unknown",
                                   "unknown",
                                   "unknown",
                                   "unknown",
                                   "unknown",
                                   "unknown",
                                   "unknown",
                                   "unknown",
                                   "unknown",
                                   "unknown",
                                   "unknown",
                                   "ACTION_EXITAPPLICATION",
                                   "ACTION_PLUGGED",
                                   "ACTION_UNPLUGGED",
                                   "ACTION_FLUSH"};
#endif

#ifdef GAME_PROSTREET
enum ActionID {
  NULL_ACTION                   = 0,
  GAMEACTION_STEERLEFT          = 1,
  GAMEACTION_STEERRIGHT         = 2,
  GAMEACTION_OVERSTEER          = 3,
  GAMEACTION_OVERSTEERLEFT      = 4,
  GAMEACTION_OVERSTEERRIGHT     = 5,
  GAMEACTION_TURNLEFT           = 6,
  GAMEACTION_TURNRIGHT          = 7,
  GAMEACTION_GAS                = 8,
  GAMEACTION_GAS_SLOTCAR        = 9,
  GAMEACTION_BRAKE              = 10,
  GAMEACTION_HANDBRAKE          = 11,
  GAMEACTION_GAMEBREAKER        = 12,
  GAMEACTION_SHIFTUP            = 13,
  GAMEACTION_SHIFTDOWN          = 14,
  GAMEACTION_SHIFTFIRST         = 15,
  GAMEACTION_SHIFTSECOND        = 16,
  GAMEACTION_SHIFTTHIRD         = 17,
  GAMEACTION_SHIFTFOURTH        = 18,
  GAMEACTION_SHIFTFIFTH         = 19,
  GAMEACTION_SHIFTSIXTH         = 20,
  GAMEACTION_SHIFTNEUTRAL       = 21,
  GAMEACTION_SHIFTREVERSE       = 22,
  GAMEACTION_NOS                = 23,
  GAMEACTION_RESET              = 24,
  GAMEACTION_CREWAGGRESSIVE     = 25,
  GAMEACTION_CREWDEFENSIVE      = 26,
  GAMEACTION_CREWDEFAULT        = 27,
  GAMEACTION_CREWSPEED          = 28,
  GAMEACTION_FORWARD            = 29,
  GAMEACTION_BACK               = 30,
  GAMEACTION_JUMP               = 31,
  GAMEACTION_DEBUGHUMAN1        = 32,
  GAMEACTION_DEBUGHUMAN2        = 33,
  GAMEACTION_DEBUGHUMAN3        = 34,
  GAMEACTION_DEBUGHUMAN4        = 35,
  GAMEACTION_CLUTCH             = 36,
  GAMEACTION_CLUTCH_KICK        = 37,
  CAMERAACTION_CHANGE           = 38,
  CAMERAACTION_DEBUG            = 39,
  CAMERAACTION_ENABLE_ICE       = 40,
  CAMERAACTION_LOOKBACK         = 41,
  CAMERAACTION_PULLBACK         = 42,
  CAMERAACTION_ROT_LEFT         = 43,
  CAMERAACTION_ROT_RIGHT        = 44,
  CAMERAACTION_360              = 45,
  FRONTENDACTION_UP             = 46,
  FRONTENDACTION_DOWN           = 47,
  FRONTENDACTION_LEFT           = 48,
  FRONTENDACTION_RIGHT          = 49,
  FRONTENDACTION_RUP            = 50,
  FRONTENDACTION_RDOWN          = 51,
  FRONTENDACTION_RLEFT          = 52,
  FRONTENDACTION_RRIGHT         = 53,
  FRONTENDACTION_ACCEPT         = 54,
  FRONTENDACTION_CANCEL         = 55,
  FRONTENDACTION_CANCEL_ALT     = 56,
  FRONTENDACTION_START          = 57,
  FRONTENDACTION_BUTTON0        = 58,
  FRONTENDACTION_BUTTON1        = 59,
  FRONTENDACTION_BUTTON2        = 60,
  FRONTENDACTION_BUTTON3        = 61,
  FRONTENDACTION_BUTTON4        = 62,
  FRONTENDACTION_BUTTON5        = 63,
  FRONTENDACTION_LTRIGGER       = 64,
  FRONTENDACTION_RTRIGGER       = 65,
  HUDACTION_PAUSEREQUEST        = 66,
  HUDACTION_ENGAGE_EVENT        = 67,
  HUDACTION_PAD_LEFT            = 68,
  HUDACTION_PAD_DOWN            = 69,
  HUDACTION_PAD_RIGHT           = 70,
  HUDACTION_SKIPNIS             = 71,
  HUDACTION_NEXTSONG            = 72,
  HUDACTION_VOIPACTIVATION      = 73,
  HUDACTION_VOIPDEACTIVATION    = 74,
  HUDACTION_TOGGLE_LEADERBOARD  = 75,
  HUDACTION_TOGGLE_MINIMAP      = 76,
  HUDACTION_TOGGLE_BESTLINE     = 77,
  HUDACTION_TOGGLE_ARROWS       = 78,
  VOIPACTION_PUSHTOTALK         = 79,
  DEBUGACTION_MOVE_UP           = 80,
  DEBUGACTION_MOVE_DOWN         = 81,
  DEBUGACTION_MOVE_LEFT         = 82,
  DEBUGACTION_MOVE_RIGHT        = 83,
  DEBUGACTION_MOVE_FORWARD      = 84,
  DEBUGACTION_MOVE_BACK         = 85,
  DEBUGACTION_MOVE_D_LEFT       = 86,
  DEBUGACTION_MOVE_D_RIGHT      = 87,
  DEBUGACTION_MOVE_D_FORWARD    = 88,
  DEBUGACTION_MOVE_D_BACK       = 89,
  DEBUGACTION_LOOK_UP           = 90,
  DEBUGACTION_LOOK_DOWN         = 91,
  DEBUGACTION_LOOK_LEFT         = 92,
  DEBUGACTION_LOOK_RIGHT        = 93,
  DEBUGACTION_LOOK_D_UP         = 94,
  DEBUGACTION_LOOK_D_DOWN       = 95,
  DEBUGACTION_LOOK_D_LEFT       = 96,
  DEBUGACTION_LOOK_D_RIGHT      = 97,
  DEBUGACTION_TURBO             = 98,
  DEBUGACTION_SUPER_TURBO       = 99,
  DEBUGACTION_DROPCAR           = 100,
  DEBUGACTION_TOGGLEAI          = 101,
  DEBUGACTION_TOGGLESIMSTEP     = 102,
  DEBUGACTION_SIMSTEP           = 103,
  DEBUGACTION_SCREENSHOT        = 104,
  DEBUGACTION_STOPRECORDPATH    = 105,
  DEBUGACTION_TOGGLECARCOLOUR   = 106,
  DEBUGACTION_TOGGLEDEMOCAMERAS = 107,
  ICE_ACTION_START              = 108,
  ICE_ACTION_SELECT             = 109,
  ICE_ACTION_SOUTH              = 110,
  ICE_ACTION_SOUTH_PRESS        = 111,
  ICE_ACTION_NORTH              = 112,
  ICE_ACTION_NORTH_PRESS        = 113,
  ICE_ACTION_WEST               = 114,
  ICE_ACTION_WEST_PRESS         = 115,
  ICE_ACTION_EAST               = 116,
  ICE_ACTION_EAST_PRESS         = 117,
  ICE_ACTION_DOWN               = 118,
  ICE_ACTION_DOWN_PRESS         = 119,
  ICE_ACTION_UP                 = 120,
  ICE_ACTION_UP_PRESS           = 121,
  ICE_ACTION_LEFT               = 122,
  ICE_ACTION_LEFT_PRESS         = 123,
  ICE_ACTION_RIGHT              = 124,
  ICE_ACTION_RIGHT_PRESS        = 125,
  ICE_ACTION_L1                 = 126,
  ICE_ACTION_L1_PRESS           = 127,
  ICE_ACTION_R1                 = 128,
  ICE_ACTION_R1_PRESS           = 129,
  ICE_ACTION_L2                 = 130,
  ICE_ACTION_L2_PRESS           = 131,
  ICE_ACTION_R2                 = 132,
  ICE_ACTION_R2_PRESS           = 133,
  ICE_ACTION_L3                 = 134,
  ICE_ACTION_L3_PRESS           = 135,
  ICE_ACTION_R3                 = 136,
  ICE_ACTION_R3_RELEASE         = 137,
  ICE_ACTION_LEFTSTICK_LEFT     = 138,
  ICE_ACTION_LEFTSTICK_RIGHT    = 139,
  ICE_ACTION_LEFTSTICK_DOWN     = 140,
  ICE_ACTION_LEFTSTICK_UP       = 141,
  ICE_ACTION_RIGHTSTICK_LEFT    = 142,
  ICE_ACTION_RIGHTSTICK_RIGHT   = 143,
  ICE_ACTION_RIGHTSTICK_DOWN    = 144,
  ICE_ACTION_RIGHTSTICK_UP      = 145,
  ACTION_EXITAPPLICATION        = 146,
  ACTION_PLUGGED                = 147,
  ACTION_UNPLUGGED              = 148,
  ACTION_FLUSH                  = 149,
  MAX_ACTIONID                  = 150,
};

char* ActionIDStr[MAX_ACTIONID] = {"NULL_ACTION",
                                   "GAMEACTION_STEERLEFT",
                                   "GAMEACTION_STEERRIGHT",
                                   "GAMEACTION_OVERSTEER",
                                   "GAMEACTION_OVERSTEERLEFT",
                                   "GAMEACTION_OVERSTEERRIGHT",
                                   "GAMEACTION_TURNLEFT",
                                   "GAMEACTION_TURNRIGHT",
                                   "GAMEACTION_GAS",
                                   "GAMEACTION_GAS_SLOTCAR",
                                   "GAMEACTION_BRAKE",
                                   "GAMEACTION_HANDBRAKE",
                                   "GAMEACTION_GAMEBREAKER",
                                   "GAMEACTION_SHIFTUP",
                                   "GAMEACTION_SHIFTDOWN",
                                   "GAMEACTION_SHIFTFIRST",
                                   "GAMEACTION_SHIFTSECOND",
                                   "GAMEACTION_SHIFTTHIRD",
                                   "GAMEACTION_SHIFTFOURTH",
                                   "GAMEACTION_SHIFTFIFTH",
                                   "GAMEACTION_SHIFTSIXTH",
                                   "GAMEACTION_SHIFTNEUTRAL",
                                   "GAMEACTION_SHIFTREVERSE",
                                   "GAMEACTION_NOS",
                                   "GAMEACTION_RESET",
                                   "GAMEACTION_CREWAGGRESSIVE",
                                   "GAMEACTION_CREWDEFENSIVE",
                                   "GAMEACTION_CREWDEFAULT",
                                   "GAMEACTION_CREWSPEED",
                                   "GAMEACTION_FORWARD",
                                   "GAMEACTION_BACK",
                                   "GAMEACTION_JUMP",
                                   "GAMEACTION_DEBUGHUMAN1",
                                   "GAMEACTION_DEBUGHUMAN2",
                                   "GAMEACTION_DEBUGHUMAN3",
                                   "GAMEACTION_DEBUGHUMAN4",
                                   "GAMEACTION_CLUTCH",
                                   "GAMEACTION_CLUTCH_KICK",
                                   "CAMERAACTION_CHANGE",
                                   "CAMERAACTION_DEBUG",
                                   "CAMERAACTION_ENABLE_ICE",
                                   "CAMERAACTION_LOOKBACK",
                                   "CAMERAACTION_PULLBACK",
                                   "CAMERAACTION_ROT_LEFT",
                                   "CAMERAACTION_ROT_RIGHT",
                                   "CAMERAACTION_360",
                                   "FRONTENDACTION_UP",
                                   "FRONTENDACTION_DOWN",
                                   "FRONTENDACTION_LEFT",
                                   "FRONTENDACTION_RIGHT",
                                   "FRONTENDACTION_RUP",
                                   "FRONTENDACTION_RDOWN",
                                   "FRONTENDACTION_RLEFT",
                                   "FRONTENDACTION_RRIGHT",
                                   "FRONTENDACTION_ACCEPT",
                                   "FRONTENDACTION_CANCEL",
                                   "FRONTENDACTION_CANCEL_ALT",
                                   "FRONTENDACTION_START",
                                   "FRONTENDACTION_BUTTON0",
                                   "FRONTENDACTION_BUTTON1",
                                   "FRONTENDACTION_BUTTON2",
                                   "FRONTENDACTION_BUTTON3",
                                   "FRONTENDACTION_BUTTON4",
                                   "FRONTENDACTION_BUTTON5",
                                   "FRONTENDACTION_LTRIGGER",
                                   "FRONTENDACTION_RTRIGGER",
                                   "HUDACTION_PAUSEREQUEST",
                                   "HUDACTION_ENGAGE_EVENT",
                                   "HUDACTION_PAD_LEFT",
                                   "HUDACTION_PAD_DOWN",
                                   "HUDACTION_PAD_RIGHT",
                                   "HUDACTION_SKIPNIS",
                                   "HUDACTION_NEXTSONG",
                                   "HUDACTION_VOIPACTIVATION",
                                   "HUDACTION_VOIPDEACTIVATION",
                                   "HUDACTION_TOGGLE_LEADERBOARD",
                                   "HUDACTION_TOGGLE_MINIMAP",
                                   "HUDACTION_TOGGLE_BESTLINE",
                                   "HUDACTION_TOGGLE_ARROWS",
                                   "VOIPACTION_PUSHTOTALK",
                                   "DEBUGACTION_MOVE_UP",
                                   "DEBUGACTION_MOVE_DOWN",
                                   "DEBUGACTION_MOVE_LEFT",
                                   "DEBUGACTION_MOVE_RIGHT",
                                   "DEBUGACTION_MOVE_FORWARD",
                                   "DEBUGACTION_MOVE_BACK",
                                   "DEBUGACTION_MOVE_D_LEFT",
                                   "DEBUGACTION_MOVE_D_RIGHT",
                                   "DEBUGACTION_MOVE_D_FORWARD",
                                   "DEBUGACTION_MOVE_D_BACK",
                                   "DEBUGACTION_LOOK_UP",
                                   "DEBUGACTION_LOOK_DOWN",
                                   "DEBUGACTION_LOOK_LEFT",
                                   "DEBUGACTION_LOOK_RIGHT",
                                   "DEBUGACTION_LOOK_D_UP",
                                   "DEBUGACTION_LOOK_D_DOWN",
                                   "DEBUGACTION_LOOK_D_LEFT",
                                   "DEBUGACTION_LOOK_D_RIGHT",
                                   "DEBUGACTION_TURBO",
                                   "DEBUGACTION_SUPER_TURBO",
                                   "DEBUGACTION_DROPCAR",
                                   "DEBUGACTION_TOGGLEAI",
                                   "DEBUGACTION_TOGGLESIMSTEP",
                                   "DEBUGACTION_SIMSTEP",
                                   "DEBUGACTION_SCREENSHOT",
                                   "DEBUGACTION_STOPRECORDPATH",
                                   "DEBUGACTION_TOGGLECARCOLOUR",
                                   "DEBUGACTION_TOGGLEDEMOCAMERAS",
                                   "ICE_ACTION_START",
                                   "ICE_ACTION_SELECT",
                                   "ICE_ACTION_SOUTH",
                                   "ICE_ACTION_SOUTH_PRESS",
                                   "ICE_ACTION_NORTH",
                                   "ICE_ACTION_NORTH_PRESS",
                                   "ICE_ACTION_WEST",
                                   "ICE_ACTION_WEST_PRESS",
                                   "ICE_ACTION_EAST",
                                   "ICE_ACTION_EAST_PRESS",
                                   "ICE_ACTION_DOWN",
                                   "ICE_ACTION_DOWN_PRESS",
                                   "ICE_ACTION_UP",
                                   "ICE_ACTION_UP_PRESS",
                                   "ICE_ACTION_LEFT",
                                   "ICE_ACTION_LEFT_PRESS",
                                   "ICE_ACTION_RIGHT",
                                   "ICE_ACTION_RIGHT_PRESS",
                                   "ICE_ACTION_L1",
                                   "ICE_ACTION_L1_PRESS",
                                   "ICE_ACTION_R1",
                                   "ICE_ACTION_R1_PRESS",
                                   "ICE_ACTION_L2",
                                   "ICE_ACTION_L2_PRESS",
                                   "ICE_ACTION_R2",
                                   "ICE_ACTION_R2_PRESS",
                                   "ICE_ACTION_L3",
                                   "ICE_ACTION_L3_PRESS",
                                   "ICE_ACTION_R3",
                                   "ICE_ACTION_R3_RELEASE",
                                   "ICE_ACTION_LEFTSTICK_LEFT",
                                   "ICE_ACTION_LEFTSTICK_RIGHT",
                                   "ICE_ACTION_LEFTSTICK_DOWN",
                                   "ICE_ACTION_LEFTSTICK_UP",
                                   "ICE_ACTION_RIGHTSTICK_LEFT",
                                   "ICE_ACTION_RIGHTSTICK_RIGHT",
                                   "ICE_ACTION_RIGHTSTICK_DOWN",
                                   "ICE_ACTION_RIGHTSTICK_UP",
                                   "ACTION_EXITAPPLICATION",
                                   "ACTION_PLUGGED",
                                   "ACTION_UNPLUGGED",
                                   "ACTION_FLUSH"};
#endif

#ifdef GAME_UC
enum ActionID {
  NULL_ACTION                   = 0,
  GAMEACTION_STEERLEFT          = 1,
  GAMEACTION_STEERRIGHT         = 2,
  GAMEACTION_OVERSTEER          = 3,
  GAMEACTION_OVERSTEERLEFT      = 4,
  GAMEACTION_OVERSTEERRIGHT     = 5,
  GAMEACTION_TURNLEFT           = 6,
  GAMEACTION_TURNRIGHT          = 7,
  GAMEACTION_GAS                = 8,
  GAMEACTION_GAS_SLOTCAR        = 9,
  GAMEACTION_BRAKE              = 10,
  GAMEACTION_HANDBRAKE          = 11,
  GAMEACTION_GAMEBREAKER        = 12,
  GAMEACTION_SHIFTUP            = 13,
  GAMEACTION_SHIFTDOWN          = 14,
  GAMEACTION_SHIFTFIRST         = 15,
  GAMEACTION_SHIFTSECOND        = 16,
  GAMEACTION_SHIFTTHIRD         = 17,
  GAMEACTION_SHIFTFOURTH        = 18,
  GAMEACTION_SHIFTFIFTH         = 19,
  GAMEACTION_SHIFTSIXTH         = 20,
  GAMEACTION_SHIFTNEUTRAL       = 21,
  GAMEACTION_SHIFTREVERSE       = 22,
  GAMEACTION_NOS                = 23,
  GAMEACTION_RESET              = 24,
  GAMEACTION_HYDRAULICALL       = 25,
  GAMEACTION_HYDRAULICFRONT     = 26,
  GAMEACTION_HYDRAULICLEFT      = 27,
  GAMEACTION_HYDRAULICREAR      = 28,
  GAMEACTION_HYDRAULICRIGHT     = 29,
  GAMEACTION_CREWAGGRESSIVE     = 30,
  GAMEACTION_CREWDEFENSIVE      = 31,
  GAMEACTION_CREWDEFAULT        = 32,
  GAMEACTION_CREWSPEED          = 33,
  GAMEACTION_FORWARD            = 34,
  GAMEACTION_BACK               = 35,
  GAMEACTION_JUMP               = 36,
  GAMEACTION_DEBUGHUMAN1        = 37,
  GAMEACTION_DEBUGHUMAN2        = 38,
  GAMEACTION_DEBUGHUMAN3        = 39,
  GAMEACTION_DEBUGHUMAN4        = 40,
  GAMEACTION_CLUTCH             = 41,
  GAMEACTION_CLUTCH_KICK        = 42,
  CAMERAACTION_CHANGE           = 43,
  CAMERAACTION_DEBUG            = 44,
  CAMERAACTION_ENABLE_ICE       = 45,
  CAMERAACTION_LOOKBACK         = 46,
  CAMERAACTION_PULLBACK         = 47,
  CAMERAACTION_ROT_LEFT         = 48,
  CAMERAACTION_ROT_RIGHT        = 49,
  CAMERAACTION_360              = 50,
  FRONTENDACTION_UP             = 51,
  FRONTENDACTION_DOWN           = 52,
  FRONTENDACTION_LEFT           = 53,
  FRONTENDACTION_RIGHT          = 54,
  FRONTENDACTION_RUP            = 55,
  FRONTENDACTION_RDOWN          = 56,
  FRONTENDACTION_RLEFT          = 57,
  FRONTENDACTION_RRIGHT         = 58,
  FRONTENDACTION_ACCEPT         = 59,
  FRONTENDACTION_CANCEL         = 60,
  FRONTENDACTION_CANCEL_ALT     = 61,
  FRONTENDACTION_START          = 62,
  FRONTENDACTION_BUTTON0        = 63,
  FRONTENDACTION_BUTTON1        = 64,
  FRONTENDACTION_BUTTON2        = 65,
  FRONTENDACTION_BUTTON3        = 66,
  FRONTENDACTION_BUTTON4        = 67,
  FRONTENDACTION_BUTTON5        = 68,
  FRONTENDACTION_LTRIGGER       = 69,
  FRONTENDACTION_RTRIGGER       = 70,
  HUDACTION_PAUSEREQUEST        = 71,
  HUDACTION_MESSENGER           = 72,
  HUDACTION_RACE_NOW            = 73,
  HUDACTION_ENGAGE_EVENT        = 74,
  HUDACTION_PAD_LEFT            = 75,
  HUDACTION_PAD_DOWN            = 76,
  HUDACTION_PAD_RIGHT           = 77,
  HUDACTION_SKIPNIS             = 78,
  HUDACTION_NEXTSONG            = 79,
  HUDACTION_VOIPACTIVATION      = 80,
  HUDACTION_VOIPDEACTIVATION    = 81,
  HUDACTION_TOGGLE_LEADERBOARD  = 82,
  HUDACTION_TOGGLE_MINIMAP      = 83,
  HUDACTION_TOGGLE_BESTLINE     = 84,
  HUDACTION_TOGGLE_ARROWS       = 85,
  VOIPACTION_PUSHTOTALK         = 86,
  DEBUGACTION_MOVE_UP           = 87,
  DEBUGACTION_MOVE_DOWN         = 88,
  DEBUGACTION_MOVE_LEFT         = 89,
  DEBUGACTION_MOVE_RIGHT        = 90,
  DEBUGACTION_MOVE_FORWARD      = 91,
  DEBUGACTION_MOVE_BACK         = 92,
  DEBUGACTION_MOVE_D_LEFT       = 93,
  DEBUGACTION_MOVE_D_RIGHT      = 94,
  DEBUGACTION_MOVE_D_FORWARD    = 95,
  DEBUGACTION_MOVE_D_BACK       = 96,
  DEBUGACTION_LOOK_UP           = 97,
  DEBUGACTION_LOOK_DOWN         = 98,
  DEBUGACTION_LOOK_LEFT         = 99,
  DEBUGACTION_LOOK_RIGHT        = 100,
  DEBUGACTION_LOOK_D_UP         = 101,
  DEBUGACTION_LOOK_D_DOWN       = 102,
  DEBUGACTION_LOOK_D_LEFT       = 103,
  DEBUGACTION_LOOK_D_RIGHT      = 104,
  DEBUGACTION_TURBO             = 105,
  DEBUGACTION_SUPER_TURBO       = 106,
  DEBUGACTION_DROPCAR           = 107,
  DEBUGACTION_TOGGLEAI          = 108,
  DEBUGACTION_TOGGLESIMSTEP     = 109,
  DEBUGACTION_SIMSTEP           = 110,
  DEBUGACTION_SCREENSHOT        = 111,
  DEBUGACTION_STOPRECORDPATH    = 112,
  DEBUGACTION_TOGGLECARCOLOUR   = 113,
  DEBUGACTION_TOGGLEDEMOCAMERAS = 114,
  DEBUGACTION_ITERCAMERATARGET  = 115,
  ICE_ACTION_START              = 116,
  ICE_ACTION_SELECT             = 117,
  ICE_ACTION_SOUTH              = 118,
  ICE_ACTION_SOUTH_PRESS        = 119,
  ICE_ACTION_NORTH              = 120,
  ICE_ACTION_NORTH_PRESS        = 121,
  ICE_ACTION_WEST               = 122,
  ICE_ACTION_WEST_PRESS         = 123,
  ICE_ACTION_EAST               = 124,
  ICE_ACTION_EAST_PRESS         = 125,
  ICE_ACTION_DOWN               = 126,
  ICE_ACTION_DOWN_PRESS         = 127,
  ICE_ACTION_UP                 = 128,
  ICE_ACTION_UP_PRESS           = 129,
  ICE_ACTION_LEFT               = 130,
  ICE_ACTION_LEFT_PRESS         = 131,
  ICE_ACTION_RIGHT              = 132,
  ICE_ACTION_RIGHT_PRESS        = 133,
  ICE_ACTION_L1                 = 134,
  ICE_ACTION_L1_PRESS           = 135,
  ICE_ACTION_R1                 = 136,
  ICE_ACTION_R1_PRESS           = 137,
  ICE_ACTION_L2                 = 138,
  ICE_ACTION_L2_PRESS           = 139,
  ICE_ACTION_R2                 = 140,
  ICE_ACTION_R2_PRESS           = 141,
  ICE_ACTION_L3                 = 142,
  ICE_ACTION_L3_PRESS           = 143,
  ICE_ACTION_R3                 = 144,
  ICE_ACTION_R3_RELEASE         = 145,
  ICE_ACTION_LEFTSTICK_LEFT     = 146,
  ICE_ACTION_LEFTSTICK_RIGHT    = 147,
  ICE_ACTION_LEFTSTICK_DOWN     = 148,
  ICE_ACTION_LEFTSTICK_UP       = 149,
  ICE_ACTION_RIGHTSTICK_LEFT    = 150,
  ICE_ACTION_RIGHTSTICK_RIGHT   = 151,
  ICE_ACTION_RIGHTSTICK_DOWN    = 152,
  ICE_ACTION_RIGHTSTICK_UP      = 153,
  ACTION_EXITAPPLICATION        = 154,
  ACTION_PLUGGED                = 155,
  ACTION_UNPLUGGED              = 156,
  ACTION_FLUSH                  = 157,
  MAX_ACTIONID
};

char* ActionIDStr[MAX_ACTIONID] = {"NULL_ACTION",
                                   "GAMEACTION_STEERLEFT",
                                   "GAMEACTION_STEERRIGHT",
                                   "GAMEACTION_OVERSTEER",
                                   "GAMEACTION_OVERSTEERLEFT",
                                   "GAMEACTION_OVERSTEERRIGHT",
                                   "GAMEACTION_TURNLEFT",
                                   "GAMEACTION_TURNRIGHT",
                                   "GAMEACTION_GAS",
                                   "GAMEACTION_GAS_SLOTCAR",
                                   "GAMEACTION_BRAKE",
                                   "GAMEACTION_HANDBRAKE",
                                   "GAMEACTION_GAMEBREAKER",
                                   "GAMEACTION_SHIFTUP",
                                   "GAMEACTION_SHIFTDOWN",
                                   "GAMEACTION_SHIFTFIRST",
                                   "GAMEACTION_SHIFTSECOND",
                                   "GAMEACTION_SHIFTTHIRD",
                                   "GAMEACTION_SHIFTFOURTH",
                                   "GAMEACTION_SHIFTFIFTH",
                                   "GAMEACTION_SHIFTSIXTH",
                                   "GAMEACTION_SHIFTNEUTRAL",
                                   "GAMEACTION_SHIFTREVERSE",
                                   "GAMEACTION_NOS",
                                   "GAMEACTION_RESET",
                                   "GAMEACTION_HYDRAULICALL",
                                   "GAMEACTION_HYDRAULICFRONT",
                                   "GAMEACTION_HYDRAULICLEFT",
                                   "GAMEACTION_HYDRAULICREAR",
                                   "GAMEACTION_HYDRAULICRIGHT",
                                   "GAMEACTION_CREWAGGRESSIVE",
                                   "GAMEACTION_CREWDEFENSIVE",
                                   "GAMEACTION_CREWDEFAULT",
                                   "GAMEACTION_CREWSPEED",
                                   "GAMEACTION_FORWARD",
                                   "GAMEACTION_BACK",
                                   "GAMEACTION_JUMP",
                                   "GAMEACTION_DEBUGHUMAN1",
                                   "GAMEACTION_DEBUGHUMAN2",
                                   "GAMEACTION_DEBUGHUMAN3",
                                   "GAMEACTION_DEBUGHUMAN4",
                                   "GAMEACTION_CLUTCH",
                                   "GAMEACTION_CLUTCH_KICK",
                                   "CAMERAACTION_CHANGE",
                                   "CAMERAACTION_DEBUG",
                                   "CAMERAACTION_ENABLE_ICE",
                                   "CAMERAACTION_LOOKBACK",
                                   "CAMERAACTION_PULLBACK",
                                   "CAMERAACTION_ROT_LEFT",
                                   "CAMERAACTION_ROT_RIGHT",
                                   "CAMERAACTION_360",
                                   "FRONTENDACTION_UP",
                                   "FRONTENDACTION_DOWN",
                                   "FRONTENDACTION_LEFT",
                                   "FRONTENDACTION_RIGHT",
                                   "FRONTENDACTION_RUP",
                                   "FRONTENDACTION_RDOWN",
                                   "FRONTENDACTION_RLEFT",
                                   "FRONTENDACTION_RRIGHT",
                                   "FRONTENDACTION_ACCEPT",
                                   "FRONTENDACTION_CANCEL",
                                   "FRONTENDACTION_CANCEL_ALT",
                                   "FRONTENDACTION_START",
                                   "FRONTENDACTION_BUTTON0",
                                   "FRONTENDACTION_BUTTON1",
                                   "FRONTENDACTION_BUTTON2",
                                   "FRONTENDACTION_BUTTON3",
                                   "FRONTENDACTION_BUTTON4",
                                   "FRONTENDACTION_BUTTON5",
                                   "FRONTENDACTION_LTRIGGER",
                                   "FRONTENDACTION_RTRIGGER",
                                   "HUDACTION_PAUSEREQUEST",
                                   "HUDACTION_MESSENGER",
                                   "HUDACTION_RACE_NOW",
                                   "HUDACTION_ENGAGE_EVENT",
                                   "HUDACTION_PAD_LEFT",
                                   "HUDACTION_PAD_DOWN",
                                   "HUDACTION_PAD_RIGHT",
                                   "HUDACTION_SKIPNIS",
                                   "HUDACTION_NEXTSONG",
                                   "HUDACTION_VOIPACTIVATION",
                                   "HUDACTION_VOIPDEACTIVATION",
                                   "HUDACTION_TOGGLE_LEADERBOARD",
                                   "HUDACTION_TOGGLE_MINIMAP",
                                   "HUDACTION_TOGGLE_BESTLINE",
                                   "HUDACTION_TOGGLE_ARROWS",
                                   "VOIPACTION_PUSHTOTALK",
                                   "DEBUGACTION_MOVE_UP",
                                   "DEBUGACTION_MOVE_DOWN",
                                   "DEBUGACTION_MOVE_LEFT",
                                   "DEBUGACTION_MOVE_RIGHT",
                                   "DEBUGACTION_MOVE_FORWARD",
                                   "DEBUGACTION_MOVE_BACK",
                                   "DEBUGACTION_MOVE_D_LEFT",
                                   "DEBUGACTION_MOVE_D_RIGHT",
                                   "DEBUGACTION_MOVE_D_FORWARD",
                                   "DEBUGACTION_MOVE_D_BACK",
                                   "DEBUGACTION_LOOK_UP",
                                   "DEBUGACTION_LOOK_DOWN",
                                   "DEBUGACTION_LOOK_LEFT",
                                   "DEBUGACTION_LOOK_RIGHT",
                                   "DEBUGACTION_LOOK_D_UP",
                                   "DEBUGACTION_LOOK_D_DOWN",
                                   "DEBUGACTION_LOOK_D_LEFT",
                                   "DEBUGACTION_LOOK_D_RIGHT",
                                   "DEBUGACTION_TURBO",
                                   "DEBUGACTION_SUPER_TURBO",
                                   "DEBUGACTION_DROPCAR",
                                   "DEBUGACTION_TOGGLEAI",
                                   "DEBUGACTION_TOGGLESIMSTEP",
                                   "DEBUGACTION_SIMSTEP",
                                   "DEBUGACTION_SCREENSHOT",
                                   "DEBUGACTION_STOPRECORDPATH",
                                   "DEBUGACTION_TOGGLECARCOLOUR",
                                   "DEBUGACTION_TOGGLEDEMOCAMERAS",
                                   "DEBUGACTION_ITERCAMERATARGET",
                                   "ICE_ACTION_START",
                                   "ICE_ACTION_SELECT",
                                   "ICE_ACTION_SOUTH",
                                   "ICE_ACTION_SOUTH_PRESS",
                                   "ICE_ACTION_NORTH",
                                   "ICE_ACTION_NORTH_PRESS",
                                   "ICE_ACTION_WEST",
                                   "ICE_ACTION_WEST_PRESS",
                                   "ICE_ACTION_EAST",
                                   "ICE_ACTION_EAST_PRESS",
                                   "ICE_ACTION_DOWN",
                                   "ICE_ACTION_DOWN_PRESS",
                                   "ICE_ACTION_UP",
                                   "ICE_ACTION_UP_PRESS",
                                   "ICE_ACTION_LEFT",
                                   "ICE_ACTION_LEFT_PRESS",
                                   "ICE_ACTION_RIGHT",
                                   "ICE_ACTION_RIGHT_PRESS",
                                   "ICE_ACTION_L1",
                                   "ICE_ACTION_L1_PRESS",
                                   "ICE_ACTION_R1",
                                   "ICE_ACTION_R1_PRESS",
                                   "ICE_ACTION_L2",
                                   "ICE_ACTION_L2_PRESS",
                                   "ICE_ACTION_R2",
                                   "ICE_ACTION_R2_PRESS",
                                   "ICE_ACTION_L3",
                                   "ICE_ACTION_L3_PRESS",
                                   "ICE_ACTION_R3",
                                   "ICE_ACTION_R3_RELEASE",
                                   "ICE_ACTION_LEFTSTICK_LEFT",
                                   "ICE_ACTION_LEFTSTICK_RIGHT",
                                   "ICE_ACTION_LEFTSTICK_DOWN",
                                   "ICE_ACTION_LEFTSTICK_UP",
                                   "ICE_ACTION_RIGHTSTICK_LEFT",
                                   "ICE_ACTION_RIGHTSTICK_RIGHT",
                                   "ICE_ACTION_RIGHTSTICK_DOWN",
                                   "ICE_ACTION_RIGHTSTICK_UP",
                                   "ACTION_EXITAPPLICATION",
                                   "ACTION_PLUGGED",
                                   "ACTION_UNPLUGGED",
                                   "ACTION_FLUSH"};

#endif

#ifdef GAME_WORLD
enum ActionID {
  NULL_ACTION                             = 0,
  GAMEACTION_STEERLEFT                    = 1,
  GAMEACTION_STEERRIGHT                   = 2,
  GAMEACTION_TURNLEFT                     = 3,
  GAMEACTION_TURNRIGHT                    = 4,
  GAMEACTION_GAS                          = 7,
  GAMEACTION_BRAKE                        = 8,
  GAMEACTION_HANDBRAKE                    = 9,
  GAMEACTION_GAMEBREAKER                  = 10,
  GAMEACTION_SHIFTUP                      = 11,
  GAMEACTION_SHIFTDOWN                    = 12,
  GAMEACTION_RESET                        = 13,
  GAMEACTION_POWERUP_0                    = 14,
  GAMEACTION_POWERUP_1                    = 15,
  GAMEACTION_POWERUP_2                    = 16,
  GAMEACTION_POWERUP_3                    = 17,
  GAMEACTION_OPTIONS_SHORTCUT             = 18,
  GAMEACTION_PROFILE_SHORTCUT             = 19,
  GAMEACTION_SAFEHOUSE_SHORTCUT           = 20,
  GAMEACTION_WORLDMAP_SHORTCUT            = 21,
  GAMEACTION_FRIENDSLIST_SHORTCUT         = 22,
  GAMEACTION_POWERUPCHANGEPAGE_UP         = 23,
  GAMEACTION_POWERUPCHANGEPAGE_DOWN       = 24,
  GAMEACTION_HELP_SHORTCUT                = 25,
  GAMEACTION_CHAT_SHORTCUT                = 26,
  GAMEACTION_FORWARD                      = 27,
  GAMEACTION_BACK                         = 28,
  GAMEACTION_JUMP                         = 29,
  GAMEACTION_DEBUGHUMAN1                  = 30,
  GAMEACTION_DEBUGHUMAN2                  = 31,
  GAMEACTION_DEBUGHUMAN3                  = 32,
  GAMEACTION_DEBUGHUMAN4                  = 33,
  GAMEACTION_CLUTCH                       = 34,
  GAMEACTION_TOGGLE_AUTODRIVE             = 35,
  GAMEACTION_TOGGLE_COPLIGHTS             = 36,
  GAMEACTION_TOGGLE_AUDIO                 = 37,
  GAMEACTION_HYDRAULICS_BACK              = 38,
  GAMEACTION_HYDRAULICS_FRONT             = 39,
  GAMEACTION_HYDRAULICS_LEFT              = 40,
  GAMEACTION_HYDRAULICS_RIGHT             = 41,
  CAMERAACTION_CHANGE                     = 42,
  CAMERAACTION_DEBUG                      = 43,
  CAMERAACTION_ENABLE_ICE                 = 44,
  CAMERAACTION_LOOKBACK                   = 45,
  CAMERAACTION_PULLBACK                   = 46,
  CAMERAACTION_TOGGLE                     = 47,
  CAMERAACTION_ROT_UP                     = 48,
  CAMERAACTION_ROT_DOWN                   = 49,
  CAMERAACTION_ROT_LEFT                   = 50,
  CAMERAACTION_ROT_RIGHT                  = 51,
  CAMERAACTION_MOVE_UP                    = 52,
  CAMERAACTION_MOVE_DOWN                  = 53,
  CAMERAACTION_MOVE_LEFT                  = 54,
  CAMERAACTION_MOVE_RIGHT                 = 55,
  CAMERAACTION_MOVE_FORWARD               = 56,
  CAMERAACTION_MOVE_BACKWARD              = 57,
  CAMERAACTION_FOV_SMALLER                = 59,
  CAMERAACTION_FOV_LARGER                 = 58,
  CAMERAACTION_CRANE_DOWN                 = 60,
  CAMERAACTION_CRANE_UP                   = 61,
  CAMERAACTION_DOLLY_LEFT                 = 62,
  CAMERAACTION_DOLLY_RIGHT                = 63,
  CAMERAACTION_TURBOSPEED                 = 64,
  CAMERAACTION_CHANGE_CAR                 = 65,
  FRONTENDACTION_UP                       = 66,
  FRONTENDACTION_DOWN                     = 67,
  FRONTENDACTION_LEFT                     = 68,
  FRONTENDACTION_RIGHT                    = 69,
  FRONTENDACTION_RUP                      = 70,
  FRONTENDACTION_RDOWN                    = 71,
  FRONTENDACTION_RLEFT                    = 72,
  FRONTENDACTION_RRIGHT                   = 73,
  FRONTENDACTION_ACCEPT                   = 74,
  FRONTENDACTION_CANCEL                   = 75,
  FRONTENDACTION_CANCEL_ALT               = 76,
  FRONTENDACTION_START                    = 77,
  FRONTENDACTION_BUTTON0                  = 78,
  FRONTENDACTION_BUTTON1                  = 79,
  FRONTENDACTION_BUTTON2                  = 80,
  FRONTENDACTION_BUTTON3                  = 81,
  FRONTENDACTION_BUTTON4                  = 82,
  FRONTENDACTION_BUTTON5                  = 83,
  FRONTENDACTION_LTRIGGER                 = 84,
  FRONTENDACTION_RTRIGGER                 = 85,
  FRONTENDACTION_PGUP                     = 86,
  FRONTENDACTION_PGDOWN                   = 87,
  HUDACTION_PAUSEREQUEST                  = 88,
  HUDACTION_ENGAGE_EVENT                  = 89,
  HUDACTION_PAD_LEFT                      = 90,
  HUDACTION_PAD_DOWN                      = 91,
  HUDACTION_PAD_RIGHT                     = 92,
  HUDACTION_SKIPNIS                       = 93,
  HUDACTION_SCREENSHOT                    = 94,
  HUDACTION_NEXTSONG                      = 95,
  HUDACTION_VOIPACTIVATION                = 96,
  HUDACTION_VOIPDEACTIVATION              = 97,
  VOIPACTION_PUSHTOTALK                   = 98,
  DEBUGACTION_MOVE_UP                     = 99,
  DEBUGACTION_MOVE_DOWN                   = 100,
  DEBUGACTION_MOVE_LEFT                   = 101,
  DEBUGACTION_MOVE_RIGHT                  = 102,
  DEBUGACTION_MOVE_FORWARD                = 103,
  DEBUGACTION_MOVE_BACK                   = 104,
  DEBUGACTION_LOOK_UP                     = 105,
  DEBUGACTION_LOOK_DOWN                   = 106,
  DEBUGACTION_LOOK_LEFT                   = 107,
  DEBUGACTION_LOOK_RIGHT                  = 108,
  DEBUGACTION_TURBO                       = 109,
  DEBUGACTION_SUPER_TURBO                 = 110,
  DEBUGACTION_CAMERA_SAVE                 = 111,
  DEBUGACTION_CAMERA_LOAD                 = 112,
  DEBUGACTION_DEBUGDRAW_DRAWTICK_DEC      = 113,
  DEBUGACTION_DEBUGDRAW_DRAWTICK_INC      = 114,
  DEBUGACTION_DEBUGDRAW_DRAWTICKRANGE_DEC = 115,
  DEBUGACTION_DEBUGDRAW_DRAWTICKRANGE_INC = 116,
  DEBUGACTION_DROPCAR                     = 117,
  DEBUGACTION_LOCKTOCAR                   = 118,
  DEBUGACTION_TOGGLESIMSTEP               = 119,
  DEBUGACTION_SIMSTEP                     = 120,
  DEBUGACTION_TOGGLESIMPAUSE              = 121,
  DEBUGACTION_SCREENSHOT                  = 122,
  DEBUGACTION_STOPRECORDPATH              = 123,
  DEBUGACTION_TOGGLECARCOLOUR             = 124,
  DEBUGACTION_TOGGLEDEMOCAMERAS           = 125,
  DEBUGACTION_TOGGLEMENU                  = 126,
  ACTION_EXITAPPLICATION                  = 164,
  ACTION_PLUGGED                          = 165,
  ACTION_UNPLUGGED                        = 166,
  ACTION_FLUSH                            = 167,
  MAX_ACTIONID
};

char* ActionIDStr[MAX_ACTIONID] = {"NULL_ACTION",
                                   "GAMEACTION_STEERLEFT",
                                   "GAMEACTION_STEERRIGHT",
                                   "GAMEACTION_TURNLEFT",
                                   "GAMEACTION_TURNRIGHT",
                                   "Unknown_5",
                                   "Unknown_6",
                                   "GAMEACTION_GAS",
                                   "GAMEACTION_BRAKE",
                                   "GAMEACTION_HANDBRAKE",
                                   "GAMEACTION_GAMEBREAKER",
                                   "GAMEACTION_SHIFTUP",
                                   "GAMEACTION_SHIFTDOWN",
                                   "GAMEACTION_RESET",
                                   "GAMEACTION_POWERUP_0",
                                   "GAMEACTION_POWERUP_1",
                                   "GAMEACTION_POWERUP_2",
                                   "GAMEACTION_POWERUP_3",
                                   "GAMEACTION_OPTIONS_SHORTCUT",
                                   "GAMEACTION_PROFILE_SHORTCUT",
                                   "GAMEACTION_SAFEHOUSE_SHORTCUT",
                                   "GAMEACTION_WORLDMAP_SHORTCUT",
                                   "GAMEACTION_FRIENDSLIST_SHORTCUT",
                                   "GAMEACTION_POWERUPCHANGEPAGE_UP",
                                   "GAMEACTION_POWERUPCHANGEPAGE_DOWN",
                                   "GAMEACTION_HELP_SHORTCUT",
                                   "GAMEACTION_CHAT_SHORTCUT",
                                   "GAMEACTION_FORWARD",
                                   "GAMEACTION_BACK",
                                   "GAMEACTION_JUMP",
                                   "GAMEACTION_DEBUGHUMAN1",
                                   "GAMEACTION_DEBUGHUMAN2",
                                   "GAMEACTION_DEBUGHUMAN3",
                                   "GAMEACTION_DEBUGHUMAN4",
                                   "GAMEACTION_CLUTCH",
                                   "GAMEACTION_TOGGLE_AUTODRIVE",
                                   "GAMEACTION_TOGGLE_COPLIGHTS",
                                   "GAMEACTION_TOGGLE_AUDIO",
                                   "GAMEACTION_HYDRAULICS_BACK",
                                   "GAMEACTION_HYDRAULICS_FRONT",
                                   "GAMEACTION_HYDRAULICS_LEFT",
                                   "GAMEACTION_HYDRAULICS_RIGHT",
                                   "CAMERAACTION_CHANGE",
                                   "CAMERAACTION_DEBUG",
                                   "CAMERAACTION_ENABLE_ICE",
                                   "CAMERAACTION_LOOKBACK",
                                   "CAMERAACTION_PULLBACK",
                                   "CAMERAACTION_TOGGLE",
                                   "CAMERAACTION_ROT_UP",
                                   "CAMERAACTION_ROT_DOWN",
                                   "CAMERAACTION_ROT_LEFT",
                                   "CAMERAACTION_ROT_RIGHT",
                                   "CAMERAACTION_MOVE_UP",
                                   "CAMERAACTION_MOVE_DOWN",
                                   "CAMERAACTION_MOVE_LEFT",
                                   "CAMERAACTION_MOVE_RIGHT",
                                   "CAMERAACTION_MOVE_FORWARD",
                                   "CAMERAACTION_MOVE_BACKWARD",
                                   "CAMERAACTION_FOV_LARGER",
                                   "CAMERAACTION_FOV_SMALLER",
                                   "CAMERAACTION_CRANE_DOWN",
                                   "CAMERAACTION_CRANE_UP",
                                   "CAMERAACTION_DOLLY_LEFT",
                                   "CAMERAACTION_DOLLY_RIGHT",
                                   "CAMERAACTION_TURBOSPEED",
                                   "CAMERAACTION_CHANGE_CAR",
                                   "FRONTENDACTION_UP",
                                   "FRONTENDACTION_DOWN",
                                   "FRONTENDACTION_LEFT",
                                   "FRONTENDACTION_RIGHT",
                                   "FRONTENDACTION_RUP",
                                   "FRONTENDACTION_RDOWN",
                                   "FRONTENDACTION_RLEFT",
                                   "FRONTENDACTION_RRIGHT",
                                   "FRONTENDACTION_ACCEPT",
                                   "FRONTENDACTION_CANCEL",
                                   "FRONTENDACTION_CANCEL_ALT",
                                   "FRONTENDACTION_START",
                                   "FRONTENDACTION_BUTTON0",
                                   "FRONTENDACTION_BUTTON1",
                                   "FRONTENDACTION_BUTTON2",
                                   "FRONTENDACTION_BUTTON3",
                                   "FRONTENDACTION_BUTTON4",
                                   "FRONTENDACTION_BUTTON5",
                                   "FRONTENDACTION_LTRIGGER",
                                   "FRONTENDACTION_RTRIGGER",
                                   "FRONTENDACTION_PGUP",
                                   "FRONTENDACTION_PGDOWN",
                                   "HUDACTION_PAUSEREQUEST",
                                   "HUDACTION_ENGAGE_EVENT",
                                   "HUDACTION_PAD_LEFT",
                                   "HUDACTION_PAD_DOWN",
                                   "HUDACTION_PAD_RIGHT",
                                   "HUDACTION_SKIPNIS",
                                   "HUDACTION_SCREENSHOT",
                                   "HUDACTION_NEXTSONG",
                                   "HUDACTION_VOIPACTIVATION",
                                   "HUDACTION_VOIPDEACTIVATION",
                                   "VOIPACTION_PUSHTOTALK",
                                   "DEBUGACTION_MOVE_UP",
                                   "DEBUGACTION_MOVE_DOWN",
                                   "DEBUGACTION_MOVE_LEFT",
                                   "DEBUGACTION_MOVE_RIGHT",
                                   "DEBUGACTION_MOVE_FORWARD",
                                   "DEBUGACTION_MOVE_BACK",
                                   "DEBUGACTION_LOOK_UP",
                                   "DEBUGACTION_LOOK_DOWN",
                                   "DEBUGACTION_LOOK_LEFT",
                                   "DEBUGACTION_LOOK_RIGHT",
                                   "DEBUGACTION_TURBO",
                                   "DEBUGACTION_SUPER_TURBO",
                                   "DEBUGACTION_CAMERA_SAVE",
                                   "DEBUGACTION_CAMERA_LOAD",
                                   "DEBUGACTION_DEBUGDRAW_DRAWTICK_DEC",
                                   "DEBUGACTION_DEBUGDRAW_DRAWTICK_INC",
                                   "DEBUGACTION_DEBUGDRAW_DRAWTICKRANGE_DEC",
                                   "DEBUGACTION_DEBUGDRAW_DRAWTICKRANGE_INC",
                                   "DEBUGACTION_DROPCAR",
                                   "DEBUGACTION_LOCKTOCAR",
                                   "DEBUGACTION_TOGGLESIMSTEP",
                                   "DEBUGACTION_SIMSTEP",
                                   "DEBUGACTION_TOGGLESIMPAUSE",
                                   "DEBUGACTION_SCREENSHOT",
                                   "DEBUGACTION_STOPRECORDPATH",
                                   "DEBUGACTION_TOGGLECARCOLOUR",
                                   "DEBUGACTION_TOGGLEDEMOCAMERAS",
                                   "DEBUGACTION_TOGGLEMENU",
                                   "Unknown_127",
                                   "Unknown_128",
                                   "Unknown_129",
                                   "Unknown_130",
                                   "Unknown_131",
                                   "Unknown_132",
                                   "Unknown_133",
                                   "Unknown_134",
                                   "Unknown_135",
                                   "Unknown_136",
                                   "Unknown_137",
                                   "Unknown_138",
                                   "Unknown_139",
                                   "Unknown_140",
                                   "Unknown_141",
                                   "Unknown_142",
                                   "Unknown_143",
                                   "Unknown_144",
                                   "Unknown_145",
                                   "Unknown_146",
                                   "Unknown_147",
                                   "Unknown_148",
                                   "Unknown_149",
                                   "Unknown_150",
                                   "Unknown_151",
                                   "Unknown_152",
                                   "Unknown_153",
                                   "Unknown_154",
                                   "Unknown_155",
                                   "Unknown_156",
                                   "Unknown_157",
                                   "Unknown_158",
                                   "Unknown_159",
                                   "Unknown_160",
                                   "Unknown_161",
                                   "Unknown_162",
                                   "Unknown_163",
                                   "ACTION_EXITAPPLICATION",
                                   "ACTION_PLUGGED",
                                   "ACTION_UNPLUGGED",
                                   "ACTION_FLUSH"};

#endif

// secondary bind names
#define FE_SECONDARY_UP_NAME    "FRONTENDACTION_UP_ALT"
#define FE_SECONDARY_DOWN_NAME  "FRONTENDACTION_DOWN_ALT"
#define FE_SECONDARY_LEFT_NAME  "FRONTENDACTION_LEFT_ALT"
#define FE_SECONDARY_RIGHT_NAME "FRONTENDACTION_RIGHT_ALT"

bool bIsActionAnalog(ActionID in) {
  switch (in) {
    case GAMEACTION_STEERLEFT:
    case GAMEACTION_STEERRIGHT:
    case GAMEACTION_TURNLEFT:
    case GAMEACTION_TURNRIGHT:
    case GAMEACTION_GAS:
    case GAMEACTION_BRAKE:
    case FRONTENDACTION_RUP:
    case FRONTENDACTION_RDOWN:
    case FRONTENDACTION_RLEFT:
    case FRONTENDACTION_RRIGHT:
    case DEBUGACTION_MOVE_UP:
    case DEBUGACTION_MOVE_DOWN:
    case DEBUGACTION_MOVE_LEFT:
    case DEBUGACTION_MOVE_RIGHT:
    case DEBUGACTION_MOVE_FORWARD:
    case DEBUGACTION_MOVE_BACK:
    case DEBUGACTION_LOOK_UP:
    case DEBUGACTION_LOOK_DOWN:
    case DEBUGACTION_LOOK_LEFT:
    case DEBUGACTION_LOOK_RIGHT:
    case DEBUGACTION_TURBO:
    case DEBUGACTION_SUPER_TURBO:
    case FRONTENDACTION_LTRIGGER:
    case FRONTENDACTION_RTRIGGER:
#ifdef GAME_DEBUG_VERSION
    case GAMEACTION_GAS_SLOTCAR:
    case ICE_ACTION_LEFTSTICK_LEFT:
    case ICE_ACTION_LEFTSTICK_RIGHT:
    case ICE_ACTION_LEFTSTICK_DOWN:
    case ICE_ACTION_LEFTSTICK_UP:
    case ICE_ACTION_RIGHTSTICK_LEFT:
    case ICE_ACTION_RIGHTSTICK_RIGHT:
    case ICE_ACTION_RIGHTSTICK_DOWN:
    case ICE_ACTION_RIGHTSTICK_UP:
#endif
      return true;
  }

  return false;
}

bool bIsActionFrontEnd(ActionID in) {
  switch (in) {
    case FRONTENDACTION_UP:
    case FRONTENDACTION_DOWN:
    case FRONTENDACTION_LEFT:
    case FRONTENDACTION_RIGHT:
    case FRONTENDACTION_RUP:
    case FRONTENDACTION_RDOWN:
    case FRONTENDACTION_RLEFT:
    case FRONTENDACTION_RRIGHT:
    case FRONTENDACTION_ACCEPT:
    case FRONTENDACTION_CANCEL:
    case FRONTENDACTION_CANCEL_ALT:
    case FRONTENDACTION_START:
    case FRONTENDACTION_BUTTON0:
    case FRONTENDACTION_BUTTON1:
    case FRONTENDACTION_BUTTON2:
    case FRONTENDACTION_BUTTON3:
    case FRONTENDACTION_BUTTON4:
    case FRONTENDACTION_BUTTON5:
    case FRONTENDACTION_LTRIGGER:
    case FRONTENDACTION_RTRIGGER:
      return true;
  }
  return false;
}

bool bIsActionHUD(ActionID in) {
  switch (in) {
    case HUDACTION_PAUSEREQUEST:
    case HUDACTION_ENGAGE_EVENT:
    case HUDACTION_PAD_LEFT:
    case HUDACTION_PAD_DOWN:
    case HUDACTION_PAD_RIGHT:
    case HUDACTION_SKIPNIS:
    case HUDACTION_NEXTSONG:
#ifndef GAME_MW
    case HUDACTION_VOIPACTIVATION:
    case HUDACTION_VOIPDEACTIVATION:
#endif
      return true;
  }
  return false;
}

bool bIsActionFrontEndNavigation(ActionID in) {
  switch (in) {
    case FRONTENDACTION_UP:
    case FRONTENDACTION_DOWN:
    case FRONTENDACTION_LEFT:
    case FRONTENDACTION_RIGHT:
      return true;
  }
  return false;
}

bool bIsActionRelativePosition(ActionID in) {
  switch (in) {
    case FRONTENDACTION_LTRIGGER:
    case FRONTENDACTION_RTRIGGER:
      return true;
  }
  return false;
}

bool bIsActionAnalogAxis(ActionID in) {
  switch (in) {
    case GAMEACTION_STEERLEFT:
    case GAMEACTION_STEERRIGHT:
    case GAMEACTION_TURNLEFT:
    case GAMEACTION_TURNRIGHT:
    case GAMEACTION_GAS:
    case GAMEACTION_BRAKE:
    case FRONTENDACTION_RUP:
    case FRONTENDACTION_RDOWN:
    case FRONTENDACTION_RLEFT:
    case FRONTENDACTION_RRIGHT:
    case FRONTENDACTION_LTRIGGER:
    case FRONTENDACTION_RTRIGGER:
    case DEBUGACTION_MOVE_UP:
    case DEBUGACTION_MOVE_DOWN:
    case DEBUGACTION_MOVE_LEFT:
    case DEBUGACTION_MOVE_RIGHT:
    case DEBUGACTION_MOVE_FORWARD:
    case DEBUGACTION_MOVE_BACK:
    case DEBUGACTION_LOOK_UP:
    case DEBUGACTION_LOOK_DOWN:
    case DEBUGACTION_LOOK_LEFT:
    case DEBUGACTION_LOOK_RIGHT:
    case DEBUGACTION_TURBO:
    case DEBUGACTION_SUPER_TURBO:
      return true;
  }
  return false;
}

bool bIsActionDigitalButton(ActionID in) {
  switch (in) {
    case GAMEACTION_GAMEBREAKER:
    case CAMERAACTION_CHANGE:
    case GAMEACTION_RESET:
    case DEBUGACTION_DROPCAR:
    case CAMERAACTION_DEBUG:
    case GAMEACTION_SHIFTUP:
    case GAMEACTION_SHIFTDOWN:
    case GAMEACTION_TURNLEFT:
    case GAMEACTION_TURNRIGHT:
    case HUDACTION_PAUSEREQUEST:
    case HUDACTION_ENGAGE_EVENT:
    case HUDACTION_PAD_LEFT:
    case HUDACTION_PAD_DOWN:
    case HUDACTION_PAD_RIGHT:
    case HUDACTION_SKIPNIS:
    case HUDACTION_NEXTSONG:
#ifndef GAME_MW
    case HUDACTION_VOIPACTIVATION:
    case HUDACTION_VOIPDEACTIVATION:
#ifndef GAME_WORLD
    case GAMEACTION_CREWAGGRESSIVE:
    case GAMEACTION_CREWDEFENSIVE:
    case GAMEACTION_CREWDEFAULT:
    case GAMEACTION_CREWSPEED:
#endif
#ifdef GAME_PROSTREET
    case HUDACTION_TOGGLE_LEADERBOARD:
    case HUDACTION_TOGGLE_MINIMAP:
    case HUDACTION_TOGGLE_BESTLINE:
    case HUDACTION_TOGGLE_ARROWS:
#endif
#ifdef GAME_WORLD
    case GAMEACTION_TOGGLE_COPLIGHTS:
    case GAMEACTION_TOGGLE_AUTODRIVE:
    case GAMEACTION_TOGGLE_AUDIO:
    case GAMEACTION_POWERUP_0:
    case GAMEACTION_POWERUP_1:
    case GAMEACTION_POWERUP_2:
    case GAMEACTION_POWERUP_3:
#endif
#endif
      return true;
  }
  return false;
}

bool bIsActionDebug(ActionID in) {
  switch (in) {
    case DEBUGACTION_MOVE_UP:
    case DEBUGACTION_MOVE_DOWN:
    case DEBUGACTION_MOVE_LEFT:
    case DEBUGACTION_MOVE_RIGHT:
    case DEBUGACTION_MOVE_FORWARD:
    case DEBUGACTION_MOVE_BACK:
    case DEBUGACTION_LOOK_UP:
    case DEBUGACTION_LOOK_DOWN:
    case DEBUGACTION_LOOK_LEFT:
    case DEBUGACTION_LOOK_RIGHT:
    case DEBUGACTION_TURBO:
    case DEBUGACTION_SUPER_TURBO:
    case DEBUGACTION_DROPCAR:
    case DEBUGACTION_TOGGLESIMSTEP:
    case DEBUGACTION_SIMSTEP:
    case DEBUGACTION_SCREENSHOT:
    case DEBUGACTION_STOPRECORDPATH:
    case DEBUGACTION_TOGGLECARCOLOUR:
    case DEBUGACTION_TOGGLEDEMOCAMERAS:
#ifdef GAME_WORLD
    case DEBUGACTION_TOGGLEMENU:
    case DEBUGACTION_TOGGLESIMPAUSE:
    case DEBUGACTION_LOCKTOCAR:
    case DEBUGACTION_DEBUGDRAW_DRAWTICK_DEC:
    case DEBUGACTION_DEBUGDRAW_DRAWTICK_INC:
    case DEBUGACTION_DEBUGDRAW_DRAWTICKRANGE_DEC:
    case DEBUGACTION_DEBUGDRAW_DRAWTICKRANGE_INC:
    case DEBUGACTION_CAMERA_SAVE:
    case DEBUGACTION_CAMERA_LOAD:
#else
    case DEBUGACTION_LOOK_D_UP:
    case DEBUGACTION_LOOK_D_DOWN:
    case DEBUGACTION_LOOK_D_LEFT:
    case DEBUGACTION_LOOK_D_RIGHT:
    case DEBUGACTION_MOVE_D_LEFT:
    case DEBUGACTION_MOVE_D_RIGHT:
    case DEBUGACTION_MOVE_D_FORWARD:
    case DEBUGACTION_MOVE_D_BACK:
    case DEBUGACTION_TOGGLEAI:
#endif
#ifdef GAME_DEBUG_VERSION
    case ICEACTION_GRAB_LEFT:
    case ICEACTION_GRAB_RIGHT:
    case ICEACTION_LEFT:
    case ICEACTION_RIGHT:
    case ICEACTION_FAST_LEFT:
    case ICEACTION_FAST_RIGHT:
    case ICEACTION_UP:
    case ICEACTION_DOWN:
    case ICEACTION_HELP:
    case ICEACTION_UNDO:
    case ICEACTION_PLAY:
    case ICEACTION_HIDE:
    case ICEACTION_SCREENSHOT:
    case ICEACTION_SCREENSHOT_STREAM:
    case ICEACTION_INSERT:
    case ICEACTION_CUT:
    case ICEACTION_COPY:
    case ICEACTION_PASTE:
    case ICEACTION_SELECT:
    case ICEACTION_CANCEL:
    case ICEACTION_ALT_1:
    case ICEACTION_CLIP_IN:
    case ICEACTION_CLIP_OUT:
    case ICEACTION_SIMSPEED_INC:
    case ICEACTION_SIMSPEED_DEC:
    case ICEACTION_DUTCH_LEFT:
    case ICEACTION_DUTCH_RIGHT:
    case ICEACTION_ZOOM_IN:
    case ICEACTION_ZOOM_OUT:
    case ICEACTION_LETTERBOX_IN:
    case ICEACTION_LETTERBOX_OUT:
    case ICEACTION_BUBBLE_IN:
    case ICEACTION_BUBBLE_OUT:
    case ICEACTION_BUBBLE_UP:
    case ICEACTION_BUBBLE_DOWN:
    case ICEACTION_BUBBLE_TILT_UP:
    case ICEACTION_BUBBLE_TILT_DOWN:
    case ICEACTION_BUBBLE_SPIN_LEFT:
    case ICEACTION_BUBBLE_SPIN_RIGHT:
    case ICEACTION_BUBBLE_LEFT:
    case ICEACTION_BUBBLE_RIGHT:
    case ICEACTION_BUBBLE_BACK:
    case ICEACTION_BUBBLE_FORTH:
    case ICEACTION_SHAKE_MAG_DEC:
    case ICEACTION_SHAKE_MAG_INC:
    case ICEACTION_SHAKE_FRQ_DEC:
    case ICEACTION_SHAKE_FRQ_INC:
    case ICEACTION_SHUTTLE_LEFT:
    case ICEACTION_SHUTTLE_RIGHT:
    case ICEACTION_SHUTTLE_UP:
    case ICEACTION_SHUTTLE_DOWN:
    case ICEACTION_SHUTTLE_SPEED_DEC:
    case ICEACTION_SHUTTLE_SPEED_INC:
#endif
    case CAMERAACTION_DEBUG:
    case CAMERAACTION_ENABLE_ICE:
      return true;
  }

  return false;
}

bool bIsActionTextureBindable(ActionID in) {
  switch (in) {
    case FRONTENDACTION_UP:
    case FRONTENDACTION_DOWN:
    case FRONTENDACTION_LEFT:
    case FRONTENDACTION_RIGHT:
    case FRONTENDACTION_RUP:
    case FRONTENDACTION_START:
    case FRONTENDACTION_ACCEPT:
    case FRONTENDACTION_CANCEL:
    case FRONTENDACTION_BUTTON0:
    case FRONTENDACTION_BUTTON1:
    case FRONTENDACTION_BUTTON2:
    case FRONTENDACTION_BUTTON3:
    case FRONTENDACTION_BUTTON4:
    case FRONTENDACTION_BUTTON5:
    case FRONTENDACTION_LTRIGGER:
    case FRONTENDACTION_RTRIGGER:
    case GAMEACTION_SHIFTUP:
    case GAMEACTION_SHIFTDOWN:
      return true;
  }
  return false;
}
