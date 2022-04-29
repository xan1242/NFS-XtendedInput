// Need for Speed (Black Box, MW & newer) - Xtended Input plugin
// Bringing native XInput to NFS
// by Xan/Tenjoin

// TODO: bring rumble/vibration function
// TODO: remapping? -- partially done, but only 1 event per key
// TODO: port to other games
// TODO: kill DInput enough so that it doesn't detect XInput controllers but still detects wheels
// TODO: proper raw input for keyboard (and maybe non XInput gamepads?)
// TODO (MW): Max performance button is visible in all submenus during Customize for some reason...
// TODO: implement the Controls settings menu - it should be possible to make it talk to the INI
// TODO (MW): CUSTOMIZE MENU IS BUGGY - during career cash status overlaps the max performance buttons...
// TODO (Carbon): shows too many objects in WorldMapMain quick list -- add exceptions for this screen specifically
// TODO (Carbon): fix mouse wheel zooming (in FE only) and keyboard zooming during photo mode


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
#include "NFS_XtendedInput_FEng.h"

#ifdef GAME_MW
#include "NFSMW_XtendedInput.h"
#endif
#ifdef GAME_CARBON
#include "NFSC_XtendedInput.h"
float FEActivationFloat = 0.999999f;
#endif

#define MAX_CONTROLLERS 4  // XInput handles up to 4 controllers 

WORD INPUT_DEADZONE_LS = (0.24f * FLOAT(0x7FFF));  // Default to 24% of the +/- 32767 range.   This is a reasonable default value but can be altered if needed.
WORD INPUT_DEADZONE_RS = (0.24f * FLOAT(0x7FFF));  // Default to 24% of the +/- 32767 range.   This is a reasonable default value but can be altered if needed.
WORD INPUT_DEADZONE_LS_P2 = (0.24f * FLOAT(0x7FFF));  // Default to 24% of the +/- 32767 range.   This is a reasonable default value but can be altered if needed.
WORD INPUT_DEADZONE_RS_P2 = (0.24f * FLOAT(0x7FFF));  // Default to 24% of the +/- 32767 range.   This is a reasonable default value but can be altered if needed.
WORD SHIFT_ANALOG_THRESHOLD = (0.75f * FLOAT(0x7FFF));  // 75% for shifting
WORD FEUPDOWN_ANALOG_THRESHOLD = (0.50f * FLOAT(0x7FFF));  // 50% for analog sticks digital activation
WORD TRIGGER_ACTIVATION_THRESHOLD = (0.12f * FLOAT(0xFF));  // 12% for analog triggers digital activation

//#define TRIGGER_ACTIVATION_THRESHOLD 0x20
//#define FEUPDOWN_ANALOG_THRESHOLD 0x3FFF

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

void*(__thiscall* UTL_Com_Object_IList_Constructor)(void* thethis, unsigned int unk) = (void*(__thiscall*)(void*, unsigned int))UTL_ILIST_CONSTRUCTOR_ADDR;

int bStringHash(char* a1)
{
	char* v1; // edx@1
	char v2; // cl@1
	int result; // eax@1

	if (a1 == NULL)
		return 0;

	v1 = a1;
	v2 = *a1;
	for (result = -1; v2; ++v1)
	{
		result = v2 + 33 * result;
		v2 = v1[1];
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

		if ((g_Controllers[0].state.Gamepad.wButtons || g_Controllers[0].state.Gamepad.sThumbLX || g_Controllers[0].state.Gamepad.sThumbLY || g_Controllers[0].state.Gamepad.sThumbRX || g_Controllers[0].state.Gamepad.sThumbRY || g_Controllers[0].state.Gamepad.bRightTrigger || g_Controllers[0].state.Gamepad.bLeftTrigger) && bUseDynamicFEngSwitching)
		{
			LastControlledDevice = LASTCONTROLLED_CONTROLLER;
		}

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
	if (g_Controllers[0].bConnected)
	{
		WORD wButtons = g_Controllers[0].state.Gamepad.wButtons;

		if ((wButtons & XINPUT_GAMEPAD_BACK) != bQuitButtonOldState)
		{
			if ((wButtons & XINPUT_GAMEPAD_BACK)) // trigger once only on button down state
			{
#ifdef GAME_MW
				FESendKeystroke('Q');
#else
				FESendKeystroke((void*)FE_KEYSTROKE_OBJ, 'Q');
#endif
			}
			bQuitButtonOldState = (wButtons & XINPUT_GAMEPAD_BACK);
		}

		//if (LastControlledDevice == LASTCONTROLLED_CONTROLLER)
		//{
		//	*(int*)FEMOUSECURSOR_CARORBIT_X_ADDR = MousePos.x - *(int*)FEMOUSECURSOR_X_ADDR;
		//	*(int*)FEMOUSECURSOR_CARORBIT_Y_ADDR = MousePos.y - *(int*)FEMOUSECURSOR_Y_ADDR;
		//}

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
}

void __stdcall ReadControllerData()
{
	UpdateControllerState();
	ReadXInput_Extra();
	if (KeyboardReadingMode == KB_READINGMODE_BUFFERED)
		GetKeyboardState(VKeyStates[0]);
}

bool bCheckSecondBind()
{
	return FE_Secondary_Up || FE_Secondary_Down || FE_Secondary_Left || FE_Secondary_Right;
}

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
#ifndef GAME_MW
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

			if (bIsActionTextureBindable((ActionID)i))
				SetBindingButtonTexture((ActionID)i, XInputBindings[i]);
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

			if (bDoPolling)
			{
				float fresult = 0;
				rdi = fDeviceIndex;
				if (KeyboardReadingMode == KB_READINGMODE_BUFFERED)
					bCurrentVKeyState = VKeyStates[0][VKeyBindings[i]] >> 7;
				else
					bCurrentVKeyState = GetAsyncKeyState(VKeyBindings[i]) >> 15;

				if (bIsActionDebug((ActionID)i))
					rdi = 1; // debug (camera) actions are always read from the second port, but the PC version omits it entirely so this is a workaround

				//wButtons = g_Controllers[rdi].state.Gamepad.wButtons;

				if (bIsActionAnalog((ActionID)i))
				{
					if (bIsBindingAnalog(XInputBindings[i]))
					{
						float result = 0.0f;
						switch (XInputBindings[i])
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
						fresult = result;
					}
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
							WORD threshold = FEUPDOWN_ANALOG_THRESHOLD;

							switch (SecondBind)
							{
							case XINPUT_GAMEPAD_LT_CONFIGDEF:
								bCurrentXInputKeyState2 = (g_Controllers[rdi].state.Gamepad.bLeftTrigger > TRIGGER_ACTIVATION_THRESHOLD);
								break;
							case XINPUT_GAMEPAD_RT_CONFIGDEF:
								bCurrentXInputKeyState2 = (g_Controllers[rdi].state.Gamepad.bRightTrigger > TRIGGER_ACTIVATION_THRESHOLD);
								break;
							case XINPUT_GAMEPAD_LS_UP_CONFIGDEF:
								bCurrentXInputKeyState2 = (g_Controllers[rdi].state.Gamepad.sThumbLY > threshold);
								break;
							case XINPUT_GAMEPAD_LS_DOWN_CONFIGDEF:
								bCurrentXInputKeyState2 = (g_Controllers[rdi].state.Gamepad.sThumbLY < -threshold);
								break;
							case XINPUT_GAMEPAD_LS_LEFT_CONFIGDEF:
								bCurrentXInputKeyState2 = (g_Controllers[rdi].state.Gamepad.sThumbLX < -threshold);
								break;
							case XINPUT_GAMEPAD_LS_RIGHT_CONFIGDEF:
								bCurrentXInputKeyState2 = (g_Controllers[rdi].state.Gamepad.sThumbLX > threshold);
								break;
							case XINPUT_GAMEPAD_RS_UP_CONFIGDEF:
								bCurrentXInputKeyState2 = (g_Controllers[rdi].state.Gamepad.sThumbRY > threshold);
								break;
							case XINPUT_GAMEPAD_RS_DOWN_CONFIGDEF:
								bCurrentXInputKeyState2 = (g_Controllers[rdi].state.Gamepad.sThumbRY < -threshold);
								break;
							case XINPUT_GAMEPAD_RS_LEFT_CONFIGDEF:
								bCurrentXInputKeyState2 = (g_Controllers[rdi].state.Gamepad.sThumbRX < -threshold);
								break;
							case XINPUT_GAMEPAD_RS_RIGHT_CONFIGDEF:
								bCurrentXInputKeyState2 = (g_Controllers[rdi].state.Gamepad.sThumbRX > threshold);
								break;
							}
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
						WORD threshold = FEUPDOWN_ANALOG_THRESHOLD;
						if (i == GAMEACTION_SHIFTUP || i == GAMEACTION_SHIFTDOWN)
							threshold = SHIFT_ANALOG_THRESHOLD;

						switch (XInputBindings[i])
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

void* (__thiscall*FastMem_Alloc)(void* FastMem, int size, char* debug_str) = (void* (__thiscall*)(void*, int, char*))FASTMEM_ALLOC_ADDR;
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

int __declspec(naked) FastMem_InitListAllocator()
{
	_asm
	{
		push    0
		push    INIT_LIST_ALLOC_SIZE
		mov     ecx, GLOBAL_FASTMEM_ADDR
		call    FastMem_Alloc
		test    eax, eax
		jz      loc_635074
		mov		[eax], eax

	loc_635074:
		lea     ecx, [eax + 4]
		test    ecx, ecx
		jz      locret_63507D
		mov		[ecx], eax

	locret_63507D:
		ret
	}
}

//void* __stdcall FastMem_ListAllocator(void* CurrentListPos, void* NextListPos, InputMapEntry* inInputMap)
//{
//	void* result = 0;
//
//	_asm
//	{
//		push    0
//		push    INIT_LIST_ALLOC_SIZE
//		mov     ecx, GLOBAL_FASTMEM_ADDR
//		call    FastMem_Alloc
//		test    eax, eax
//		jz      locret_635281
//		mov     ecx, CurrentListPos
//		mov     edx, NextListPos
//		push    esi
//		mov     esi, inInputMap
//		push    edi
//		mov		[eax], ecx
//		lea     edi, [eax + 8]
//		mov     ecx, 8 // count of elements in InputMapEntry
//		mov		[eax + 4], edx
//		rep movsd
//
//	locret_635281:
//		mov result, eax
//	}
//
//	return result;
//}

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

void* __stdcall InputMapping_Constructor(InputDevice* device, void* AttribCollection)
{
	volatile unsigned int thethis = 0;
	volatile unsigned int list_current = 0;
	volatile unsigned int list_next = 0;
	volatile int alloc_result = 0;

	_asm mov thethis, ecx

	//printf("InputMapping: 0x%X\n", thethis);

	// use game's own memory management to avoid trouble
	*(void**)(thethis) = (void*)thethis;
	*(void**)(thethis+4) = (void*)thethis;
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
			map.ComboDeviceScalarIndex = i;
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

#ifdef GAME_CARBON
// fix for Carbon Photo Mode (ingame)
void __stdcall FEPhotoModeStateManager_HandleScreenTick_Hook()
{
	unsigned int thethis = 0;
	_asm mov thethis, ecx

	if (g_Controllers[0].state.Gamepad.sThumbRX || g_Controllers[0].state.Gamepad.sThumbRY || bMousePressedDown)
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
				SelectCarCameraMover_SetHRotateSpeed((void*)*(int*)(thethis + 0x28), HSpeed, true);
			else
				SelectCarCameraMover_SetHRotateSpeed((void*)*(int*)(thethis + 0x28), 0, false);
			if (VSpeed)
				SelectCarCameraMover_SetVRotateSpeed((void*)*(int*)(thethis + 0x28), VSpeed, true);
			else
				SelectCarCameraMover_SetVRotateSpeed((void*)*(int*)(thethis + 0x28), 0, false);
		}

		if (g_Controllers[0].state.Gamepad.sThumbRX)
		{
			float HSpeed = -g_Controllers[0].state.Gamepad.sThumbRX / float(0x7FFF);
			SelectCarCameraMover_SetHRotateSpeed((void*)*(int*)(thethis + 0x28), HSpeed, true);
		}
		if (g_Controllers[0].state.Gamepad.sThumbRY)
		{
			float VSpeed = g_Controllers[0].state.Gamepad.sThumbRY / float(0x7FFF);
			SelectCarCameraMover_SetVRotateSpeed((void*)*(int*)(thethis + 0x28), VSpeed, true);
		}
	}
	else
	{
		SelectCarCameraMover_SetHRotateSpeed((void*)*(int*)(thethis + 0x28), 0, false);
		SelectCarCameraMover_SetVRotateSpeed((void*)*(int*)(thethis + 0x28), 0, false);
	}

	if (g_Controllers[0].state.Gamepad.bLeftTrigger || (MouseWheelValue < 0))
	{
		if (MouseWheelValue) // TODO: maybe enhance it with a proper accel delta???
			SelectCarCameraMover_SetZoomSpeed((void*)*(int*)(thethis + 0x28), 1.0, true);
		if (g_Controllers[0].state.Gamepad.bLeftTrigger)
		{
			float Speed = g_Controllers[0].state.Gamepad.bLeftTrigger / float(0xFF);
			SelectCarCameraMover_SetZoomSpeed((void*)*(int*)(thethis + 0x28), Speed, true);
		}
	}

	else if (g_Controllers[0].state.Gamepad.bRightTrigger || (MouseWheelValue > 0))
	{
		if (MouseWheelValue)
			SelectCarCameraMover_SetZoomSpeed((void*)*(int*)(thethis + 0x28), -1.0, true);
		if (g_Controllers[0].state.Gamepad.bRightTrigger)
		{
			float Speed = g_Controllers[0].state.Gamepad.bRightTrigger / float(0xFF);
			SelectCarCameraMover_SetZoomSpeed((void*)*(int*)(thethis + 0x28), -Speed, true);
		}
	}
	else
		SelectCarCameraMover_SetZoomSpeed((void*)*(int*)(thethis + 0x28), 0, false);
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

#endif
#pragma runtime_checks( "", restore )

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

void InitConfig()
{
	CIniReader inireader("");

	KeyboardReadingMode = inireader.ReadInteger("Input", "KeyboardReadingMode", 0);
	bConfineMouse = inireader.ReadInteger("Input", "ConfineMouse", 0);
	bUseWin32Cursor = inireader.ReadInteger("Input", "UseWin32Cursor", 1);
	bUseCustomCursor = inireader.ReadInteger("Input", "UseCustomCursor", 1);
	INPUT_DEADZONE_LS = (inireader.ReadFloat("Input", "DeadzonePercentLS", 0.24f) * FLOAT(0x7FFF));
	INPUT_DEADZONE_RS = (inireader.ReadFloat("Input", "DeadzonePercentRS", 0.24f) * FLOAT(0x7FFF));
	INPUT_DEADZONE_LS_P2 = (inireader.ReadFloat("Input", "DeadzonePercentLS_P2", 0.24f) * FLOAT(0x7FFF));
	INPUT_DEADZONE_RS_P2 = (inireader.ReadFloat("Input", "DeadzonePercentRS_P2", 0.24f) * FLOAT(0x7FFF));
	SHIFT_ANALOG_THRESHOLD = (inireader.ReadFloat("Input", "DeadzonePercent_Shifting", 0.75f) * FLOAT(0x7FFF));
	FEUPDOWN_ANALOG_THRESHOLD = (inireader.ReadFloat("Input", "DeadzonePercent_AnalogStickDigital", 0.50f) * FLOAT(0x7FFF));
	TRIGGER_ACTIVATION_THRESHOLD = (inireader.ReadFloat("Input", "DeadzonePercent_AnalogTriggerDigital", 0.12f) * FLOAT(0x7FFF));

	ControllerIconMode = inireader.ReadInteger("Icons", "ControllerIconMode", 0);
	LastControlledDevice = inireader.ReadInteger("Icons", "FirstControlDevice", 0);
	bUseDynamicFEngSwitching = inireader.ReadInteger("Icons", "UseDynamicFEngSwitching", 1);
}

int Init()
{
	InitConfig();


	// dereference the current WndProc from the game executable and write to the function pointer (to maximize compatibility)
	GameWndProcAddr = *(unsigned int*)WNDPROC_POINTER_ADDR;
	GameWndProc = (LRESULT(WINAPI*)(HWND, UINT, WPARAM, LPARAM))GameWndProcAddr;
	injector::WriteMemory<unsigned int>(WNDPROC_POINTER_ADDR, (unsigned int)&CustomWndProc, true);

	injector::WriteMemory<unsigned int>(INPUTDEVICE_FACTORY_INITIALIZER_ADDR, (unsigned int)&InputDeviceFactory, true);
	injector::MakeCALL(INPUTMAPPING_CONSTRUCTOR_CALL_ADDR, InputMapping_Constructor, true);
	// kill DInput initialization
	injector::MakeNOP(DINPUT_KILL_ADDR, 5, true);

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
		injector::MakeNOP(0x00711EE2, 6, true);
		injector::MakeCALL(0x00711EE2, DummyFuncStd, true);
		injector::MakeNOP(0x00711EF3, 6, true);
		injector::MakeCALL(0x00711EF3, DummyFuncStd, true);
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
		injector::MakeNOP(CFENG_RENDEROBJ_NOP_ADDR, 6, true); // cFEng render object
		injector::MakeJMP(FENG_SETVISIBLE_ADDR, FEngSetVisible, true);
		injector::MakeNOP(0x0052AEC3, 2, true);
		injector::MakeNOP(0x0052F33C, 2, true);
		injector::MakeNOP(0x0052F3FA, 3, true);
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

	// Press START button initial hook... for the widescreen splash
	injector::MakeCALL(0x005A3147, FEngSetLanguageHash_Hook, true);

#else
	
	if (bUseDynamicFEngSwitching)
	{
		injector::MakeNOP(CFENG_RENDEROBJ_NOP_ADDR, 2, true); // cFEng render object
		injector::MakeCALL(CFENG_SERVICE_CALL_ADDR, cFEng_Service_Hook, true);
		injector::MakeCALL(FENGINE_PROCESSPADSFORPACKAGE_CALL_ADDR, FEngine_ProcessPadsForPackage_Hook, true);
	}
	else
	{
		if (LastControlledDevice == LASTCONTROLLED_CONTROLLER)
			injector::WriteMemory<unsigned char>(CFENG_RENDEROBJ_FLAG_ADDR, FE_CONTROL_FLAG_PC, true);
	}
	// force analog zooming in FE orbit camera
	injector::MakeJMP(0x0084FBDA, 0x0084FBE2, true);
	// Lower hardcoded deadzone to 0.000001 - VERY IMPORTANT
	injector::WriteMemory<unsigned int>(0x696071, 0x9C1760, true);
	// remove deadzone for FE activations...
	//injector::MakeCALL(0x0059FF72, ftol2_to_int_bool, true); // CAUSES CAR FLICKERING IN FE!!! ftol2 is a very fast function...
	injector::WriteMemory<int>(0x59FF6E, (int)&FEActivationFloat, true);


#ifdef GAME_CARBON
	injector::MakeJMP(0x0057BB69, FEPhotoModeStateManager_HandleScreenTick_Hook, true);
	injector::WriteMemory<unsigned int>(0x009D306C, (unsigned int)&FEPhotoModeStateManager_HandleLTrigger_Hook, true);
	injector::WriteMemory<unsigned int>(0x009D308C, (unsigned int)&FEPhotoModeStateManager_HandleRTrigger_Hook, true);
#endif

	//freopen("CON", "w", stdout);
	//freopen("CON", "w", stderr);

#endif

	SetUnbindableButtonTextures();

	// Init state
	ZeroMemory(g_Controllers, sizeof(CONTROLLER_STATE) * MAX_CONTROLLERS);

	return 0;
}


BOOL APIENTRY DllMain(HMODULE /*hModule*/, DWORD reason, LPVOID /*lpReserved*/)
{
	if (reason == DLL_PROCESS_ATTACH)
	{
		Init();
	}
	return TRUE;
}

