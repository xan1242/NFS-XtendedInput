// Need for Speed (Black Box, MW & newer) - Xtended Input plugin
// Bringing native XInput to NFS
// by Xan/Tenjoin

// TODO: bring rumble/vibration function
// TODO: remapping? -- partially done, but only 1 event per key
// TODO: port to other games
// TODO: kill DInput enough so that it doesn't detect XInput controllers but still detects wheels
// TODO: proper raw input for keyboard (and maybe non XInput gamepads?)
// TODO: implement the Controls settings menu - it should be possible to make it talk to the INI
// TODO: loading screen control tips
// TODO (MW): Max performance button is visible in all submenus during Customize for some reason...
// TODO (MW): CUSTOMIZE MENU IS BUGGY - during career cash status overlaps the max performance buttons...
// TODO (Carbon): fix mouse wheel zooming (in FE only) and keyboard zooming during photo mode
// TODO (Carbon): ingame button prompt during tutorial - find the texture hash for that and replace it
// TODO (Undercover): UI texture switching & maybe higher res support - it was excluded because UI is working with APT and not FEng (only the HUD works on FEng)
// TODO (Pro Street): multiplayer menu UI shows some missing strings on console objects... also shows too many of them at the end of race so that needs investigating. Tested in LAN mode.
// TODO (World): IT'S RANDOM! It sometimes works, sometimes not... I'm speechless at the moment. Currently no idea if even the powerups trigger on the D-Pad at all! (because I have no way to test it currently! I'm testing with Berkay's old Offline Server which has broken powerups.)
// TODO (World): make proper config, currently using a mostly copy-paste from Carbon...
// TODO (World): mouselook, maybe


#include "stdafx.h"
#include "stdio.h"
#include <windows.h>
#include "includes\injector\injector.hpp"
#include "includes\IniReader.h"
#include "NFS_XtendedInput.h"
#include "NFS_XtentedInput_ActionID.h"

#if (_WIN32_WINNT >= 0x0602 /*_WIN32_WINNT_WIN8*/)
#include <XInput.h>
#pragma comment(lib,"xinput.lib")
#else
#include <XInput.h>
#pragma comment(lib,"xinput9_1_0.lib")
#endif

#include "NFS_XtendedInput_XInputConfig.h"
#include "NFS_XtendedInput_VKHash.h"

#ifdef GAME_MW
#include "NFSMW_XtendedInput.h"
#endif
#ifdef GAME_CARBON
#include "NFSC_XtendedInput.h"
float FEActivationFloat = 0.999999f;
#endif
#ifdef GAME_PROSTREET
#include "NFSPS_XtendedInput.h"
float FEActivationFloat = 0.999999f;
#endif
#ifdef GAME_UC
#include "NFSUC_XtendedInput.h"
#endif
#ifdef GAME_WORLD
#include "NFSW_XtendedInput.h"
#endif

#ifndef NO_FENG
#include "NFS_XtendedInput_FEng.h"
#endif

#define MAX_CONTROLLERS 4  // XInput handles up to 4 controllers 

WORD INPUT_DEADZONE_LS = (0.24f * FLOAT(0x7FFF));  // Default to 24% of the +/- 32767 range.   This is a reasonable default value but can be altered if needed.
WORD INPUT_DEADZONE_RS = (0.24f * FLOAT(0x7FFF));  // Default to 24% of the +/- 32767 range.   This is a reasonable default value but can be altered if needed.
WORD INPUT_DEADZONE_LS_P2 = (0.24f * FLOAT(0x7FFF));  // Default to 24% of the +/- 32767 range.   This is a reasonable default value but can be altered if needed.
WORD INPUT_DEADZONE_RS_P2 = (0.24f * FLOAT(0x7FFF));  // Default to 24% of the +/- 32767 range.   This is a reasonable default value but can be altered if needed.
WORD SHIFT_ANALOG_THRESHOLD = (0.75f * FLOAT(0x7FFF));  // 75% for shifting
WORD FEUPDOWN_ANALOG_THRESHOLD = (0.50f * FLOAT(0x7FFF));  // 50% for analog sticks digital activation
WORD TRIGGER_ACTIVATION_THRESHOLD = (0.12f * FLOAT(0xFF));  // 12% for analog triggers digital activation

// for triggering the over-zelaous inputs once in a tick...
WORD bQuitButtonOldState = 0;

struct CONTROLLER_STATE
{
	XINPUT_STATE state;
	bool bConnected;
}g_Controllers[MAX_CONTROLLERS];

// KB Input declarations
//unbuffered -- calls GetAsyncKeyState during scanning process itself -- probably more taxing, but very good input latency (default)
#define KB_READINGMODE_UNBUFFERED_ASYNC 0
// buffered -- calls GetKeyboardState right after reading joypads, updates VKeyStates
#define KB_READINGMODE_BUFFERED 1

BYTE VKeyStates[2][256];
unsigned int KeyboardReadingMode = 0; // 0 = buffered synchronous, 1 = unbuffered asynchronous
bool bMouseLook = true;

void*(__thiscall* UTL_Com_Object_IList_Constructor)(void* thethis, unsigned int unk) = (void*(__thiscall*)(void*, unsigned int))UTL_ILIST_CONSTRUCTOR_ADDR;
void (__thiscall* DebugWorldCameraMover_Update)(void* thethis, float unk) = (void (__thiscall*)(void*, float))DebugWorldCameraMover_Update_Addr;

void* (__thiscall* DebugWorldCameraMover_Constructor)(void* thethis, void* vector1, void* vector2, int joyport, int unk) = (void* (__thiscall*)(void*, void*, void*, int, int))DEBUGWORLDCAMERAMOVER_CONSTRUCTOR_ADDR;
void* (__thiscall* DebugWorldCameraMover_Destructor)(void* thethis) = (void* (__thiscall*)(void*))DEBUGWORLDCAMERAMOVER_DESTRUCTOR_ADDR;

int bStringHash(char* str)
{
	if (str == NULL)
		return 0;

	char* _str = ((char*)str);
	int result = -1;

	while (*_str != 0)
	{
		result = result * 0x21 + (unsigned int)(*_str);
		_str = _str + 1;
	}

	return result;
}

// global var for the action ID values
float PrevValues[2][MAX_ACTIONID];
float CurrValues[2][MAX_ACTIONID];
// button bindings per action ID
WORD XInputBindings[MAX_ACTIONID];
// secondary bindings, exclusively for FE navigation with left stick as well (not just D-Pad)
WORD FE_Secondary_Up;
WORD FE_Secondary_Down;
WORD FE_Secondary_Left;
WORD FE_Secondary_Right;

BYTE VKeyBindings[MAX_ACTIONID];

// mouselook vars
int MouseLook_XSpeed = 0;
int MouseLook_YSpeed = 0;
float MouseLookSensitivity = 1.0f;

bool bEnteredWorldMapOnce = false;

enum DeviceScalarType
{
	kJoyAxis = 0,
	kAnalogButton = 1,
	kDigitalButton = 2,
};

struct DeviceScalarInfo
{
	const char* name;
	DeviceScalarType type;
	int system_index;
	int space[10]; // space for multiple binds?
};

class DeviceScalar
{
public:
	DeviceScalarType fType;
	unsigned int fName_CRC32;
	float* fPrevValue;
	float* fCurrentValue;
	DeviceScalar()
	{
		fType = (DeviceScalarType)0;
		fName_CRC32 = 0;
		fPrevValue = NULL;
		fCurrentValue = NULL;
	}
};

enum ShiftPotential
{
	SHIFT_POTENTIAL_NONE = 0,
	SHIFT_POTENTIAL_DOWN = 1,
	SHIFT_POTENTIAL_UP = 2,
	SHIFT_POTENTIAL_GOOD = 3,
	SHIFT_POTENTIAL_PERFECT = 4,
	SHIFT_POTENTIAL_MISS = 5,
};

enum InputUpdateType
{
	kUpdate = 0,
	kPress = 1,
	kRelease = 2,
	kAnalogPress = 3,
	kAnalogRelease = 4,
	kCenterControl = 5,
};

struct InputMapEntry
{
	InputUpdateType UpdateType;
	float LowerDZ;
	float UpperDZ;
	ActionID Action;
	int DeviceScalarIndex;
#ifndef GAME_MW
	int ComboDeviceScalarIndex;
#endif
	float PreviousValue;
	float CurrentValue;
};

HRESULT UpdateControllerState()
{
	DWORD dwResult;

	dwResult = XInputGetState(0, &g_Controllers[0].state);

	if (dwResult == ERROR_SUCCESS)
	{
		g_Controllers[0].bConnected = true;

		// Zero value if thumbsticks are within the dead zone 
		if ((g_Controllers[0].state.Gamepad.sThumbLX < INPUT_DEADZONE_LS &&
			g_Controllers[0].state.Gamepad.sThumbLX > -INPUT_DEADZONE_LS) &&
			(g_Controllers[0].state.Gamepad.sThumbLY < INPUT_DEADZONE_LS &&
				g_Controllers[0].state.Gamepad.sThumbLY > -INPUT_DEADZONE_LS))
		{
			g_Controllers[0].state.Gamepad.sThumbLX = 0;
			g_Controllers[0].state.Gamepad.sThumbLY = 0;
		}

		if ((g_Controllers[0].state.Gamepad.sThumbRX < INPUT_DEADZONE_RS &&
			g_Controllers[0].state.Gamepad.sThumbRX > -INPUT_DEADZONE_RS) &&
			(g_Controllers[0].state.Gamepad.sThumbRY < INPUT_DEADZONE_RS &&
				g_Controllers[0].state.Gamepad.sThumbRY > -INPUT_DEADZONE_RS))
		{
			g_Controllers[0].state.Gamepad.sThumbRX = 0;
			g_Controllers[0].state.Gamepad.sThumbRY = 0;
		}
#ifndef NO_FENG
		if ((g_Controllers[0].state.Gamepad.wButtons || g_Controllers[0].state.Gamepad.sThumbLX || g_Controllers[0].state.Gamepad.sThumbLY || g_Controllers[0].state.Gamepad.sThumbRX || g_Controllers[0].state.Gamepad.sThumbRY || g_Controllers[0].state.Gamepad.bRightTrigger || g_Controllers[0].state.Gamepad.bLeftTrigger) && bUseDynamicFEngSwitching)
		{
			LastControlledDevice = LASTCONTROLLED_CONTROLLER;
		}
#endif

	}
	else
	{
		g_Controllers[0].bConnected = false;
	}

	dwResult = XInputGetState(1, &g_Controllers[1].state);

	if (dwResult == ERROR_SUCCESS)
	{
		g_Controllers[1].bConnected = true;

		// Zero value if thumbsticks are within the dead zone 
		if ((g_Controllers[1].state.Gamepad.sThumbLX < INPUT_DEADZONE_LS_P2 &&
			g_Controllers[1].state.Gamepad.sThumbLX > -INPUT_DEADZONE_LS_P2) &&
			(g_Controllers[1].state.Gamepad.sThumbLY < INPUT_DEADZONE_LS_P2 &&
				g_Controllers[1].state.Gamepad.sThumbLY > -INPUT_DEADZONE_LS_P2))
		{
			g_Controllers[1].state.Gamepad.sThumbLX = 0;
			g_Controllers[1].state.Gamepad.sThumbLY = 0;
		}

		if ((g_Controllers[1].state.Gamepad.sThumbRX < INPUT_DEADZONE_RS_P2 &&
			g_Controllers[1].state.Gamepad.sThumbRX > -INPUT_DEADZONE_RS_P2) &&
			(g_Controllers[1].state.Gamepad.sThumbRY < INPUT_DEADZONE_RS_P2 &&
				g_Controllers[1].state.Gamepad.sThumbRY > -INPUT_DEADZONE_RS_P2))
		{
			g_Controllers[1].state.Gamepad.sThumbRX = 0;
			g_Controllers[1].state.Gamepad.sThumbRY = 0;
		}
	}
	else
	{
		g_Controllers[1].bConnected = false;
	}

	return S_OK;
}

// old function -- repurposed to send keyboard-exclusive commands to the game
void ReadXInput_Extra()
{
#ifndef NO_QUIT_BUTTON
	if (g_Controllers[0].bConnected)
	{
		WORD wButtons = g_Controllers[0].state.Gamepad.wButtons;

		if ((wButtons & XINPUT_GAMEPAD_BACK) != bQuitButtonOldState)
		{
			if ((wButtons & XINPUT_GAMEPAD_BACK)) // trigger once only on button down state
			{

				FESendKeystroke('Q');

			}
			bQuitButtonOldState = (wButtons & XINPUT_GAMEPAD_BACK);
		}

#ifdef GAME_MW
		// simulate mouse cursor for map movement
		if (*(int*)GAMEFLOWMANAGER_STATUS_ADDR == 6)
		{
			if (g_Controllers[0].state.Gamepad.sThumbRX || g_Controllers[0].state.Gamepad.sThumbRY)
				bLastUsedVirtualMouse = true;

			if (bLastUsedVirtualMouse && LastControlledDevice == LASTCONTROLLED_CONTROLLER && cFEng_FindPackage("WorldMapMain.fng"))
			{
				if (bUseWin32Cursor)
				{
					if (!bEnteredWorldMapOnce && !bConfineMouse) // force confine a mouse if not already, we don't want the mouse to stray away from the game if we're using a controller
					{
						RECT windowRect;
						GetWindowRect(*(HWND*)GAME_HWND_ADDR, &windowRect);
						ClipCursor(&windowRect);
					}

					POINT MousePos;
					GetCursorPos(&MousePos);
					SetCursorPos(MousePos.x + ((g_Controllers[0].state.Gamepad.sThumbRX) / 0x1000), MousePos.y + (-g_Controllers[0].state.Gamepad.sThumbRY / 0x1000));
				}
				else
				{
					*(int*)FEMOUSECURSOR_X_ADDR += (g_Controllers[0].state.Gamepad.sThumbRX / 0x1000);
					*(int*)FEMOUSECURSOR_Y_ADDR += (-g_Controllers[0].state.Gamepad.sThumbRY / 0x1000);
				}

				bMousePressedDown = g_Controllers[0].state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER;
				bEnteredWorldMapOnce = true;
			}
			else
			{
				if (bEnteredWorldMapOnce && !bConfineMouse && bUseWin32Cursor)
					ClipCursor(NULL);
				bEnteredWorldMapOnce = false;
				bLastUsedVirtualMouse = false;
			}
		}
		else
		{
			if (bEnteredWorldMapOnce && !bConfineMouse && bUseWin32Cursor)
				ClipCursor(NULL);
			bEnteredWorldMapOnce = false;
			bLastUsedVirtualMouse = false;
		}
#endif
	}
#endif // GAME_UC
}

void __stdcall ReadControllerData()
{
	UpdateControllerState();
	ReadXInput_Extra();
	if (KeyboardReadingMode == KB_READINGMODE_BUFFERED)
		GetKeyboardState(VKeyStates[0]);

	// read mouse info for Debug World Camera
	if (bInDebugWorldCamera)
	{
		RECT windowRect;
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
bool bIsHudVisible()
{
	if (*(int*)GAMEFLOWMANAGER_STATUS_ADDR == 6)
	{
		int FirstLocalPlayer = **(int**)PLAYER_ILISTABLE_ADDR;
		if (FirstLocalPlayer)
		{
			int LocalPlayerVtable = *(int*)(FirstLocalPlayer);
			int(__thiscall * LocalPlayer_GetHUD)(void* dis) = (int(__thiscall*)(void*)) * (int*)(LocalPlayerVtable + PLAYER_GETHUD_VTABLE_OFFSET);
			int LocalPlayerHUD = LocalPlayer_GetHUD((void*)FirstLocalPlayer);
			if (LocalPlayerHUD)
			{
				int LocalPlayerHUDVtable = *(int*)(LocalPlayerHUD);
				int(__thiscall * FEngHud_IsHudVisible)(void* dis) = (int(__thiscall*)(void*)) * (int*)(LocalPlayerHUDVtable + FENGHUD_ISVISIBLE_VTABLE_OFFSET);

				if (FEngHud_IsHudVisible((void*)LocalPlayerHUD)) // is HUD being drawn at all
					return true;
			}
		}
	}
	return false;
}
#else
bool bIsHudVisible()
{
	return false;
}

// world, for whatever reason, uses direct pointers to the class instead of accessing the device scalar array normally
// copied this code snippet from Carbon 0x00696166
unsigned int loc_75E375 = 0x75E375 + MainBase;
unsigned int loc_75E363 = 0x75E363 + MainBase;
void __declspec(naked) UnscrewWorldDeviceScalars()
{
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
void __declspec(naked) UnscrewWorldDeviceScalars2()
{
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
void __declspec(naked) UnscrewWorldDeviceScalars3()
{
	_asm
	{
		mov edx, [ebx + 0x18]
		shl edi, 4
		add edi, edx
		jmp loc_75E25F
	}
}


#endif
bool bCheckSecondBind()
{
	return FE_Secondary_Up || FE_Secondary_Down || FE_Secondary_Left || FE_Secondary_Right;
}

float GetAnalogStickValue(int index, WORD bind)
{
	float result = 0.0f;
	int rdi = index;
	switch (bind)
	{
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
		if (g_Controllers[rdi].state.Gamepad.sThumbLY > 0)
			result = g_Controllers[rdi].state.Gamepad.sThumbLY / (float)(0x7FFF);
		break;
	case XINPUT_GAMEPAD_LS_DOWN_CONFIGDEF:
		if (g_Controllers[rdi].state.Gamepad.sThumbLY < 0)
			result = -g_Controllers[rdi].state.Gamepad.sThumbLY / (float)(0x7FFF);
		break;
	case XINPUT_GAMEPAD_LS_LEFT_CONFIGDEF:
		if (g_Controllers[rdi].state.Gamepad.sThumbLX < 0)
			result = -g_Controllers[rdi].state.Gamepad.sThumbLX / (float)(0x7FFF);
		break;
	case XINPUT_GAMEPAD_LS_RIGHT_CONFIGDEF:
		if (g_Controllers[rdi].state.Gamepad.sThumbLX > 0)
			result = g_Controllers[rdi].state.Gamepad.sThumbLX / (float)(0x7FFF);
		break;
	case XINPUT_GAMEPAD_RS_UP_CONFIGDEF:
		if (g_Controllers[rdi].state.Gamepad.sThumbRY > 0)
			result = g_Controllers[rdi].state.Gamepad.sThumbRY / (float)(0x7FFF);
		break;
	case XINPUT_GAMEPAD_RS_DOWN_CONFIGDEF:
		if (g_Controllers[rdi].state.Gamepad.sThumbRY < 0)
			result = -g_Controllers[rdi].state.Gamepad.sThumbRY / (float)(0x7FFF);
		break;
	case XINPUT_GAMEPAD_RS_LEFT_CONFIGDEF:
		if (g_Controllers[rdi].state.Gamepad.sThumbRX < 0)
			result = -g_Controllers[rdi].state.Gamepad.sThumbRX / (float)(0x7FFF);
		break;
	case XINPUT_GAMEPAD_RS_RIGHT_CONFIGDEF:
		if (g_Controllers[rdi].state.Gamepad.sThumbRX > 0)
			result = g_Controllers[rdi].state.Gamepad.sThumbRX / (float)(0x7FFF);
		break;
	default:
		break;
	}
	return result;
}

bool bGetAnalogDigitalActivationState(int index, WORD bind, int actid)
{
	WORD threshold = FEUPDOWN_ANALOG_THRESHOLD;
	if (actid == GAMEACTION_SHIFTUP || actid == GAMEACTION_SHIFTDOWN)
		threshold = SHIFT_ANALOG_THRESHOLD;

	bool bCurrentXInputKeyState = false;
	int rdi = index;
	switch (bind)
	{
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

#pragma runtime_checks( "", off )
float DebugWorldCameraMover_16bit_max = 65536.0f;
bool bDisableDebugWorldCameraUpdating = true;

// caves necessary as original values are in 16-bit values, this extends it to 32-bits

int DebugWorldCameraMover_Cave1_Exit = DEBUGWORLDCAMERAMOVER_CAVE1_EXIT;
int DebugWorldCameraMover_Cave1_FullExit = DEBUGWORLDCAMERAMOVER_CAVE1_FULLEXIT;
void __declspec(naked) DebugWorldCameraMover_Cave1()
{
	if (bDisableDebugWorldCameraUpdating)
		_asm jmp DebugWorldCameraMover_Cave1_FullExit

#ifndef DEBUGWORLDCAMERAMOVER_MMX
	_asm
	{
		mov eax, MouseLook_XSpeed
#ifdef DEBUGWORLDCAMERAMOVER_FSTP
		fstp st
#endif
		mov [esp+DEBUGWORLDCAMERAMOVER_STACK_OFFSET], eax
		jmp DebugWorldCameraMover_Cave1_Exit
	}
#else // NFS Undercover and World use the MMX registers and SSE2 instructions extensively
#ifdef GAME_UC
	_asm
	{
		movss xmm2, dword ptr [esp+0x28]
		cvtps2pd xmm0, xmm0
		mov eax, MouseLook_XSpeed
		cvtsi2ss xmm1, eax
		jmp DebugWorldCameraMover_Cave1_Exit
	}
#endif
#ifdef GAME_WORLD
	_asm
	{
		movss   xmm1, DebugWorldCameraMover_16bit_max
		movss   xmm0, dword ptr[ebp - 0x18]
		mulss   xmm0, xmm1
		cvttss2si eax, xmm0
		movss   xmm0, dword ptr[ebp - 0x20]
		movaps  xmm2, xmm0
		mulss   xmm2, xmm0
		movss   xmm0, dword ptr[ebp - 0x1C]
		movaps  xmm3, xmm0
		mulss   xmm3, xmm0
		addss   xmm2, xmm3
		mov ecx, MouseLook_XSpeed
		add edi, ecx
		jmp DebugWorldCameraMover_Cave1_Exit
	}
#endif
#endif
}

int DebugWorldCameraMover_Cave2_Exit = DEBUGWORLDCAMERAMOVER_CAVE2_EXIT;
void __declspec(naked) DebugWorldCameraMover_Cave2()
{
#ifndef DEBUGWORLDCAMERAMOVER_MMX
	_asm
	{
		mov eax, MouseLook_YSpeed
		mov	[esp+DEBUGWORLDCAMERAMOVER_STACK_OFFSET2], eax
		jmp DebugWorldCameraMover_Cave2_Exit
	}
#else // NFS Undercover and World use the MMX registers and SSE2 instructions extensively
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
	_asm
	{
		add eax, MouseLook_YSpeed
		mov [ebp-4], eax
		jmp DebugWorldCameraMover_Cave2_Exit
	}
#endif
#endif
}

// during DebugWorldCameraMover - set the cursor position to the center of the window for relative calculations
void __stdcall DebugWorldCameraMover_Update_Hook(float unk)
{
	unsigned int thethis;
	_asm mov thethis, ecx

	// add original speeds
	MouseLook_XSpeed += *(short*)(thethis + DEBUGWORLDCAMERAMOVER_XSPEED_OFFSET);
	MouseLook_YSpeed += *(short*)(thethis + DEBUGWORLDCAMERAMOVER_YSPEED_OFFSET);

#ifndef DEBUGWORLDCAMERAMOVER_MMX
	if (!MouseLook_XSpeed && !MouseLook_YSpeed && !CurrValues[0][DEBUGACTION_MOVE_UP] && !CurrValues[0][DEBUGACTION_MOVE_DOWN])
		bDisableDebugWorldCameraUpdating = true;
	else
		bDisableDebugWorldCameraUpdating = false;
#else
	if (!MouseLook_XSpeed && !MouseLook_YSpeed && !CurrValues[0][DEBUGACTION_MOVE_UP] && !CurrValues[0][DEBUGACTION_MOVE_DOWN] && !CurrValues[0][DEBUGACTION_MOVE_LEFT] && !CurrValues[0][DEBUGACTION_MOVE_RIGHT])
		bDisableDebugWorldCameraUpdating = true;
	else
		bDisableDebugWorldCameraUpdating = false;
#endif
	return DebugWorldCameraMover_Update((void*)thethis, unk);
}

// constructor/destructor hooks for mouse hiding

void* __stdcall DebugWorldCameraMover_Constructor_Hook(void* vector1, void* vector2, int joyport, int unk)
{
	unsigned int thethis;
	_asm mov thethis, ecx
	bInDebugWorldCamera = true;
	RECT windowRect;
	GetWindowRect(*(HWND*)GAME_HWND_ADDR, &windowRect);
	int CenterX = ((windowRect.right - windowRect.left) / 2) + windowRect.left;
	int CenterY = ((windowRect.bottom - windowRect.top) / 2) + windowRect.top;
	SetCursorPos(CenterX, CenterY);


	return DebugWorldCameraMover_Constructor((void*)thethis, vector1, vector2, joyport, unk);
}
void* DebugWorldCameraMover_Destructor_Hook()
{
	unsigned int thethis;
	_asm mov thethis, ecx

	bInDebugWorldCamera = false;

	return DebugWorldCameraMover_Destructor((void*)thethis);
}

#pragma runtime_checks( "", restore )

// Mouselook stuff end

// based on MW -- may change across games
class InputDevice
{
	long Padding1;
	long FeedbackHandle;
	long UnkHandle;
	long UnkHandle2;
	long Padding2;
public:
	DeviceScalar* fDeviceScalar;
	float* fPrevValues;
	float* fCurrentValues;
	int fDeviceIndex;
	float fControllerCurve;

	InputDevice(int DeviceIndex)
	{
		memset(&Padding1, 0, sizeof(long) * 5);
		UTL_Com_Object_IList_Constructor(&Padding1, 4);
		fDeviceIndex = DeviceIndex;
		fControllerCurve = 1.0f;
		fPrevValues = PrevValues[DeviceIndex];
		fCurrentValues = CurrValues[DeviceIndex];
		fDeviceScalar = new (nothrow) DeviceScalar[MAX_ACTIONID];
#ifndef NO_PROFILE_SETTINGS
		InitProfileSettings();
#endif
	}
	virtual void* dtor(bool something)
	{
		delete(this);
		return this;
	}
	virtual bool IsConnected()
	{
		if (fDeviceIndex)
			return g_Controllers[fDeviceIndex].bConnected;
		return true;
	}
	virtual bool IsWheel()
	{
		return false;
	}
	virtual void Initialize()
	{
		//printf("Called InputDevice::Initialize\n");
		//*(bool*)FEMOUSECURSOR_ISHIDDEN_ADDR = false;

		CIniReader inireader("");
		unsigned int inXInputConfigDef = 0;

		// for each action ID generate an appropriate config
		for (unsigned int i = 0; i < MAX_ACTIONID; i++)
		{
			if (bIsActionDigitalButton((ActionID)i))
				fDeviceScalar[i].fType = kDigitalButton;
			else
				fDeviceScalar[i].fType = kJoyAxis;

			fDeviceScalar[i].fName_CRC32 = stringhash32(ActionIDStr[i]);
			fDeviceScalar[i].fPrevValue = &PrevValues[fDeviceIndex][i];
			fDeviceScalar[i].fCurrentValue = &CurrValues[fDeviceIndex][i];

			// read the key bindings
			VKeyBindings[i] = ConvertVKNameToValue(inireader.ReadString("EventsKB", ActionIDStr[i], ""));
			if (VKeyBindings[i] == 0)
			{
				// try checking for single-char
				char lettercheck[32];
				strcpy(lettercheck, inireader.ReadString("EventsKB", ActionIDStr[i], ""));
				if (strlen(lettercheck) == 1)
					VKeyBindings[i] = toupper(lettercheck[0]);
			}

			
			inXInputConfigDef = ConvertXInputOtherConfigDef(inireader.ReadString("Events", ActionIDStr[i], ""));
			if (!inXInputConfigDef)
				XInputBindings[i] = ConvertXInputNameToBitmask(inireader.ReadString("Events", ActionIDStr[i], ""));
			else
				XInputBindings[i] = inXInputConfigDef;
#ifndef NO_FENG
			if (bIsActionTextureBindable((ActionID)i))
				SetBindingButtonTexture((ActionID)i, XInputBindings[i]);
#endif
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
	virtual void PollDevice()
	{
		ReadControllerData();
		WORD wButtons = g_Controllers[fDeviceIndex].state.Gamepad.wButtons;
		WORD SecondBind = 0;
		int rdi = fDeviceIndex;
		bool bCurrentXInputKeyState = false;
		bool bCurrentXInputKeyState2 = false;
		bool bCurrentVKeyState = false;
		bool bDoPolling = true;

		//printf("FEngPkg: %s\n", cFEng_FindPackageWithControl_Name());

#ifdef GAME_MW
		bool bInShowcase = false;
		if (*(int*)GAMEFLOWMANAGER_STATUS_ADDR == 3)
		{
			if (cFEng_FindPackage(FE_SHOWCASE_FNG_NAME) || cFEng_FindPackage("UI_DebugCarCustomize.fng") || *(bool*)CARGUYSCAMERA_ADDR)
				bInShowcase = true;
		}
#endif
		for (unsigned int i = 0; i < MAX_ACTIONID; i++)
		{
			if (bIsActionFrontEnd((ActionID)i) && bIsHudVisible())
				bDoPolling = false;

#ifdef GAME_WORLD
			// I *truly* do not understand this game. So, the inputs will only start working if a button was pressed BEFORE entering the game.
			// So I'll set an action to a high state while in FE. FE isn't even controlled by the controller, so it should be safe.
			if (*(int*)GAMEFLOWMANAGER_STATUS_ADDR != 6)
			{
				CurrValues[fDeviceIndex][i] = 1.0;
				bDoPolling = false;
			}
#endif

			if (bDoPolling)
			{
				float fresult = 0;
				rdi = fDeviceIndex;
				if (KeyboardReadingMode == KB_READINGMODE_BUFFERED)
					bCurrentVKeyState = VKeyStates[0][VKeyBindings[i]] >> 7;
				else
					bCurrentVKeyState = GetAsyncKeyState(VKeyBindings[i]) >> 15;

				if (bIsActionDebug((ActionID)i))
				{
					rdi = 1; // debug (camera) actions are always read from the second port, but the PC version omits it entirely so this is a workaround

				}


				wButtons = g_Controllers[rdi].state.Gamepad.wButtons;

				if (bIsActionAnalog((ActionID)i))
				{
					if (bIsBindingAnalog(XInputBindings[i]))
						fresult = GetAnalogStickValue(rdi, XInputBindings[i]);
					else
					{
						bCurrentXInputKeyState = wButtons & XInputBindings[i];
						// keyboard & digital buttons
						if (bCurrentXInputKeyState)
							fresult = 1.0f;
					}

					if (bCurrentVKeyState)
						fresult = 1.0f;
				}
				else
				{
					if (bCheckSecondBind() && bIsActionFrontEndNavigation((ActionID)i))
					{
						switch (i)
						{
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

						if (bIsBindingAnalog(SecondBind)) // digital directions with an axis
						{
							bCurrentXInputKeyState2 = bGetAnalogDigitalActivationState(rdi, SecondBind, i);
						}

						else
							bCurrentXInputKeyState2 = wButtons & SecondBind;
					}
					else
					{
						SecondBind = 0;
						bCurrentXInputKeyState2 = false;
					}

					if (bIsBindingAnalog(XInputBindings[i])) // digital directions with an axis
					{
						bCurrentXInputKeyState = bGetAnalogDigitalActivationState(rdi, XInputBindings[i], i);
						if (bCurrentXInputKeyState || bCurrentXInputKeyState2)
							fresult = 1.0f;
					}
					else
					{
						bCurrentXInputKeyState = wButtons & XInputBindings[i];
						// determine final digital status
						if (bCurrentXInputKeyState || bCurrentXInputKeyState2)
							fresult = 1.0f;
					}
					if (bCurrentVKeyState)
						fresult = 1.0f;
				}

				// write value to the main array
				CurrValues[fDeviceIndex][i] = fresult;

				// update prev values
				if (CurrValues[fDeviceIndex][i] != PrevValues[fDeviceIndex][i])
				{
					// do one-time triggers here...
#ifdef GAME_MW
					if (bIsActionRelativePosition((ActionID)i) && bInShowcase)
					{
						if (CurrValues[fDeviceIndex][i] > 0)
							CurrValues[fDeviceIndex][i] = CurrValues[fDeviceIndex][i] - PrevValues[fDeviceIndex][i];
						else
							CurrValues[fDeviceIndex][i] = 0;
					}
#endif
					PrevValues[fDeviceIndex][i] = CurrValues[fDeviceIndex][i];
				}
				else if (bIsActionDigitalButton((ActionID)i))
				{
					if (CurrValues[fDeviceIndex][i] > 0)
						CurrValues[fDeviceIndex][i] = 0;
				}
			}
			bDoPolling = true;
		}

	}
	virtual int GetNumDeviceScalar()
	{
		return MAX_ACTIONID;
	}
	virtual void StartVibration()
	{
		//printf("Called InputDevice::StartVibration\n");
	}
	virtual void StopVibration()
	{
		// this seems to be only called when the controller is unplugged...
		//printf("Called InputDevice::StopVibration\n");
	}
	virtual int GetInterfaces()
	{
		return 0;
	}
	virtual int GetSecondaryDevice()
	{
		return 0;
	}
#ifndef GAME_MW
	virtual int GetBatteryLife()
	{
		return 0;
	}
	virtual bool IsBatteryLow()
	{
		return false;
	}
	virtual int GetControllerTypeMask()
	{
		return 0;
	}
#endif
	virtual bool DeviceHasChanged()
	{
		return false;
	}
#ifndef GAME_MW
	virtual bool DeviceHasAnyActivity()
	{
		return false;
	}
#endif
	virtual void* SaveCurrentState()
	{
		return NULL;
	}
	virtual void RestoreToState(float* unk)
	{
		return;
	}
#ifndef GAME_MW
	virtual int GetDeviceNoiseAllowance()
	{
		return 0;
	}
#endif
};

InputDevice* InputDeviceFactory(int DeviceIndex)
{
	InputDevice* out = new InputDevice(DeviceIndex);

	return out;
}

#pragma runtime_checks( "", off )


#ifndef GAME_WORLD
void* (__thiscall*FastMem_Alloc)(void* FastMem, int size, char* debug_str) = (void* (__thiscall*)(void*, int, char*))FASTMEM_ALLOC_ADDR;
#endif


#ifdef GAME_MW
void* (*FastMem_InitListAllocator)() = (void* (*)())FASTMEM_INITLISTALLOCATOR_ADDR;
void* (__stdcall*FastMem_ListAllocator)(void* CurrentListPos, void* NextListPos, InputMapEntry* inInputMap) = (void* (__stdcall*)(void*, void*, InputMapEntry*))FASTMEM_LISTALLOCATOR_ADDR;
void* (__thiscall*STL_List_Add)(void* thethis, int num) = (void* (__thiscall*)(void*, int))STL_LIST_ADD_ADDR;

void* __stdcall InputMapping_Constructor(InputDevice* device, void* AttribCollection)
{
	unsigned int thethis = 0;
	unsigned int list_current = 0;
	unsigned int list_next = 0;
	int alloc_result = 0;

	_asm mov thethis, ecx

	// use game's own memory management to avoid trouble
	* (void**)(thethis + 4) = (void*)FastMem_InitListAllocator();
	list_current = (unsigned int)*(void**)(thethis + 4);

	for (unsigned int i = 0; i < MAX_ACTIONID; i++)
	{
		if (VKeyBindings[i] != 0 || XInputBindings[i] != 0) // is it mapped
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
			map.PreviousValue = -1.0f;
			map.CurrentValue = -1.0f;
			list_next = *(int*)(list_current + 4);
			alloc_result = (int)FastMem_ListAllocator((void*)list_current, (void*)list_next, &map);
			//STL_List_Add((void*)thethis, 1);
			*(int*)(list_current + 4) = alloc_result;
			**(int**)(alloc_result + 4) = alloc_result;
		}
	}


	return (void*)thethis;
}
#else

#ifndef GAME_WORLD
void* __stdcall FastMem_ListAllocator(void* CurrentListPos, void* NextListPos, InputMapEntry* inInputMap)
{
	void* result = FastMem_Alloc((void*)GLOBAL_FASTMEM_ADDR, INIT_LIST_ALLOC_SIZE, NULL);
	if (result)
	{
		*(void**)result = CurrentListPos;
		*(void**)((int)result + 4) = NextListPos;
		memcpy((void*)((int)result + 8), inInputMap, sizeof(InputMapEntry));
	}
	return result;
}
#else
void* (*Managed_Alloc)(int size) = (void* (*)(int))MANAGED_ALLOCATOR_ADDR;
void* __stdcall FastMem_ListAllocator(void* CurrentListPos, void* NextListPos, InputMapEntry* inInputMap)
{
	void* result = Managed_Alloc(INIT_LIST_ALLOC_SIZE);
	if (result)
	{
		*(void**)result = CurrentListPos;
		*(void**)((int)result + 4) = NextListPos;
		memcpy((void*)((int)result + 8), inInputMap, sizeof(InputMapEntry));
	}
	return result;
}
#endif
void* __stdcall InputMapping_Constructor(InputDevice* device, void* AttribCollection)
{
	volatile unsigned int thethis = 0;
	volatile unsigned int list_current = 0;
	volatile unsigned int list_next = 0;
	volatile int alloc_result = 0;

	_asm mov thethis, ecx

	// use game's own memory management to avoid trouble
	* (void**)(thethis) = (void*)thethis;
	*(void**)(thethis + 4) = (void*)thethis;
	*(int*)(thethis + 8) = 0;

	for (unsigned int i = 0; i < MAX_ACTIONID; i++)
	{
		if (VKeyBindings[i] != 0 || XInputBindings[i] != 0) // is it mapped
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
void HandleSelectCarCamera(void* SelectCarObj, SHORT XAxis, SHORT YAxis)
{
	if (XAxis || YAxis || bMousePressedDown)
	{
		if (bMousePressedDown)
		{
			float HSpeed = *(int*)FEMOUSECURSOR_CARORBIT_X_ADDR;
			float VSpeed = *(int*)FEMOUSECURSOR_CARORBIT_Y_ADDR;

			if (HSpeed > 25.0)
				HSpeed = 25.0;
			if (HSpeed < -25.0)
				HSpeed = -25.0;
			if (VSpeed > 25.0)
				VSpeed = 25.0;
			if (VSpeed < -25.0)
				VSpeed = -25.0;

			if (HSpeed)
				SelectCarCameraMover_SetHRotateSpeed(SelectCarObj, HSpeed, true);
			else
				SelectCarCameraMover_SetHRotateSpeed(SelectCarObj, 0, false);
			if (VSpeed)
				SelectCarCameraMover_SetVRotateSpeed(SelectCarObj, VSpeed, true);
			else
				SelectCarCameraMover_SetVRotateSpeed(SelectCarObj, 0, false);
		}

		if (XAxis)
		{
			float HSpeed = -XAxis / float(0x7FFF);
			SelectCarCameraMover_SetHRotateSpeed(SelectCarObj, HSpeed, true);
		}
		if (YAxis)
		{
			float VSpeed = YAxis / float(0x7FFF);
			SelectCarCameraMover_SetVRotateSpeed(SelectCarObj, VSpeed, true);
		}
	}
	else
	{
		SelectCarCameraMover_SetHRotateSpeed(SelectCarObj, 0, false);
		SelectCarCameraMover_SetVRotateSpeed(SelectCarObj, 0, false);
	}

	if (g_Controllers[0].state.Gamepad.bLeftTrigger || (MouseWheelValue < 0))
	{
		if (MouseWheelValue) // TODO: maybe enhance it with a proper accel delta???
			SelectCarCameraMover_SetZoomSpeed(SelectCarObj, 1.0, true);
		if (g_Controllers[0].state.Gamepad.bLeftTrigger)
		{
			float Speed = g_Controllers[0].state.Gamepad.bLeftTrigger / float(0xFF);
			SelectCarCameraMover_SetZoomSpeed(SelectCarObj, Speed, true);
		}
	}

	else if (g_Controllers[0].state.Gamepad.bRightTrigger || (MouseWheelValue > 0))
	{
		if (MouseWheelValue)
			SelectCarCameraMover_SetZoomSpeed(SelectCarObj, -1.0, true);
		if (g_Controllers[0].state.Gamepad.bRightTrigger)
		{
			float Speed = g_Controllers[0].state.Gamepad.bRightTrigger / float(0xFF);
			SelectCarCameraMover_SetZoomSpeed(SelectCarObj, -Speed, true);
		}
	}
	else
		SelectCarCameraMover_SetZoomSpeed(SelectCarObj, 0, false);
}

// fix for Carbon & Pro Street Photo Mode
void __stdcall FEPhotoModeStateManager_HandleScreenTick_Hook()
{
	unsigned int thethis = 0;
	_asm mov thethis, ecx
	void* SelectCarObj = (void*)*(int*)(thethis + SELECTCAROBJ_OFFSET);
#ifdef GAME_PROSTREET
	HandleSelectCarCamera(SelectCarObj, g_Controllers[0].state.Gamepad.sThumbLX, g_Controllers[0].state.Gamepad.sThumbLY);
#else
	HandleSelectCarCamera(SelectCarObj, g_Controllers[0].state.Gamepad.sThumbRX, g_Controllers[0].state.Gamepad.sThumbRY);
#endif
}

// fix for ALL zooming (ingame and FE)
void __stdcall FEPhotoModeStateManager_HandleLTrigger_Hook()
{
	return;
}

void __stdcall FEPhotoModeStateManager_HandleRTrigger_Hook()
{
	return;
}
#ifdef GAME_CARBON
int __stdcall ftol2_to_int_bool()
{
	unsigned int obj = 0;
	_asm mov obj, ecx

	if (obj)
	{
		float num = *(float*)(obj + 8);
		if (num)
			return 1;
	}
	return 0;
}

#endif

#endif // GAME_UC
#pragma runtime_checks( "", restore )
#endif

#ifndef NO_FENG
unsigned int GameWndProcAddr = 0;
LRESULT(WINAPI* GameWndProc)(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT WINAPI CustomWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_LBUTTONDOWN:
		if (bUseDynamicFEngSwitching)
			LastControlledDevice = LASTCONTROLLED_KB;
		bMousePressedDown = true;
		return 0;
	case WM_LBUTTONUP:
		bMousePressedDown = false;
		return 0;
	case WM_RBUTTONDOWN:
		if (bUseDynamicFEngSwitching)
			LastControlledDevice = LASTCONTROLLED_KB;
		bMouse3PressedDown = true;
		return 0;
	case WM_RBUTTONUP:
		bMouse3PressedDown = false;
		return 0;
	case WM_MBUTTONDOWN:
		if (bUseDynamicFEngSwitching)
			LastControlledDevice = LASTCONTROLLED_KB;
		bMouse2PressedDown = true;
		return 0;
	case WM_MBUTTONUP:
		bMouse2PressedDown = false;
		return 0;
	case WM_MOUSEWHEEL:
		if (bUseDynamicFEngSwitching)
			LastControlledDevice = LASTCONTROLLED_KB;
		MouseWheelValue = (GET_WHEEL_DELTA_WPARAM(wParam));
		return 0;
	case WM_SETFOCUS:
		// confine mouse within the game window
		if (bConfineMouse)
		{
			GetWindowRect(*(HWND*)GAME_HWND_ADDR, &windowRect);
			ClipCursor(&windowRect);
		}
		break;
	case WM_KEYDOWN:
		if (bUseDynamicFEngSwitching)
			LastControlledDevice = LASTCONTROLLED_KB;
		break;
	case WM_MOUSEMOVE:
		if (!bUseWin32Cursor)
			bLastUsedVirtualMouse = false;
		return 0;
	default: 
		break;
	}

	return GameWndProc(hWnd, msg, wParam, lParam);
}

ATOM WINAPI RegisterClassHook(WNDCLASSEXA* wcex)
{
	(*wcex).hCursor = NULL; // this is necessary to be able to hide the mouse quickly enough
	return RegisterClassExA(wcex);
}


void DummyFunc()
{
	return;
}

void __stdcall DummyFuncStd(BOOL dummy)
{
	return;
}
#endif // GAME_UC

void InitConfig()
{
	CIniReader inireader("");

	KeyboardReadingMode = inireader.ReadInteger("Input", "KeyboardReadingMode", 0);
#ifndef NO_FENG
	bConfineMouse = inireader.ReadInteger("Input", "ConfineMouse", 0);
	bUseWin32Cursor = inireader.ReadInteger("Input", "UseWin32Cursor", 1);
	bUseCustomCursor = inireader.ReadInteger("Input", "UseCustomCursor", 1);
	bMouseLook = inireader.ReadInteger("Input", "MouseLook", 1);
	MouseLookSensitivity = inireader.ReadFloat("Input", "MouseLookSensitivity", 1.0f);
#endif
	INPUT_DEADZONE_LS = (inireader.ReadFloat("Input", "DeadzonePercentLS", 0.24f) * FLOAT(0x7FFF));
	INPUT_DEADZONE_RS = (inireader.ReadFloat("Input", "DeadzonePercentRS", 0.24f) * FLOAT(0x7FFF));
	INPUT_DEADZONE_LS_P2 = (inireader.ReadFloat("Input", "DeadzonePercentLS_P2", 0.24f) * FLOAT(0x7FFF));
	INPUT_DEADZONE_RS_P2 = (inireader.ReadFloat("Input", "DeadzonePercentRS_P2", 0.24f) * FLOAT(0x7FFF));
	SHIFT_ANALOG_THRESHOLD = (inireader.ReadFloat("Input", "DeadzonePercent_Shifting", 0.75f) * FLOAT(0x7FFF));
	FEUPDOWN_ANALOG_THRESHOLD = (inireader.ReadFloat("Input", "DeadzonePercent_AnalogStickDigital", 0.50f) * FLOAT(0x7FFF));
	TRIGGER_ACTIVATION_THRESHOLD = (inireader.ReadFloat("Input", "DeadzonePercent_AnalogTriggerDigital", 0.12f) * FLOAT(0xFF));
#ifndef NO_FENG
	ControllerIconMode = inireader.ReadInteger("Icons", "ControllerIconMode", 0);
	LastControlledDevice = inireader.ReadInteger("Icons", "FirstControlDevice", 0);
	bUseDynamicFEngSwitching = inireader.ReadInteger("Icons", "UseDynamicFEngSwitching", 1);
	strcpy(ButtonTexFilename, inireader.ReadString("Icons", "ButtonTexFilename", "GLOBAL\\XtendedInputButtons.tpk"));
#endif
}

int Init()
{
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
	GameWndProc = (LRESULT(WINAPI*)(HWND, UINT, WPARAM, LPARAM))GameWndProcAddr;
	injector::WriteMemory<unsigned int>(WNDPROC_POINTER_ADDR, (unsigned int)&CustomWndProc, true);


	// Mouse stuff
	injector::MakeJMP(FEMOUSE_UPDATE_CALL_ADDR, UpdateFECursorPos, true);

	// kill mouse wheel updating from InputDevice vars
	injector::MakeNOP(FEMOUSE_WHEEL_UPDATE_ADDR, 5, true);

	// window class register hook to change the default mouse cursor
	injector::MakeNOP(REGISTERCLASSHOOK_CALL_ADDR, 6, true);
	injector::MakeCALL(REGISTERCLASSHOOK_CALL_ADDR, RegisterClassHook, true);

	if (bUseWin32Cursor)
	{
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
	}
	else
		SetCursor(NULL);

	// kill garage camera control with FE pad buttons (leave only the stick)
	injector::MakeJMP(FENG_SHOWCASECAM_JMP_FROM, FENG_SHOWCASECAM_JMP_TO, true);

	// FEngHud -- joy handler fix
	injector::MakeJMP(FENGHUD_JOYHANDLER_JMP_FROM, FENGHUD_JOYHANDLER_JMP_TO, true);

	// FE VISUAL STUFF

	// custom Win32 mouse cursor
	if (bUseWin32Cursor)
	{
		if (bUseCustomCursor)
			NFSCursor = LoadCursorFromFile("nfs_cursor.cur");
		else
			NFSCursor = LoadCursor(NULL, IDC_ARROW);
	}

	// FORCE CONSOLE OBJECTS TO BE RENDERABLE ON PC
	//injector::MakeJMP(FENG_HIDEPCOBJ_JMP_FROM, FENG_HIDEPCOBJ_JMP_TO, true); // FEngHidePCObjects
	injector::WriteMemory<unsigned int>(FENG_HIDEPCOBJ_VT_ADDR, (unsigned int)&FEObjectCallback_Function, true);

#ifdef GAME_MW

	// dereference the current function after initing to maximize compatibility
	MouseStateArrayOffsetUpdater_Address = *(unsigned int*)FE_MOUSEUPDATER_CALLBACK_VT_ADDR;
	MouseStateArrayOffsetUpdater = (bool(__thiscall*)(void*, void*))MouseStateArrayOffsetUpdater_Address;
	// it looks decieving, it's not related to mouse, it's for accessing all FEObjects during FE updating
	injector::WriteMemory<unsigned int>(FE_MOUSEUPDATER_CALLBACK_VT_ADDR, (unsigned int)&MouseStateArrayOffsetUpdater_Callback_Hook, true);

	if (bUseDynamicFEngSwitching)
	{
		injector::MakeNOP(CFENG_RENDEROBJ_NOP_ADDR, CFENG_RENDEOBJ_NOP_AMOUNT, true); // cFEng render object
		injector::MakeJMP(FENG_SETVISIBLE_ADDR, FEngSetVisible, true);
		injector::MakeNOP(0x0052AEC3, 2, true);
		injector::MakeNOP(0x0052F33C, 2, true);
		injector::MakeNOP(0x0052F3FA, 6, true);
		injector::MakeNOP(0x00562C60, 2, true);
	}
	else
	{
		// make MW PC behave exactly like console version here... hide all PC elements instead
		if (LastControlledDevice == LASTCONTROLLED_CONTROLLER)
		{
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
	injector::MakeCALL(PRESS_START_HOOK_ADDR, FEngSetLanguageHash_Hook, true);

	if (bUseDynamicFEngSwitching)
	{
		injector::MakeNOP(CFENG_RENDEROBJ_NOP_ADDR, CFENG_RENDEOBJ_NOP_AMOUNT, true); // cFEng render object
		injector::MakeCALL(CFENG_SERVICE_CALL_ADDR, cFEng_Service_Hook, true);
		injector::MakeCALL(FENGINE_PROCESSPADSFORPACKAGE_CALL_ADDR, FEngine_ProcessPadsForPackage_Hook, true);
#ifdef GAME_PROSTREET
		injector::MakeNOP(CFENG_RENDEROBJ_NOP2_ADDR, CFENG_RENDEOBJ_NOP_AMOUNT, true); // cFEng render object
#endif
	}
	else
	{
		if (LastControlledDevice == LASTCONTROLLED_CONTROLLER)
		{
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

	if (bUseDynamicFEngSwitching)
	{
		injector::MakeCALL(0x005C670E, WorldMap_UnfocusQuickList_Hook, true);
		injector::MakeCALL(0x005CE0B9, WorldMap_SetQuickListInFocus_Hook, true);
		injector::MakeCALL(0x005CE309, WorldMap_SetQuickListInFocus_Hook, true);

	}
	else
	{
		if (LastControlledDevice == LASTCONTROLLED_CONTROLLER)
		{
			injector::WriteMemory<unsigned int>(0x005C3910, WORLDMAP_BUTTONGROUP_CONSOLE, true);
			injector::WriteMemory<unsigned int>(0x005CDD67, WORLDMAP_BUTTONGROUP_CONSOLE, true);
			injector::WriteMemory<unsigned int>(0x005B901D, WORLDMAP_BUTTONGROUP_CONSOLE, true);
			injector::WriteMemory<unsigned int>(0x005AF3E8, WORLDMAP_BUTTONGROUP_CONSOLE, true);
		}
	}

#endif // GAME_CARBON
#ifdef GAME_PROSTREET
	// fix the "view details" string update in the FEHubMap screen
	injector::MakeCALL(0x007ECD78, FE_SetLanguageHash_Hook, true);
	injector::MakeCALL(0x007ECD59, FE_SetLanguageHash_Hook, true);
	injector::MakeCALL(0x007ECD0D, FE_SetLanguageHash_Hook, true);
#endif


	//AttachConsole(ATTACH_PARENT_PROCESS);
	//AllocConsole();
	//freopen("CON", "w", stdout);
	//freopen("CON", "w", stderr);
	

#endif

	SetUnbindableButtonTextures();
#endif // GAME_UC

#ifdef GAME_WORLD
	// RECALCULATE FUNCTION POINTERS -- they shift around
	UTL_Com_Object_IList_Constructor = (void* (__thiscall*)(void*, unsigned int))UTL_ILIST_CONSTRUCTOR_ADDR;
	//InitProfileSettings = (void(*)())(0x756960 + MainBase);
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
	if (bMouseLook)
	{
		// Mouselook hook
		// dereference the current function after initing to maximize compatibility
		DebugWorldCameraMover_Update_Addr = *(unsigned int*)DEBUGWORLDCAMERAMOVER_UPDATE_VT_ADDR;
		DebugWorldCameraMover_Update = (void(__thiscall*)(void*, float))DebugWorldCameraMover_Update_Addr;
		injector::WriteMemory<unsigned int>(DEBUGWORLDCAMERAMOVER_UPDATE_VT_ADDR, (unsigned int)&DebugWorldCameraMover_Update_Hook, true);

		injector::MakeJMP(DEBUGWORLDCAMERAMOVER_CAVE1_ENTRY, DebugWorldCameraMover_Cave1, true);
		injector::MakeJMP(DEBUGWORLDCAMERAMOVER_CAVE2_ENTRY, DebugWorldCameraMover_Cave2, true);
#ifdef GAME_WORLD
		DebugWorldCameraMover_Cave1_Exit = DEBUGWORLDCAMERAMOVER_CAVE1_EXIT;
		DebugWorldCameraMover_Cave1_FullExit = DEBUGWORLDCAMERAMOVER_CAVE1_FULLEXIT;
		DebugWorldCameraMover_Cave2_Exit = DEBUGWORLDCAMERAMOVER_CAVE2_EXIT;
		DebugWorldCameraMover_Constructor = (void* (__thiscall*)(void*, void*, void*, int, int))DEBUGWORLDCAMERAMOVER_CONSTRUCTOR_ADDR;
		DebugWorldCameraMover_Destructor = (void* (__thiscall*)(void*))DEBUGWORLDCAMERAMOVER_DESTRUCTOR_ADDR;
#endif

		// implement DebugWorldCameraMover detection -- for cursor hiding
		injector::MakeCALL(DEBUGWORLDCAMERAMOVER_CONSTRUCTOR_HOOK_ADDR, DebugWorldCameraMover_Constructor_Hook, true);
		injector::MakeCALL(DEBUGWORLDCAMERAMOVER_DESTRUCTOR_HOOK_ADDR, DebugWorldCameraMover_Destructor_Hook, true);

	}
	// Init state
	ZeroMemory(g_Controllers, sizeof(CONTROLLER_STATE) * MAX_CONTROLLERS);

	return 0;
}


BOOL APIENTRY DllMain(HMODULE /*hModule*/, DWORD reason, LPVOID /*lpReserved*/)
{
	if (reason == DLL_PROCESS_ATTACH)
	{
#ifdef GAME_WORLD
		uintptr_t base = (uintptr_t)GetModuleHandleA(NULL);
		MainBase = base - 0x400000;
#endif

		Init();
	}
	return TRUE;
}

