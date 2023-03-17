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
#include <windows.h>
#include <WinUser.h>
int bStringHash(char* a1);

#define VK_LBUTTON_HASH							NFS_HASH("VK_LBUTTON")							
#define VK_RBUTTON_HASH							NFS_HASH("VK_RBUTTON")							
#define VK_CANCEL_HASH							NFS_HASH("VK_CANCEL")							
#define VK_MBUTTON_HASH							NFS_HASH("VK_MBUTTON")							
#define VK_XBUTTON1_HASH						NFS_HASH("VK_XBUTTON1")						
#define VK_XBUTTON2_HASH						NFS_HASH("VK_XBUTTON2")						
#define VK_BACK_HASH							NFS_HASH("VK_BACK")							
#define VK_TAB_HASH								NFS_HASH("VK_TAB")								
#define VK_CLEAR_HASH							NFS_HASH("VK_CLEAR")							
#define VK_RETURN_HASH							NFS_HASH("VK_RETURN")							
#define VK_SHIFT_HASH							NFS_HASH("VK_SHIFT")							
#define VK_CONTROL_HASH							NFS_HASH("VK_CONTROL")							
#define VK_MENU_HASH							NFS_HASH("VK_MENU")							
#define VK_PAUSE_HASH							NFS_HASH("VK_PAUSE")							
#define VK_CAPITAL_HASH							NFS_HASH("VK_CAPITAL")							
#define VK_KANA_HASH							NFS_HASH("VK_KANA")							
#define VK_HANGEUL_HASH							NFS_HASH("VK_HANGEUL")							
#define VK_HANGUL_HASH							NFS_HASH("VK_HANGUL")							
#define VK_IME_ON_HASH							NFS_HASH("VK_IME_ON")							
#define VK_JUNJA_HASH							NFS_HASH("VK_JUNJA")							
#define VK_FINAL_HASH							NFS_HASH("VK_FINAL")							
#define VK_HANJA_HASH							NFS_HASH("VK_HANJA")							
#define VK_KANJI_HASH							NFS_HASH("VK_KANJI")							
#define VK_IME_OFF_HASH							NFS_HASH("VK_IME_OFF")							
#define VK_ESCAPE_HASH							NFS_HASH("VK_ESCAPE")							
#define VK_CONVERT_HASH							NFS_HASH("VK_CONVERT")							
#define VK_NONCONVERT_HASH						NFS_HASH("VK_NONCONVERT")						
#define VK_ACCEPT_HASH							NFS_HASH("VK_ACCEPT")							
#define VK_MODECHANGE_HASH						NFS_HASH("VK_MODECHANGE")						
#define VK_SPACE_HASH							NFS_HASH("VK_SPACE")							
#define VK_PRIOR_HASH							NFS_HASH("VK_PRIOR")							
#define VK_NEXT_HASH							NFS_HASH("VK_NEXT")							
#define VK_END_HASH								NFS_HASH("VK_END")								
#define VK_HOME_HASH							NFS_HASH("VK_HOME")							
#define VK_LEFT_HASH							NFS_HASH("VK_LEFT")							
#define VK_UP_HASH								NFS_HASH("VK_UP")								
#define VK_RIGHT_HASH							NFS_HASH("VK_RIGHT")							
#define VK_DOWN_HASH							NFS_HASH("VK_DOWN")							
#define VK_SELECT_HASH							NFS_HASH("VK_SELECT")							
#define VK_PRINT_HASH							NFS_HASH("VK_PRINT")							
#define VK_EXECUTE_HASH							NFS_HASH("VK_EXECUTE")							
#define VK_SNAPSHOT_HASH						NFS_HASH("VK_SNAPSHOT")						
#define VK_INSERT_HASH							NFS_HASH("VK_INSERT")							
#define VK_DELETE_HASH							NFS_HASH("VK_DELETE")							
#define VK_HELP_HASH							NFS_HASH("VK_HELP")							
#define VK_LWIN_HASH							NFS_HASH("VK_LWIN")							
#define VK_RWIN_HASH							NFS_HASH("VK_RWIN")							
#define VK_APPS_HASH							NFS_HASH("VK_APPS")							
#define VK_SLEEP_HASH							NFS_HASH("VK_SLEEP")							
#define VK_NUMPAD0_HASH							NFS_HASH("VK_NUMPAD0")							
#define VK_NUMPAD1_HASH							NFS_HASH("VK_NUMPAD1")							
#define VK_NUMPAD2_HASH							NFS_HASH("VK_NUMPAD2")							
#define VK_NUMPAD3_HASH							NFS_HASH("VK_NUMPAD3")							
#define VK_NUMPAD4_HASH							NFS_HASH("VK_NUMPAD4")							
#define VK_NUMPAD5_HASH							NFS_HASH("VK_NUMPAD5")							
#define VK_NUMPAD6_HASH							NFS_HASH("VK_NUMPAD6")							
#define VK_NUMPAD7_HASH							NFS_HASH("VK_NUMPAD7")							
#define VK_NUMPAD8_HASH							NFS_HASH("VK_NUMPAD8")							
#define VK_NUMPAD9_HASH							NFS_HASH("VK_NUMPAD9")							
#define VK_MULTIPLY_HASH						NFS_HASH("VK_MULTIPLY")						
#define VK_ADD_HASH								NFS_HASH("VK_ADD")								
#define VK_SEPARATOR_HASH						NFS_HASH("VK_SEPARATOR")						
#define VK_SUBTRACT_HASH						NFS_HASH("VK_SUBTRACT")						
#define VK_DECIMAL_HASH							NFS_HASH("VK_DECIMAL")							
#define VK_DIVIDE_HASH							NFS_HASH("VK_DIVIDE")							
#define VK_F1_HASH								NFS_HASH("VK_F1")								
#define VK_F2_HASH								NFS_HASH("VK_F2")								
#define VK_F3_HASH								NFS_HASH("VK_F3")								
#define VK_F4_HASH								NFS_HASH("VK_F4")								
#define VK_F5_HASH								NFS_HASH("VK_F5")								
#define VK_F6_HASH								NFS_HASH("VK_F6")								
#define VK_F7_HASH								NFS_HASH("VK_F7")								
#define VK_F8_HASH								NFS_HASH("VK_F8")								
#define VK_F9_HASH								NFS_HASH("VK_F9")								
#define VK_F10_HASH								NFS_HASH("VK_F10")								
#define VK_F11_HASH								NFS_HASH("VK_F11")								
#define VK_F12_HASH								NFS_HASH("VK_F12")								
#define VK_F13_HASH								NFS_HASH("VK_F13")								
#define VK_F14_HASH								NFS_HASH("VK_F14")								
#define VK_F15_HASH								NFS_HASH("VK_F15")								
#define VK_F16_HASH								NFS_HASH("VK_F16")								
#define VK_F17_HASH								NFS_HASH("VK_F17")								
#define VK_F18_HASH								NFS_HASH("VK_F18")								
#define VK_F19_HASH								NFS_HASH("VK_F19")								
#define VK_F20_HASH								NFS_HASH("VK_F20")								
#define VK_F21_HASH								NFS_HASH("VK_F21")								
#define VK_F22_HASH								NFS_HASH("VK_F22")								
#define VK_F23_HASH								NFS_HASH("VK_F23")								
#define VK_F24_HASH								NFS_HASH("VK_F24")								
#define VK_NAVIGATION_VIEW_HASH					NFS_HASH("VK_NAVIGATION_VIEW")					
#define VK_NAVIGATION_MENU_HASH					NFS_HASH("VK_NAVIGATION_MENU")					
#define VK_NAVIGATION_UP_HASH					NFS_HASH("VK_NAVIGATION_UP")					
#define VK_NAVIGATION_DOWN_HASH					NFS_HASH("VK_NAVIGATION_DOWN")					
#define VK_NAVIGATION_LEFT_HASH					NFS_HASH("VK_NAVIGATION_LEFT")					
#define VK_NAVIGATION_RIGHT_HASH				NFS_HASH("VK_NAVIGATION_RIGHT")				
#define VK_NAVIGATION_ACCEPT_HASH				NFS_HASH("VK_NAVIGATION_ACCEPT")				
#define VK_NAVIGATION_CANCEL_HASH				NFS_HASH("VK_NAVIGATION_CANCEL")				
#define VK_NUMLOCK_HASH							NFS_HASH("VK_NUMLOCK")							
#define VK_SCROLL_HASH							NFS_HASH("VK_SCROLL")							
#define VK_OEM_NEC_EQUAL_HASH					NFS_HASH("VK_OEM_NEC_EQUAL")					
#define VK_OEM_FJ_JISHO_HASH					NFS_HASH("VK_OEM_FJ_JISHO")					
#define VK_OEM_FJ_MASSHOU_HASH					NFS_HASH("VK_OEM_FJ_MASSHOU")					
#define VK_OEM_FJ_TOUROKU_HASH					NFS_HASH("VK_OEM_FJ_TOUROKU")					
#define VK_OEM_FJ_LOYA_HASH						NFS_HASH("VK_OEM_FJ_LOYA")						
#define VK_OEM_FJ_ROYA_HASH						NFS_HASH("VK_OEM_FJ_ROYA")						
#define VK_LSHIFT_HASH							NFS_HASH("VK_LSHIFT")							
#define VK_RSHIFT_HASH							NFS_HASH("VK_RSHIFT")							
#define VK_LCONTROL_HASH						NFS_HASH("VK_LCONTROL")						
#define VK_RCONTROL_HASH						NFS_HASH("VK_RCONTROL")						
#define VK_LMENU_HASH							NFS_HASH("VK_LMENU")							
#define VK_RMENU_HASH							NFS_HASH("VK_RMENU")							
#define VK_BROWSER_BACK_HASH					NFS_HASH("VK_BROWSER_BACK")					
#define VK_BROWSER_FORWARD_HASH					NFS_HASH("VK_BROWSER_FORWARD")					
#define VK_BROWSER_REFRESH_HASH					NFS_HASH("VK_BROWSER_REFRESH")					
#define VK_BROWSER_STOP_HASH					NFS_HASH("VK_BROWSER_STOP")					
#define VK_BROWSER_SEARCH_HASH					NFS_HASH("VK_BROWSER_SEARCH")					
#define VK_BROWSER_FAVORITES_HASH				NFS_HASH("VK_BROWSER_FAVORITES")				
#define VK_BROWSER_HOME_HASH					NFS_HASH("VK_BROWSER_HOME")					
#define VK_VOLUME_MUTE_HASH						NFS_HASH("VK_VOLUME_MUTE")						
#define VK_VOLUME_DOWN_HASH						NFS_HASH("VK_VOLUME_DOWN")						
#define VK_VOLUME_UP_HASH						NFS_HASH("VK_VOLUME_UP")						
#define VK_MEDIA_NEXT_TRACK_HASH				NFS_HASH("VK_MEDIA_NEXT_TRACK")				
#define VK_MEDIA_PREV_TRACK_HASH				NFS_HASH("VK_MEDIA_PREV_TRACK")				
#define VK_MEDIA_STOP_HASH						NFS_HASH("VK_MEDIA_STOP")						
#define VK_MEDIA_PLAY_PAUSE_HASH				NFS_HASH("VK_MEDIA_PLAY_PAUSE")				
#define VK_LAUNCH_MAIL_HASH						NFS_HASH("VK_LAUNCH_MAIL")						
#define VK_LAUNCH_MEDIA_SELECT_HASH				NFS_HASH("VK_LAUNCH_MEDIA_SELECT")				
#define VK_LAUNCH_APP1_HASH						NFS_HASH("VK_LAUNCH_APP1")						
#define VK_LAUNCH_APP2_HASH						NFS_HASH("VK_LAUNCH_APP2")						
#define VK_OEM_1_HASH							NFS_HASH("VK_OEM_1")							
#define VK_OEM_PLUS_HASH						NFS_HASH("VK_OEM_PLUS")						
#define VK_OEM_COMMA_HASH						NFS_HASH("VK_OEM_COMMA")						
#define VK_OEM_MINUS_HASH						NFS_HASH("VK_OEM_MINUS")						
#define VK_OEM_PERIOD_HASH						NFS_HASH("VK_OEM_PERIOD")						
#define VK_OEM_2_HASH							NFS_HASH("VK_OEM_2")							
#define VK_OEM_3_HASH							NFS_HASH("VK_OEM_3")							
#define VK_GAMEPAD_A_HASH						NFS_HASH("VK_GAMEPAD_A")						
#define VK_GAMEPAD_B_HASH						NFS_HASH("VK_GAMEPAD_B")						
#define VK_GAMEPAD_X_HASH						NFS_HASH("VK_GAMEPAD_X")						
#define VK_GAMEPAD_Y_HASH						NFS_HASH("VK_GAMEPAD_Y")						
#define VK_GAMEPAD_RIGHT_SHOULDER_HASH			NFS_HASH("VK_GAMEPAD_RIGHT_SHOULDER")			
#define VK_GAMEPAD_LEFT_SHOULDER_HASH			NFS_HASH("VK_GAMEPAD_LEFT_SHOULDER")			
#define VK_GAMEPAD_LEFT_TRIGGER_HASH			NFS_HASH("VK_GAMEPAD_LEFT_TRIGGER")			
#define VK_GAMEPAD_RIGHT_TRIGGER_HASH			NFS_HASH("VK_GAMEPAD_RIGHT_TRIGGER")			
#define VK_GAMEPAD_DPAD_UP_HASH					NFS_HASH("VK_GAMEPAD_DPAD_UP")					
#define VK_GAMEPAD_DPAD_DOWN_HASH				NFS_HASH("VK_GAMEPAD_DPAD_DOWN")				
#define VK_GAMEPAD_DPAD_LEFT_HASH				NFS_HASH("VK_GAMEPAD_DPAD_LEFT")				
#define VK_GAMEPAD_DPAD_RIGHT_HASH				NFS_HASH("VK_GAMEPAD_DPAD_RIGHT")				
#define VK_GAMEPAD_MENU_HASH					NFS_HASH("VK_GAMEPAD_MENU")					
#define VK_GAMEPAD_VIEW_HASH					NFS_HASH("VK_GAMEPAD_VIEW")					
#define VK_GAMEPAD_LEFT_THUMBSTICK_BUTTON_HASH	NFS_HASH("VK_GAMEPAD_LEFT_THUMBSTICK_BUTTON")	
#define VK_GAMEPAD_RIGHT_THUMBSTICK_BUTTON_HASH	NFS_HASH("VK_GAMEPAD_RIGHT_THUMBSTICK_BUTTON")	
#define VK_GAMEPAD_LEFT_THUMBSTICK_UP_HASH		NFS_HASH("VK_GAMEPAD_LEFT_THUMBSTICK_UP")		
#define VK_GAMEPAD_LEFT_THUMBSTICK_DOWN_HASH	NFS_HASH("VK_GAMEPAD_LEFT_THUMBSTICK_DOWN")	
#define VK_GAMEPAD_LEFT_THUMBSTICK_RIGHT_HASH	NFS_HASH("VK_GAMEPAD_LEFT_THUMBSTICK_RIGHT")	
#define VK_GAMEPAD_LEFT_THUMBSTICK_LEFT_HASH	NFS_HASH("VK_GAMEPAD_LEFT_THUMBSTICK_LEFT")	
#define VK_GAMEPAD_RIGHT_THUMBSTICK_UP_HASH		NFS_HASH("VK_GAMEPAD_RIGHT_THUMBSTICK_UP")		
#define VK_GAMEPAD_RIGHT_THUMBSTICK_DOWN_HASH	NFS_HASH("VK_GAMEPAD_RIGHT_THUMBSTICK_DOWN")	
#define VK_GAMEPAD_RIGHT_THUMBSTICK_RIGHT_HASH	NFS_HASH("VK_GAMEPAD_RIGHT_THUMBSTICK_RIGHT")	
#define VK_GAMEPAD_RIGHT_THUMBSTICK_LEFT_HASH	NFS_HASH("VK_GAMEPAD_RIGHT_THUMBSTICK_LEFT")	
#define VK_OEM_4_HASH							NFS_HASH("VK_OEM_4")							
#define VK_OEM_5_HASH							NFS_HASH("VK_OEM_5")							
#define VK_OEM_6_HASH							NFS_HASH("VK_OEM_6")							
#define VK_OEM_7_HASH							NFS_HASH("VK_OEM_7")							
#define VK_OEM_8_HASH							NFS_HASH("VK_OEM_8")							
#define VK_OEM_AX_HASH							NFS_HASH("VK_OEM_AX")							
#define VK_OEM_102_HASH							NFS_HASH("VK_OEM_102")							
#define VK_ICO_HELP_HASH						NFS_HASH("VK_ICO_HELP")						
#define VK_ICO_00_HASH							NFS_HASH("VK_ICO_00")							
#define VK_PROCESSKEY_HASH						NFS_HASH("VK_PROCESSKEY")						
#define VK_ICO_CLEAR_HASH						NFS_HASH("VK_ICO_CLEAR")						
#define VK_PACKET_HASH							NFS_HASH("VK_PACKET")							
#define VK_OEM_RESET_HASH						NFS_HASH("VK_OEM_RESET")						
#define VK_OEM_JUMP_HASH						NFS_HASH("VK_OEM_JUMP")						
#define VK_OEM_PA1_HASH							NFS_HASH("VK_OEM_PA1")							
#define VK_OEM_PA2_HASH							NFS_HASH("VK_OEM_PA2")							
#define VK_OEM_PA3_HASH							NFS_HASH("VK_OEM_PA3")							
#define VK_OEM_WSCTRL_HASH						NFS_HASH("VK_OEM_WSCTRL")						
#define VK_OEM_CUSEL_HASH						NFS_HASH("VK_OEM_CUSEL")						
#define VK_OEM_ATTN_HASH						NFS_HASH("VK_OEM_ATTN")						
#define VK_OEM_FINISH_HASH						NFS_HASH("VK_OEM_FINISH")						
#define VK_OEM_COPY_HASH						NFS_HASH("VK_OEM_COPY")						
#define VK_OEM_AUTO_HASH						NFS_HASH("VK_OEM_AUTO")						
#define VK_OEM_ENLW_HASH						NFS_HASH("VK_OEM_ENLW")						
#define VK_OEM_BACKTAB_HASH						NFS_HASH("VK_OEM_BACKTAB")						
#define VK_ATTN_HASH							NFS_HASH("VK_ATTN")							
#define VK_CRSEL_HASH							NFS_HASH("VK_CRSEL")							
#define VK_EXSEL_HASH							NFS_HASH("VK_EXSEL")							
#define VK_EREOF_HASH							NFS_HASH("VK_EREOF")							
#define VK_PLAY_HASH							NFS_HASH("VK_PLAY")							
#define VK_ZOOM_HASH							NFS_HASH("VK_ZOOM")							
#define VK_NONAME_HASH							NFS_HASH("VK_NONAME")							
#define VK_PA1_HASH								NFS_HASH("VK_PA1")								
#define VK_OEM_CLEAR_HASH						NFS_HASH("VK_OEM_CLEAR")

unsigned char ConvertVKNameToValue(char* name)
{
	for (int i = 0; i<strlen(name); ++i)
		name[i] = toupper(name[i]);

	switch (bStringHash(name))
	{
	case VK_LBUTTON_HASH:
		return VK_LBUTTON;
	case VK_RBUTTON_HASH:
		return VK_RBUTTON;
	case VK_CANCEL_HASH:
		return VK_CANCEL;
	case VK_MBUTTON_HASH:
		return VK_MBUTTON;
	case VK_XBUTTON1_HASH:
		return VK_XBUTTON1;
	case VK_XBUTTON2_HASH:
		return VK_XBUTTON2;
	case VK_BACK_HASH:
		return VK_BACK;
	case VK_TAB_HASH:
		return VK_TAB;
	case VK_CLEAR_HASH:
		return VK_CLEAR;
	case VK_RETURN_HASH:
		return VK_RETURN;
	case VK_SHIFT_HASH:
		return VK_SHIFT;
	case VK_CONTROL_HASH:
		return VK_CONTROL;
	case VK_MENU_HASH:
		return VK_MENU;
	case VK_PAUSE_HASH:
		return VK_PAUSE;
	case VK_CAPITAL_HASH:
		return VK_CAPITAL;
	case VK_KANA_HASH:
		return VK_KANA;
	case VK_HANGEUL_HASH:
		return VK_HANGEUL;
	case VK_HANGUL_HASH:
		return VK_HANGUL;
	case VK_IME_ON_HASH:
		return VK_IME_ON;
	case VK_JUNJA_HASH:
		return VK_JUNJA;
	case VK_FINAL_HASH:
		return VK_FINAL;
	case VK_HANJA_HASH:
		return VK_HANJA;
	case VK_KANJI_HASH:
		return VK_KANJI;
	case VK_IME_OFF_HASH:
		return VK_IME_OFF;
	case VK_ESCAPE_HASH:
		return VK_ESCAPE;
	case VK_CONVERT_HASH:
		return VK_CONVERT;
	case VK_NONCONVERT_HASH:
		return VK_NONCONVERT;
	case VK_ACCEPT_HASH:
		return VK_ACCEPT;
	case VK_MODECHANGE_HASH:
		return VK_MODECHANGE;
	case VK_SPACE_HASH:
		return VK_SPACE;
	case VK_PRIOR_HASH:
		return VK_PRIOR;
	case VK_NEXT_HASH:
		return VK_NEXT;
	case VK_END_HASH:
		return VK_END;
	case VK_HOME_HASH:
		return VK_HOME;
	case VK_LEFT_HASH:
		return VK_LEFT;
	case VK_UP_HASH:
		return VK_UP;
	case VK_RIGHT_HASH:
		return VK_RIGHT;
	case VK_DOWN_HASH:
		return VK_DOWN;
	case VK_SELECT_HASH:
		return VK_SELECT;
	case VK_PRINT_HASH:
		return VK_PRINT;
	case VK_EXECUTE_HASH:
		return VK_EXECUTE;
	case VK_SNAPSHOT_HASH:
		return VK_SNAPSHOT;
	case VK_INSERT_HASH:
		return VK_INSERT;
	case VK_DELETE_HASH:
		return VK_DELETE;
	case VK_HELP_HASH:
		return VK_HELP;
	case VK_LWIN_HASH:
		return VK_LWIN;
	case VK_RWIN_HASH:
		return VK_RWIN;
	case VK_APPS_HASH:
		return VK_APPS;
	case VK_SLEEP_HASH:
		return VK_SLEEP;
	case VK_NUMPAD0_HASH:
		return VK_NUMPAD0;
	case VK_NUMPAD1_HASH:
		return VK_NUMPAD1;
	case VK_NUMPAD2_HASH:
		return VK_NUMPAD2;
	case VK_NUMPAD3_HASH:
		return VK_NUMPAD3;
	case VK_NUMPAD4_HASH:
		return VK_NUMPAD4;
	case VK_NUMPAD5_HASH:
		return VK_NUMPAD5;
	case VK_NUMPAD6_HASH:
		return VK_NUMPAD6;
	case VK_NUMPAD7_HASH:
		return VK_NUMPAD7;
	case VK_NUMPAD8_HASH:
		return VK_NUMPAD8;
	case VK_NUMPAD9_HASH:
		return VK_NUMPAD9;
	case VK_MULTIPLY_HASH:
		return VK_MULTIPLY;
	case VK_ADD_HASH:
		return VK_ADD;
	case VK_SEPARATOR_HASH:
		return VK_SEPARATOR;
	case VK_SUBTRACT_HASH:
		return VK_SUBTRACT;
	case VK_DECIMAL_HASH:
		return VK_DECIMAL;
	case VK_DIVIDE_HASH:
		return VK_DIVIDE;
	case VK_F1_HASH:
		return VK_F1;
	case VK_F2_HASH:
		return VK_F2;
	case VK_F3_HASH:
		return VK_F3;
	case VK_F4_HASH:
		return VK_F4;
	case VK_F5_HASH:
		return VK_F5;
	case VK_F6_HASH:
		return VK_F6;
	case VK_F7_HASH:
		return VK_F7;
	case VK_F8_HASH:
		return VK_F8;
	case VK_F9_HASH:
		return VK_F9;
	case VK_F10_HASH:
		return VK_F10;
	case VK_F11_HASH:
		return VK_F11;
	case VK_F12_HASH:
		return VK_F12;
	case VK_F13_HASH:
		return VK_F13;
	case VK_F14_HASH:
		return VK_F14;
	case VK_F15_HASH:
		return VK_F15;
	case VK_F16_HASH:
		return VK_F16;
	case VK_F17_HASH:
		return VK_F17;
	case VK_F18_HASH:
		return VK_F18;
	case VK_F19_HASH:
		return VK_F19;
	case VK_F20_HASH:
		return VK_F20;
	case VK_F21_HASH:
		return VK_F21;
	case VK_F22_HASH:
		return VK_F22;
	case VK_F23_HASH:
		return VK_F23;
	case VK_F24_HASH:
		return VK_F24;
	case VK_NAVIGATION_VIEW_HASH:
		return VK_NAVIGATION_VIEW;
	case VK_NAVIGATION_MENU_HASH:
		return VK_NAVIGATION_MENU;
	case VK_NAVIGATION_UP_HASH:
		return VK_NAVIGATION_UP;
	case VK_NAVIGATION_DOWN_HASH:
		return VK_NAVIGATION_DOWN;
	case VK_NAVIGATION_LEFT_HASH:
		return VK_NAVIGATION_LEFT;
	case VK_NAVIGATION_RIGHT_HASH:
		return VK_NAVIGATION_RIGHT;
	case VK_NAVIGATION_ACCEPT_HASH:
		return VK_NAVIGATION_ACCEPT;
	case VK_NAVIGATION_CANCEL_HASH:
		return VK_NAVIGATION_CANCEL;
	case VK_NUMLOCK_HASH:
		return VK_NUMLOCK;
	case VK_SCROLL_HASH:
		return VK_SCROLL;
	case VK_OEM_NEC_EQUAL_HASH:
		return VK_OEM_NEC_EQUAL;
	case VK_OEM_FJ_JISHO_HASH:
		return VK_OEM_FJ_JISHO;
	case VK_OEM_FJ_MASSHOU_HASH:
		return VK_OEM_FJ_MASSHOU;
	case VK_OEM_FJ_TOUROKU_HASH:
		return VK_OEM_FJ_TOUROKU;
	case VK_OEM_FJ_LOYA_HASH:
		return VK_OEM_FJ_LOYA;
	case VK_OEM_FJ_ROYA_HASH:
		return VK_OEM_FJ_ROYA;
	case VK_LSHIFT_HASH:
		return VK_LSHIFT;
	case VK_RSHIFT_HASH:
		return VK_RSHIFT;
	case VK_LCONTROL_HASH:
		return VK_LCONTROL;
	case VK_RCONTROL_HASH:
		return VK_RCONTROL;
	case VK_LMENU_HASH:
		return VK_LMENU;
	case VK_RMENU_HASH:
		return VK_RMENU;
	case VK_BROWSER_BACK_HASH:
		return VK_BROWSER_BACK;
	case VK_BROWSER_FORWARD_HASH:
		return VK_BROWSER_FORWARD;
	case VK_BROWSER_REFRESH_HASH:
		return VK_BROWSER_REFRESH;
	case VK_BROWSER_STOP_HASH:
		return VK_BROWSER_STOP;
	case VK_BROWSER_SEARCH_HASH:
		return VK_BROWSER_SEARCH;
	case VK_BROWSER_FAVORITES_HASH:
		return VK_BROWSER_FAVORITES;
	case VK_BROWSER_HOME_HASH:
		return VK_BROWSER_HOME;
	case VK_VOLUME_MUTE_HASH:
		return VK_VOLUME_MUTE;
	case VK_VOLUME_DOWN_HASH:
		return VK_VOLUME_DOWN;
	case VK_VOLUME_UP_HASH:
		return VK_VOLUME_UP;
	case VK_MEDIA_NEXT_TRACK_HASH:
		return VK_MEDIA_NEXT_TRACK;
	case VK_MEDIA_PREV_TRACK_HASH:
		return VK_MEDIA_PREV_TRACK;
	case VK_MEDIA_STOP_HASH:
		return VK_MEDIA_STOP;
	case VK_MEDIA_PLAY_PAUSE_HASH:
		return VK_MEDIA_PLAY_PAUSE;
	case VK_LAUNCH_MAIL_HASH:
		return VK_LAUNCH_MAIL;
	case VK_LAUNCH_MEDIA_SELECT_HASH:
		return VK_LAUNCH_MEDIA_SELECT;
	case VK_LAUNCH_APP1_HASH:
		return VK_LAUNCH_APP1;
	case VK_LAUNCH_APP2_HASH:
		return VK_LAUNCH_APP2;
	case VK_OEM_1_HASH:
		return VK_OEM_1;
	case VK_OEM_PLUS_HASH:
		return VK_OEM_PLUS;
	case VK_OEM_COMMA_HASH:
		return VK_OEM_COMMA;
	case VK_OEM_MINUS_HASH:
		return VK_OEM_MINUS;
	case VK_OEM_PERIOD_HASH:
		return VK_OEM_PERIOD;
	case VK_OEM_2_HASH:
		return VK_OEM_2;
	case VK_OEM_3_HASH:
		return VK_OEM_3;
	case VK_GAMEPAD_A_HASH:
		return VK_GAMEPAD_A;
	case VK_GAMEPAD_B_HASH:
		return VK_GAMEPAD_B;
	case VK_GAMEPAD_X_HASH:
		return VK_GAMEPAD_X;
	case VK_GAMEPAD_Y_HASH:
		return VK_GAMEPAD_Y;
	case VK_GAMEPAD_RIGHT_SHOULDER_HASH:
		return VK_GAMEPAD_RIGHT_SHOULDER;
	case VK_GAMEPAD_LEFT_SHOULDER_HASH:
		return VK_GAMEPAD_LEFT_SHOULDER;
	case VK_GAMEPAD_LEFT_TRIGGER_HASH:
		return VK_GAMEPAD_LEFT_TRIGGER;
	case VK_GAMEPAD_RIGHT_TRIGGER_HASH:
		return VK_GAMEPAD_RIGHT_TRIGGER;
	case VK_GAMEPAD_DPAD_UP_HASH:
		return VK_GAMEPAD_DPAD_UP;
	case VK_GAMEPAD_DPAD_DOWN_HASH:
		return VK_GAMEPAD_DPAD_DOWN;
	case VK_GAMEPAD_DPAD_LEFT_HASH:
		return VK_GAMEPAD_DPAD_LEFT;
	case VK_GAMEPAD_DPAD_RIGHT_HASH:
		return VK_GAMEPAD_DPAD_RIGHT;
	case VK_GAMEPAD_MENU_HASH:
		return VK_GAMEPAD_MENU;
	case VK_GAMEPAD_VIEW_HASH:
		return VK_GAMEPAD_VIEW;
	case VK_GAMEPAD_LEFT_THUMBSTICK_BUTTON_HASH:
		return VK_GAMEPAD_LEFT_THUMBSTICK_BUTTON;
	case VK_GAMEPAD_RIGHT_THUMBSTICK_BUTTON_HASH:
		return VK_GAMEPAD_RIGHT_THUMBSTICK_BUTTON;
	case VK_GAMEPAD_LEFT_THUMBSTICK_UP_HASH:
		return VK_GAMEPAD_LEFT_THUMBSTICK_UP;
	case VK_GAMEPAD_LEFT_THUMBSTICK_DOWN_HASH:
		return VK_GAMEPAD_LEFT_THUMBSTICK_DOWN;
	case VK_GAMEPAD_LEFT_THUMBSTICK_RIGHT_HASH:
		return VK_GAMEPAD_LEFT_THUMBSTICK_RIGHT;
	case VK_GAMEPAD_LEFT_THUMBSTICK_LEFT_HASH:
		return VK_GAMEPAD_LEFT_THUMBSTICK_LEFT;
	case VK_GAMEPAD_RIGHT_THUMBSTICK_UP_HASH:
		return VK_GAMEPAD_RIGHT_THUMBSTICK_UP;
	case VK_GAMEPAD_RIGHT_THUMBSTICK_DOWN_HASH:
		return VK_GAMEPAD_RIGHT_THUMBSTICK_DOWN;
	case VK_GAMEPAD_RIGHT_THUMBSTICK_RIGHT_HASH:
		return VK_GAMEPAD_RIGHT_THUMBSTICK_RIGHT;
	case VK_GAMEPAD_RIGHT_THUMBSTICK_LEFT_HASH:
		return VK_GAMEPAD_RIGHT_THUMBSTICK_LEFT;
	case VK_OEM_4_HASH:
		return VK_OEM_4;
	case VK_OEM_5_HASH:
		return VK_OEM_5;
	case VK_OEM_6_HASH:
		return VK_OEM_6;
	case VK_OEM_7_HASH:
		return VK_OEM_7;
	case VK_OEM_8_HASH:
		return VK_OEM_8;
	case VK_OEM_AX_HASH:
		return VK_OEM_AX;
	case VK_OEM_102_HASH:
		return VK_OEM_102;
	case VK_ICO_HELP_HASH:
		return VK_ICO_HELP;
	case VK_ICO_00_HASH:
		return VK_ICO_00;
	case VK_PROCESSKEY_HASH:
		return VK_PROCESSKEY;
	case VK_ICO_CLEAR_HASH:
		return VK_ICO_CLEAR;
	case VK_PACKET_HASH:
		return VK_PACKET;
	case VK_OEM_RESET_HASH:
		return VK_OEM_RESET;
	case VK_OEM_JUMP_HASH:
		return VK_OEM_JUMP;
	case VK_OEM_PA1_HASH:
		return VK_OEM_PA1;
	case VK_OEM_PA2_HASH:
		return VK_OEM_PA2;
	case VK_OEM_PA3_HASH:
		return VK_OEM_PA3;
	case VK_OEM_WSCTRL_HASH:
		return VK_OEM_WSCTRL;
	case VK_OEM_CUSEL_HASH:
		return VK_OEM_CUSEL;
	case VK_OEM_ATTN_HASH:
		return VK_OEM_ATTN;
	case VK_OEM_FINISH_HASH:
		return VK_OEM_FINISH;
	case VK_OEM_COPY_HASH:
		return VK_OEM_COPY;
	case VK_OEM_AUTO_HASH:
		return VK_OEM_AUTO;
	case VK_OEM_ENLW_HASH:
		return VK_OEM_ENLW;
	case VK_OEM_BACKTAB_HASH:
		return VK_OEM_BACKTAB;
	case VK_ATTN_HASH:
		return VK_ATTN;
	case VK_CRSEL_HASH:
		return VK_CRSEL;
	case VK_EXSEL_HASH:
		return VK_EXSEL;
	case VK_EREOF_HASH:
		return VK_EREOF;
	case VK_PLAY_HASH:
		return VK_PLAY;
	case VK_ZOOM_HASH:
		return VK_ZOOM;
	case VK_NONAME_HASH:
		return VK_NONAME;
	case VK_PA1_HASH:
		return VK_PA1;
	case VK_OEM_CLEAR_HASH:
		return VK_OEM_CLEAR;

	default:
		break;
	}
	return 0;
}
