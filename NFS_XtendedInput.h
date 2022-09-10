#pragma once

template <unsigned int V> static constexpr unsigned int compiler_stringhash_template = V;
static constexpr unsigned int compiler_stringhash(char* str)
{
	if (str == NULL)
		return 0;

	char* _str = ((char*)str);
	unsigned int result = -1;

	while (*_str != 0)
	{
		result = result * 0x21 + (unsigned int)(*_str);
		_str = _str + 1;
	}

	return result;
}

#define NFS_HASH(s) compiler_stringhash_template<compiler_stringhash(s)>

#define FE_SPLASH_TEXT_XBOX "Press MENU or A button"
#define FE_SPLASH_TEXT_XBOX360 "Press START or A button"
#define FE_SPLASH_TEXT_PS4 "Press OPTIONS or X button"
#define FE_SPLASH_TEXT_PS3 "Press START or X button"
#define FE_SPLASH_TEXT_PC "CLICK or ENTER to continue"

#define LASTCONTROLLED_KB 0
#define LASTCONTROLLED_CONTROLLER 1
unsigned int LastControlledDevice = 0; // 0 = keyboard, 1 = controller
unsigned int LastControlledDeviceOldState = 0;

#define CONTROLLERICON_XBOXONE 0
#define CONTROLLERICON_PS4 1
#define CONTROLLERICON_XBOX360 2
#define CONTROLLERICON_PS3 3

unsigned int ControllerIconMode = 0; // read modes above

bool bInDebugWorldCamera = false;
