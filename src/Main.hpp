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

// TODO: bring rumble/vibration function
// TODO: remapping? -- partially done, but only 1 event per key
// TODO: kill DInput enough so that it doesn't detect XInput controllers but still detects wheels
// TODO: proper raw input for keyboard (and maybe non XInput gamepads?)
// TODO: implement the Controls settings menu - it should be possible to make it talk to the INI
// TODO: loading screen control tips
// TODO (MW): Max performance button is visible in all submenus during Customize for some reason...
// TODO (MW): CUSTOMIZE MENU IS BUGGY - during career cash status overlaps the max performance buttons...
// TODO (Carbon): fix mouse wheel zooming (in FE only) and keyboard zooming during photo mode
// TODO (Carbon): ingame button prompt during tutorial - find the texture hash for that and replace it
// TODO (Undercover): UI texture switching & maybe higher res support - it was excluded because UI is working with APT and not FEng (only the HUD works on FEng)
// TODO (Pro Street): multiplayer menu UI shows some missing strings on console objects... also shows too many of them at the end of race so that needs
// investigating. Tested in LAN mode.
// TODO (World): IT'S RANDOM! It sometimes works, sometimes not... I'm speechless at the moment. Currently no idea if even the powerups trigger on the D-Pad at
// all! (because I have no way to test it currently! I'm testing with Berkay's old Offline Server which has broken powerups.)
// TODO (World): make proper config, currently using a mostly copy-paste from Carbon...
// TODO (World): mouselook, maybe

#include "stdafx.h"
#include "stdio.h"
#include <windows.h>
#include <injector\injector.hpp>
#include <IniReader.h>
#include "NFS_XtendedInput.h"
#include "NFS_XtentedInput_ActionID.h"

#ifdef XINPUT_OLD
#include <XInput.h>
#pragma comment(lib, "xinput9_1_0.lib")
#else
#include <XInput.h>
#pragma comment(lib, "xinput.lib")
#endif

#include "NFS_XtendedInput_XInputConfig.h"
#include "NFS_XtendedInput_VKHash.h"

#include "Definitions.h"
#ifdef GAME_CARBON
float FEActivationFloat = 0.999999f;
#endif
#ifdef GAME_PROSTREET
float FEActivationFloat = 0.999999f;
#endif

#ifndef NO_FENG
#include "NFS_XtendedInput_FEng.h"
#endif

#define MAX_CONTROLLERS 4  // XInput handles up to 4 controllers

SHORT INPUT_DEADZONE_LS = (0.24f * std::numeric_limits<SHORT>::max());  
SHORT INPUT_DEADZONE_RS = (0.24f * std::numeric_limits<SHORT>::max());
SHORT INPUT_DEADZONE_LS_P2 = (0.24f * std::numeric_limits<SHORT>::max());
SHORT INPUT_DEADZONE_RS_P2 = (0.24f * std::numeric_limits<SHORT>::max());
SHORT SHIFT_ANALOG_THRESHOLD       = (0.75f * std::numeric_limits<SHORT>::max());  // 75% for shifting
SHORT FEUPDOWN_ANALOG_THRESHOLD    = (0.50f * std::numeric_limits<SHORT>::max());  // 50% for analog sticks digital activation
SHORT TRIGGER_ACTIVATION_THRESHOLD = (0.12f * std::numeric_limits<SHORT>::max());  // 12% for analog triggers digital activation

// for triggering the over-zelaous inputs once in a tick...
WORD bQuitButtonOldState = 0;

struct CONTROLLER_STATE {
  XINPUT_STATE state;
  bool         bConnected;
} g_Controllers[MAX_CONTROLLERS];

bool bXInputOmniMode = false;
bool bPassConnStatus = true;

// KB Input declarations
BYTE         VKeyStates[2][256];
unsigned int KeyboardReadingMode = 0;  // 0 = unbuffered asynchronous, 1 = buffered synchronous
bool         bMouseLook          = true;

void*(__thiscall* UTL_Com_Object_IList_Constructor)(void* thethis, unsigned int unk) = (void*(__thiscall*)(void*, unsigned int))UTL_ILIST_CONSTRUCTOR_ADDR;
void(__thiscall* DebugWorldCameraMover_Update)(void* thethis, float unk)             = (void(__thiscall*)(void*, float))DebugWorldCameraMover_Update_Addr;

void*(__thiscall* DebugWorldCameraMover_Constructor)(void* thethis, void* vector1, void* vector2, int joyport,
                                                     int unk)      = (void*(__thiscall*)(void*, void*, void*, int, int))DEBUGWORLDCAMERAMOVER_CONSTRUCTOR_ADDR;
void*(__thiscall* DebugWorldCameraMover_Destructor)(void* thethis) = (void*(__thiscall*)(void*))DEBUGWORLDCAMERAMOVER_DESTRUCTOR_ADDR;

int bStringHash(char* str) {
  if (str == NULL) return 0;

  char* _str   = ((char*)str);
  int   result = -1;

  while (*_str != 0) {
    result = result * 0x21 + (unsigned int)(*_str);
    _str   = _str + 1;
  }

  return result;
}

// global var for the action ID values
float PrevValues[2][MAX_ACTIONID];
float CurrValues[2][MAX_ACTIONID];
// button bindings per action ID
WORD XInputBindings_PRIMARY[MAX_ACTIONID];
WORD XInputBindings_SECONDARY[MAX_ACTIONID];
// secondary bindings, exclusively for FE navigation with left stick as well (not just D-Pad)
WORD FE_Secondary_Up;
WORD FE_Secondary_Down;
WORD FE_Secondary_Left;
WORD FE_Secondary_Right;

BYTE VKeyBindings_PRIMARY[MAX_ACTIONID];
BYTE VKeyBindings_SECONDARY[MAX_ACTIONID];

// mouselook vars
int   MouseLook_XSpeed     = 0;
int   MouseLook_YSpeed     = 0;
float MouseLookSensitivity = 1.0f;

bool bEnteredWorldMapOnce = false;
#ifdef GAME_PROSTREET
bool     g_bRemapClear           = false;
uint32_t g_bRemapClear_IsPrimary = std::numeric_limits<std::uint32_t>::max();
uint32_t g_iRemapClear_BtnIndex  = std::numeric_limits<std::uint32_t>::max();
#endif

enum DeviceScalarType {
  kJoyAxis       = 0,
  kAnalogButton  = 1,
  kDigitalButton = 2,
};

struct DeviceScalarInfo {
  const char*      name;
  DeviceScalarType type;
  int              system_index;
  int              space[10];  // space for multiple binds?
};

class DeviceScalar {
 public:
  DeviceScalarType fType;
  unsigned int     fName_CRC32;
  float*           fPrevValue;
  float*           fCurrentValue;
  DeviceScalar() {
    fType         = (DeviceScalarType)0;
    fName_CRC32   = 0;
    fPrevValue    = NULL;
    fCurrentValue = NULL;
  }
};

enum ShiftPotential {
  SHIFT_POTENTIAL_NONE    = 0,
  SHIFT_POTENTIAL_DOWN    = 1,
  SHIFT_POTENTIAL_UP      = 2,
  SHIFT_POTENTIAL_GOOD    = 3,
  SHIFT_POTENTIAL_PERFECT = 4,
  SHIFT_POTENTIAL_MISS    = 5,
};

enum InputUpdateType {
  kUpdate        = 0,
  kPress         = 1,
  kRelease       = 2,
  kAnalogPress   = 3,
  kAnalogRelease = 4,
  kCenterControl = 5,
};

struct InputMapEntry {
  InputUpdateType UpdateType;
  float           LowerDZ;
  float           UpperDZ;
  ActionID        Action;
  int             DeviceScalarIndex;
#ifndef GAME_MW
  int ComboDeviceScalarIndex;
#endif
  float PreviousValue;
  float CurrentValue;
};

HRESULT UpdateControllerState() {
  DWORD dwResult;

  if (bXInputOmniMode) {
    XINPUT_STATE omni_state = {0};
    bool         bConnStatus = false;

    for (int i = 0; i < MAX_CONTROLLERS; i++) {
      dwResult = XInputGetState(i, &g_Controllers[i].state);
      if (dwResult == ERROR_SUCCESS) {
        g_Controllers[0].bConnected = true; // not a bug
        g_Controllers[1].bConnected = true;

        // Zero value if thumbsticks are within the dead zone
        auto& state = g_Controllers[i].state;
        if (std::abs(state.Gamepad.sThumbLX) < INPUT_DEADZONE_LS) state.Gamepad.sThumbLX = 0;
        if (std::abs(state.Gamepad.sThumbLY) < INPUT_DEADZONE_LS) state.Gamepad.sThumbLY = 0;
        if (std::abs(state.Gamepad.sThumbRX) < INPUT_DEADZONE_RS) state.Gamepad.sThumbRX = 0;
        if (std::abs(state.Gamepad.sThumbRY) < INPUT_DEADZONE_RS) state.Gamepad.sThumbRY = 0;

        if ((state.Gamepad.wButtons || state.Gamepad.sThumbLX || state.Gamepad.sThumbLY || state.Gamepad.sThumbRX || state.Gamepad.sThumbRY ||
             state.Gamepad.bRightTrigger || state.Gamepad.bLeftTrigger)) {
          LastControlledDevice = LASTCONTROLLED_CONTROLLER;
#ifndef NO_FENG
          if (bUseDynamicFEngSwitching) bConsoleFEng = true;
#endif

          // combine states into one
          omni_state.Gamepad.wButtons |= state.Gamepad.wButtons;
          omni_state.Gamepad.sThumbLX |= state.Gamepad.sThumbLX;
          omni_state.Gamepad.sThumbLY |= state.Gamepad.sThumbLY;
          omni_state.Gamepad.sThumbRX |= state.Gamepad.sThumbRX;
          omni_state.Gamepad.sThumbRY |= state.Gamepad.sThumbRY;
          omni_state.Gamepad.bRightTrigger |= state.Gamepad.bRightTrigger;
          omni_state.Gamepad.bLeftTrigger |= state.Gamepad.bLeftTrigger;
        }
      }
    }
    memcpy(&(g_Controllers[0].state), &omni_state, sizeof(XINPUT_STATE));
    memcpy(&(g_Controllers[1].state), &omni_state, sizeof(XINPUT_STATE));
  }

  else {
    dwResult = XInputGetState(0, &g_Controllers[0].state);

    if (dwResult == ERROR_SUCCESS) {
      g_Controllers[0].bConnected = true;

      // Zero value if thumbsticks are within the dead zone
      auto& state = g_Controllers[0].state;
      if (std::abs(state.Gamepad.sThumbLX) < INPUT_DEADZONE_LS) state.Gamepad.sThumbLX = 0;
      if (std::abs(state.Gamepad.sThumbLY) < INPUT_DEADZONE_LS) state.Gamepad.sThumbLY = 0;
      if (std::abs(state.Gamepad.sThumbRX) < INPUT_DEADZONE_RS) state.Gamepad.sThumbRX = 0;
      if (std::abs(state.Gamepad.sThumbRY) < INPUT_DEADZONE_RS) state.Gamepad.sThumbRY = 0;

      if ((state.Gamepad.wButtons || state.Gamepad.sThumbLX || state.Gamepad.sThumbLY || state.Gamepad.sThumbRX || state.Gamepad.sThumbRY ||
           state.Gamepad.bRightTrigger || state.Gamepad.bLeftTrigger)) {
        LastControlledDevice = LASTCONTROLLED_CONTROLLER;
#ifndef NO_FENG
        if (bUseDynamicFEngSwitching) bConsoleFEng = true;
#endif
      }

    } else
      g_Controllers[0].bConnected = false;

    dwResult = XInputGetState(1, &g_Controllers[1].state);

    if (dwResult == ERROR_SUCCESS) {
      g_Controllers[1].bConnected = true;

      // Zero value if thumbsticks are within the dead zone
      auto& state = g_Controllers[1].state;
      if (std::abs(state.Gamepad.sThumbLX) < INPUT_DEADZONE_LS) state.Gamepad.sThumbLX = 0;
      if (std::abs(state.Gamepad.sThumbLY) < INPUT_DEADZONE_LS) state.Gamepad.sThumbLY = 0;
      if (std::abs(state.Gamepad.sThumbRX) < INPUT_DEADZONE_RS) state.Gamepad.sThumbRX = 0;
      if (std::abs(state.Gamepad.sThumbRY) < INPUT_DEADZONE_RS) state.Gamepad.sThumbRY = 0;
    } else {
      g_Controllers[1].bConnected = false;
    }
  }
  return S_OK;
}

// old function -- repurposed to send keyboard-exclusive commands to the game
void ReadXInput_Extra() {
#ifndef NO_QUIT_BUTTON
  if (g_Controllers[0].bConnected && bGlobalDoPolling) {
    WORD wButtons = g_Controllers[0].state.Gamepad.wButtons;

    if ((wButtons & XINPUT_GAMEPAD_BACK) != bQuitButtonOldState) {
      if ((wButtons & XINPUT_GAMEPAD_BACK))  // trigger once only on button down state
      {
        FESendKeystroke('Q');
      }
      bQuitButtonOldState = (wButtons & XINPUT_GAMEPAD_BACK);
    }

#ifdef GAME_MW
    // simulate mouse cursor for map movement
    if (*(int*)GAMEFLOWMANAGER_STATUS_ADDR == 6) {
      if (g_Controllers[0].state.Gamepad.sThumbRX || g_Controllers[0].state.Gamepad.sThumbRY) bLastUsedVirtualMouse = true;

      if (bLastUsedVirtualMouse && LastControlledDevice == LASTCONTROLLED_CONTROLLER && cFEng_FindPackage("WorldMapMain.fng")) {
        if (bUseWin32Cursor) {
          if (!bEnteredWorldMapOnce && !bConfineMouse)  // force confine a mouse if not already, we don't want the mouse to stray away from the game if we're
                                                        // using a controller
          {
            RECT windowRect;
            GetWindowRect(*(HWND*)GAME_HWND_ADDR, &windowRect);
            ClipCursor(&windowRect);
          }

          POINT MousePos;
          GetCursorPos(&MousePos);
          SetCursorPos(MousePos.x + ((g_Controllers[0].state.Gamepad.sThumbRX) / 0x1000), MousePos.y + (-g_Controllers[0].state.Gamepad.sThumbRY / 0x1000));
        } else {
          *(int*)FEMOUSECURSOR_X_ADDR += (g_Controllers[0].state.Gamepad.sThumbRX / 0x1000);
          *(int*)FEMOUSECURSOR_Y_ADDR += (-g_Controllers[0].state.Gamepad.sThumbRY / 0x1000);
        }

        bMousePressedDown    = g_Controllers[0].state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER;
        bEnteredWorldMapOnce = true;
      } else {
        if (bEnteredWorldMapOnce && !bConfineMouse && bUseWin32Cursor) ClipCursor(NULL);
        bEnteredWorldMapOnce  = false;
        bLastUsedVirtualMouse = false;
      }
    } else {
      if (bEnteredWorldMapOnce && !bConfineMouse && bUseWin32Cursor) ClipCursor(NULL);
      bEnteredWorldMapOnce  = false;
      bLastUsedVirtualMouse = false;
    }
#endif
  }
#endif  // GAME_UC
}

void __stdcall ReadControllerData() {
  UpdateControllerState();
  ReadXInput_Extra();
  if (KeyboardReadingMode == KB_READINGMODE_BUFFERED) GetKeyboardState(VKeyStates[0]);

  // read mouse info for Debug World Camera
  if (bInDebugWorldCamera && bGlobalDoPolling) {
    RECT  windowRect;
    POINT MousePos;
    GetCursorPos(&MousePos);
    GetWindowRect(*(HWND*)GAME_HWND_ADDR, &windowRect);
    int CenterX = ((windowRect.right - windowRect.left) / 2) + windowRect.left;
    int CenterY = ((windowRect.bottom - windowRect.top) / 2) + windowRect.top;

#ifndef GAME_WORLD
    // add mouse speed to the original speed
    MouseLook_XSpeed = -(int)((float)((MousePos.x - CenterX) * 1000) * MouseLookSensitivity);
    MouseLook_YSpeed = -(int)((float)((MousePos.y - CenterY) * 1000) * MouseLookSensitivity);
#else
    MouseLook_XSpeed = -(int)((float)((MousePos.x - CenterX) * 100) * MouseLookSensitivity);
    MouseLook_YSpeed = -(int)((float)((MousePos.y - CenterY) * 100) * MouseLookSensitivity);
#endif
    SetCursorPos(CenterX, CenterY);
  }
}

#ifndef GAME_WORLD
bool bIsHudVisible() {
  if (*(int*)GAMEFLOWMANAGER_STATUS_ADDR == 6) {
    int FirstLocalPlayer = **(int**)PLAYER_ILISTABLE_ADDR;
    if (FirstLocalPlayer) {
      int LocalPlayerVtable                           = *(int*)(FirstLocalPlayer);
      int(__thiscall * LocalPlayer_GetHUD)(void* dis) = (int(__thiscall*)(void*)) * (int*)(LocalPlayerVtable + PLAYER_GETHUD_VTABLE_OFFSET);
      int LocalPlayerHUD                              = LocalPlayer_GetHUD((void*)FirstLocalPlayer);
      if (LocalPlayerHUD) {
        int LocalPlayerHUDVtable                          = *(int*)(LocalPlayerHUD);
        int(__thiscall * FEngHud_IsHudVisible)(void* dis) = (int(__thiscall*)(void*)) * (int*)(LocalPlayerHUDVtable + FENGHUD_ISVISIBLE_VTABLE_OFFSET);

        if (FEngHud_IsHudVisible((void*)LocalPlayerHUD))  // is HUD being drawn at all
          return true;
      }
    }
  }
  return false;
}
#else
bool bIsHudVisible() { return false; }

// world, for whatever reason, uses direct pointers to the class instead of accessing the device scalar array normally
// copied this code snippet from Carbon 0x00696166
unsigned int loc_75E375 = 0x75E375 + MainBase;
unsigned int loc_75E363 = 0x75E363 + MainBase;
void __declspec(naked) UnscrewWorldDeviceScalars() {
  _asm
  {
		mov ecx, [ebx+0x18]
		shl esi, 4
		add esi, ecx
		test esi, esi
		jnz unscrew_jnz
		jmp loc_75E363
		unscrew_jnz:
		jmp loc_75E375
  }
}

unsigned int loc_75E237 = 0x75E237 + MainBase;
unsigned int loc_75E225 = 0x75E225 + MainBase;
void __declspec(naked) UnscrewWorldDeviceScalars2() {
  _asm
  {
		mov ecx, [ebx + 0x18]
		shl edi, 4
		add edi, ecx
		mov [ebp-8], edi
		test edi, edi
		jnz unscrew2_jnz
		jmp loc_75E225
		unscrew2_jnz :
		jmp loc_75E237
  }
}

unsigned int loc_75E25F = 0x75E25F + MainBase;
void __declspec(naked) UnscrewWorldDeviceScalars3() {
  _asm
  {
		mov edx, [ebx + 0x18]
		shl edi, 4
		add edi, edx
		jmp loc_75E25F
  }
}

#endif
bool bCheckSecondBind() { return FE_Secondary_Up || FE_Secondary_Down || FE_Secondary_Left || FE_Secondary_Right; }

float GetAnalogStickValue(int index, WORD bind) {
  float result = 0.0f;
  int   rdi    = index;
  switch (bind) {
    case XINPUT_GAMEPAD_LT_CONFIGDEF:
      result = g_Controllers[rdi].state.Gamepad.bLeftTrigger / 255.0f;
      break;
    case XINPUT_GAMEPAD_RT_CONFIGDEF:
      result = g_Controllers[rdi].state.Gamepad.bRightTrigger / 255.0f;
      break;
    case XINPUT_GAMEPAD_LS_X_CONFIGDEF:
      result = g_Controllers[rdi].state.Gamepad.sThumbLX / (float)(0x7FFF);
      break;
    case XINPUT_GAMEPAD_RS_X_CONFIGDEF:
      result = g_Controllers[rdi].state.Gamepad.sThumbRX / (float)(0x7FFF);
      break;
    case XINPUT_GAMEPAD_LS_Y_CONFIGDEF:
      result = g_Controllers[rdi].state.Gamepad.sThumbLY / (float)(0x7FFF);
      break;
    case XINPUT_GAMEPAD_RS_Y_CONFIGDEF:
      result = g_Controllers[rdi].state.Gamepad.sThumbRY / (float)(0x7FFF);
      break;
    case XINPUT_GAMEPAD_LS_UP_CONFIGDEF:
      if (g_Controllers[rdi].state.Gamepad.sThumbLY > 0) result = g_Controllers[rdi].state.Gamepad.sThumbLY / (float)(0x7FFF);
      break;
    case XINPUT_GAMEPAD_LS_DOWN_CONFIGDEF:
      if (g_Controllers[rdi].state.Gamepad.sThumbLY < 0) result = -g_Controllers[rdi].state.Gamepad.sThumbLY / (float)(0x7FFF);
      break;
    case XINPUT_GAMEPAD_LS_LEFT_CONFIGDEF:
      if (g_Controllers[rdi].state.Gamepad.sThumbLX < 0) result = -g_Controllers[rdi].state.Gamepad.sThumbLX / (float)(0x7FFF);
      break;
    case XINPUT_GAMEPAD_LS_RIGHT_CONFIGDEF:
      if (g_Controllers[rdi].state.Gamepad.sThumbLX > 0) result = g_Controllers[rdi].state.Gamepad.sThumbLX / (float)(0x7FFF);
      break;
    case XINPUT_GAMEPAD_RS_UP_CONFIGDEF:
      if (g_Controllers[rdi].state.Gamepad.sThumbRY > 0) result = g_Controllers[rdi].state.Gamepad.sThumbRY / (float)(0x7FFF);
      break;
    case XINPUT_GAMEPAD_RS_DOWN_CONFIGDEF:
      if (g_Controllers[rdi].state.Gamepad.sThumbRY < 0) result = -g_Controllers[rdi].state.Gamepad.sThumbRY / (float)(0x7FFF);
      break;
    case XINPUT_GAMEPAD_RS_LEFT_CONFIGDEF:
      if (g_Controllers[rdi].state.Gamepad.sThumbRX < 0) result = -g_Controllers[rdi].state.Gamepad.sThumbRX / (float)(0x7FFF);
      break;
    case XINPUT_GAMEPAD_RS_RIGHT_CONFIGDEF:
      if (g_Controllers[rdi].state.Gamepad.sThumbRX > 0) result = g_Controllers[rdi].state.Gamepad.sThumbRX / (float)(0x7FFF);
      break;
    default:
      break;
  }
  return result;
}

uint16_t GetAnalogActivity() {
  if (GetAnalogStickValue(0, XINPUT_GAMEPAD_LT_CONFIGDEF) >= 1.0f) return XINPUT_GAMEPAD_LT_CONFIGDEF;
  if (GetAnalogStickValue(0, XINPUT_GAMEPAD_RT_CONFIGDEF) >= 1.0f) return XINPUT_GAMEPAD_RT_CONFIGDEF;
  if (GetAnalogStickValue(0, XINPUT_GAMEPAD_LS_UP_CONFIGDEF) >= 1.0f) return XINPUT_GAMEPAD_LS_UP_CONFIGDEF;
  if (GetAnalogStickValue(0, XINPUT_GAMEPAD_LS_DOWN_CONFIGDEF) >= 1.0f) return XINPUT_GAMEPAD_LS_DOWN_CONFIGDEF;
  if (GetAnalogStickValue(0, XINPUT_GAMEPAD_LS_LEFT_CONFIGDEF) >= 1.0f) return XINPUT_GAMEPAD_LS_LEFT_CONFIGDEF;
  if (GetAnalogStickValue(0, XINPUT_GAMEPAD_LS_RIGHT_CONFIGDEF) >= 1.0f) return XINPUT_GAMEPAD_LS_RIGHT_CONFIGDEF;
  if (GetAnalogStickValue(0, XINPUT_GAMEPAD_RS_UP_CONFIGDEF) >= 1.0f) return XINPUT_GAMEPAD_RS_UP_CONFIGDEF;
  if (GetAnalogStickValue(0, XINPUT_GAMEPAD_RS_DOWN_CONFIGDEF) >= 1.0f) return XINPUT_GAMEPAD_RS_DOWN_CONFIGDEF;
  if (GetAnalogStickValue(0, XINPUT_GAMEPAD_RS_LEFT_CONFIGDEF) >= 1.0f) return XINPUT_GAMEPAD_RS_LEFT_CONFIGDEF;
  if (GetAnalogStickValue(0, XINPUT_GAMEPAD_RS_RIGHT_CONFIGDEF) >= 1.0f) return XINPUT_GAMEPAD_RS_RIGHT_CONFIGDEF;
  return 0;
}

bool bGetAnalogDigitalActivationState(int index, WORD bind, int actid) {
  WORD threshold = FEUPDOWN_ANALOG_THRESHOLD;
  if (actid == GAMEACTION_SHIFTUP || actid == GAMEACTION_SHIFTDOWN) threshold = SHIFT_ANALOG_THRESHOLD;

  bool bCurrentXInputKeyState = false;
  int  rdi                    = index;
  switch (bind) {
    case XINPUT_GAMEPAD_LT_CONFIGDEF:
      bCurrentXInputKeyState = (g_Controllers[rdi].state.Gamepad.bLeftTrigger > TRIGGER_ACTIVATION_THRESHOLD);
      break;
    case XINPUT_GAMEPAD_RT_CONFIGDEF:
      bCurrentXInputKeyState = (g_Controllers[rdi].state.Gamepad.bRightTrigger > TRIGGER_ACTIVATION_THRESHOLD);
      break;
    case XINPUT_GAMEPAD_LS_UP_CONFIGDEF:
      bCurrentXInputKeyState = (g_Controllers[rdi].state.Gamepad.sThumbLY > threshold);
      break;
    case XINPUT_GAMEPAD_LS_DOWN_CONFIGDEF:
      bCurrentXInputKeyState = (g_Controllers[rdi].state.Gamepad.sThumbLY < -threshold);
      break;
    case XINPUT_GAMEPAD_LS_LEFT_CONFIGDEF:
      bCurrentXInputKeyState = (g_Controllers[rdi].state.Gamepad.sThumbLX < -threshold);
      break;
    case XINPUT_GAMEPAD_LS_RIGHT_CONFIGDEF:
      bCurrentXInputKeyState = (g_Controllers[rdi].state.Gamepad.sThumbLX > threshold);
      break;
    case XINPUT_GAMEPAD_RS_UP_CONFIGDEF:
      bCurrentXInputKeyState = (g_Controllers[rdi].state.Gamepad.sThumbRY > threshold);
      break;
    case XINPUT_GAMEPAD_RS_DOWN_CONFIGDEF:
      bCurrentXInputKeyState = (g_Controllers[rdi].state.Gamepad.sThumbRY < -threshold);
      break;
    case XINPUT_GAMEPAD_RS_LEFT_CONFIGDEF:
      bCurrentXInputKeyState = (g_Controllers[rdi].state.Gamepad.sThumbRX < -threshold);
      break;
    case XINPUT_GAMEPAD_RS_RIGHT_CONFIGDEF:
      bCurrentXInputKeyState = (g_Controllers[rdi].state.Gamepad.sThumbRX > threshold);
      break;
  }

  return bCurrentXInputKeyState;
}

// Mouselook stuff start

#pragma runtime_checks("", off)
float DebugWorldCameraMover_16bit_max  = 65536.0f;
bool  bDisableDebugWorldCameraUpdating = true;

// caves necessary as original values are in 16-bit values, this extends it to 32-bits

int DebugWorldCameraMover_Cave1_Exit     = DEBUGWORLDCAMERAMOVER_CAVE1_EXIT;
int DebugWorldCameraMover_Cave1_FullExit = DEBUGWORLDCAMERAMOVER_CAVE1_FULLEXIT;
void __declspec(naked) DebugWorldCameraMover_Cave1() {
  // clang-format off
  if (bDisableDebugWorldCameraUpdating) _asm jmp DebugWorldCameraMover_Cave1_FullExit;
#ifndef DEBUGWORLDCAMERAMOVER_MMX
  _asm {
      mov eax, MouseLook_XSpeed
#ifdef DEBUGWORLDCAMERAMOVER_FSTP
      fstp st
#endif
      mov[esp + DEBUGWORLDCAMERAMOVER_STACK_OFFSET], eax
      jmp DebugWorldCameraMover_Cave1_Exit
  }
#else  // NFS Undercover and World use the MMX registers and SSE2 instructions extensively
#ifdef GAME_UC
  _asm {
      movss xmm2, dword ptr[esp + 0x28]
      cvtps2pd xmm0, xmm0
      mov eax, MouseLook_XSpeed
      cvtsi2ss xmm1, eax
      jmp DebugWorldCameraMover_Cave1_Exit
  }
#endif
#ifdef GAME_WORLD
  _asm {
    movss xmm1, DebugWorldCameraMover_16bit_max
    movss xmm0, dword ptr[ebp - 0x18]
    mulss xmm0, xmm1 
    cvttss2si eax, xmm0
    movss xmm0, dword ptr[ebp - 0x20]
    movaps xmm2, xmm0
    mulss xmm2, xmm0
    movss xmm0, dword ptr[ebp - 0x1C]
    movaps xmm3, xmm0 
    mulss xmm3, xmm0
    addss xmm2, xmm3
    mov ecx, MouseLook_XSpeed
    add edi, ecx
    jmp DebugWorldCameraMover_Cave1_Exit
  }
#endif
#endif
  // clang-format on
}

int DebugWorldCameraMover_Cave2_Exit = DEBUGWORLDCAMERAMOVER_CAVE2_EXIT;
void __declspec(naked) DebugWorldCameraMover_Cave2() {
#ifndef DEBUGWORLDCAMERAMOVER_MMX
  // clang-format off
  _asm
  {
		mov eax, MouseLook_YSpeed
		mov	[esp+DEBUGWORLDCAMERAMOVER_STACK_OFFSET2], eax
		jmp DebugWorldCameraMover_Cave2_Exit
  }
#else  // NFS Undercover and World use the MMX registers and SSE2 instructions extensively
#ifdef GAME_UC
  _asm
  {
		mov edx, MouseLook_YSpeed
		movzx ecx, ax
		cvtsi2ss xmm0, edx
		jmp DebugWorldCameraMover_Cave2_Exit
  }
#endif
#ifdef GAME_WORLD
  _asm { 
    add eax, MouseLook_YSpeed
    mov[ebp - 4], eax
    jmp DebugWorldCameraMover_Cave2_Exit 
  }
#endif
#endif
  // clang-format on
}

// during DebugWorldCameraMover - set the cursor position to the center of the window for relative calculations
void __stdcall DebugWorldCameraMover_Update_Hook(float unk) {
  unsigned int thethis;
  _asm mov     thethis, ecx;

  // add original speeds
  MouseLook_XSpeed += *(short*)(thethis + DEBUGWORLDCAMERAMOVER_XSPEED_OFFSET);
  MouseLook_YSpeed += *(short*)(thethis + DEBUGWORLDCAMERAMOVER_YSPEED_OFFSET);

#ifndef DEBUGWORLDCAMERAMOVER_MMX
  if (!MouseLook_XSpeed && !MouseLook_YSpeed && !CurrValues[0][DEBUGACTION_MOVE_UP] && !CurrValues[0][DEBUGACTION_MOVE_DOWN])
    bDisableDebugWorldCameraUpdating = true;
  else
    bDisableDebugWorldCameraUpdating = false;
#else
  if (!MouseLook_XSpeed && !MouseLook_YSpeed && !CurrValues[0][DEBUGACTION_MOVE_UP] && !CurrValues[0][DEBUGACTION_MOVE_DOWN] &&
      !CurrValues[0][DEBUGACTION_MOVE_LEFT] && !CurrValues[0][DEBUGACTION_MOVE_RIGHT])
    bDisableDebugWorldCameraUpdating = true;
  else
    bDisableDebugWorldCameraUpdating = false;
#endif
  return DebugWorldCameraMover_Update((void*)thethis, unk);
}

// constructor/destructor hooks for mouse hiding

void* __stdcall DebugWorldCameraMover_Constructor_Hook(void* vector1, void* vector2, int joyport, int unk) {
  unsigned int thethis;
  _asm mov     thethis, ecx;
  bInDebugWorldCamera = true;
  RECT windowRect;
  GetWindowRect(*(HWND*)GAME_HWND_ADDR, &windowRect);
  int CenterX = ((windowRect.right - windowRect.left) / 2) + windowRect.left;
  int CenterY = ((windowRect.bottom - windowRect.top) / 2) + windowRect.top;
  SetCursorPos(CenterX, CenterY);

  return DebugWorldCameraMover_Constructor((void*)thethis, vector1, vector2, joyport, unk);
}
void* DebugWorldCameraMover_Destructor_Hook() {
  unsigned int thethis;
  _asm mov     thethis, ecx;

  bInDebugWorldCamera = false;

  return DebugWorldCameraMover_Destructor((void*)thethis);
}

#pragma runtime_checks("", restore)

// Mouselook stuff end

#ifdef GAME_PROSTREET

bool bIgnorableVKey(int k) {
  switch (k) {
    case VK_MENU:
    case VK_CONTROL:
    case VK_SHIFT:
    case VK_ESCAPE:
    case VK_LWIN:
    case VK_RWIN:
    case VK_APPS:
    case VK_LBUTTON:
    case VK_RBUTTON:
    case VK_MBUTTON:
    case VK_XBUTTON1:
    case VK_XBUTTON2:
      return true;
    default:
      return false;
  }

  return false;
}

int KB_GetCurrentPressedKey() {
  switch (KeyboardReadingMode) {
    case KB_READINGMODE_BUFFERED:

      for (int i = 1; i < 255; i++) {
        if (i && VKeyStates[0][i] >> 7)
          if (!bIgnorableVKey(i))  // TODO: separate Left Control and Right Alt
            return i;
      }
      return -1;
    case KB_READINGMODE_UNBUFFERED_ASYNC:
    default:

      for (int i = 1; i < 255; i++) {
        if (i && GetAsyncKeyState(i) >> 15)
          if (!bIgnorableVKey(i)) return i;
      }
      return -1;
  }
}

void SaveBindingToIni(ActionID id, bool isPrimary, uint16_t bind) {
  CIniReader inireader("");

  if (bind == 0) {
    if (inireader.ReadString(isPrimary ? "Events" : "Events_Secondary", ActionIDStr[id], nullptr))
      inireader.WriteString(isPrimary ? "Events" : "Events_Secondary", ActionIDStr[id], "");
    return;
  }

  inireader.WriteString(isPrimary ? "Events" : "Events_Secondary", ActionIDStr[id], (char*)ConvertXInputBitmaskToName(bind));
}

void SaveBindingToIniKB(ActionID id, bool isPrimary, int bind) {
  CIniReader inireader("");

  if (bind == 0) {
    if (inireader.ReadString(isPrimary ? "EventsKB" : "EventsKB_Secondary", ActionIDStr[id], nullptr))
      inireader.WriteString(isPrimary ? "EventsKB" : "EventsKB_Secondary", ActionIDStr[id], "");
    return;
  }

  inireader.WriteString(isPrimary ? "EventsKB" : "EventsKB_Secondary", ActionIDStr[id], (char*)VKeyStrings[bind]);
}

void ClearExistingMapKB(ActionID id, int bind) {
  for (int i = 0; i < MAX_ACTIONID; i++) {
    if (VKeyBindings_PRIMARY[i] == bind) {
      VKeyBindings_PRIMARY[i] = 0;
      SaveBindingToIniKB((ActionID)i, true, 0);
    }
    if (VKeyBindings_SECONDARY[i] == bind) {
      VKeyBindings_SECONDARY[i] = 0;
      SaveBindingToIniKB((ActionID)i, false, 0);
    }
  }
}

void ClearExistingMap(ActionID id, uint16_t bind) {
  for (int i = 0; i < MAX_ACTIONID; i++) {
    if (XInputBindings_PRIMARY[i] == bind) {
      XInputBindings_PRIMARY[i] = 0;
      SaveBindingToIni((ActionID)i, true, 0);
    }
    if (XInputBindings_SECONDARY[i] == bind) {
      XInputBindings_SECONDARY[i] = 0;
      SaveBindingToIni((ActionID)i, false, 0);
    }
  }
}

bool HandleKeyboardRemap(uint32_t index, uint32_t isPrimary) {
  int      KBkey = g_bRemapClear ? 0 : KB_GetCurrentPressedKey();
  ActionID id    = FindPCRemapActionID(index);
  if (KBkey != -1) {
    ClearExistingMapKB(id, KBkey);
    if (isPrimary)
      VKeyBindings_PRIMARY[id] = KBkey;
    else
      VKeyBindings_SECONDARY[id] = KBkey;
    SaveBindingToIniKB(id, isPrimary, KBkey);
    return true;
  }
  return false;
}

bool HandleGamepadRemap(uint32_t index, uint32_t isPrimary, int fDeviceIndex) {
  ActionID id = FindPCRemapActionID(index);
  if (g_bRemapClear) {
    if (isPrimary)
      XInputBindings_PRIMARY[id] = 0;
    else
      XInputBindings_SECONDARY[id] = 0;
    SaveBindingToIni(id, isPrimary, 0);
    if (bIsActionTextureBindable(id))
      SetBindingButtonTexture(id, isPrimary ? XInputBindings_PRIMARY[id] : XInputBindings_SECONDARY[id]);
    return true;
  }

  uint16_t act = GetAnalogActivity();
  if (act) {
    ClearExistingMap(id, act);
    if (isPrimary)
      XInputBindings_PRIMARY[id] = act;
    else
      XInputBindings_SECONDARY[id] = act;
    SaveBindingToIni(id, isPrimary, act);
    if (bIsActionTextureBindable(id))
      SetBindingButtonTexture(id, isPrimary ? XInputBindings_PRIMARY[id] : XInputBindings_SECONDARY[id]);
    return true;
  }

  WORD buttons = g_Controllers[fDeviceIndex].state.Gamepad.wButtons;
  if (buttons) {
    ClearExistingMap(id, buttons);
    if (isPrimary)
      XInputBindings_PRIMARY[id] = buttons;
    else
      XInputBindings_SECONDARY[id] = buttons;
    SaveBindingToIni(id, isPrimary, buttons);
    SetBindingButtonTexture(id, isPrimary ? XInputBindings_PRIMARY[id] : XInputBindings_SECONDARY[id]);
    return true;
  }

  return false;
}

void __stdcall ResetMappingsToDefault() {

    void* that;
    _asm mov that, ecx

    CIniReader   inireader("NFS_XtendedInput.default.ini");
    unsigned int inXInputConfigDef = 0;

    for (unsigned int i = 0; i < MAX_ACTIONID; i++) {
      // read the key bindings
      VKeyBindings_PRIMARY[i] = ConvertVKNameToValue(inireader.ReadString("EventsKB", ActionIDStr[i], ""));
      if (VKeyBindings_PRIMARY[i] == 0) {
        // try checking for single-char
        char lettercheck[32];
        strcpy(lettercheck, inireader.ReadString("EventsKB", ActionIDStr[i], ""));
        if (strlen(lettercheck) == 1) VKeyBindings_PRIMARY[i] = toupper(lettercheck[0]);
      }

      // read the key bindings [SECONDARY]
      VKeyBindings_SECONDARY[i] = ConvertVKNameToValue(inireader.ReadString("EventsKB_Secondary", ActionIDStr[i], ""));
      if (VKeyBindings_SECONDARY[i] == 0) {
        // try checking for single-char
        char lettercheck[32];
        strcpy(lettercheck, inireader.ReadString("EventsKB_Secondary", ActionIDStr[i], ""));
        if (strlen(lettercheck) == 1) VKeyBindings_SECONDARY[i] = toupper(lettercheck[0]);
      }

      inXInputConfigDef = ConvertXInputOtherConfigDef(inireader.ReadString("Events", ActionIDStr[i], ""));
      if (!inXInputConfigDef)
        XInputBindings_PRIMARY[i] = ConvertXInputNameToBitmask(inireader.ReadString("Events", ActionIDStr[i], ""));
      else
        XInputBindings_PRIMARY[i] = inXInputConfigDef;
#ifndef NO_FENG
      if (bIsActionTextureBindable((ActionID)i)) SetBindingButtonTexture((ActionID)i, XInputBindings_PRIMARY[i]);
#endif

      inXInputConfigDef = ConvertXInputOtherConfigDef(inireader.ReadString("Events_Secondary", ActionIDStr[i], ""));
      if (!inXInputConfigDef)
        XInputBindings_SECONDARY[i] = ConvertXInputNameToBitmask(inireader.ReadString("Events_Secondary", ActionIDStr[i], ""));
      else
        XInputBindings_SECONDARY[i] = inXInputConfigDef;
    }

    for (unsigned int i = 0; i < MAX_ACTIONID; i++) {
      SaveBindingToIni((ActionID)i, true, XInputBindings_PRIMARY[i]);
      SaveBindingToIni((ActionID)i, false, XInputBindings_SECONDARY[i]);
      SaveBindingToIniKB((ActionID)i, true, VKeyBindings_PRIMARY[i]);
      SaveBindingToIniKB((ActionID)i, false, VKeyBindings_SECONDARY[i]);
    }


    reinterpret_cast<void(__thiscall*)(void*)>(0x0070BDE0)(that);
}

#endif

// based on MW -- may change across games
class InputDevice {
  long Padding1;
  long FeedbackHandle;
  long UnkHandle;
  long UnkHandle2;
  long Padding2;

 public:
  DeviceScalar* fDeviceScalar;
  float*        fPrevValues;
  float*        fCurrentValues;
  int           fDeviceIndex;
  float         fControllerCurve;
#ifdef GAME_PROSTREET
  bool     bRemappingMode;
  uint32_t RemapType;
  uint32_t RemapIndex;
  uint32_t RemapIsPrimary;
#endif

  InputDevice(int DeviceIndex) {
    memset(&Padding1, 0, sizeof(long) * 5);
    UTL_Com_Object_IList_Constructor(&Padding1, 4);
    fDeviceIndex     = DeviceIndex;
    fControllerCurve = 1.0f;
    fPrevValues      = PrevValues[DeviceIndex];
    fCurrentValues   = CurrValues[DeviceIndex];
    fDeviceScalar    = new (nothrow) DeviceScalar[MAX_ACTIONID];
#ifdef GAME_PROSTREET
    bRemappingMode = false;
    RemapType      = 0;
    RemapIndex     = 0;
    RemapIsPrimary = 0;
#endif
#ifndef NO_PROFILE_SETTINGS
    InitProfileSettings();
#endif
  }
  virtual void* dtor(bool something) {
    delete (this);
    return this;
  }
  virtual bool IsConnected() { 
    if ((LastControlledDevice == LASTCONTROLLED_CONTROLLER) && bPassConnStatus) return g_Controllers[fDeviceIndex].bConnected;
    return true;
  }
  virtual bool IsWheel() { return false; }
  virtual void Initialize() {
    // printf("Called InputDevice::Initialize\n");
    //*(bool*)FEMOUSECURSOR_ISHIDDEN_ADDR = false;

    CIniReader   inireader("");
    unsigned int inXInputConfigDef = 0;

    // for each action ID generate an appropriate config
    for (unsigned int i = 0; i < MAX_ACTIONID; i++) {
      if (bIsActionDigitalButton((ActionID)i))
        fDeviceScalar[i].fType = kDigitalButton;
      else
        fDeviceScalar[i].fType = kJoyAxis;

      fDeviceScalar[i].fName_CRC32   = stringhash32(ActionIDStr[i]);
      fDeviceScalar[i].fPrevValue    = &PrevValues[fDeviceIndex][i];
      fDeviceScalar[i].fCurrentValue = &CurrValues[fDeviceIndex][i];

      // read the key bindings
      VKeyBindings_PRIMARY[i] = ConvertVKNameToValue(inireader.ReadString("EventsKB", ActionIDStr[i], ""));
      if (VKeyBindings_PRIMARY[i] == 0) {
        // try checking for single-char
        char lettercheck[32];
        strcpy(lettercheck, inireader.ReadString("EventsKB", ActionIDStr[i], ""));
        if (strlen(lettercheck) == 1) VKeyBindings_PRIMARY[i] = toupper(lettercheck[0]);
      }

      // read the key bindings [SECONDARY]
      VKeyBindings_SECONDARY[i] = ConvertVKNameToValue(inireader.ReadString("EventsKB_Secondary", ActionIDStr[i], ""));
      if (VKeyBindings_SECONDARY[i] == 0) {
        // try checking for single-char
        char lettercheck[32];
        strcpy(lettercheck, inireader.ReadString("EventsKB_Secondary", ActionIDStr[i], ""));
        if (strlen(lettercheck) == 1) VKeyBindings_SECONDARY[i] = toupper(lettercheck[0]);
      }

      inXInputConfigDef = ConvertXInputOtherConfigDef(inireader.ReadString("Events", ActionIDStr[i], ""));
      if (!inXInputConfigDef)
        XInputBindings_PRIMARY[i] = ConvertXInputNameToBitmask(inireader.ReadString("Events", ActionIDStr[i], ""));
      else
        XInputBindings_PRIMARY[i] = inXInputConfigDef;
#ifndef NO_FENG
      if (bIsActionTextureBindable((ActionID)i)) SetBindingButtonTexture((ActionID)i, XInputBindings_PRIMARY[i]);
#endif

      inXInputConfigDef = ConvertXInputOtherConfigDef(inireader.ReadString("Events_Secondary", ActionIDStr[i], ""));
      if (!inXInputConfigDef)
        XInputBindings_SECONDARY[i] = ConvertXInputNameToBitmask(inireader.ReadString("Events_Secondary", ActionIDStr[i], ""));
      else
        XInputBindings_SECONDARY[i] = inXInputConfigDef;
      // #ifndef NO_FENG
      //       if (bIsActionTextureBindable((ActionID)i)) SetBindingButtonTexture((ActionID)i, XInputBindings_SECONDARY[i]);
      // #endif
    }

    inXInputConfigDef = ConvertXInputOtherConfigDef(inireader.ReadString("Events", FE_SECONDARY_UP_NAME, ""));
    if (!inXInputConfigDef)
      FE_Secondary_Up = ConvertXInputNameToBitmask(inireader.ReadString("Events", FE_SECONDARY_UP_NAME, ""));
    else
      FE_Secondary_Up = inXInputConfigDef;

    inXInputConfigDef = ConvertXInputOtherConfigDef(inireader.ReadString("Events", FE_SECONDARY_DOWN_NAME, ""));
    if (!inXInputConfigDef)
      FE_Secondary_Down = ConvertXInputNameToBitmask(inireader.ReadString("Events", FE_SECONDARY_DOWN_NAME, ""));
    else
      FE_Secondary_Down = inXInputConfigDef;

    inXInputConfigDef = ConvertXInputOtherConfigDef(inireader.ReadString("Events", FE_SECONDARY_LEFT_NAME, ""));
    if (!inXInputConfigDef)
      FE_Secondary_Left = ConvertXInputNameToBitmask(inireader.ReadString("Events", FE_SECONDARY_LEFT_NAME, ""));
    else
      FE_Secondary_Left = inXInputConfigDef;

    inXInputConfigDef = ConvertXInputOtherConfigDef(inireader.ReadString("Events", FE_SECONDARY_RIGHT_NAME, ""));
    if (!inXInputConfigDef)
      FE_Secondary_Right = ConvertXInputNameToBitmask(inireader.ReadString("Events", FE_SECONDARY_RIGHT_NAME, ""));
    else
      FE_Secondary_Right = inXInputConfigDef;
  }
  virtual void PollDevice() {
    if (!bGlobalDoPolling) return;

    ReadControllerData();
    WORD wButtons                = g_Controllers[fDeviceIndex].state.Gamepad.wButtons;
    WORD SecondBind              = 0;
    int  rdi                     = fDeviceIndex;
    bool bCurrentXInputKeyState  = false;
    bool bCurrentXInputKeyState2 = false;
    bool bCurrentVKeyState       = false;
    bool bCurrentVKeyState_SECONDARY = false;
    bool bDoPolling              = true;

    // printf("FEngPkg: %s\n", cFEng_FindPackageWithControl_Name());

#ifdef GAME_MW
    bool bInShowcase = false;
    if (*(int*)GAMEFLOWMANAGER_STATUS_ADDR == 3) {
      if (cFEng_FindPackage(FE_SHOWCASE_FNG_NAME) || cFEng_FindPackage("UI_DebugCarCustomize.fng") || *(bool*)CARGUYSCAMERA_ADDR) bInShowcase = true;
    }
#endif
#ifdef GAME_PROSTREET
    if (bRemappingMode) {
      if (RemapType == 1) {
        if (HandleKeyboardRemap(RemapIndex, RemapIsPrimary)) {
          g_bRemapClear           = false;
          g_bRemapClear_IsPrimary = std::numeric_limits<std::uint32_t>::max();
          g_iRemapClear_BtnIndex  = std::numeric_limits<std::uint32_t>::max();
          bRemappingMode          = false;
        }
      }

      if (RemapType == 3) {
        if (HandleGamepadRemap(RemapIndex, RemapIsPrimary, fDeviceIndex)) {
          g_bRemapClear           = false;
          g_bRemapClear_IsPrimary = std::numeric_limits<std::uint32_t>::max();
          g_iRemapClear_BtnIndex  = std::numeric_limits<std::uint32_t>::max();
          bRemappingMode          = false;
        }
      }
    }
#endif
    for (unsigned int j = 0; j < MAX_ACTIONID; j++) {
      ActionID i = (ActionID)j;

      if (bIsActionFrontEnd((ActionID)i) && bIsHudVisible()) bDoPolling = false;

#ifdef GAME_WORLD
      // I *truly* do not understand this game. So, the inputs will only start working if a button was pressed BEFORE entering the game.
      // So I'll set an action to a high state while in FE. FE isn't even controlled by the controller, so it should be safe.
      if (*(int*)GAMEFLOWMANAGER_STATUS_ADDR != 6) {
        CurrValues[fDeviceIndex][i] = 1.0;
        bDoPolling                  = false;
      }
#endif

      if (bDoPolling) {
        float fresult = 0;
        rdi           = fDeviceIndex;
        if (VKeyBindings_PRIMARY[i]) {
          if (KeyboardReadingMode == KB_READINGMODE_BUFFERED)
            bCurrentVKeyState = VKeyStates[0][VKeyBindings_PRIMARY[i]] >> 7;
          else
            bCurrentVKeyState = GetAsyncKeyState(VKeyBindings_PRIMARY[i]) >> 15;
        } else
          bCurrentVKeyState = false;

        if (VKeyBindings_SECONDARY[i]) {
          if (KeyboardReadingMode == KB_READINGMODE_BUFFERED)
            bCurrentVKeyState_SECONDARY = VKeyStates[0][VKeyBindings_SECONDARY[i]] >> 7;
          else
            bCurrentVKeyState_SECONDARY = GetAsyncKeyState(VKeyBindings_SECONDARY[i]) >> 15;
        } else
          bCurrentVKeyState_SECONDARY = false;

        bCurrentVKeyState |= bCurrentVKeyState_SECONDARY;

        if (bIsActionDebug((ActionID)i)) {
          rdi = 1;  // debug (camera) actions are always read from the second port, but the PC version omits it entirely so this is a workaround
        }

        wButtons = g_Controllers[rdi].state.Gamepad.wButtons;

        if (bIsActionAnalog((ActionID)i)) {
          if (bIsBindingAnalog(XInputBindings_PRIMARY[i])) {
            fresult = GetAnalogStickValue(rdi, XInputBindings_PRIMARY[i]);
          } else {
            bCurrentXInputKeyState = wButtons & XInputBindings_PRIMARY[i];
            // keyboard & digital buttons
            if (bCurrentXInputKeyState) fresult = 1.0f;
          }

          if (bIsBindingAnalog(XInputBindings_SECONDARY[i])) {
            if (fresult == 0.0f) fresult = GetAnalogStickValue(rdi, XInputBindings_SECONDARY[i]);
          } else {
            if (!bCurrentXInputKeyState) {
              bCurrentXInputKeyState = wButtons & XInputBindings_SECONDARY[i];
              // keyboard & digital buttons
              if (bCurrentXInputKeyState) fresult = 1.0f;
            }
          }

          if (bCurrentVKeyState) fresult = 1.0f;
        } else {
          if (bCheckSecondBind() && bIsActionFrontEndNavigation((ActionID)i)) {
            switch (i) {
              case FRONTENDACTION_UP:
                SecondBind = FE_Secondary_Up;
                break;
              case FRONTENDACTION_DOWN:
                SecondBind = FE_Secondary_Down;
                break;
              case FRONTENDACTION_LEFT:
                SecondBind = FE_Secondary_Left;
                break;
              case FRONTENDACTION_RIGHT:
                SecondBind = FE_Secondary_Right;
                break;
              default:
                SecondBind = 0;
                break;
            }

            if (bIsBindingAnalog(SecondBind))  // digital directions with an axis
            {
              bCurrentXInputKeyState2 = bGetAnalogDigitalActivationState(rdi, SecondBind, i);
            }

            else
              bCurrentXInputKeyState2 = wButtons & SecondBind;
          } else {
            SecondBind              = 0;
            bCurrentXInputKeyState2 = false;
          }

          if (bIsBindingAnalog(XInputBindings_PRIMARY[i]) || bIsBindingAnalog(XInputBindings_SECONDARY[i]))  // digital directions with an axis
          {
            bCurrentXInputKeyState = bGetAnalogDigitalActivationState(rdi, XInputBindings_PRIMARY[i], i);
            if (!bCurrentXInputKeyState) bCurrentXInputKeyState = bGetAnalogDigitalActivationState(rdi, XInputBindings_SECONDARY[i], i);
            if (bCurrentXInputKeyState || bCurrentXInputKeyState2) fresult = 1.0f;
          } else {
            bCurrentXInputKeyState = wButtons & XInputBindings_PRIMARY[i];
            if (!bCurrentXInputKeyState) bCurrentXInputKeyState = wButtons & XInputBindings_SECONDARY[i];
            // determine final digital status
            if (bCurrentXInputKeyState || bCurrentXInputKeyState2) fresult = 1.0f;
          }
          if (bCurrentVKeyState) fresult = 1.0f;
        }

        // write value to the main array
        CurrValues[fDeviceIndex][i] = fresult;

        // update prev values
        if (CurrValues[fDeviceIndex][i] != PrevValues[fDeviceIndex][i]) {
          // do one-time triggers here...
#ifdef GAME_MW
          if (bIsActionRelativePosition((ActionID)i) && bInShowcase) {
            if (CurrValues[fDeviceIndex][i] > 0)
              CurrValues[fDeviceIndex][i] = CurrValues[fDeviceIndex][i] - PrevValues[fDeviceIndex][i];
            else
              CurrValues[fDeviceIndex][i] = 0;
          }
#endif
          PrevValues[fDeviceIndex][i] = CurrValues[fDeviceIndex][i];
        } else if (bIsActionDigitalButton((ActionID)i)) {
          if (CurrValues[fDeviceIndex][i] > 0) CurrValues[fDeviceIndex][i] = 0;
        }
      }
      bDoPolling = true;
    }
  }
  virtual int  GetNumDeviceScalar() { return MAX_ACTIONID; }
  virtual void StartVibration() {
    // printf("Called InputDevice::StartVibration\n");
  }
  virtual void StopVibration() {
    // this seems to be only called when the controller is unplugged...
    // printf("Called InputDevice::StopVibration\n");
  }
  virtual int GetInterfaces() { return 0; }
  virtual int GetSecondaryDevice() { return 0; }
#ifndef GAME_MW
  virtual int  GetBatteryLife() { return 0; }
  virtual bool IsBatteryLow() { return false; }
  virtual int  GetControllerTypeMask() { return 0; }
#endif
  virtual bool DeviceHasChanged() { return false; }
#ifndef GAME_MW
  virtual bool DeviceHasAnyActivity() { return false; }
#endif
  virtual void* SaveCurrentState() { return NULL; }
  virtual void  RestoreToState(float* unk) { return; }
#ifndef GAME_MW
  virtual int GetDeviceNoiseAllowance() { return 0; }
#endif
#ifdef GAME_PROSTREET
  virtual void GoRemapButton(uint32_t type, uint32_t button, uint32_t isPrimary, uint32_t unk) {
    bRemappingMode = true;
    RemapType      = type;
    RemapIndex     = button;
    RemapIsPrimary = isPrimary;
  }
#endif
};

InputDevice* InputDeviceFactory(int DeviceIndex) {
  InputDevice* out = new InputDevice(DeviceIndex);

  return out;
}

#pragma runtime_checks("", off)

#ifdef GAME_PROSTREET

void __stdcall GoRemapButtonHook(uint32_t type, uint32_t button_index, uint32_t isPrimary, uint32_t unk) {
  InputDevice* that;
  _asm mov     that, ecx;
  return that->GoRemapButton(type, button_index, isPrimary, unk);
}

void __stdcall GoClearButtonHook(uint32_t type, uint32_t button_index, uint32_t isPrimary, uint32_t unk) {
  InputDevice* that;
  _asm mov     that, ecx;
  g_bRemapClear           = true;
  g_bRemapClear_IsPrimary = isPrimary;
  g_iRemapClear_BtnIndex  = button_index;
  return that->GoRemapButton(type, button_index, isPrimary, unk);
}

bool __stdcall GetRemappingModeHook() {
  InputDevice* that;
  _asm mov     that, ecx;

  return that->bRemappingMode;
}

void GetMappingStringHook(char* str, uint32_t type, uint32_t button_index, uint32_t isPrimary, uint32_t unk) {
  if (g_bRemapClear && button_index == g_iRemapClear_BtnIndex && isPrimary == g_bRemapClear_IsPrimary) {
    strcpy(str, "---");
    return;
  }
  if (type == 1) {
    strcpy(str,
           ControlsTextsPC[isPrimary ? VKeyBindings_PRIMARY[FindPCRemapActionID(button_index)] : VKeyBindings_SECONDARY[FindPCRemapActionID(button_index)]]);
    return;
  }

  if (type == 3) {
    strcpy(str, ConvertBitmaskToControlString(ControllerIconMode, isPrimary ? XInputBindings_PRIMARY[FindPCRemapActionID(button_index)]
                                                                            : XInputBindings_SECONDARY[FindPCRemapActionID(button_index)]));
    return;
  }

  strcpy(str, "---");
}

uint32_t GetNumExternalDevices() { return 1; }

uint32_t GetNumExternalDevicesZero() { return 0; }

const char* __stdcall GetXtendedInputDeviceName(uint32_t type, uint32_t index) {
  if (type == 1) return "Keyboard";
  if (type == 3) return "XInput Controller";

  return "Unknown Input Device";
}

#endif

#ifndef GAME_WORLD
void*(__thiscall* FastMem_Alloc)(void* FastMem, int size, char* debug_str) = (void*(__thiscall*)(void*, int, char*))FASTMEM_ALLOC_ADDR;
#endif

#ifdef GAME_MW
void* (*FastMem_InitListAllocator)()                               = (void* (*)())FASTMEM_INITLISTALLOCATOR_ADDR;
void*(__stdcall* FastMem_ListAllocator)(void* CurrentListPos, void* NextListPos,
                                        InputMapEntry* inInputMap) = (void*(__stdcall*)(void*, void*, InputMapEntry*))FASTMEM_LISTALLOCATOR_ADDR;
void*(__thiscall* STL_List_Add)(void* thethis, int num)            = (void*(__thiscall*)(void*, int))STL_LIST_ADD_ADDR;

void* __stdcall InputMapping_Constructor(InputDevice* device, void* AttribCollection) {
  unsigned int thethis      = 0;
  unsigned int list_current = 0;
  unsigned int list_next    = 0;
  int          alloc_result = 0;

  _asm mov thethis, ecx;

  // use game's own memory management to avoid trouble
  *(void**)(thethis + 4) = (void*)FastMem_InitListAllocator();
  list_current           = (unsigned int)*(void**)(thethis + 4);

  for (unsigned int i = 0; i < MAX_ACTIONID; i++) {
    if (VKeyBindings_PRIMARY[i] != 0 || VKeyBindings_SECONDARY[i] != 0 || XInputBindings_PRIMARY[i] != 0 || XInputBindings_SECONDARY[i] != 0)  // is it mapped
    {
      InputMapEntry map;
      if (device->fDeviceScalar[i].fType == kDigitalButton)
        map.UpdateType = kPress;
      else
        map.UpdateType = kUpdate;
      map.LowerDZ           = 0.0;
      map.UpperDZ           = 0.0;
      map.Action            = (ActionID)i;
      map.DeviceScalarIndex = i;
      map.PreviousValue     = -1.0f;
      map.CurrentValue      = -1.0f;
      list_next             = *(int*)(list_current + 4);
      alloc_result          = (int)FastMem_ListAllocator((void*)list_current, (void*)list_next, &map);
      // STL_List_Add((void*)thethis, 1);
      *(int*)(list_current + 4)   = alloc_result;
      **(int**)(alloc_result + 4) = alloc_result;
    }
  }

  return (void*)thethis;
}
#else

#ifndef GAME_WORLD
void* __stdcall FastMem_ListAllocator(void* CurrentListPos, void* NextListPos, InputMapEntry* inInputMap) {
  void* result = FastMem_Alloc((void*)GLOBAL_FASTMEM_ADDR, INIT_LIST_ALLOC_SIZE, NULL);
  if (result) {
    *(void**)result = CurrentListPos;
    *(void**)((int)result + 4) = NextListPos;
    memcpy((void*)((int)result + 8), inInputMap, sizeof(InputMapEntry));
  }
  return result;
}
#else
void* (*Managed_Alloc)(int size) = (void* (*)(int))MANAGED_ALLOCATOR_ADDR;
void* __stdcall FastMem_ListAllocator(void* CurrentListPos, void* NextListPos, InputMapEntry* inInputMap) {
  void* result = Managed_Alloc(INIT_LIST_ALLOC_SIZE);
  if (result) {
    *(void**)result            = CurrentListPos;
    *(void**)((int)result + 4) = NextListPos;
    memcpy((void*)((int)result + 8), inInputMap, sizeof(InputMapEntry));
  }
  return result;
}
#endif
void* __stdcall InputMapping_Constructor(InputDevice* device, void* AttribCollection) {
  volatile unsigned int thethis = 0;
  volatile unsigned int list_current = 0;
  volatile unsigned int list_next = 0;
  volatile int alloc_result = 0;

  _asm mov thethis, ecx;

  // use game's own memory management to avoid trouble
  *(void**)(thethis) = (void*)thethis;
  *(void**)(thethis + 4) = (void*)thethis;
  *(int*)(thethis + 8) = 0;

  for (unsigned int i = 0; i < MAX_ACTIONID; i++) {
    if (VKeyBindings_PRIMARY[i] != 0 || VKeyBindings_SECONDARY[i] != 0 || XInputBindings_PRIMARY[i] != 0 || XInputBindings_SECONDARY[i] != 0)  // is it mapped
    {
      InputMapEntry map;
      if (device->fDeviceScalar[i].fType == kDigitalButton)
        map.UpdateType = kPress;
      else
        map.UpdateType = kUpdate;

      map.LowerDZ = 0.0;
      map.UpperDZ = 0.0;
      map.Action = (ActionID)i;
      map.DeviceScalarIndex = i;
      map.ComboDeviceScalarIndex = -1;
      map.PreviousValue = -1.0f;
      map.CurrentValue = -1.0f;
      list_current = *(int*)(thethis + 4);
      list_next = *(int*)(list_current + 4);
      alloc_result = (int)FastMem_ListAllocator((void*)thethis, (void*)list_next, &map);

      *(int*)alloc_result = thethis;
      *(int*)(alloc_result + 4) = *(int*)(thethis + 4);
      **(int**)(thethis + 4) = alloc_result;
      *(int*)(thethis + 8) += 1;
      *(int*)(thethis + 4) = alloc_result;
    }
  }

  return (void*)thethis;
}

#ifndef NO_FENG
void HandleSelectCarCamera(void* SelectCarObj, SHORT XAxis, SHORT YAxis) {
  if (XAxis || YAxis || bMousePressedDown) {
    if (bMousePressedDown) {
      float HSpeed = *(int*)FEMOUSECURSOR_CARORBIT_X_ADDR;
      float VSpeed = *(int*)FEMOUSECURSOR_CARORBIT_Y_ADDR;

      if (HSpeed > 25.0) HSpeed = 25.0;
      if (HSpeed < -25.0) HSpeed = -25.0;
      if (VSpeed > 25.0) VSpeed = 25.0;
      if (VSpeed < -25.0) VSpeed = -25.0;

      if (HSpeed)
        SelectCarCameraMover_SetHRotateSpeed(SelectCarObj, HSpeed, true);
      else
        SelectCarCameraMover_SetHRotateSpeed(SelectCarObj, 0, false);
      if (VSpeed)
        SelectCarCameraMover_SetVRotateSpeed(SelectCarObj, VSpeed, true);
      else
        SelectCarCameraMover_SetVRotateSpeed(SelectCarObj, 0, false);
    }

    if (XAxis) {
      float HSpeed = -XAxis / float(0x7FFF);
      SelectCarCameraMover_SetHRotateSpeed(SelectCarObj, HSpeed, true);
    }
    if (YAxis) {
      float VSpeed = YAxis / float(0x7FFF);
      SelectCarCameraMover_SetVRotateSpeed(SelectCarObj, VSpeed, true);
    }
  } else {
    SelectCarCameraMover_SetHRotateSpeed(SelectCarObj, 0, false);
    SelectCarCameraMover_SetVRotateSpeed(SelectCarObj, 0, false);
  }

  if (g_Controllers[0].state.Gamepad.bLeftTrigger || (MouseWheelValue < 0)) {
    if (MouseWheelValue)  // TODO: maybe enhance it with a proper accel delta???
      SelectCarCameraMover_SetZoomSpeed(SelectCarObj, 1.0, true);
    if (g_Controllers[0].state.Gamepad.bLeftTrigger) {
      float Speed = g_Controllers[0].state.Gamepad.bLeftTrigger / float(0xFF);
      SelectCarCameraMover_SetZoomSpeed(SelectCarObj, Speed, true);
    }
  }

  else if (g_Controllers[0].state.Gamepad.bRightTrigger || (MouseWheelValue > 0)) {
    if (MouseWheelValue) SelectCarCameraMover_SetZoomSpeed(SelectCarObj, -1.0, true);
    if (g_Controllers[0].state.Gamepad.bRightTrigger) {
      float Speed = g_Controllers[0].state.Gamepad.bRightTrigger / float(0xFF);
      SelectCarCameraMover_SetZoomSpeed(SelectCarObj, -Speed, true);
    }
  } else
    SelectCarCameraMover_SetZoomSpeed(SelectCarObj, 0, false);
}

// fix for Carbon & Pro Street Photo Mode
void __stdcall FEPhotoModeStateManager_HandleScreenTick_Hook() {
  unsigned int thethis = 0;
  _asm mov thethis, ecx;
  void* SelectCarObj = (void*)*(int*)(thethis + SELECTCAROBJ_OFFSET);
#ifdef GAME_PROSTREET
  HandleSelectCarCamera(SelectCarObj, g_Controllers[0].state.Gamepad.sThumbLX, g_Controllers[0].state.Gamepad.sThumbLY);
#else
  HandleSelectCarCamera(SelectCarObj, g_Controllers[0].state.Gamepad.sThumbRX, g_Controllers[0].state.Gamepad.sThumbRY);
#endif
}

// fix for ALL zooming (ingame and FE)
void __stdcall FEPhotoModeStateManager_HandleLTrigger_Hook() { return; }

void __stdcall FEPhotoModeStateManager_HandleRTrigger_Hook() { return; }
#ifdef GAME_CARBON
int __stdcall ftol2_to_int_bool() {
  unsigned int obj = 0;
  _asm mov obj, ecx

      if (obj) {
    float num = *(float*)(obj + 8);
    if (num) return 1;
  }
  return 0;
}

#endif

#endif  // GAME_UC
#pragma runtime_checks("", restore)
#endif

#ifndef NO_FENG
unsigned int GameWndProcAddr = 0;
LRESULT(WINAPI* GameWndProc)(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT WINAPI CustomWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
  switch (msg) {
    case WM_LBUTTONDOWN:
      LastControlledDevice = LASTCONTROLLED_KB;
      bMousePressedDown = true;
#ifndef NO_FENG
      if (bUseDynamicFEngSwitching) bConsoleFEng = false;
#endif
      return 0;
    case WM_LBUTTONUP:
      bMousePressedDown = false;
      return 0;
    case WM_RBUTTONDOWN:
      LastControlledDevice = LASTCONTROLLED_KB;
      bMouse3PressedDown = true;
#ifndef NO_FENG
      if (bUseDynamicFEngSwitching) bConsoleFEng = false;
#endif
      return 0;
    case WM_RBUTTONUP:
      bMouse3PressedDown = false;
      return 0;
    case WM_MBUTTONDOWN:
      LastControlledDevice = LASTCONTROLLED_KB;
      bMouse2PressedDown = true;
#ifndef NO_FENG
      if (bUseDynamicFEngSwitching) bConsoleFEng = false;
#endif
      return 0;
    case WM_MBUTTONUP:
      bMouse2PressedDown = false;
      return 0;
    case WM_MOUSEWHEEL:
      LastControlledDevice = LASTCONTROLLED_KB;
      MouseWheelValue = (GET_WHEEL_DELTA_WPARAM(wParam));
#ifndef NO_FENG
      if (bUseDynamicFEngSwitching) bConsoleFEng = false;
#endif
      return 0;
    case WM_SETFOCUS:
      // confine mouse within the game window
      if (bConfineMouse) {
        GetWindowRect(*(HWND*)GAME_HWND_ADDR, &windowRect);
        ClipCursor(&windowRect);
      }
      break;
    case WM_KEYDOWN:
      LastControlledDevice = LASTCONTROLLED_KB;
#ifndef NO_FENG
      if (bUseDynamicFEngSwitching) bConsoleFEng = false;
#endif
      break;
    case WM_MOUSEMOVE:
      if (!bUseWin32Cursor) bLastUsedVirtualMouse = false;
      return 0;
    default:
      break;
  }

  return GameWndProc(hWnd, msg, wParam, lParam);
}

ATOM WINAPI RegisterClassHook(WNDCLASSEXA* wcex) {
  (*wcex).hCursor = NULL;  // this is necessary to be able to hide the mouse quickly enough
  return RegisterClassExA(wcex);
}

void DummyFunc() { return; }

void __stdcall DummyFuncStd(BOOL dummy) { return; }
#endif  // GAME_UC

void InitConfig() {
  CIniReader inireader("");

  KeyboardReadingMode = inireader.ReadInteger("Input", "KeyboardReadingMode", 0);
  bXInputOmniMode     = inireader.ReadInteger("Input", "XInputOmniMode", 0) != 0;
  bPassConnStatus     = inireader.ReadInteger("Input", "PassConnStatus", 1) != 0;
#ifndef NO_FENG
  bConfineMouse        = inireader.ReadInteger("Input", "ConfineMouse", 0) != 0;
  bUseWin32Cursor      = inireader.ReadInteger("Input", "UseWin32Cursor", 1) != 0;
  bUseCustomCursor     = inireader.ReadInteger("Input", "UseCustomCursor", 1) != 0;
  bEnableMouseHiding   = inireader.ReadInteger("Input", "EnableMouseHiding", 1) != 0;
  bMouseLook           = inireader.ReadInteger("Input", "MouseLook", 1) != 0;
  MouseLookSensitivity = inireader.ReadFloat("Input", "MouseLookSensitivity", 1.0f);
#endif
  INPUT_DEADZONE_LS = static_cast<SHORT>(clampf(inireader.ReadFloat("Deadzone", "PercentLS", 0.24f), 0.0f, 1.0f) * std::numeric_limits<SHORT>::max());
  INPUT_DEADZONE_RS = static_cast<SHORT>(clampf(inireader.ReadFloat("Deadzone", "PercentRS", 0.24f), 0.0f, 1.0f) * std::numeric_limits<SHORT>::max());
  INPUT_DEADZONE_LS_P2 =
      static_cast<SHORT>(clampf(inireader.ReadFloat("Deadzone", "PercentLS_P2", 0.24f), 0.0f, 1.0f) * std::numeric_limits<SHORT>::max());
  INPUT_DEADZONE_RS_P2 =
      static_cast<SHORT>(clampf(inireader.ReadFloat("Deadzone", "PercentRS_P2", 0.24f), 0.0f, 1.0f) * std::numeric_limits<SHORT>::max());
  SHIFT_ANALOG_THRESHOLD =
      static_cast<SHORT>(clampf(inireader.ReadFloat("Deadzone", "Percent_Shifting", 0.75f), 0.0f, 1.0f) * std::numeric_limits<SHORT>::max());
  FEUPDOWN_ANALOG_THRESHOLD =
      static_cast<SHORT>(clampf(inireader.ReadFloat("Deadzone", "Percent_AnalogStickDigital", 0.50f), 0.0f, 1.0f) * std::numeric_limits<SHORT>::max());
  TRIGGER_ACTIVATION_THRESHOLD =
      static_cast<SHORT>(clampf(inireader.ReadFloat("Deadzone", "Percent_AnalogTriggerDigital", 0.12f), 0.0f, 1.0f) * std::numeric_limits<SHORT>::max());
#ifndef NO_FENG
  ControllerIconMode       = inireader.ReadInteger("Icons", "ControllerIconMode", 0);
  LastControlledDevice     = inireader.ReadInteger("Icons", "FirstControlDevice", 0);
  bUseDynamicFEngSwitching = inireader.ReadInteger("Icons", "UseDynamicFEngSwitching", 1) != 0;
  bEnableSplashTakeover    = inireader.ReadInteger("Icons", "EnableSplashTakeover", 1) != 0;
  strcpy(ButtonTexFilename, inireader.ReadString("Icons", "ButtonTexFilename", "GLOBAL\\XtendedInputButtons.tpk"));
#endif
}

int Init() {
  InitConfig();

  injector::WriteMemory<unsigned int>(INPUTDEVICE_FACTORY_INITIALIZER_ADDR, (unsigned int)&InputDeviceFactory, true);
  injector::MakeCALL(INPUTMAPPING_CONSTRUCTOR_CALL_ADDR, InputMapping_Constructor, true);
#ifndef GAME_MW
  // Lower hardcoded deadzone to 0.000001 - VERY IMPORTANT
  injector::WriteMemory<unsigned int>(DEADZONE_FLOAT_POINTER_ADDR, (unsigned int)SMALL_FLOAT_ADDR, true);
#endif

  // kill DInput initialization
  injector::MakeNOP(DINPUT_KILL_ADDR, 5, true);

#ifndef NO_FENG
  // dereference the current WndProc from the game executable and write to the function pointer (to maximize compatibility)
  GameWndProcAddr = *(unsigned int*)WNDPROC_POINTER_ADDR;
  GameWndProc     = (LRESULT(WINAPI*)(HWND, UINT, WPARAM, LPARAM))GameWndProcAddr;
  injector::WriteMemory<unsigned int>(WNDPROC_POINTER_ADDR, (unsigned int)&CustomWndProc, true);

  // Mouse stuff
  injector::MakeJMP(FEMOUSE_UPDATE_CALL_ADDR, UpdateFECursorPos, true);

  // kill mouse wheel updating from InputDevice vars
  injector::MakeNOP(FEMOUSE_WHEEL_UPDATE_ADDR, 5, true);

  // window class register hook to change the default mouse cursor
  injector::MakeNOP(REGISTERCLASSHOOK_CALL_ADDR, 6, true);
  injector::MakeCALL(REGISTERCLASSHOOK_CALL_ADDR, RegisterClassHook, true);

  if (bUseWin32Cursor) {
    // disable PC_CURSOR rendering
    injector::MakeJMP(FEMOUSE_RENDER_JMP_FROM, FEMOUSE_RENDER_JMP_TO, true);
    // disable Win32 cursor hiding
    injector::MakeJMP(WIN32_MOUSEHIDE_JMP_FROM, WIN32_MOUSEHIDE_JMP_TO, true);
#ifndef GAME_MW
    injector::MakeNOP(SHOWCURSOR_HOOK_ADDR1, 6, true);
    injector::MakeCALL(SHOWCURSOR_HOOK_ADDR1, DummyFuncStd, true);
    injector::MakeNOP(SHOWCURSOR_HOOK_ADDR2, 6, true);
    injector::MakeCALL(SHOWCURSOR_HOOK_ADDR2, DummyFuncStd, true);
#endif

    if (LastControlledDevice == LASTCONTROLLED_CONTROLLER) bConsoleFEng = true;

  } else
    SetCursor(NULL);

  // kill garage camera control with FE pad buttons (leave only the stick)
  injector::MakeJMP(FENG_SHOWCASECAM_JMP_FROM, FENG_SHOWCASECAM_JMP_TO, true);

  // FEngHud -- joy handler fix
  injector::MakeJMP(FENGHUD_JOYHANDLER_JMP_FROM, FENGHUD_JOYHANDLER_JMP_TO, true);

  // FE VISUAL STUFF

  // custom Win32 mouse cursor
  if (bUseWin32Cursor) {
    if (bUseCustomCursor)
      NFSCursor = LoadCursorFromFile("nfs_cursor.cur");
    else
      NFSCursor = LoadCursor(NULL, IDC_ARROW);
  }

  // FORCE CONSOLE OBJECTS TO BE RENDERABLE ON PC
  // injector::MakeJMP(FENG_HIDEPCOBJ_JMP_FROM, FENG_HIDEPCOBJ_JMP_TO, true); // FEngHidePCObjects
  injector::WriteMemory<unsigned int>(FENG_HIDEPCOBJ_VT_ADDR, (unsigned int)&FEObjectCallback_Function, true);

#ifdef GAME_MW

  // dereference the current function after initing to maximize compatibility
  MouseStateArrayOffsetUpdater_Address = *(unsigned int*)FE_MOUSEUPDATER_CALLBACK_VT_ADDR;
  MouseStateArrayOffsetUpdater         = (bool(__thiscall*)(void*, void*))MouseStateArrayOffsetUpdater_Address;
  // it looks decieving, it's not related to mouse, it's for accessing all FEObjects during FE updating
  injector::WriteMemory<unsigned int>(FE_MOUSEUPDATER_CALLBACK_VT_ADDR, (unsigned int)&MouseStateArrayOffsetUpdater_Callback_Hook, true);

  if (bUseDynamicFEngSwitching) {
    injector::MakeNOP(CFENG_RENDEROBJ_NOP_ADDR, CFENG_RENDEOBJ_NOP_AMOUNT, true);  // cFEng render object
    injector::MakeJMP(FENG_SETVISIBLE_ADDR, FEngSetVisible, true);
    injector::MakeNOP(0x0052AEC3, 2, true);
    injector::MakeNOP(0x0052F33C, 2, true);
    injector::MakeNOP(0x0052F3FA, 6, true);
    injector::MakeNOP(0x00562C60, 2, true);
  } else {
    // make MW PC behave exactly like console version here... hide all PC elements instead
    if (LastControlledDevice == LASTCONTROLLED_CONTROLLER) {
      injector::WriteMemory<unsigned char>(0x005A45B9, FE_CONTROL_FLAG_PC, true);
      injector::WriteMemory<unsigned char>(0x00514CCD, FE_CONTROL_FLAG_PC, true);
      injector::WriteMemory<unsigned char>(0x0052AEC2, FE_CONTROL_FLAG_PC, true);
      injector::WriteMemory<unsigned char>(0x0052F33B, FE_CONTROL_FLAG_PC, true);
      injector::WriteMemory<unsigned char>(0x0052F3F9, FE_CONTROL_FLAG_PC, true);
      injector::WriteMemory<unsigned char>(0x00562C5F, FE_CONTROL_FLAG_PC, true);
    }
  }

  // remove the Controls option entry - it crashes the game
  injector::MakeJMP(0x0052905D, 0x005290B4, true);

  // hook for the performance tuning screen to show the setting names
  injector::WriteMemory<unsigned int>(0x008A2A78, (unsigned int)&CustomTuningScreen_NotificationMessage_Hook, true);
#else
  // Press START button initial hook...
  if (bEnableSplashTakeover) injector::MakeCALL(PRESS_START_HOOK_ADDR, FEngSetLanguageHash_Hook, true);

  if (bUseDynamicFEngSwitching) {
    injector::MakeNOP(CFENG_RENDEROBJ_NOP_ADDR, CFENG_RENDEOBJ_NOP_AMOUNT, true);  // cFEng render object
    injector::MakeCALL(CFENG_SERVICE_CALL_ADDR, cFEng_Service_Hook, true);
    injector::MakeCALL(FENGINE_PROCESSPADSFORPACKAGE_CALL_ADDR, FEngine_ProcessPadsForPackage_Hook, true);
#ifdef GAME_PROSTREET
    injector::MakeNOP(CFENG_RENDEROBJ_NOP2_ADDR, CFENG_RENDEOBJ_NOP_AMOUNT, true);  // cFEng render object
#endif
  } else {
    if (LastControlledDevice == LASTCONTROLLED_CONTROLLER) {
      injector::WriteMemory<unsigned char>(CFENG_RENDEROBJ_FLAG_ADDR, FE_CONTROL_FLAG_PC, true);
    }
  }
  // force analog zooming in FE orbit camera
  injector::MakeJMP(FE_ANALOGZOOM_JMP_FROM, FE_ANALOGZOOM_JMP_TO, true);
  // remove deadzone for FE activations...
  injector::WriteMemory<int>(FE_DEADZONE_POINTER_ADDR, (int)&FEActivationFloat, true);

  injector::MakeJMP(FEPHOTOMODE_HANDLESCREENTICK_HOOK_ADDR, FEPhotoModeStateManager_HandleScreenTick_Hook, true);
  injector::WriteMemory<unsigned int>(FEPHOTOMODE_HANDLELTRIGGER_HOOK_ADDR, (unsigned int)&FEPhotoModeStateManager_HandleLTrigger_Hook, true);
  injector::WriteMemory<unsigned int>(FEPHOTOMODE_HANDLERTRIGGER_HOOK_ADDR, (unsigned int)&FEPhotoModeStateManager_HandleRTrigger_Hook, true);

#ifdef GAME_CARBON
  injector::MakeCALL(0x005C3916, FEPackage_FindObjectByHash_Show_Hook, true);
  injector::MakeCALL(0x005CDD6D, FEPackage_FindObjectByHash_Hide_Hook, true);
  injector::MakeCALL(0x005B9023, FEPackage_FindObjectByHash_Hide_Hook, true);
  injector::MakeCALL(0x005AF3EE, FEPackage_FindObjectByHash_Hide_Hook, true);

  injector::MakeCALL(0x005AED20, FEPrintf_Hook_WorldMap, true);

  injector::MakeNOP(0x008577AC, 2, true);

  injector::WriteMemory<unsigned int>(FEWORLDMAPSTATEMANAGER_TICK_VT_ADDR, (unsigned int)&FEWorldMapStateManager_HandleScreenTick_Hook, true);

  if (bUseDynamicFEngSwitching) {
    injector::MakeCALL(0x005C670E, WorldMap_UnfocusQuickList_Hook, true);
    injector::MakeCALL(0x005CE0B9, WorldMap_SetQuickListInFocus_Hook, true);
    injector::MakeCALL(0x005CE309, WorldMap_SetQuickListInFocus_Hook, true);

  } else {
    if (LastControlledDevice == LASTCONTROLLED_CONTROLLER) {
      injector::WriteMemory<unsigned int>(0x005C3910, WORLDMAP_BUTTONGROUP_CONSOLE, true);
      injector::WriteMemory<unsigned int>(0x005CDD67, WORLDMAP_BUTTONGROUP_CONSOLE, true);
      injector::WriteMemory<unsigned int>(0x005B901D, WORLDMAP_BUTTONGROUP_CONSOLE, true);
      injector::WriteMemory<unsigned int>(0x005AF3E8, WORLDMAP_BUTTONGROUP_CONSOLE, true);
    }
  }

#endif  // GAME_CARBON
#ifdef GAME_PROSTREET
  // fix the "view details" string update in the FEHubMap screen
  injector::MakeCALL(0x007ECD78, FE_SetLanguageHash_Hook, true);
  injector::MakeCALL(0x007ECD59, FE_SetLanguageHash_Hook, true);
  injector::MakeCALL(0x007ECD0D, FE_SetLanguageHash_Hook, true);
  // remapping
  injector::MakeCALL(0x007028AA, GetRemappingModeHook, true);
  injector::MakeCALL(0x006FC03B, GoRemapButtonHook, true);
  injector::MakeCALL(0x6FC0E9, GoClearButtonHook, true);
  injector::MakeJMP(0x006A5940, GetNumExternalDevices, true);
  injector::MakeCALL(0x005352FF, GetNumExternalDevicesZero, true);
  injector::MakeCALL(0x005354FC, GetNumExternalDevicesZero, true);
  injector::MakeCALL(0x00535596, GetNumExternalDevicesZero, true);
  injector::MakeJMP(0x006A5950, GetXtendedInputDeviceName, true);
  injector::MakeCALL(0x0070BCEC, GetMappingStringHook, true);
  injector::MakeCALL(0x0070BD0A, GetMappingStringHook, true);
  injector::WriteMemory<uintptr_t>(0x00991AFC, (uintptr_t)&ResetMappingsToDefault, true);
  // sell car string
  injector::WriteMemory<uint32_t>(0x005C6216, 0x6DBAAF8C, true);
  injector::WriteMemory<uint32_t>(0x005D8723, 0x6DBAAF8C, true);
  injector::WriteMemory<uint32_t>(0x005DDAF1, 0x6DBAAF8C, true);
  injector::WriteMemory<uint32_t>(0x005E2B3C, 0x6DBAAF8C, true);
  injector::WriteMemory<uint32_t>(0x005EB296, 0x6DBAAF8C, true);
  injector::WriteMemory<uint32_t>(0x007EACC0, 0x6DBAAF8C, true);
  injector::WriteMemory<uint32_t>(0x007EAD0B, 0x6DBAAF8C, true);

#endif

#endif

  SetUnbindableButtonTextures();
#endif  // GAME_UC

#ifdef GAME_WORLD
  // RECALCULATE FUNCTION POINTERS -- they shift around
  UTL_Com_Object_IList_Constructor = (void*(__thiscall*)(void*, unsigned int))UTL_ILIST_CONSTRUCTOR_ADDR;
  // InitProfileSettings = (void(*)())(0x756960 + MainBase);
  Managed_Alloc = (void* (*)(int))MANAGED_ALLOCATOR_ADDR;

  loc_75E375 = 0x75E375 + MainBase;
  loc_75E363 = 0x75E363 + MainBase;
  loc_75E237 = 0x75E237 + MainBase;
  loc_75E225 = 0x75E225 + MainBase;
  loc_75E25F = 0x75E25F + MainBase;

  injector::MakeJMP(0x0075E358 + MainBase, UnscrewWorldDeviceScalars, true);
  injector::MakeJMP(0x0075E217 + MainBase, UnscrewWorldDeviceScalars2, true);
  injector::MakeJMP(0x0075E254 + MainBase, UnscrewWorldDeviceScalars3, true);
#endif
  if (bMouseLook) {
    // Mouselook hook
    // dereference the current function after initing to maximize compatibility
    DebugWorldCameraMover_Update_Addr = *(unsigned int*)DEBUGWORLDCAMERAMOVER_UPDATE_VT_ADDR;
    DebugWorldCameraMover_Update      = (void(__thiscall*)(void*, float))DebugWorldCameraMover_Update_Addr;
    injector::WriteMemory<unsigned int>(DEBUGWORLDCAMERAMOVER_UPDATE_VT_ADDR, (unsigned int)&DebugWorldCameraMover_Update_Hook, true);

    injector::MakeJMP(DEBUGWORLDCAMERAMOVER_CAVE1_ENTRY, DebugWorldCameraMover_Cave1, true);
    injector::MakeJMP(DEBUGWORLDCAMERAMOVER_CAVE2_ENTRY, DebugWorldCameraMover_Cave2, true);
#ifdef GAME_WORLD
    DebugWorldCameraMover_Cave1_Exit     = DEBUGWORLDCAMERAMOVER_CAVE1_EXIT;
    DebugWorldCameraMover_Cave1_FullExit = DEBUGWORLDCAMERAMOVER_CAVE1_FULLEXIT;
    DebugWorldCameraMover_Cave2_Exit     = DEBUGWORLDCAMERAMOVER_CAVE2_EXIT;
    DebugWorldCameraMover_Constructor    = (void*(__thiscall*)(void*, void*, void*, int, int))DEBUGWORLDCAMERAMOVER_CONSTRUCTOR_ADDR;
    DebugWorldCameraMover_Destructor     = (void*(__thiscall*)(void*))DEBUGWORLDCAMERAMOVER_DESTRUCTOR_ADDR;
#endif

    // implement DebugWorldCameraMover detection -- for cursor hiding
    injector::MakeCALL(DEBUGWORLDCAMERAMOVER_CONSTRUCTOR_HOOK_ADDR, DebugWorldCameraMover_Constructor_Hook, true);
    injector::MakeCALL(DEBUGWORLDCAMERAMOVER_DESTRUCTOR_HOOK_ADDR, DebugWorldCameraMover_Destructor_Hook, true);
  }
  // Init state
  ZeroMemory(g_Controllers, sizeof(CONTROLLER_STATE) * MAX_CONTROLLERS);

  // AttachConsole(ATTACH_PARENT_PROCESS);
  // AllocConsole();
  // freopen("CON", "w", stdout);
  // freopen("CON", "w", stderr);

  return 0;
}

extern "C" __declspec(dllexport) bool GetPollingState() { return bGlobalDoPolling; }

extern "C" __declspec(dllexport) bool SetPollingState(bool state) {
  bGlobalDoPolling = state;
  return state;
}
