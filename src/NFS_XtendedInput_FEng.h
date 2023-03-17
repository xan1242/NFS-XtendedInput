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
#include "NFS_XtendedInput.h"
#include "NFS_XtendedInputButtons.h"

#include "Definitions.h"

#include <windows.h>
#include <timeapi.h>
#pragma comment(lib,"winmm.lib")

// Win32 mouse API stuff
RECT windowRect;
bool bMousePressedDown = false;
bool bMousePressedDownOldState = false;

bool bMouse2PressedDown = false;
bool bMouse2PressedDownOldState = false;

bool bMouse3PressedDown = false;
bool bMouse3PressedDownOldState = false;

int MouseWheelValue = 0;

bool bConfineMouse = false;
int TimeSinceLastMouseMovement = 0;
bool bUseWin32Cursor = true;
bool bUseCustomCursor = true;
bool bLastUsedVirtualMouse = false;
bool bUseDynamicFEngSwitching = true;
HCURSOR NFSCursor;
#define MOUSEHIDE_TIME 5000

// FE texture button re-mapping
unsigned int ButtonTexHashes[FE_TEX_HASH_COUNT];
bool bLoadedConsoleButtonTex = false;
char ButtonTexFilename[128];

enum FEObjType
{
	FE_None = 0,
	FE_Image = 1,
	FE_String = 2,
	FE_Model = 3,
	FE_List = 4,
	FE_Group = 5,
	FE_CodeList = 6,
	FE_Movie = 7,
	FE_Effect = 8,
	FE_ColoredImage = 9,
	FE_AnimImage = 10,
	FE_SimpleImage = 11,
	FE_MultiImage = 12,
	FE_UserMin = 256,
};

enum FEObject_Flags
{
	FF_HideInEdit = -2147483648,
	FF_AffectAllScripts = 0x40000000,
	FF_PerspectiveProjection = 0x20000000,
	FF_IsButton = 0x10000000,
	FF_ObjectLocked = 0x8000000,
	FF_IgnoreButton = 0x4000000,
	FF_Dirty = 0x2000000,
	FF_DirtyTransform = 0x1000000,
	FF_DirtyColor = 0x800000,
	FF_DirtyCode = 0x400000,
	FF_CodeSuppliedResource = 0x200000,
	FF_UsesLibraryObject = 0x100000,
	FF_DontNavigate = 0x80000,
	FF_SaveStaticTracks = 0x40000,
	FF_MouseObject = 0x20000,
};

struct FEObject
{
	void* pDestructorCallback;
	long Padding[2];
	unsigned long GUID;
	unsigned long NameHash;
	char* pName;
	FEObjType Type;
	unsigned long Flags;
	unsigned short RenderContext;
	unsigned short ResourceIndex;
	unsigned long Handle;
	unsigned long UserParam;
	unsigned char* pData;
	unsigned long DataSize;
	int Responses;
	int Scripts;
	void* pCurrentScript;
	void* Cached;
};
#ifdef GAME_MW
#define FE_CONTROL_FLAG_PC 0x8
#else
#define FE_CONTROL_FLAG_PC 0x1008
#endif
#define FE_CONTROL_FLAG_CONSOLE 0x40
#define CFE_FLAG_WAS_VISIBLE_ORIGINALLY 0x01000000

#pragma runtime_checks( "", off )
int (__thiscall* _cFEng_FindPackage)(void* cFEng, char* pkg) = (int (__thiscall*)(void*, char*))CFENG_FINDPACKAGE_ADDR;
bool(__thiscall* _cFEng_IsPackageInControl)(void* cFEng, char* pkg) = (bool(__thiscall*)(void*, char*))CFENG_ISPACKAGEINCONTROL_ADDR;
FEObject* (*FEngFindObject)(char* pkg_name, unsigned int obj_hash) = (FEObject* (*)(char*, unsigned int))FENG_FINDOBJECT_ADDR;

#ifdef GAME_MW
void(*FEngSetLanguageHash)(void* FEObject, unsigned int langhash) = (void(*)(void*, unsigned int))FENG_SETLANGHASH_ADDR;
#endif
void* (*CreateResourceFile)(char* filename, int ResFileType, int unk1, int unk2, int unk3) = (void* (*)(char*, int, int, int, int))CREATERESOURCEFILE_ADDR;
void(__thiscall* ResourceFile_BeginLoading)(void* ResourceFile, void* callback, void* unk) = (void(__thiscall*)(void*, void*, void*))RESFILE_BEGINLOADING_ADDR;
void(*ServiceResourceLoading)() = (void(*)())SERVICERESOURCELOADING_ADDR;


void __stdcall LoadResourceFile(char* filename, int ResType, int unk1, void* unk2, void* unk3, int unk4, int unk5)
{
	ResourceFile_BeginLoading(CreateResourceFile(filename, ResType, unk1, unk4, unk5), unk2, unk3);
}

int __declspec(naked) cFEng_FindPackageWithControl()
{
	_asm
	{
		mov ecx, ds: [CFENG_INSTANCE_ADDR]
		mov     eax, [ecx]
		add     eax, 0xD8
		jz      loc_516BAE
		mov     eax, [eax + 0xC]
		test    eax, eax
		jz      loc_516BAE

	loc_516BA0 :
		mov     ecx, [eax + 0x20]
		test    ecx, ecx
		jnz     loc_516BB3
		mov     eax, [eax + 8]
		test    eax, eax
		jnz     loc_516BA0

	loc_516BAE :
		xor eax, eax
		ret

	loc_516BB3 :
		ret
	}
}


#ifdef GAME_MW
unsigned int MouseStateArrayOffsetUpdater_Address = 0;
bool(__thiscall* MouseStateArrayOffsetUpdater)(void* CB_Obj, void* FEObject) = (bool(__thiscall*)(void*, void*))FE_MOUSEUPDATER_CALLBACK_ADDR;

void FEngSetLanguageHash_Hook(char* pkg_name, int obj_hash, int lang_hash)
{
	if (LastControlledDevice == LASTCONTROLLED_CONTROLLER)
	{
		if (ControllerIconMode == CONTROLLERICON_PS4)
			FEPrintf(pkg_name, FEngFindObject(pkg_name, obj_hash), FE_SPLASH_TEXT_PS4);
		else
			FEPrintf(pkg_name, FEngFindObject(pkg_name, obj_hash), FE_SPLASH_TEXT_XBOX);
	}
	else
		FEPrintf(pkg_name, FEngFindObject(pkg_name, obj_hash), FE_SPLASH_TEXT_PC);
}
//void(*FEngSetVisible)(void* FEObject) = (void(*)(void*))FENG_SETVISIBLE_ADDR;
void(*FEngSetInvisible)(void* FEObject) = (void(*)(void*))FENG_SETINVISIBLE_ADDR;

void(__thiscall* CustomTuningScreen_NotificationMessage)(void* thethis, unsigned int unk1, void* FEObject, unsigned int unk2, unsigned int unk3) = (void(__thiscall*)(void*, unsigned int, void*, unsigned int, unsigned int))0x005A9910;

// custom FEngSetVisible - necessary to stop elements from appearing we don't want dynamically (flickering buttons)
void __declspec(naked) FEngSetVisible(FEObject* obj)
{
	_asm
	{
		mov     eax, [esp+4]
		test    eax, eax
		jz      locret_514D01
		mov		ecx, LastControlledDevice
		cmp		cl, LASTCONTROLLED_CONTROLLER
		mov     ecx, [eax + 0x1C]
		je		cont

		test	cl, FE_CONTROL_FLAG_CONSOLE
		jnz     locret_514D01
		jmp fengvisible_continue

	cont:
		test	cl, FE_CONTROL_FLAG_PC
		jnz     locret_514D01
		
	fengvisible_continue:
		and		ecx, 0x0FFFFFFFE
		or		ecx, 0x2400000
		mov		[eax + 0x1C], ecx
		cmp     dword ptr[eax + 0x18], 5
		jnz     locret_514D01
		push    esi
		mov     esi, [eax + 0x64]
		mov     eax, [eax + 0x60]
		test    eax, eax
		jle     loc_514D00
		push    edi
		mov     edi, eax

	loc_514CF0 :
		push    esi
		call    FEngSetVisible
		mov     esi, [esi + 4]
		add     esp, 4
		dec     edi
		jnz     loc_514CF0
		pop     edi

	loc_514D00 :
		pop     esi

	locret_514D01 :
		retn
	}
}
#else
void FEngSetLanguageHash_Hook(char* pkg_name, int obj_hash, int lang_hash)
{
	if (LastControlledDevice == LASTCONTROLLED_CONTROLLER)
	{
		if (ControllerIconMode == CONTROLLERICON_PS4)
			FE_String_Printf(FEngFindObject(pkg_name, obj_hash), FE_SPLASH_TEXT_PS4);
		else
			FE_String_Printf(FEngFindObject(pkg_name, obj_hash), FE_SPLASH_TEXT_XBOX);
	}
	else
		FE_String_Printf(FEngFindObject(pkg_name, obj_hash), FE_SPLASH_TEXT_PC);
}

void(*FE_Object_SetVisibility)(void* FEObject, bool visibility) = (void(*)(void*, bool))FENG_SETOBJECTVISIBILITY_ADDR;
void(__thiscall* FEPackage_ForAllObjects)(void* FEPackage, void* CallbackVT) = (void(__thiscall*)(void*, void*))FEPACKAGE_FORALLOBJ_ADDR;
void(__thiscall* cFEng_Service)(void* cFEng, int unk1, int unk2) = (void(__thiscall*)(void*, int unk1, int unk2))CFENG_SERVICE_ADDR;
char* (*GetLocalizedString)(unsigned int langhash) = (char* (*)(unsigned int))GETLOCALIZEDSTRING_ADDR;


void(__thiscall* SelectCarCameraMover_SetHRotateSpeed)(void* SelectCarCameraMover, float speed, bool activate) = (void(__thiscall*)(void*, float, bool))SELECTCAR_SETHROTATESPEED_ADDR;
void(__thiscall* SelectCarCameraMover_SetVRotateSpeed)(void* SelectCarCameraMover, float speed, bool activate) = (void(__thiscall*)(void*, float, bool))SELECTCAR_SETVROTATESPEED_ADDR;
void(__thiscall* SelectCarCameraMover_SetZoomSpeed)(void* SelectCarCameraMover, float speed, bool activate) = (void(__thiscall*)(void*, float, bool))SELECTCAR_SETZOOMSPEED_ADDR;
void(__thiscall* FeGarageMain_ZoomCameraView)(void* FEGarageMain, float speed, bool activate) = (void(__thiscall*)(void*, float, bool))FEGARAGEMAIN_ZOOMCAMERAVIEW_ADDR;
void(__thiscall* FEngine_ProcessPadsForPackage)(void* FEngine, void* FEPackage) = (void(__thiscall*)(void*, void*))FENGINE_PROCESSPADSFORPACKAGE_ADDR;
#ifdef GAME_CARBON
void*(__thiscall* FEPackage_FindObjectByHash)(void* FEPackage, unsigned int namehash) = (void*(__thiscall*)(void*, unsigned int))FEPACKAGE_FINDOBJBYHASH_ADDR;
void(__thiscall* FEWorldMapStateManager_HandleScreenTick)(void* FEWorldMapStateManager) = (void(__thiscall*)(void*))FEWORLDMAPSTATEMANAGER_TICK_ADDR;
void(__thiscall* WorldMap_UnfocusQuickList)(void* WorldMap) = (void(__thiscall*)(void*))WORLDMAP_UNFOCUSQUICKLIST_ADDR;
void(__thiscall* WorldMap_SetQuickListInFocus)(void* WorldMap) = (void(__thiscall*)(void*))WORLDMAP_SETQUICKLISTINFOCUS_ADDR;


#endif


void FEngSetVisible(void* FEObject)
{
	return FE_Object_SetVisibility(FEObject, true);
}
void FEngSetInvisible(void* FEObject)
{
	return FE_Object_SetVisibility(FEObject, false);
}


#endif


char* cFEng_FindPackageWithControl_Name()
{
	int pkg = cFEng_FindPackageWithControl();
	if (pkg)
		return *(char**)(pkg + 0xC);
	return NULL;
}

// fast version using hashes instead of string comparison
bool cFEng_IsPackageInControl_Fast(unsigned int pkg_name_hash)
{
	if (bStringHash(cFEng_FindPackageWithControl_Name()) == pkg_name_hash)
		return true;
	return false;
}

unsigned int __stdcall FEngSetTextureHash(unsigned int FEImage, int hash)
{
	int v2;

	if (FEImage)
	{
		if (*(int*)(FEImage + 36) != hash)
		{
			v2 = *(int*)(FEImage + 28);
			*(int*)(FEImage + 36) = hash;
			*(int*)(FEImage + 28) = v2 | 0x400000;
		}
	}
	return FEImage;
}

unsigned int __stdcall FEngGetTextureHash(unsigned int FEImage)
{
	int result = 0;

	if (FEImage)
		result = *(int*)(FEImage + 36);

	return result;
}

bool FEngSwapTextureHash(FEObject* inobj, unsigned int target_hash, unsigned int new_hash)
{
	if (inobj->Type == FE_Image)
	{
		int in_hash = FEngGetTextureHash((int)inobj);

		if ((in_hash) && (in_hash == target_hash))
		{
			FEngSetTextureHash((int)inobj, new_hash);
			return true;
		}
	}
	return false;
}

int __stdcall FEngFindImage(char* pkgname, int hash)
{
	int result;

	result = (int)FEngFindObject(pkgname, hash);
	if (!result || *(int*)(result + 24) != 1)
		result = 0;
	return result;
}

#ifndef GAME_PROSTREET
void(*FESendKeystroke)(unsigned int key) = (void(*)(unsigned int))FE_SENDKEY_ADDR;
#endif

// functions wrapped because the compiler loves to check the ESP during debugging which causes errors
int __stdcall cFEng_FindPackage(char* pkg)
{
	return _cFEng_FindPackage(*(void**)CFENG_INSTANCE_ADDR, pkg);
}

bool __stdcall cFEng_IsPackageInControl(char* pkg)
{
	return _cFEng_IsPackageInControl(*(void**)CFENG_INSTANCE_ADDR, pkg);
}

void FEngSetVisible_Name(char* pkg_name, unsigned int obj_hash)
{
	FEObject* obj = FEngFindObject(pkg_name, obj_hash);
	if (obj)
		FEngSetVisible(obj);
}

void FEngSetInvisible_Name(char* pkg_name, unsigned int obj_hash)
{
	void* obj = FEngFindObject(pkg_name, obj_hash);
	if (obj)
		FEngSetInvisible(obj);
}

int FEngSetTextureHash_Name(char* pkg_name, unsigned int obj_hash, unsigned int tex_hash)
{
	int obj = FEngFindImage(pkg_name, obj_hash);
	if (obj)
		FEngSetTextureHash(obj, tex_hash);
	return obj;
}

void FEngSetVisible_CheckObj(FEObject* inobj, unsigned int obj_hash)
{
	if (inobj->NameHash == obj_hash)
		FEngSetVisible(inobj);
}

void FEngSetInvisible_CheckObj(FEObject* inobj, unsigned int obj_hash)
{
	if (inobj->NameHash == obj_hash)
		FEngSetInvisible(inobj);
}

#ifdef GAME_MW
void FEngSetLanguageHash_CheckObj(FEObject* inobj, unsigned int obj_hash, unsigned int lang_hash)
{
	if (inobj->NameHash == obj_hash)
		FEngSetLanguageHash(inobj, lang_hash);
}
#endif

void* FEngSetTextureHash_CheckObj(FEObject* inobj, unsigned int obj_hash, unsigned int tex_hash)
{
	if (inobj->NameHash == obj_hash)
		FEngSetTextureHash((int)inobj, tex_hash);
	
	return inobj;
}

#pragma runtime_checks( "", restore )

void UpdateFECursorPos()
{
	if ((*(int*)GAMEFLOWMANAGER_STATUS_ADDR != 0) && bGlobalDoPolling) // don't execute until we're in the game... fixes BSOD crash on old ATI video drivers...
	{
		bool bMouseInGameWindow = false;
		bool bShowMouse = true;
		POINT MousePos;
		GetCursorPos(&MousePos);
		GetWindowRect(*(HWND*)GAME_HWND_ADDR, &windowRect);

		float ratio = 480.0 / (windowRect.bottom - windowRect.top); // scaling it to 480 height since that's what FE wants

		if ((MousePos.x >= windowRect.left) || (MousePos.x <= windowRect.right) && (MousePos.y >= windowRect.top) || (MousePos.y <= windowRect.bottom))
			bMouseInGameWindow = true;

		MousePos.x = MousePos.x - windowRect.left;
		MousePos.y = MousePos.y - windowRect.top;

		MousePos.x = (int)((float)(MousePos.x) * ratio);
		MousePos.y = (int)((float)(MousePos.y) * ratio);

#ifdef GAME_PROSTREET
		float window_aspect = (float)(windowRect.right - windowRect.left) / (float)(windowRect.bottom - windowRect.top);
		float shiftpos = ((window_aspect * 480.0f) - 640.0f) / 2.0f;
		MousePos.x = MousePos.x - shiftpos;
#endif

		// car orbiting position calculation - always relative to old
		* (int*)FEMOUSECURSOR_CARORBIT_X_ADDR = MousePos.x - *(int*)FEMOUSECURSOR_X_ADDR;
		*(int*)FEMOUSECURSOR_CARORBIT_Y_ADDR = MousePos.y - *(int*)FEMOUSECURSOR_Y_ADDR;

		// get time since last movement and hide it after a while (unless the cursor is within the game window so we don't hide it)
		if ((MousePos.x != *(int*)FEMOUSECURSOR_X_ADDR) || (MousePos.y != *(int*)FEMOUSECURSOR_Y_ADDR))
			TimeSinceLastMouseMovement = timeGetTime();
		else
		{
			if ((TimeSinceLastMouseMovement + MOUSEHIDE_TIME) < timeGetTime())
				bShowMouse = false;
		}
#ifdef GAME_MW
		if (*(int*)GAMEFLOWMANAGER_STATUS_ADDR == 6)
		{
			if (*(bool*)FEMOUSECURSOR_ISHIDDEN_ADDR && cFEng_IsPackageInControl_Fast(WORLDMAPMAIN_FNG_NAMEHASH))
				bShowMouse = false;
		}
#else
#ifdef GAME_CARBON
		if (*(bool*)FEMOUSECURSOR_ISHIDDEN_ADDR && cFEng_IsPackageInControl_Fast(WORLDMAPMAIN_FNG_NAMEHASH))
			bShowMouse = false;
#endif // GAME_CARBON
#endif // GAME_MW

		if (bInDebugWorldCamera)
			bShowMouse = false;

		if (bUseWin32Cursor)
		{
			if (bShowMouse)
				SetCursor(NFSCursor);
			else
				SetCursor(NULL);
		}
		else
		{
			SetCursor(NULL);
			if (bShowMouse)
				*(bool*)FEMOUSECURSOR_ISHIDDEN_ADDR = false;
			else
				*(bool*)FEMOUSECURSOR_ISHIDDEN_ADDR = true;
		}

		if (!bLastUsedVirtualMouse || bUseWin32Cursor)
		{
			*(int*)FEMOUSECURSOR_X_ADDR = MousePos.x;
			*(int*)FEMOUSECURSOR_Y_ADDR = MousePos.y;
		}

		// track mouse click state - make sure it lasts for exactly 1 tick of a loop, because the game is a little overzelaous with reading this input
		if (bMousePressedDown != bMousePressedDownOldState)
		{
			*(bool*)FEMOUSECURSOR_BUTTONPRESS_ADDR = bMousePressedDown;
			*(bool*)FEMOUSECURSOR_BUTTONPRESS2_ADDR = bMousePressedDown;
			*(bool*)FEMOUSECURSOR_BUTTONPRESS3_ADDR = !bMousePressedDown;
			bMousePressedDownOldState = bMousePressedDown;
		}
		else
		{
			//*(bool*)FEMOUSECURSOR_BUTTONPRESS_ADDR = false; // except this one, it's used for car orbiting
			*(bool*)FEMOUSECURSOR_BUTTONPRESS2_ADDR = false;
			*(bool*)FEMOUSECURSOR_BUTTONPRESS3_ADDR = false;
		}

		// track mouse click state - make sure it lasts for exactly 1 tick of a loop, because the game is a little overzelaous with reading this input
		if (bMouse2PressedDown != bMouse2PressedDownOldState)
		{
			*(bool*)FEMOUSECURSOR_BUTTON2PRESS_ADDR = bMouse2PressedDown;
			*(bool*)FEMOUSECURSOR_BUTTON2PRESS2_ADDR = bMouse2PressedDown;
			*(bool*)FEMOUSECURSOR_BUTTON2PRESS3_ADDR = !bMouse2PressedDown;
			bMouse2PressedDownOldState = bMouse2PressedDown;
		}
		else
		{
			*(bool*)FEMOUSECURSOR_BUTTON2PRESS2_ADDR = false;
			*(bool*)FEMOUSECURSOR_BUTTON2PRESS3_ADDR = false;
		}
		// track mouse click state - make sure it lasts for exactly 1 tick of a loop, because the game is a little overzelaous with reading this input
		if (bMouse3PressedDown != bMouse3PressedDownOldState)
		{
			*(bool*)FEMOUSECURSOR_BUTTON3PRESS_ADDR = bMouse3PressedDown;
			*(bool*)FEMOUSECURSOR_BUTTON3PRESS2_ADDR = bMouse3PressedDown;
			*(bool*)FEMOUSECURSOR_BUTTON3PRESS3_ADDR = !bMouse3PressedDown;
			bMouse3PressedDownOldState = bMouse3PressedDown;
		}
		else
		{
			*(bool*)FEMOUSECURSOR_BUTTON3PRESS2_ADDR = false;
			*(bool*)FEMOUSECURSOR_BUTTON3PRESS3_ADDR = false;
		}

		if (MouseWheelValue)
			*(int*)FEMOUSEWHEEL_ADDR = MouseWheelValue;
		MouseWheelValue = 0;
	}
}


bool bIsHudVisible();


#ifdef GAME_MW
#pragma runtime_checks( "", off )
void __stdcall CustomTuningScreen_NotificationMessage_Hook(unsigned int unk1, void* FEObject, unsigned int unk2, unsigned int unk3)
{
	unsigned int thethis = 0;
	_asm mov thethis, ecx

	// force visibility of the following:
	// 0x05CDDED4 -- SETTING NAME GROUP
	// 0x79354351 -- R1 ARROW
	// 0x6B364F8B -- L1 ARROW

	FEngSetVisible_Name(*(char**)(thethis + 0x10), 0x05CDDED4);
	FEngSetVisible_Name(*(char**)(thethis + 0x10), 0x79354351);
	FEngSetVisible_Name(*(char**)(thethis + 0x10), 0x6B364F8B);

	return CustomTuningScreen_NotificationMessage((void*)thethis, unk1, FEObject, unk2, unk3);
}
#pragma runtime_checks( "", restore )
#endif

void SetUnbindableButtonTextures()
{
	ButtonTexHashes[FE_LANALOG_TEX] = GetAppropriateButtonTex(XINPUT_GAMEPAD_LS_CONFIGDEF);
	ButtonTexHashes[FE_LANALOG_UP_TEX] = GetAppropriateButtonTex(XINPUT_GAMEPAD_LS_UP_CONFIGDEF);
	ButtonTexHashes[FE_LANALOG_UP_SWEDISH_TEX] = GetAppropriateButtonTex(XINPUT_GAMEPAD_LS_UP_CONFIGDEF);
	ButtonTexHashes[FE_LANALOG_UP_FRENCH_TEX] = GetAppropriateButtonTex(XINPUT_GAMEPAD_LS_UP_CONFIGDEF);
	ButtonTexHashes[FE_LANALOG_DOWN_TEX] = GetAppropriateButtonTex(XINPUT_GAMEPAD_LS_DOWN_CONFIGDEF);
	ButtonTexHashes[FE_LANALOG_DOWN_FRENCH_TEX] = GetAppropriateButtonTex(XINPUT_GAMEPAD_LS_DOWN_CONFIGDEF);
	ButtonTexHashes[FE_LANALOG_DOWN_SWEDISH_TEX] = GetAppropriateButtonTex(XINPUT_GAMEPAD_LS_DOWN_CONFIGDEF);
}

void SetBindingButtonTexture(ActionID id, WORD buttonmask)
{
	int navtype = 0;

	switch (id)
	{
	case FRONTENDACTION_UP:
		navtype = IsBindingNavigation(buttonmask);
		switch (navtype)
		{
		case 1:
			ButtonTexHashes[FE_DPAD_TEX] = GetAppropriateButtonTex(XINPUT_GAMEPAD_DPAD_CONFIGDEF);
			ButtonTexHashes[FE_DPAD_UPDOWN_TEX] = GetAppropriateButtonTex(XINPUT_GAMEPAD_DPAD_UPDOWN_CONFIGDEF);
			break;
		case 2:
			ButtonTexHashes[FE_DPAD_TEX] = GetAppropriateButtonTex(XINPUT_GAMEPAD_LS_CONFIGDEF);
			ButtonTexHashes[FE_DPAD_UPDOWN_TEX] = GetAppropriateButtonTex(XINPUT_GAMEPAD_LS_Y_CONFIGDEF);			
			break;
		case 3:
			ButtonTexHashes[FE_DPAD_TEX] = GetAppropriateButtonTex(XINPUT_GAMEPAD_RS_CONFIGDEF);
			ButtonTexHashes[FE_DPAD_UPDOWN_TEX] = GetAppropriateButtonTex(XINPUT_GAMEPAD_RS_Y_CONFIGDEF);			
			break;
		default:
			ButtonTexHashes[FE_DPAD_TEX] = GetAppropriateButtonTex(buttonmask);
			ButtonTexHashes[FE_DPAD_UPDOWN_TEX] = GetAppropriateButtonTex(buttonmask);
			break;
		}
		ButtonTexHashes[FE_DPAD_UP_TEX] = GetAppropriateButtonTex(buttonmask);
		break;
	case FRONTENDACTION_DOWN:
		ButtonTexHashes[FE_DPAD_DOWN_TEX] = GetAppropriateButtonTex(buttonmask);
		break;
	case FRONTENDACTION_LEFT:
		navtype = IsBindingNavigation(buttonmask);
		switch (navtype)
		{
		case 1:
			ButtonTexHashes[FE_DPAD_LEFTRIGHT_TEX] = GetAppropriateButtonTex(XINPUT_GAMEPAD_DPAD_LEFTRIGHT_CONFIGDEF);
			break;
		case 2:
			ButtonTexHashes[FE_DPAD_LEFTRIGHT_TEX] = GetAppropriateButtonTex(XINPUT_GAMEPAD_LS_X_CONFIGDEF);
			break;
		case 3:
			ButtonTexHashes[FE_DPAD_LEFTRIGHT_TEX] = GetAppropriateButtonTex(XINPUT_GAMEPAD_RS_X_CONFIGDEF);
			break;
		default:
			ButtonTexHashes[FE_DPAD_LEFTRIGHT_TEX] = GetAppropriateButtonTex(buttonmask);
			break;
		}
		ButtonTexHashes[FE_DPAD_LEFT_TEX] = GetAppropriateButtonTex(buttonmask);
		break;
	case FRONTENDACTION_RIGHT:
		ButtonTexHashes[FE_DPAD_RIGHT_TEX] = GetAppropriateButtonTex(buttonmask);
		break;
	case FRONTENDACTION_RUP:
		navtype = IsBindingNavigation(buttonmask);
		switch (navtype)
		{
		case 1:
			ButtonTexHashes[FE_RANALOG_TEX] = GetAppropriateButtonTex(XINPUT_GAMEPAD_DPAD_CONFIGDEF);
			break;
		case 2:
			ButtonTexHashes[FE_RANALOG_TEX] = GetAppropriateButtonTex(XINPUT_GAMEPAD_LS_CONFIGDEF);
			break;
		case 3:
			ButtonTexHashes[FE_RANALOG_TEX] = GetAppropriateButtonTex(XINPUT_GAMEPAD_RS_CONFIGDEF);
			break;
		default:
			ButtonTexHashes[FE_RANALOG_TEX] = GetAppropriateButtonTex(buttonmask);
			break;
		}
		break;
	case GAMEACTION_SHIFTUP:
		ButtonTexHashes[FE_RANALOG_UP_TEX] = GetAppropriateButtonTex(buttonmask);
		ButtonTexHashes[FE_RANALOG_UP_SWEDISH_TEX] = GetAppropriateButtonTex(buttonmask);
		ButtonTexHashes[FE_RANALOG_UP_FRENCH_TEX] = GetAppropriateButtonTex(buttonmask);
		break;
	case GAMEACTION_SHIFTDOWN:
		ButtonTexHashes[FE_RANALOG_DOWN_TEX] = GetAppropriateButtonTex(buttonmask);
		ButtonTexHashes[FE_RANALOG_DOWN_SWEDISH_TEX] = GetAppropriateButtonTex(buttonmask);
		ButtonTexHashes[FE_RANALOG_DOWN_FRENCH_TEX] = GetAppropriateButtonTex(buttonmask);
		break;
	case FRONTENDACTION_ACCEPT:
		ButtonTexHashes[FE_ACCEPT_TEX] = GetAppropriateButtonTex(buttonmask);
		break;
	case FRONTENDACTION_CANCEL:
		ButtonTexHashes[FE_CANCEL_TEX] = GetAppropriateButtonTex(buttonmask);
		break;
	case FRONTENDACTION_BUTTON0:
		ButtonTexHashes[FE_SELECT_TEX] = GetAppropriateButtonTex(buttonmask);
#ifdef GAME_PROSTREET
		ButtonTexHashes[FE_BUTTON1_TEX] = GetAppropriateButtonTex(buttonmask);
#endif
		break;
	case FRONTENDACTION_BUTTON1:
#ifndef GAME_PROSTREET
		ButtonTexHashes[FE_BUTTON1_TEX] = GetAppropriateButtonTex(buttonmask);
#endif
#ifndef GAME_MW
		ButtonTexHashes[FE_BUTTON0_TEX] = GetAppropriateButtonTex(buttonmask);
#endif
		break;
	case FRONTENDACTION_BUTTON2:
		ButtonTexHashes[FE_BUTTON2_TEX] = GetAppropriateButtonTex(buttonmask);
		break;
	case FRONTENDACTION_BUTTON3:
		ButtonTexHashes[FE_BUTTON3_TEX] = GetAppropriateButtonTex(buttonmask);
		break;
	case FRONTENDACTION_BUTTON4:
		ButtonTexHashes[FE_BUTTON4_TEX] = GetAppropriateButtonTex(buttonmask);
		break;
	case FRONTENDACTION_BUTTON5:
		ButtonTexHashes[FE_BUTTON5_TEX] = GetAppropriateButtonTex(buttonmask);
		break;
	case FRONTENDACTION_LTRIGGER:
		ButtonTexHashes[FE_LTRIGGER_TEX] = GetAppropriateButtonTex(buttonmask);
		break;
	case FRONTENDACTION_RTRIGGER:
		ButtonTexHashes[FE_RTRIGGER_TEX] = GetAppropriateButtonTex(buttonmask);
		break;
	case FRONTENDACTION_START:
		ButtonTexHashes[FE_START_TEX] = GetAppropriateButtonTex(buttonmask);
		break;
	default:
		break;
	}
	
}

void SetControllerFEng(FEObject* inobj)
{
	int HideFlags = FE_CONTROL_FLAG_CONSOLE;
	int ShowFlags = FE_CONTROL_FLAG_PC;

	if (LastControlledDevice == LASTCONTROLLED_CONTROLLER)
	{
		HideFlags = FE_CONTROL_FLAG_PC;
		ShowFlags = FE_CONTROL_FLAG_CONSOLE;
	}

	if (inobj->Flags & HideFlags)
	{
		if (inobj->Flags & ~1)
		{
			inobj->UserParam |= CFE_FLAG_WAS_VISIBLE_ORIGINALLY;
			FEngSetInvisible(inobj);
		}
	}

	if ((inobj->Flags & ShowFlags) && bUseDynamicFEngSwitching)
	{
		if ((inobj->UserParam & CFE_FLAG_WAS_VISIBLE_ORIGINALLY))
		{
			FEngSetVisible(inobj);
		}
	}
	


	if (inobj->Type == FE_Image)
	{
		for (int i = 0; i < FE_TEX_HASH_COUNT; i++)
		{
			FEngSwapTextureHash(inobj, FE_DefaultButtonTexHashes[i], ButtonTexHashes[i]);
		}
	}
	if (bEnableSplashTakeover)
		SetTitleScreenText();
}



#pragma runtime_checks( "", off )

#ifdef GAME_PROSTREET
bool bInPCControllerConfig()
{
	return cFEng_IsPackageInControl_Fast(NFS_HASH("FeControllerConfig_PC.fng")) || cFEng_IsPackageInControl_Fast(NFS_HASH("FeControllerConfig_PC_ReMap_Dialog.fng"));
}
#endif

void UpdateControllerFEng(FEObject* inobj)
{
	if (!bLoadedConsoleButtonTex)
	{
		LoadResourceFile(ButtonTexFilename, 0, 0, NULL, 0, 0, 0);
		ServiceResourceLoading();
		bLoadedConsoleButtonTex = true;
	}
#ifdef GAME_PROSTREET
	if (!bIsHudVisible() && !bInPCControllerConfig()) // HUD has no controller elements anyway, so no need to check it...
		SetControllerFEng(inobj);
#else
	if (!bIsHudVisible()) // HUD has no controller elements anyway, so no need to check it...
		SetControllerFEng(inobj);
#endif
}

bool __stdcall FEObjectCallback_Function(FEObject* inobj)
{
	UpdateControllerFEng(inobj);

	return true;
}

#ifdef GAME_MW
bool __stdcall MouseStateArrayOffsetUpdater_Callback_Hook(FEObject* inobj)
{
	unsigned int thethis = 0;
	_asm mov thethis, ecx

	UpdateControllerFEng(inobj);

	return MouseStateArrayOffsetUpdater((void*)thethis, inobj);
}

#else

struct FEObjectCallbackStruct
{
	void* Destructor;
	void* Function;
};

void __stdcall cFEng_Service_Hook(int unk1, int unk2)
{
	unsigned int thethis = 0;
	_asm mov thethis, ecx

	FEObjectCallbackStruct callback = { NULL, &FEObjectCallback_Function };
	void* cbpointer = &callback;

	int FEPackage = *(int*)(thethis + 0xE0);
	int NextFEPackage = 0;
	
	if (FEPackage)
	{
		while (FEPackage != NextFEPackage)
		{
			FEPackage_ForAllObjects((void*)FEPackage, &cbpointer);
			NextFEPackage = *(int*)(FEPackage + 4);
			if (NextFEPackage == FEPackage)
				break;
			NextFEPackage = FEPackage;
		}
	}

	return cFEng_Service((void*)thethis, unk1, unk2);
}
#ifdef GAME_CARBON
// world map fixes for Carbon
void __stdcall FEngine_ProcessPadsForPackage_Hook(void* FEPackage)
{
	unsigned int thethis = 0;
	_asm mov thethis, ecx

	if (cFEng_IsPackageInControl_Fast(WORLDMAPMAIN_FNG_NAMEHASH) || cFEng_IsPackageInControl_Fast(WORLDMAPQUICKLIST_FNG_NAMEHASH) || cFEng_IsPackageInControl_Fast(WORLDMAPWORLDVIEW_FNG_NAMEHASH))
	{
		FEObjectCallbackStruct callback = { NULL, &FEObjectCallback_Function };
		void* cbpointer = &callback;

		FEPackage_ForAllObjects(FEPackage, &cbpointer);
	}
	return FEngine_ProcessPadsForPackage((void*)thethis, FEPackage);
}

void* __stdcall FEPackage_FindObjectByHash_Hide_Hook(unsigned int namehash)
{
	unsigned int thethis = 0;
	_asm mov thethis, ecx

	FEngSetInvisible(FEPackage_FindObjectByHash((void*)thethis, WORLDMAP_BUTTONGROUP_CONSOLE));

	return FEPackage_FindObjectByHash((void*)thethis, namehash);
}

void* __stdcall FEPackage_FindObjectByHash_Show_Hook(unsigned int namehash)
{
	unsigned int thethis = 0;
	_asm mov thethis, ecx

	FEngSetVisible(FEPackage_FindObjectByHash((void*)thethis, WORLDMAP_BUTTONGROUP_CONSOLE));

	return FEPackage_FindObjectByHash((void*)thethis, namehash);
}

bool bQuickListFocused = false;

void __stdcall FEWorldMapStateManager_HandleScreenTick_Hook()
{
	unsigned int thethis = 0;
	_asm mov thethis, ecx

	if ((*(int*)GAMEFLOWMANAGER_STATUS_ADDR == 3) && *(int*)WORLDMAP_INSTANCE_ADDR)
	{
		// change text from "Free Roam" to "Back" during FE for the console UI element
		FE_String_Printf(FEngFindObject(*(char**)(*(int*)WORLDMAP_INSTANCE_ADDR + 0xC), 0x0EF36117), GetLocalizedString(0x8CD567B8));
		FE_String_Printf(FEngFindObject(*(char**)(*(int*)WORLDMAP_INSTANCE_ADDR + 0xC), 0xE2DACDBC), GetLocalizedString(0x8CD567B8));
	}

	if (bUseDynamicFEngSwitching)
	{
		if (bQuickListFocused && *(int*)WORLDMAP_INSTANCE_ADDR && cFEng_IsPackageInControl_Fast(WORLDMAPQUICKLIST_FNG_NAMEHASH))
		{
			FEngSetInvisible(FEngFindObject(*(char**)(*(int*)WORLDMAP_INSTANCE_ADDR + 0xC), WORLDMAP_BUTTONGROUP_CONSOLE));
			FEngSetInvisible(FEngFindObject(*(char**)(*(int*)WORLDMAP_INSTANCE_ADDR + 0xC), WORLDMAP_BUTTONGROUP_PC));
		}
		else if (*(int*)WORLDMAP_INSTANCE_ADDR)
		{
			if (LastControlledDevice == LASTCONTROLLED_CONTROLLER)
				FEngSetVisible(FEngFindObject(*(char**)(*(int*)WORLDMAP_INSTANCE_ADDR + 0xC), WORLDMAP_BUTTONGROUP_CONSOLE));
			if (LastControlledDevice == LASTCONTROLLED_KB)
				FEngSetVisible(FEngFindObject(*(char**)(*(int*)WORLDMAP_INSTANCE_ADDR + 0xC), WORLDMAP_BUTTONGROUP_PC));
		}
	}

	return FEWorldMapStateManager_HandleScreenTick((void*)thethis);
}

void __stdcall WorldMap_SetQuickListInFocus_Hook()
{
	unsigned int thethis = 0;
	_asm mov thethis, ecx

	bQuickListFocused = true;

	return WorldMap_SetQuickListInFocus((void*)thethis);
}

void __stdcall WorldMap_UnfocusQuickList_Hook()
{
	unsigned int thethis = 0;
	_asm mov thethis, ecx

	bQuickListFocused = false;

	if (LastControlledDevice == LASTCONTROLLED_CONTROLLER)
		FEngSetVisible(FEngFindObject(*(char**)(*(int*)WORLDMAP_INSTANCE_ADDR + 0xC), WORLDMAP_BUTTONGROUP_CONSOLE));
	if (LastControlledDevice == LASTCONTROLLED_KB)
		FEngSetVisible(FEngFindObject(*(char**)(*(int*)WORLDMAP_INSTANCE_ADDR + 0xC), WORLDMAP_BUTTONGROUP_PC));

	return WorldMap_UnfocusQuickList((void*)thethis);
}

void FEPrintf_Hook_WorldMap(char* pkg_name, unsigned int object_hash, char* fmt, char* str)
{
	FE_String_Printf(FEngFindObject(*(char**)(*(int*)WORLDMAP_INSTANCE_ADDR + 0xC), 0xCB235910), str);
	FE_String_Printf(FEngFindObject(*(char**)(*(int*)WORLDMAP_INSTANCE_ADDR + 0xC), 0x300B8082), str);
}

#endif // GAME_CARBON

#ifdef GAME_PROSTREET
void __stdcall FEngine_ProcessPadsForPackage_Hook(void* FEPackage)
{
	unsigned int thethis = 0;
	_asm mov thethis, ecx

	FEObjectCallbackStruct callback = { NULL, &FEObjectCallback_Function };
	void* cbpointer = &callback;

	FEPackage_ForAllObjects(FEPackage, &cbpointer);

	return FEngine_ProcessPadsForPackage((void*)thethis, FEPackage);
}

void FE_SetLanguageHash_Hook(char* pkg_name, unsigned int obj_hash, unsigned int lang_hash)
{
	FE_String_Printf(FEngFindObject(pkg_name, 0xBC928EF5), GetLocalizedString(lang_hash));
	FE_String_Printf(FEngFindObject(pkg_name, obj_hash), GetLocalizedString(lang_hash));
}

#endif // GAME_PROSTREET

#endif // GAME_MW

#pragma runtime_checks( "", restore )
