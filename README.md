# Need for Speed (MW & newer) - Xtended Input

This is a plugin for NFS games by Black Box (MW and newer only) which brings XInput support to the games!

Currently a work in progress, but fully usable and playable!

Planned to bring to other NFS games in the future (currently only supports MW & Carbon & Pro Street)!

[Also available for Underground and Underground 2 here](https://github.com/xan1242/NFSU-XtendedInput)

## Features

- Automatic UI and text swapping for button icons - dynamically detects when you use either a keyboard or a controller

- Choice between Xbox and PS4 icons (made by [AeroWidescreen (Aero_)](https://github.com/AeroWidescreen) )

- Console control feature parity (except rumble): restored debug camera analog control

- Re-done keyboard input code - not using DInput8 anymore. Now talks directly over Win32 (GetAsyncKeyState or GetKeyboardState)

- Re-done mouse input - also using Win32 for mouse input (with auto hiding after 5 seconds)

- Optional - Win32 / OS mouse rendering with a custom cursor

- All game actions are now rebindable

## Button mappings

You can only remap the buttons in the configuration file (NFS_XtendedInput.ini), however as of now there is 1 limitation: you may only assign 1 button to an event. Refer to the EventReference.txt file to see what's available.

You cannot enter the "Controls" settings menu as it currently crashes the game, therefore it has been disabled. If you need to access it, disable this plugin.

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

- Engage event: Enter

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

## TODO list:

- Check the top of the source file

- Currently KILLS Direct Input, beware

# Credits

- AeroWidescreen - for the amazing button icons!

- LINK/2012 - injector
