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
#ifdef GAME_PROSTREET
#include <vector>
using namespace std;
#endif

int bStringHash(char* a1);

//
// Constants for gamepad buttons -- from XInput.h
//
#define XINPUT_GAMEPAD_DPAD_UP        0x0001
#define XINPUT_GAMEPAD_DPAD_DOWN      0x0002
#define XINPUT_GAMEPAD_DPAD_LEFT      0x0004
#define XINPUT_GAMEPAD_DPAD_RIGHT     0x0008
#define XINPUT_GAMEPAD_START          0x0010
#define XINPUT_GAMEPAD_BACK           0x0020
#define XINPUT_GAMEPAD_LEFT_THUMB     0x0040
#define XINPUT_GAMEPAD_RIGHT_THUMB    0x0080
#define XINPUT_GAMEPAD_LEFT_SHOULDER  0x0100
#define XINPUT_GAMEPAD_RIGHT_SHOULDER 0x0200
#define XINPUT_GAMEPAD_A              0x1000
#define XINPUT_GAMEPAD_B              0x2000
#define XINPUT_GAMEPAD_X              0x4000
#define XINPUT_GAMEPAD_Y              0x8000

// other gamepad stuff -- sticks & triggers
#define XINPUT_GAMEPAD_LT_CONFIGDEF       0xABC0
#define XINPUT_GAMEPAD_RT_CONFIGDEF       0xABC1
#define XINPUT_GAMEPAD_LS_X_CONFIGDEF     0xABC2
#define XINPUT_GAMEPAD_LS_Y_CONFIGDEF     0xABC3
#define XINPUT_GAMEPAD_RS_X_CONFIGDEF     0xABC4
#define XINPUT_GAMEPAD_RS_Y_CONFIGDEF     0xABC5
#define XINPUT_GAMEPAD_LS_UP_CONFIGDEF    0xABC7
#define XINPUT_GAMEPAD_LS_DOWN_CONFIGDEF  0xABC8
#define XINPUT_GAMEPAD_LS_LEFT_CONFIGDEF  0xABC9
#define XINPUT_GAMEPAD_LS_RIGHT_CONFIGDEF 0xABCA
#define XINPUT_GAMEPAD_RS_UP_CONFIGDEF    0xABCB
#define XINPUT_GAMEPAD_RS_DOWN_CONFIGDEF  0xABCC
#define XINPUT_GAMEPAD_RS_LEFT_CONFIGDEF  0xABCD
#define XINPUT_GAMEPAD_RS_RIGHT_CONFIGDEF 0xABCE

#define XINPUT_GAMEPAD_DPAD_CONFIGDEF 0xDAD0  // only for internal use
#define XINPUT_GAMEPAD_LS_CONFIGDEF   0xDAD1  // only for internal use
#define XINPUT_GAMEPAD_RS_CONFIGDEF   0xDAD2  // only for internal use

#define XINPUT_GAMEPAD_DPAD_UPDOWN_CONFIGDEF    0xDAD3  // only for internal use
#define XINPUT_GAMEPAD_DPAD_LEFTRIGHT_CONFIGDEF 0xDAD4  // only for internal use

// comparing strings by hash
#define XINPUT_GAMEPAD_DPAD_UP_HASH        NFS_HASH("XINPUT_GAMEPAD_DPAD_UP")
#define XINPUT_GAMEPAD_DPAD_DOWN_HASH      NFS_HASH("XINPUT_GAMEPAD_DPAD_DOWN")
#define XINPUT_GAMEPAD_DPAD_LEFT_HASH      NFS_HASH("XINPUT_GAMEPAD_DPAD_LEFT")
#define XINPUT_GAMEPAD_DPAD_RIGHT_HASH     NFS_HASH("XINPUT_GAMEPAD_DPAD_RIGHT")
#define XINPUT_GAMEPAD_START_HASH          NFS_HASH("XINPUT_GAMEPAD_START")
#define XINPUT_GAMEPAD_BACK_HASH           NFS_HASH("XINPUT_GAMEPAD_BACK")
#define XINPUT_GAMEPAD_LEFT_THUMB_HASH     NFS_HASH("XINPUT_GAMEPAD_LEFT_THUMB")
#define XINPUT_GAMEPAD_RIGHT_THUMB_HASH    NFS_HASH("XINPUT_GAMEPAD_RIGHT_THUMB")
#define XINPUT_GAMEPAD_LEFT_SHOULDER_HASH  NFS_HASH("XINPUT_GAMEPAD_LEFT_SHOULDER")
#define XINPUT_GAMEPAD_RIGHT_SHOULDER_HASH NFS_HASH("XINPUT_GAMEPAD_RIGHT_SHOULDER")
#define XINPUT_GAMEPAD_LB_HASH             NFS_HASH("XINPUT_GAMEPAD_LB")
#define XINPUT_GAMEPAD_RB_HASH             NFS_HASH("XINPUT_GAMEPAD_RB")
#define XINPUT_GAMEPAD_A_HASH              NFS_HASH("XINPUT_GAMEPAD_A")
#define XINPUT_GAMEPAD_B_HASH              NFS_HASH("XINPUT_GAMEPAD_B")
#define XINPUT_GAMEPAD_X_HASH              NFS_HASH("XINPUT_GAMEPAD_X")
#define XINPUT_GAMEPAD_Y_HASH              NFS_HASH("XINPUT_GAMEPAD_Y")
#define XINPUT_GAMEPAD_LT_HASH             NFS_HASH("XINPUT_GAMEPAD_LT")
#define XINPUT_GAMEPAD_RT_HASH             NFS_HASH("XINPUT_GAMEPAD_RT")
#define XINPUT_GAMEPAD_LS_X_HASH           NFS_HASH("XINPUT_GAMEPAD_LS_X")
#define XINPUT_GAMEPAD_LS_Y_HASH           NFS_HASH("XINPUT_GAMEPAD_LS_Y")
#define XINPUT_GAMEPAD_RS_X_HASH           NFS_HASH("XINPUT_GAMEPAD_RS_X")
#define XINPUT_GAMEPAD_RS_Y_HASH           NFS_HASH("XINPUT_GAMEPAD_RS_Y")
#define XINPUT_GAMEPAD_DPAD_HASH           NFS_HASH("XINPUT_GAMEPAD_DPAD")
#define XINPUT_GAMEPAD_LS_UP_HASH          NFS_HASH("XINPUT_GAMEPAD_LS_UP")
#define XINPUT_GAMEPAD_LS_DOWN_HASH        NFS_HASH("XINPUT_GAMEPAD_LS_DOWN")
#define XINPUT_GAMEPAD_LS_LEFT_HASH        NFS_HASH("XINPUT_GAMEPAD_LS_LEFT")
#define XINPUT_GAMEPAD_LS_RIGHT_HASH       NFS_HASH("XINPUT_GAMEPAD_LS_RIGHT")
#define XINPUT_GAMEPAD_RS_UP_HASH          NFS_HASH("XINPUT_GAMEPAD_RS_UP")
#define XINPUT_GAMEPAD_RS_DOWN_HASH        NFS_HASH("XINPUT_GAMEPAD_RS_DOWN")
#define XINPUT_GAMEPAD_RS_LEFT_HASH        NFS_HASH("XINPUT_GAMEPAD_RS_LEFT")
#define XINPUT_GAMEPAD_RS_RIGHT_HASH       NFS_HASH("XINPUT_GAMEPAD_RS_RIGHT")

WORD ConvertXInputNameToBitmask(char* InName) {
  for (int i = 0; i < strlen(InName); ++i) InName[i] = toupper(InName[i]);

  switch (bStringHash(InName)) {
    case XINPUT_GAMEPAD_DPAD_UP_HASH:
      return XINPUT_GAMEPAD_DPAD_UP;
    case XINPUT_GAMEPAD_DPAD_DOWN_HASH:
      return XINPUT_GAMEPAD_DPAD_DOWN;
    case XINPUT_GAMEPAD_DPAD_LEFT_HASH:
      return XINPUT_GAMEPAD_DPAD_LEFT;
    case XINPUT_GAMEPAD_DPAD_RIGHT_HASH:
      return XINPUT_GAMEPAD_DPAD_RIGHT;
    case XINPUT_GAMEPAD_START_HASH:
      return XINPUT_GAMEPAD_START;
    case XINPUT_GAMEPAD_BACK_HASH:
      return XINPUT_GAMEPAD_BACK;
    case XINPUT_GAMEPAD_LEFT_THUMB_HASH:
      return XINPUT_GAMEPAD_LEFT_THUMB;
    case XINPUT_GAMEPAD_RIGHT_THUMB_HASH:
      return XINPUT_GAMEPAD_RIGHT_THUMB;
    case XINPUT_GAMEPAD_LB_HASH:
    case XINPUT_GAMEPAD_LEFT_SHOULDER_HASH:
      return XINPUT_GAMEPAD_LEFT_SHOULDER;
    case XINPUT_GAMEPAD_RB_HASH:
    case XINPUT_GAMEPAD_RIGHT_SHOULDER_HASH:
      return XINPUT_GAMEPAD_RIGHT_SHOULDER;
    case XINPUT_GAMEPAD_A_HASH:
      return XINPUT_GAMEPAD_A;
    case XINPUT_GAMEPAD_B_HASH:
      return XINPUT_GAMEPAD_B;
    case XINPUT_GAMEPAD_X_HASH:
      return XINPUT_GAMEPAD_X;
    case XINPUT_GAMEPAD_Y_HASH:
      return XINPUT_GAMEPAD_Y;

    default:
      break;
  }
  return 0;
}

int ConvertXInputOtherConfigDef(char* InName) {
  for (int i = 0; i < strlen(InName); ++i) InName[i] = toupper(InName[i]);

  switch (bStringHash(InName)) {
    case XINPUT_GAMEPAD_LT_HASH:
      return XINPUT_GAMEPAD_LT_CONFIGDEF;
    case XINPUT_GAMEPAD_RT_HASH:
      return XINPUT_GAMEPAD_RT_CONFIGDEF;
    case XINPUT_GAMEPAD_LS_X_HASH:
      return XINPUT_GAMEPAD_LS_X_CONFIGDEF;
    case XINPUT_GAMEPAD_LS_Y_HASH:
      return XINPUT_GAMEPAD_LS_Y_CONFIGDEF;
    case XINPUT_GAMEPAD_RS_X_HASH:
      return XINPUT_GAMEPAD_RS_X_CONFIGDEF;
    case XINPUT_GAMEPAD_RS_Y_HASH:
      return XINPUT_GAMEPAD_RS_Y_CONFIGDEF;
    case XINPUT_GAMEPAD_LS_UP_HASH:
      return XINPUT_GAMEPAD_LS_UP_CONFIGDEF;
    case XINPUT_GAMEPAD_LS_DOWN_HASH:
      return XINPUT_GAMEPAD_LS_DOWN_CONFIGDEF;
    case XINPUT_GAMEPAD_LS_LEFT_HASH:
      return XINPUT_GAMEPAD_LS_LEFT_CONFIGDEF;
    case XINPUT_GAMEPAD_LS_RIGHT_HASH:
      return XINPUT_GAMEPAD_LS_RIGHT_CONFIGDEF;
    case XINPUT_GAMEPAD_RS_UP_HASH:
      return XINPUT_GAMEPAD_RS_UP_CONFIGDEF;
    case XINPUT_GAMEPAD_RS_DOWN_HASH:
      return XINPUT_GAMEPAD_RS_DOWN_CONFIGDEF;
    case XINPUT_GAMEPAD_RS_LEFT_HASH:
      return XINPUT_GAMEPAD_RS_LEFT_CONFIGDEF;
    case XINPUT_GAMEPAD_RS_RIGHT_HASH:
      return XINPUT_GAMEPAD_RS_RIGHT_CONFIGDEF;
  }
  return 0;
}

bool bIsBindingAnalog(WORD in) {
  if (((in >> 4) & 0xFFF) == 0xABC) return true;
  return false;
}

int IsBindingNavigation(WORD in) {
  switch (in) {
    case XINPUT_GAMEPAD_DPAD_UP:
    case XINPUT_GAMEPAD_DPAD_DOWN:
    case XINPUT_GAMEPAD_DPAD_LEFT:
    case XINPUT_GAMEPAD_DPAD_RIGHT:
      return 1;
    case XINPUT_GAMEPAD_LS_X_CONFIGDEF:
    case XINPUT_GAMEPAD_LS_Y_CONFIGDEF:
    case XINPUT_GAMEPAD_LS_UP_CONFIGDEF:
    case XINPUT_GAMEPAD_LS_DOWN_CONFIGDEF:
    case XINPUT_GAMEPAD_LS_LEFT_CONFIGDEF:
    case XINPUT_GAMEPAD_LS_RIGHT_CONFIGDEF:
      return 2;
    case XINPUT_GAMEPAD_RS_X_CONFIGDEF:
    case XINPUT_GAMEPAD_RS_Y_CONFIGDEF:
    case XINPUT_GAMEPAD_RS_UP_CONFIGDEF:
    case XINPUT_GAMEPAD_RS_DOWN_CONFIGDEF:
    case XINPUT_GAMEPAD_RS_LEFT_CONFIGDEF:
    case XINPUT_GAMEPAD_RS_RIGHT_CONFIGDEF:
      return 3;

    default:
      break;
  }

  return 0;
}

#ifdef GAME_PROSTREET
const char* ConvertXInputBitmaskToName(uint16_t in) {
  switch (in) {
    case XINPUT_GAMEPAD_DPAD_UP:
      return "XINPUT_GAMEPAD_DPAD_UP";
    case XINPUT_GAMEPAD_DPAD_DOWN:
      return "XINPUT_GAMEPAD_DPAD_DOWN";
    case XINPUT_GAMEPAD_DPAD_LEFT:
      return "XINPUT_GAMEPAD_DPAD_LEFT";
    case XINPUT_GAMEPAD_DPAD_RIGHT:
      return "XINPUT_GAMEPAD_DPAD_RIGHT";
    case XINPUT_GAMEPAD_START:
      return "XINPUT_GAMEPAD_START";
    case XINPUT_GAMEPAD_BACK:
      return "XINPUT_GAMEPAD_BACK";
    case XINPUT_GAMEPAD_LEFT_THUMB:
      return "XINPUT_GAMEPAD_LEFT_THUMB";
    case XINPUT_GAMEPAD_RIGHT_THUMB:
      return "XINPUT_GAMEPAD_RIGHT_THUMB";
    case XINPUT_GAMEPAD_LEFT_SHOULDER:
      return "XINPUT_GAMEPAD_LEFT_SHOULDER";
    case XINPUT_GAMEPAD_RIGHT_SHOULDER:
      return "XINPUT_GAMEPAD_RIGHT_SHOULDER";
    case XINPUT_GAMEPAD_A:
      return "XINPUT_GAMEPAD_A";
    case XINPUT_GAMEPAD_B:
      return "XINPUT_GAMEPAD_B";
    case XINPUT_GAMEPAD_X:
      return "XINPUT_GAMEPAD_X";
    case XINPUT_GAMEPAD_Y:
      return "XINPUT_GAMEPAD_Y";
    case XINPUT_GAMEPAD_LT_CONFIGDEF:
      return "XINPUT_GAMEPAD_LT";
    case XINPUT_GAMEPAD_RT_CONFIGDEF:
      return "XINPUT_GAMEPAD_RT";
    case XINPUT_GAMEPAD_LS_X_CONFIGDEF:
      return "XINPUT_GAMEPAD_LS_X";
    case XINPUT_GAMEPAD_LS_Y_CONFIGDEF:
      return "XINPUT_GAMEPAD_LS_Y";
    case XINPUT_GAMEPAD_RS_X_CONFIGDEF:
      return "XINPUT_GAMEPAD_RS_X";
    case XINPUT_GAMEPAD_RS_Y_CONFIGDEF:
      return "XINPUT_GAMEPAD_RS_Y";
    case XINPUT_GAMEPAD_LS_UP_CONFIGDEF:
      return "XINPUT_GAMEPAD_LS_UP";
    case XINPUT_GAMEPAD_LS_DOWN_CONFIGDEF:
      return "XINPUT_GAMEPAD_LS_DOWN";
    case XINPUT_GAMEPAD_LS_LEFT_CONFIGDEF:
      return "XINPUT_GAMEPAD_LS_LEFT";
    case XINPUT_GAMEPAD_LS_RIGHT_CONFIGDEF:
      return "XINPUT_GAMEPAD_LS_RIGHT";
    case XINPUT_GAMEPAD_RS_UP_CONFIGDEF:
      return "XINPUT_GAMEPAD_RS_UP";
    case XINPUT_GAMEPAD_RS_DOWN_CONFIGDEF:
      return "XINPUT_GAMEPAD_RS_DOWN";
    case XINPUT_GAMEPAD_RS_LEFT_CONFIGDEF:
      return "XINPUT_GAMEPAD_RS_LEFT";
    case XINPUT_GAMEPAD_RS_RIGHT_CONFIGDEF:
      return "XINPUT_GAMEPAD_RS_RIGHT";
    default:
      return "-1";
  }
}

const char* unboundstr = "---";

const char* ControlsTextsXBOX[] = {"A",
                                   "B",
                                   "X",
                                   "Y",
                                   "LB",
                                   "RB",
                                   "View (Select)",
                                   "Menu (Start)",
                                   "Left stick",
                                   "Right stick",
                                   "D-pad Up",
                                   "D-pad Down",
                                   "D-pad Left",
                                   "D-pad Right",
                                   "Right stick X",
                                   "Right stick Y",
                                   "Right stick Left",
                                   "Right stick Right",
                                   "Right stick Up",
                                   "Right stick Down",
                                   "Left stick X",
                                   "Left stick Y",
                                   "Left stick Left",
                                   "Left stick Right",
                                   "Left stick Up",
                                   "Left stick Down",
                                   "LT",
                                   "RT"};
const char* ControlsTextsPS[]   = {"Cross",
                                   "Circle",
                                   "Square",
                                   "Triangle",
                                   "L1",
                                   "R1",
                                   "Share",
                                   "Options",
                                   "L3",
                                   "R3",
                                   "D-pad Up",
                                   "D-pad Down",
                                   "D-pad Left",
                                   "D-pad Right",
                                   "Right stick X",
                                   "Right stick Y",
                                   "Right stick Left",
                                   "Right stick Right",
                                   "Right stick Up",
                                   "Right stick Down",
                                   "Left stick X",
                                   "Left stick Y",
                                   "Left stick Left",
                                   "Left stick Right",
                                   "Left stick Up",
                                   "Left stick Down",
                                   "L2",
                                   "R2"};
const char* ControlsTextsPS3[]  = {"Cross",
                                   "Circle",
                                   "Square",
                                   "Triangle",
                                   "L1",
                                   "R1",
                                   "Select",
                                   "Start",
                                   "L3",
                                   "R3",
                                   "D-pad Up",
                                   "D-pad Down",
                                   "D-pad Left",
                                   "D-pad Right",
                                   "Right stick X",
                                   "Right stick Y",
                                   "Right stick Left",
                                   "Right stick Right",
                                   "Right stick Up",
                                   "Right stick Down",
                                   "Left stick X",
                                   "Left stick Y",
                                   "Left stick Left",
                                   "Left stick Right",
                                   "Left stick Up",
                                   "Left stick Down",
                                   "L2",
                                   "R2"};
const char* ControlsTextsNSW[]  = {"B",
                                   "A",
                                   "Y",
                                   "X",
                                   "L",
                                   "R",
                                   "Select (-)",
                                   "Start (+)",
                                   "Left stick",
                                   "Right stick",
                                   "D-pad Up",
                                   "D-pad Down",
                                   "D-pad Left",
                                   "D-pad Right",
                                   "Right stick X",
                                   "Right stick Y",
                                   "Right stick Left",
                                   "Right stick Right",
                                   "Right stick Up",
                                   "Right stick Down",
                                   "Left stick X",
                                   "Left stick Y",
                                   "Left stick Left",
                                   "Left stick Right",
                                   "Left stick Up",
                                   "Left stick Down",
                                   "ZL",
                                   "ZR"};

const char* ControlsTextsPC[] = {unboundstr,
                                 "Left Mouse Button",
                                 "Right Mouse Button",
                                 "Break",
                                 "Middle Mouse Button",
                                 "X Mouse Button 1",
                                 "X Mouse Button 2",
                                 "Reserved_0x7",
                                 "Backspace",
                                 "Tab",
                                 "Reserved_0xA",
                                 "Reserved_0xB",
                                 "Clear",
                                 "Enter",
                                 "Reserved_0xE",
                                 "Reserved_0xF",
                                 "L&R Shift",
                                 "L&R Control",
                                 "L&R Alt",
                                 "Pause",
                                 "Caps Lock",
                                 "Kana",
                                 "IME On",
                                 "Junja",
                                 "Final",
                                 "Kanji",
                                 "Reserved_0x1A",
                                 "Esc",
                                 "Convert",
                                 "Non Convert",
                                 "Accept",
                                 "Mode Change",
                                 "Space",
                                 "Page Up",
                                 "Page Down",
                                 "End",
                                 "Home",
                                 "Arrow Left",
                                 "Arrow Up",
                                 "Arrow Right",
                                 "Arrow Down",
                                 "Select",
                                 "Print",
                                 "Execute",
                                 "Print Screen",
                                 "Insert",
                                 "Delete",
                                 "Help",
                                 "0",
                                 "1",
                                 "2",
                                 "3",
                                 "4",
                                 "5",
                                 "6",
                                 "7",
                                 "8",
                                 "9",
                                 "Reserved_0x3A",
                                 "Reserved_0x3B",
                                 "Reserved_0x3C",
                                 "Reserved_0x3D",
                                 "Reserved_0x3E",
                                 "Reserved_0x3F",
                                 "Reserved_0x40",
                                 "A",
                                 "B",
                                 "C",
                                 "D",
                                 "E",
                                 "F",
                                 "G",
                                 "H",
                                 "I",
                                 "J",
                                 "K",
                                 "L",
                                 "M",
                                 "N",
                                 "O",
                                 "P",
                                 "Q",
                                 "R",
                                 "S",
                                 "T",
                                 "U",
                                 "V",
                                 "W",
                                 "X",
                                 "Y",
                                 "Z",
                                 "Left Win",
                                 "Right Win",
                                 "Context Menu",
                                 "Reserved_0x5E",
                                 "Sleep",
                                 "Numpad 0",
                                 "Numpad 1",
                                 "Numpad 2",
                                 "Numpad 3",
                                 "Numpad 4",
                                 "Numpad 5",
                                 "Numpad 6",
                                 "Numpad 7",
                                 "Numpad 8",
                                 "Numpad 9",
                                 "Numpad *",
                                 "Numpad +",
                                 "Separator",
                                 "Numpad -",
                                 "Numpad .",
                                 "Numpad /",
                                 "F1",
                                 "F2",
                                 "F3",
                                 "F4",
                                 "F5",
                                 "F6",
                                 "F7",
                                 "F8",
                                 "F9",
                                 "F10",
                                 "F11",
                                 "F12",
                                 "F13",
                                 "F14",
                                 "F15",
                                 "F16",
                                 "F17",
                                 "F18",
                                 "F19",
                                 "F20",
                                 "F21",
                                 "F22",
                                 "F23",
                                 "F24",
                                 "Reserved_0x88",
                                 "Reserved_0x89",
                                 "Reserved_0x8A",
                                 "Reserved_0x8B",
                                 "Reserved_0x8C",
                                 "Reserved_0x8D",
                                 "Reserved_0x8E",
                                 "Reserved_0x8F",
                                 "Num Lock",
                                 "Scroll Lock",
                                 "Jisho",
                                 "Mashu",
                                 "Touroku",
                                 "Loya",
                                 "Roya",
                                 "Reserved_0x97",
                                 "Reserved_0x98",
                                 "Reserved_0x99",
                                 "Reserved_0x9A",
                                 "Reserved_0x9B",
                                 "Reserved_0x9C",
                                 "Reserved_0x9D",
                                 "Reserved_0x9E",
                                 "Reserved_0x9F",
                                 "Left Shift",
                                 "Right Shift",
                                 "Left Ctrl",
                                 "Right Ctrl",
                                 "Left Alt",
                                 "Right Alt",
                                 "Browser Back",
                                 "Browser Forward",
                                 "Browser Refresh",
                                 "Browser Stop",
                                 "Browser Search",
                                 "Browser Favorites",
                                 "Browser Home",
                                 "Volume Mute",
                                 "Volume Down",
                                 "Volume Up",
                                 "Next Track",
                                 "Previous Track",
                                 "Stop",
                                 "Play / Pause",
                                 "Mail",
                                 "Media",
                                 "App1",
                                 "App2",
                                 "Reserved_0xB8",
                                 "Reserved_0xB9",
                                 "Colon (: ;)",
                                 "Plus (+ =)",
                                 "Comma (< ,)",
                                 "Minus (_ -)",
                                 "Period (> .)",
                                 "Question (? /)",
                                 "Tilde (~ `)",
                                 "Abnt C1",
                                 "Abnt C2",
                                 "Reserved_0xC3",
                                 "Reserved_0xC4",
                                 "Reserved_0xC5",
                                 "Reserved_0xC6",
                                 "Reserved_0xC7",
                                 "Reserved_0xC8",
                                 "Reserved_0xC9",
                                 "Reserved_0xCA",
                                 "Reserved_0xCB",
                                 "Reserved_0xCC",
                                 "Reserved_0xCD",
                                 "Reserved_0xCE",
                                 "Reserved_0xCF",
                                 "Reserved_0xD0",
                                 "Reserved_0xD1",
                                 "Reserved_0xD2",
                                 "Reserved_0xD3",
                                 "Reserved_0xD4",
                                 "Reserved_0xD5",
                                 "Reserved_0xD6",
                                 "Reserved_0xD7",
                                 "Reserved_0xD8",
                                 "Reserved_0xD9",
                                 "Reserved_0xDA",
                                 "Sq.Bracket Open ({ [)",
                                 "Pipe (| \\)",
                                 "Sq.Bracket Close (} ])",
                                 "Quotation (\" ')",
                                 "Section (§ !)",
                                 "Reserved_0xE0",
                                 "Ax",
                                 "<> (\\\\|)",
                                 "IcoHlp",
                                 "Ico00",
                                 "Process",
                                 "IcoClr",
                                 "Packet",
                                 "Reserved_0xE8",
                                 "Reset",
                                 "Jump",
                                 "OemPa1",
                                 "OemPa2",
                                 "OemPa3",
                                 "WsCtrl",
                                 "Cu Sel",
                                 "Oem Attn",
                                 "Finish",
                                 "Copy",
                                 "Auto",
                                 "Enlw",
                                 "Back Tab",
                                 "Attn",
                                 "Cr Sel",
                                 "Ex Sel",
                                 "Er Eof",
                                 "Play",
                                 "Zoom",
                                 "NoName",
                                 "Pa1",
                                 "OemClr"};

const char* VKeyStrings[] = {"VK_NULL",
                             "VK_LBUTTON",
                             "VK_RBUTTON",
                             "VK_CANCEL",
                             "VK_MBUTTON",
                             "VK_XBUTTON1",
                             "VK_XBUTTON2",
                             "VK_0x7",
                             "VK_BACK",
                             "VK_TAB",
                             "VK_0xA",
                             "VK_0xB",
                             "VK_CLEAR",
                             "VK_RETURN",
                             "VK_0xE",
                             "VK_0xF",
                             "VK_SHIFT",
                             "VK_CONTROL",
                             "VK_MENU",
                             "VK_PAUSE",
                             "VK_CAPITAL",
                             "VK_KANA",
                             "VK_IME_ON",
                             "VK_JUNJA",
                             "VK_FINAL",
                             "VK_KANJI",
                             "VK_0x1A",
                             "VK_ESCAPE",
                             "VK_CONVERT",
                             "VK_NONCONVERT",
                             "VK_ACCEPT",
                             "VK_MODECHANGE",
                             "VK_SPACE",
                             "VK_PRIOR",
                             "VK_NEXT",
                             "VK_END",
                             "VK_HOME",
                             "VK_LEFT",
                             "VK_UP",
                             "VK_RIGHT",
                             "VK_DOWN",
                             "VK_SELECT",
                             "VK_PRINT",
                             "VK_EXECUTE",
                             "VK_SNAPSHOT",
                             "VK_INSERT",
                             "VK_DELETE",
                             "VK_HELP",
                             "0",
                             "1",
                             "2",
                             "3",
                             "4",
                             "5",
                             "6",
                             "7",
                             "8",
                             "9",
                             "VK_0x3A",
                             "VK_0x3B",
                             "VK_0x3C",
                             "VK_0x3D",
                             "VK_0x3E",
                             "VK_0x3F",
                             "VK_0x40",
                             "A",
                             "B",
                             "C",
                             "D",
                             "E",
                             "F",
                             "G",
                             "H",
                             "I",
                             "J",
                             "K",
                             "L",
                             "M",
                             "N",
                             "O",
                             "P",
                             "Q",
                             "R",
                             "S",
                             "T",
                             "U",
                             "V",
                             "W",
                             "X",
                             "Y",
                             "Z",
                             "VK_LWIN",
                             "VK_RWIN",
                             "VK_APPS",
                             "VK_0x5E",
                             "VK_SLEEP",
                             "VK_NUMPAD0",
                             "VK_NUMPAD1",
                             "VK_NUMPAD2",
                             "VK_NUMPAD3",
                             "VK_NUMPAD4",
                             "VK_NUMPAD5",
                             "VK_NUMPAD6",
                             "VK_NUMPAD7",
                             "VK_NUMPAD8",
                             "VK_NUMPAD9",
                             "VK_MULTIPLY",
                             "VK_ADD",
                             "VK_SEPARATOR",
                             "VK_SUBTRACT",
                             "VK_DECIMAL",
                             "VK_DIVIDE",
                             "VK_F1",
                             "VK_F2",
                             "VK_F3",
                             "VK_F4",
                             "VK_F5",
                             "VK_F6",
                             "VK_F7",
                             "VK_F8",
                             "VK_F9",
                             "VK_F10",
                             "VK_F11",
                             "VK_F12",
                             "VK_F13",
                             "VK_F14",
                             "VK_F15",
                             "VK_F16",
                             "VK_F17",
                             "VK_F18",
                             "VK_F19",
                             "VK_F20",
                             "VK_F21",
                             "VK_F22",
                             "VK_F23",
                             "VK_F24",
                             "VK_0x88",
                             "VK_0x89",
                             "VK_0x8A",
                             "VK_0x8B",
                             "VK_0x8C",
                             "VK_0x8D",
                             "VK_0x8E",
                             "VK_0x8F",
                             "VK_NUMLOCK",
                             "VK_SCROLL",
                             "VK_OEM_FJ_JISHO",
                             "VK_OEM_FJ_MASSHOU",
                             "VK_OEM_FJ_TOUROKU",
                             "VK_OEM_FJ_LOYA",
                             "VK_OEM_FJ_ROYA",
                             "VK_0x97",
                             "VK_0x98",
                             "VK_0x99",
                             "VK_0x9A",
                             "VK_0x9B",
                             "VK_0x9C",
                             "VK_0x9D",
                             "VK_0x9E",
                             "VK_0x9F",
                             "VK_LSHIFT",
                             "VK_RSHIFT",
                             "VK_LCONTROL",
                             "VK_RCONTROL",
                             "VK_LMENU",
                             "VK_RMENU",
                             "VK_BROWSER_BACK",
                             "VK_BROWSER_FORWARD",
                             "VK_BROWSER_REFRESH",
                             "VK_BROWSER_STOP",
                             "VK_BROWSER_SEARCH",
                             "VK_BROWSER_FAVORITES",
                             "VK_BROWSER_HOME",
                             "VK_VOLUME_MUTE",
                             "VK_VOLUME_DOWN",
                             "VK_VOLUME_UP",
                             "VK_MEDIA_NEXT_TRACK",
                             "VK_MEDIA_PREV_TRACK",
                             "VK_MEDIA_STOP",
                             "VK_MEDIA_PLAY_PAUSE",
                             "VK_LAUNCH_MAIL",
                             "VK_LAUNCH_MEDIA_SELECT",
                             "VK_LAUNCH_APP1",
                             "VK_LAUNCH_APP2",
                             "VK_0xB8",
                             "VK_0xB9",
                             "VK_OEM_1",
                             "VK_OEM_PLUS",
                             "VK_OEM_COMMA",
                             "VK_OEM_MINUS",
                             "VK_OEM_PERIOD",
                             "VK_OEM_2",
                             "VK_OEM_3",
                             "VK_ABNT_C1",
                             "VK_ABNT_C2",
                             "VK_0xC3",
                             "VK_0xC4",
                             "VK_0xC5",
                             "VK_0xC6",
                             "VK_0xC7",
                             "VK_0xC8",
                             "VK_0xC9",
                             "VK_0xCA",
                             "VK_0xCB",
                             "VK_0xCC",
                             "VK_0xCD",
                             "VK_0xCE",
                             "VK_0xCF",
                             "VK_0xD0",
                             "VK_0xD1",
                             "VK_0xD2",
                             "VK_0xD3",
                             "VK_0xD4",
                             "VK_0xD5",
                             "VK_0xD6",
                             "VK_0xD7",
                             "VK_0xD8",
                             "VK_0xD9",
                             "VK_0xDA",
                             "VK_OEM_4",
                             "VK_OEM_5",
                             "VK_OEM_6",
                             "VK_OEM_7",
                             "VK_OEM_8",
                             "VK_0xE0",
                             "VK_OEM_AX",
                             "VK_OEM_102",
                             "VK_ICO_HELP",
                             "VK_ICO_00",
                             "VK_PROCESSKEY",
                             "VK_ICO_CLEAR",
                             "VK_PACKET",
                             "VK_0xE8",
                             "VK_OEM_RESET",
                             "VK_OEM_JUMP",
                             "VK_OEM_PA1",
                             "VK_OEM_PA2",
                             "VK_OEM_PA3",
                             "VK_OEM_WSCTRL",
                             "VK_OEM_CUSEL",
                             "VK_OEM_ATTN",
                             "VK_OEM_FINISH",
                             "VK_OEM_COPY",
                             "VK_OEM_AUTO",
                             "VK_OEM_ENLW",
                             "VK_OEM_BACKTAB",
                             "VK_ATTN",
                             "VK_CRSEL",
                             "VK_EXSEL",
                             "VK_EREOF",
                             "VK_PLAY",
                             "VK_ZOOM",
                             "VK_NONAME",
                             "VK_PA1",
                             "VK_OEM_CLEAR"};

const char* __stdcall ConvertBitmaskToControlString_XBOX(uint16_t in) {
  switch (in) {
    case XINPUT_GAMEPAD_DPAD_UP:
      return ControlsTextsXBOX[10];
    case XINPUT_GAMEPAD_DPAD_DOWN:
      return ControlsTextsXBOX[11];
    case XINPUT_GAMEPAD_DPAD_LEFT:
      return ControlsTextsXBOX[12];
    case XINPUT_GAMEPAD_DPAD_RIGHT:
      return ControlsTextsXBOX[13];
    case XINPUT_GAMEPAD_START:
      return ControlsTextsXBOX[7];
    case XINPUT_GAMEPAD_BACK:
      return ControlsTextsXBOX[6];
    case XINPUT_GAMEPAD_LEFT_THUMB:
      return ControlsTextsXBOX[8];
    case XINPUT_GAMEPAD_RIGHT_THUMB:
      return ControlsTextsXBOX[9];
    case XINPUT_GAMEPAD_LEFT_SHOULDER:
      return ControlsTextsXBOX[4];
    case XINPUT_GAMEPAD_RIGHT_SHOULDER:
      return ControlsTextsXBOX[5];
    case XINPUT_GAMEPAD_A:
      return ControlsTextsXBOX[0];
    case XINPUT_GAMEPAD_B:
      return ControlsTextsXBOX[1];
    case XINPUT_GAMEPAD_X:
      return ControlsTextsXBOX[2];
    case XINPUT_GAMEPAD_Y:
      return ControlsTextsXBOX[3];
    case XINPUT_GAMEPAD_LT_CONFIGDEF:
      return ControlsTextsXBOX[26];
    case XINPUT_GAMEPAD_RT_CONFIGDEF:
      return ControlsTextsXBOX[27];
    case XINPUT_GAMEPAD_LS_X_CONFIGDEF:
      return ControlsTextsXBOX[20];
    case XINPUT_GAMEPAD_LS_Y_CONFIGDEF:
      return ControlsTextsXBOX[21];
    case XINPUT_GAMEPAD_RS_X_CONFIGDEF:
      return ControlsTextsXBOX[14];
    case XINPUT_GAMEPAD_RS_Y_CONFIGDEF:
      return ControlsTextsXBOX[15];
    case XINPUT_GAMEPAD_LS_UP_CONFIGDEF:
      return ControlsTextsXBOX[24];
    case XINPUT_GAMEPAD_LS_DOWN_CONFIGDEF:
      return ControlsTextsXBOX[25];
    case XINPUT_GAMEPAD_LS_LEFT_CONFIGDEF:
      return ControlsTextsXBOX[22];
    case XINPUT_GAMEPAD_LS_RIGHT_CONFIGDEF:
      return ControlsTextsXBOX[23];
    case XINPUT_GAMEPAD_RS_UP_CONFIGDEF:
      return ControlsTextsXBOX[18];
    case XINPUT_GAMEPAD_RS_DOWN_CONFIGDEF:
      return ControlsTextsXBOX[19];
    case XINPUT_GAMEPAD_RS_LEFT_CONFIGDEF:
      return ControlsTextsXBOX[16];
    case XINPUT_GAMEPAD_RS_RIGHT_CONFIGDEF:
      return ControlsTextsXBOX[17];

    default:
      break;
  }
  return unboundstr;
}

const char* __stdcall ConvertBitmaskToControlString_PS(uint16_t in) {
  switch (in) {
    case XINPUT_GAMEPAD_DPAD_UP:
      return ControlsTextsPS[10];
    case XINPUT_GAMEPAD_DPAD_DOWN:
      return ControlsTextsPS[11];
    case XINPUT_GAMEPAD_DPAD_LEFT:
      return ControlsTextsPS[12];
    case XINPUT_GAMEPAD_DPAD_RIGHT:
      return ControlsTextsPS[13];
    case XINPUT_GAMEPAD_START:
      return ControlsTextsPS[7];
    case XINPUT_GAMEPAD_BACK:
      return ControlsTextsPS[6];
    case XINPUT_GAMEPAD_LEFT_THUMB:
      return ControlsTextsPS[8];
    case XINPUT_GAMEPAD_RIGHT_THUMB:
      return ControlsTextsPS[9];
    case XINPUT_GAMEPAD_LEFT_SHOULDER:
      return ControlsTextsPS[4];
    case XINPUT_GAMEPAD_RIGHT_SHOULDER:
      return ControlsTextsPS[5];
    case XINPUT_GAMEPAD_A:
      return ControlsTextsPS[0];
    case XINPUT_GAMEPAD_B:
      return ControlsTextsPS[1];
    case XINPUT_GAMEPAD_X:
      return ControlsTextsPS[2];
    case XINPUT_GAMEPAD_Y:
      return ControlsTextsPS[3];
    case XINPUT_GAMEPAD_LT_CONFIGDEF:
      return ControlsTextsPS[26];
    case XINPUT_GAMEPAD_RT_CONFIGDEF:
      return ControlsTextsPS[27];
    case XINPUT_GAMEPAD_LS_X_CONFIGDEF:
      return ControlsTextsPS[20];
    case XINPUT_GAMEPAD_LS_Y_CONFIGDEF:
      return ControlsTextsPS[21];
    case XINPUT_GAMEPAD_RS_X_CONFIGDEF:
      return ControlsTextsPS[14];
    case XINPUT_GAMEPAD_RS_Y_CONFIGDEF:
      return ControlsTextsPS[15];
    case XINPUT_GAMEPAD_LS_UP_CONFIGDEF:
      return ControlsTextsPS[24];
    case XINPUT_GAMEPAD_LS_DOWN_CONFIGDEF:
      return ControlsTextsPS[25];
    case XINPUT_GAMEPAD_LS_LEFT_CONFIGDEF:
      return ControlsTextsPS[22];
    case XINPUT_GAMEPAD_LS_RIGHT_CONFIGDEF:
      return ControlsTextsPS[23];
    case XINPUT_GAMEPAD_RS_UP_CONFIGDEF:
      return ControlsTextsPS[18];
    case XINPUT_GAMEPAD_RS_DOWN_CONFIGDEF:
      return ControlsTextsPS[19];
    case XINPUT_GAMEPAD_RS_LEFT_CONFIGDEF:
      return ControlsTextsPS[16];
    case XINPUT_GAMEPAD_RS_RIGHT_CONFIGDEF:
      return ControlsTextsPS[17];

    default:
      break;
  }
  return unboundstr;
}

const char* __stdcall ConvertBitmaskToControlString_Step2(uint16_t in, const char** strs) {
  switch (in) {
    case XINPUT_GAMEPAD_DPAD_UP:
      return strs[10];
    case XINPUT_GAMEPAD_DPAD_DOWN:
      return strs[11];
    case XINPUT_GAMEPAD_DPAD_LEFT:
      return strs[12];
    case XINPUT_GAMEPAD_DPAD_RIGHT:
      return strs[13];
    case XINPUT_GAMEPAD_START:
      return strs[7];
    case XINPUT_GAMEPAD_BACK:
      return strs[6];
    case XINPUT_GAMEPAD_LEFT_THUMB:
      return strs[8];
    case XINPUT_GAMEPAD_RIGHT_THUMB:
      return strs[9];
    case XINPUT_GAMEPAD_LEFT_SHOULDER:
      return strs[4];
    case XINPUT_GAMEPAD_RIGHT_SHOULDER:
      return strs[5];
    case XINPUT_GAMEPAD_A:
      return strs[0];
    case XINPUT_GAMEPAD_B:
      return strs[1];
    case XINPUT_GAMEPAD_X:
      return strs[2];
    case XINPUT_GAMEPAD_Y:
      return strs[3];
    case XINPUT_GAMEPAD_LT_CONFIGDEF:
      return strs[26];
    case XINPUT_GAMEPAD_RT_CONFIGDEF:
      return strs[27];
    case XINPUT_GAMEPAD_LS_X_CONFIGDEF:
      return strs[20];
    case XINPUT_GAMEPAD_LS_Y_CONFIGDEF:
      return strs[21];
    case XINPUT_GAMEPAD_RS_X_CONFIGDEF:
      return strs[14];
    case XINPUT_GAMEPAD_RS_Y_CONFIGDEF:
      return strs[15];
    case XINPUT_GAMEPAD_LS_UP_CONFIGDEF:
      return strs[24];
    case XINPUT_GAMEPAD_LS_DOWN_CONFIGDEF:
      return strs[25];
    case XINPUT_GAMEPAD_LS_LEFT_CONFIGDEF:
      return strs[22];
    case XINPUT_GAMEPAD_LS_RIGHT_CONFIGDEF:
      return strs[23];
    case XINPUT_GAMEPAD_RS_UP_CONFIGDEF:
      return strs[18];
    case XINPUT_GAMEPAD_RS_DOWN_CONFIGDEF:
      return strs[19];
    case XINPUT_GAMEPAD_RS_LEFT_CONFIGDEF:
      return strs[16];
    case XINPUT_GAMEPAD_RS_RIGHT_CONFIGDEF:
      return strs[17];

    default:
      break;
  }
  return unboundstr;
}

const char* __stdcall ConvertBitmaskToControlString(int mode, uint16_t in) {
  switch (mode) {
    case CONTROLLERICON_PS4:
      return ConvertBitmaskToControlString_Step2(in, ControlsTextsPS);
      // return ConvertBitmaskToControlString_PS(in);
    case CONTROLLERICON_PS3:
      return ConvertBitmaskToControlString_Step2(in, ControlsTextsPS3);
    case CONTROLLERICON_SWITCH:
      return ConvertBitmaskToControlString_Step2(in, ControlsTextsNSW);
    case CONTROLLERICON_XB360MW:
    case CONTROLLERICON_XBOX360:
    case CONTROLLERICON_XBOXONE:
    default:
      return ConvertBitmaskToControlString_Step2(in, ControlsTextsXBOX);
      // return ConvertBitmaskToControlString_XBOX(in);
  }
}

vector<ActionID> PCRemapActionList = {GAMEACTION_GAS,
                                      GAMEACTION_BRAKE,
                                      GAMEACTION_STEERLEFT,
                                      GAMEACTION_STEERRIGHT,
                                      GAMEACTION_HANDBRAKE,
                                      GAMEACTION_NOS,
                                      GAMEACTION_SHIFTDOWN,
                                      GAMEACTION_SHIFTUP,
                                      GAMEACTION_CLUTCH,
                                      GAMEACTION_SHIFTFIRST,
                                      GAMEACTION_SHIFTSECOND,
                                      GAMEACTION_SHIFTTHIRD,
                                      GAMEACTION_SHIFTFOURTH,
                                      GAMEACTION_SHIFTFIFTH,
                                      GAMEACTION_SHIFTSIXTH,
                                      GAMEACTION_SHIFTREVERSE,
                                      GAMEACTION_SHIFTNEUTRAL,
                                      GAMEACTION_RESET,
                                      HUDACTION_TOGGLE_LEADERBOARD,
                                      HUDACTION_TOGGLE_MINIMAP,
                                      HUDACTION_TOGGLE_BESTLINE,
                                      HUDACTION_TOGGLE_ARROWS,
                                      CAMERAACTION_CHANGE,
                                      CAMERAACTION_LOOKBACK,
                                      DEBUGACTION_SCREENSHOT,
                                      HUDACTION_PAUSEREQUEST};

ActionID FindPCRemapActionID(uint32_t idx) {
  if (idx > PCRemapActionList.size()) return NULL_ACTION;
  return PCRemapActionList.at(idx);
}

#endif
