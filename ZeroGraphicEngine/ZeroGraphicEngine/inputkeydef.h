/*****************************************************************************/
/*!
\file			inputkeydef.h
\brief			marco for input keys
*/
/*****************************************************************************/
#ifndef INPUTKEYDEF_H_
#define INPUTKEYDEF_H_

#include <Xinput.h>

#define MOUSE_LEFT		 VK_LBUTTON	    // Left		mouse button
#define MOUSE_RIGHT		 VK_RBUTTON	    // Right	mouse button
#define MOUSE_MIDDLE	 VK_MBUTTON	    // Middle	mouse button

#define KEY_BACK         VK_BACK        // Backspace
#define KEY_TAB          VK_TAB      
#define KEY_NO_NUMLK_5	 VK_CLEAR
#define KEY_RETURN		 VK_RETURN	

#define KEY_SHIFT		 VK_SHIFT		// either one of the Shift
#define KEY_CTRL		 VK_CONTROL		// either one of the Ctrl
#define KEY_ALT			 VK_MENU		// either one of the Alt
#define KEY_PAUSE		 VK_PAUSE		// Pause key, the key right of ScrollLock
#define KEY_CAPSLOCK	 VK_CAPITAL		// Caps lock

#define KEY_ESCAPE       VK_ESCAPE   

#define KEY_SPACE		 VK_SPACE
#define KEY_PAGEUP		 VK_PRIOR		
#define KEY_PAGEDOWN	 VK_NEXT
#define KEY_END			 VK_END		
#define KEY_HOME		 VK_HOME		
#define KEY_LEFT		 VK_LEFT		
#define KEY_UP			 VK_UP			
#define KEY_RIGHT		 VK_RIGHT		
#define KEY_DOWN		 VK_DOWN	
//#define KEY_SELECT	 VK_SELECT
//#define KEY_PRINT		 VK_PRINT		// Print (only used by Nokia keyboards)
//#define KEY_EXECUTE	 VK_EXECUTE		// Execute (not used)
#define KEY_PRINTSCREEN  VK_SNAPSHOT
#define KEY_INSERT		 VK_INSERT		
#define KEY_DELETE		 VK_DELETE	
//#define KEY_HELP		 VK_HELP	

#define KEY_SLEEP		VK_SLEEP		// Computer Sleep key

#define KEY_LSHIFT		 VK_LSHIFT		
#define KEY_RSHIFT		 VK_RSHIFT
#define KEY_LCTRL		 VK_LCONTROL		
#define KEY_RCTRL		 VK_RCONTROL		
#define KEY_LALT		 VK_LMENU		
#define KEY_RALT		 VK_RMENU		

#define KEY_SCROLLLOCK	 VK_SCROLL

#define KEY_0            0x30  // 48    
#define KEY_1            0x31  // 49     
#define KEY_2            0x32  // 50   
#define KEY_3            0x33  // 51   
#define KEY_4            0x34  // 52   
#define KEY_5            0x35  // 53   
#define KEY_6            0x36  // 54   
#define KEY_7            0x37  // 55   
#define KEY_8            0x38  // 56   
#define KEY_9            0x39  // 57    

#define KEY_A            0x41  // 65      
#define KEY_B            0x42  // 66     
#define KEY_C            0x43  // 67     
#define KEY_D            0x44  // 68     
#define KEY_E            0x45  // 69     
#define KEY_F            0x46  // 70     
#define KEY_G            0x47  // 71     
#define KEY_H            0x48  // 72     
#define KEY_I            0x49  // 73      
#define KEY_J            0x4A  // 74      
#define KEY_K            0x4B  // 75      
#define KEY_L            0x4C  // 76      
#define KEY_M            0x4D  // 77      
#define KEY_N            0x4E  // 78      
#define KEY_O            0x4F  // 79      
#define KEY_P            0x50  // 80      
#define KEY_Q            0x51  // 81      
#define KEY_R            0x52  // 82      
#define KEY_S            0x53  // 83      
#define KEY_T            0x54  // 84      
#define KEY_U            0x55  // 85      
#define KEY_V            0x56  // 86      
#define KEY_W            0x57  // 87      
#define KEY_X            0x58  // 88      
#define KEY_Y            0x59  // 89      
#define KEY_Z            0x5A  // 90  

#define KEY_NUMPAD0	     VK_NUMPAD0	
#define KEY_NUMPAD1	     VK_NUMPAD1	
#define KEY_NUMPAD2	     VK_NUMPAD2	
#define KEY_NUMPAD3	     VK_NUMPAD3	
#define KEY_NUMPAD4	     VK_NUMPAD4	
#define KEY_NUMPAD5	     VK_NUMPAD5	
#define KEY_NUMPAD6	     VK_NUMPAD6	
#define KEY_NUMPAD7	     VK_NUMPAD7	
#define KEY_NUMPAD8	     VK_NUMPAD8	
#define KEY_NUMPAD9	     VK_NUMPAD9	

#define KEY_NUM_MULTIPLY    VK_MULTIPLY
#define KEY_NUM_PLUS	    VK_ADD
#define KEY_NUM_MINUS	    VK_SUBTRACT
#define KEY_NUM_PERIOD	    VK_DECIMAL
#define KEY_NUM_DIVIDE	    VK_DIVIDE
#define KEY_NUMLOCK			VK_NUMLOCK	

#define KEY_F1			    VK_F1
#define KEY_F2			    VK_F2
#define KEY_F3			    VK_F3
#define KEY_F4			    VK_F4
#define KEY_F5			    VK_F5
#define KEY_F6			    VK_F6
#define KEY_F7			    VK_F7
#define KEY_F8			    VK_F8
#define KEY_F9			    VK_F9
#define KEY_F10				VK_F10
#define KEY_F11				VK_F11
#define KEY_F12				VK_F12

#define KEY_SEMICOLON	    VK_OEM_1        // ; :
#define KEY_SLASH		    VK_OEM_2		// / ?
#define KEY_BACKQUOTE		VK_OEM_3        // ` ~
#define KEY_LBRACKET	    VK_OEM_4        // [ {
#define KEY_BACKSLASH	    VK_OEM_5        // \ |
#define KEY_RBRACKET	    VK_OEM_6        // ] }
#define KEY_QUOTE		    VK_OEM_7	    // ' "
											  
#define KEY_EQUAL		    VK_OEM_PLUS     // = +		
#define KEY_MINUS		    VK_OEM_MINUS    // - _		
#define KEY_PERIOD		    VK_OEM_PERIOD   // . >		
#define KEY_COMMA		    VK_OEM_COMMA    // , <		

//Controller
#define GAME_DPAD_UP		XINPUT_GAMEPAD_DPAD_UP       		
#define GAME_DPAD_DOWN		XINPUT_GAMEPAD_DPAD_DOWN     
#define GAME_DPAD_LEFT		XINPUT_GAMEPAD_DPAD_LEFT     
#define GAME_DPAD_RIGHT		XINPUT_GAMEPAD_DPAD_RIGHT    
#define GAME_START			XINPUT_GAMEPAD_START         
#define GAME_BACK			XINPUT_GAMEPAD_BACK          
#define GAME_A				XINPUT_GAMEPAD_A             
#define GAME_B				XINPUT_GAMEPAD_B             
#define GAME_X				XINPUT_GAMEPAD_X             
#define GAME_Y				XINPUT_GAMEPAD_Y           
#define GAME_LEFT_BUMP		XINPUT_GAMEPAD_LEFT_SHOULDER 
#define GAME_RIGHT_BUMP		XINPUT_GAMEPAD_RIGHT_SHOULDER

#define GAME_LEFT_TRIGGER	VK_PAD_LTRIGGER
#define GAME_RIGHT_TRIGGER	VK_PAD_RTRIGGER
#define GAME_LSTICK_PRESS	XINPUT_GAMEPAD_LEFT_THUMB
#define GAME_RSTICK_PRESS	XINPUT_GAMEPAD_RIGHT_THUMB

#define GAME_LSTICK_UP		0x3001
#define GAME_LSTICK_DOWN	0x5001
#define GAME_LSTICK_LEFT	0x6001
#define GAME_LSTICK_RIGHT	0x7001
								 
#define GAME_RSTICK_UP		0x9001
#define GAME_RSTICK_DOWN	0xA001
#define GAME_RSTICK_LEFT	0xB001
#define GAME_RSTICK_RIGHT	0xC001

#define GAME_LSTICK			0xD001
#define GAME_RSTICK			0xE001

#define NUMBER_OF_KEYBOARD_KEY 103
ZG_API extern const char* LIST_KEYBOARD_NAME_[NUMBER_OF_KEYBOARD_KEY];
ZG_API extern const int LIST_KEYBOARD_CODE_[NUMBER_OF_KEYBOARD_KEY];

#define NUMBER_OF_CONTROLLER_KEY 26
ZG_API extern const char* LIST_CONTROLLER_NAME_[NUMBER_OF_CONTROLLER_KEY];
ZG_API extern const int LIST_CONTROLLER_CODE_[NUMBER_OF_CONTROLLER_KEY];

#endif //INPUTKEYDEF_H_
