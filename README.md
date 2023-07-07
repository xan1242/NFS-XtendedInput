# Need for Speed (MW & newer) - Xtended Input

This is a plugin for NFS games by Black Box (MW and newer only) which brings XInput support to the games!

Currently a work in progress, but fully usable and playable!

(NFS World currently at ALPHA stage!)

[Also available for Underground and Underground 2 here](https://github.com/xan1242/NFSU-XtendedInput)

## Features

- Automatic UI and text swapping for button icons - dynamically detects when you use either a keyboard or a controller

- Choice between Xbox (One and 360), PS4/PS3 and Switch icons (made by [AeroWidescreen (Aero_)](https://github.com/AeroWidescreen) )

- Console control feature parity (except rumble): restored debug camera analog control

- Re-done keyboard input code - not using DInput8 anymore. Now talks directly over Win32 (GetAsyncKeyState or GetKeyboardState)

- Re-done mouse input - also using Win32 for mouse input (with auto hiding after 5 seconds)

- Mouse look for debug camera

- Optional - Win32 / OS mouse rendering with a custom cursor

- All game actions are now rebindable

## Button mappings & configuration

You may remap the buttons in the `NFS_XtendedInput.usermap.ini` file found inside the `scripts\XtendedInputMaps\<your save name>` folder.

For secondary maps, use the sections `[Events_Secondary]` for gamepads and `[EventsKB_Secondary]` for keyboards.

Refer to the EventReference.txt file to see what's available.

(PROSTREET ONLY) You may also freely remap inside the game's settings menu. This feature will become available in other games soon.



You can configure other settings (such as deadzones, omni mode, etc.) in the `NFS_XtendedInput.ini` file found inside the `scripts` folder.



`NFS_XtendedInput.default.ini` contains default mappings for when a new profile is created or settings reset to default in the remap menu.



(MW ONLY) You cannot enter the "Controls" settings menu as it currently crashes the game, therefore it has been disabled. If you need to access it, disable this plugin.

The button remaps will change the textures as well according to what you set it to (only for gamepads).

Default mapping follows the Xbox 360 version of the games.

Default controller button mappings are:

### Ingame

- Throttle: Right Trigger

- Brake: Left Trigger

- Steering: Left Stick X axis

- E-Brake: A button

- NOS: B button

- Speedbreaker: X button

- Look back: Left shoulder (Pro Street: Y)

- Change camera: Right shoulder (Pro Street: X)

- Shift up/down: Right Stick Y axis up/down

- Car reset: View/Back

- Pause/Unpause: Menu/Start

- SMS/Map/Status/Engage Event: D-Pad

(Carbon only)

- Activate crew member: Y

(Pro Street only)

- Clutch: Left shoulder

- Toggle arrow indicators: D-Pad Up

- Toggle best line indicators: D-Pad Down

- Toggle Leaderboards: D-Pad Left

- Toggle minimap: D-Pad Right

One note about the World Map screen: during this screen and only then, you can control the mouse cursor with the right analog stick (and click with the Left shoulder button). If you're using the OS mouse, it **will ** take control of it (and subequently confine the mouse wihin the game window).

### FrontEnd

Please note that some menus may not advertise their functions properly, but they should be fully functional and controllable like in the console versions.

- Navigation: D-Pad & Left Analog Stick

- Accept: A button

- Back: B button

- L1: Left Trigger

- R1: Right Trigger

- Car orbit: Right Stick

- Button 0: Right shoulder

- Button 1: Light shoulder

- Button 2: Left Thumbstick (L3)

- Button 3: Right Thumbstick (R3)

- Button 4: X

- Button 5: Y

- Start: Start

- Quit game: View/Back button

### Debug camera

You must use a controller on the second port to use it. Invoke it with a debug camera plugin like Extra Options.

- Debug camera trigger: View/Back

- Move forward: A button

- Move backward: Y button

- Move left: X button

- Move right: B button

- Move up: Left Shoulder

- Move down: Right Shoulder

- Look up/down/left/right: D-Pad

- Turbo: Left Trigger

- Super Turbo: Right Trigger

- Movement (analog): Left Stick X/Y

- Look (analog): Right stick X/Y

- Drop car: Start

## Keyboard controls

They're slightly changed from default.

By default it'll use GetAsyncKeyState (unbuffered) to read keys which might cause some issues with other hooks to the WndProc of this game, but will deliver the most optimal performance. A key's status is being read as it's being scanned in this mode.

Second mode is GetKeyboardState (buffered). This will store all keys in a buffer and read them during `InputDevice::PollDevice()` (which starts right before eDisplayFrame). Use this if you have some issues.

### Ingame

- Throttle: Up

- Brake: Down

- Steering: Left/Right

- E-Brake: Space (Pro Street: Right Control)

- NOS: Left Alt

- Speedbreaker: X

- Look back: L

- Change camera: C

- Pull camera back: P

- Shift up/down: Left Shift / Left Control

- Pause: Escape

- Quit game: Q

- SMS: Tab

- Map: M

- Blacklist menu: B

- Engage event: Enter (Undercover: Tab)

(Carbon only)

- Activate crew member: Right Control

(Pro Street only)

- Clutch: Space

### FrontEnd

They should be identical to the default game, with one exception: the navigation keys do not control the camera anymore. They were moved to their own buttons.

- Navigation: Navigation keys (U/D/L/R)

- Accept: Enter

- Back: Escape

- Left shoulder & Right shoulder buttons: 9 & 0

- Car orbit: WASD

- Button 0: Most Wanted: 3, Carbon: 5, Pro Street: 3

- Button 1: Most Wanted: 5, Carbon: 3, Pro Street: 4

- Button 2: Most Wanted: T, Pro Street: 5

- Button 3: Most Wanted: R, Pro Street: M

- Button 4: 2

- Button 5: 1

- Start: Most Wanted: 4, Carbon & newer: Space

- Quit game: Q

### Debug camera

- Debug camera trigger: Subtract (minus key on numpad)

- Move forward: W

- Move backward: S

- Move left: A

- Move right: D

- Move up: Space

- Move down: Left Control

- Look up: I

- Look down: K

- Look left: J

- Look right: L

- Turbo: Right Shift

- Super Turbo: F

- Drop car: 5

## Windows 7 compatibility

To use in Windows 7 (and maybe Vista & XP), you need to use the "Win7" builds.

Requirements are:

- [Xbox 360 Controller Driver](https://archive.org/details/xbox-360-64-engwin-7)

- [DirectX Runtime](https://www.microsoft.com/en-us/download/details.aspx?id=8109) (for XInput 9.1.0)

- [Visual C++ Redistributable](https://aka.ms/vs/17/release/vc_redist.x86.exe) (2019 are used by ASI Loader and XtendedInput, both are included in the installation package)

This was tested on a Pentium 3 machine running Windows 7 and it is fully working.

These builds are made with SSE instructions enabled (and other extensions disabled), so it should also work on Athlon XP as well.

The release package comes with a special build of ASI loader which is made without any CPU extensions required. It is also patched to work in Windows XP.

These builds may or may not work in Windows XP with some hacks, YMMV as it wasn't tested. Follow the instructions found [here](https://learn.microsoft.com/en-us/cpp/windows/latest-supported-vc-redist?view=msvc-170) to find out more. TLDR: "The last version of the Visual C++ Redistributable that works on Windows XP shipped in Visual Studio 2019 version 16.7 (file versions starting with **14.27**)."

## TODO list:

- Check the top of the source file

- Currently KILLS Direct Input, beware

# Credits

- AeroWidescreen - for the amazing button icons!

- LINK/2012 - injector
