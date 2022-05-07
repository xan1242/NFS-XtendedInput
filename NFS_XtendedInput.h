#pragma once

#define FE_SPLASH_TEXT_XBOX "Press MENU or A button"
#define FE_SPLASH_TEXT_PS4 "Press OPTIONS or X button"
#define FE_SPLASH_TEXT_PC "CLICK or ENTER to continue"

#define LASTCONTROLLED_KB 0
#define LASTCONTROLLED_CONTROLLER 1
unsigned int LastControlledDevice = 0; // 0 = keyboard, 1 = controller
unsigned int LastControlledDeviceOldState = 0;

#define CONTROLLERICON_XBOXONE 0
#define CONTROLLERICON_PS4 1

unsigned int ControllerIconMode = 0; // 0 = Xbox (One and later only for now), 1 = PlayStation (4 only now) -- planned to add: Nintendo (Wii/U Classic Controller, Switch), PS3/PS2, Xbox 360
