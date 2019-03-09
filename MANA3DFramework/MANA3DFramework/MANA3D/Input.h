#ifndef MANA3D_INPUT_H
#define MANA3D_INPUT_H

#include <Windows.h>
//#include <list>
//using std::list;
#include <set>
using std::set;

namespace MANA3D
{
	enum Key
	{
		LEFT_MOUSE_BUTTON		= 0x01,			// [VK_LBUTTON] Left mouse button.
		RIGHT_MOUSE_BUTTON		= 0x02,			// [VK_RBUTTON] Right mouse button.
		CANCEL					= 0x03,			// [VK_CANCEL] Control - break processing.
		MIDDEL_MOUSE_BUTTON		= 0x04,			// [VK_MBUTTON] Middle mouse button(three - button mouse).
		X1_MOUSE_BUTTON			= 0x05,			// [VK_XBUTTON1] X1 mouse button.
		X2_MOUSE_BUTTON			= 0x06,			// [VK_XBUTTON2] X2 mouse button.
		// 0x07 Undefined
		BACKSPACE				= 0x08,			// [VK_BACK BACKSPACE] key.
		TAB						= 0x09,			// [VK_TAB] Tab Key.
		// 0x0A-0B Reserved
		CLEAR					= 0x0C,			// [VK_CLEAR] CLEAR key.
		ENTER					= 0x0D,			// [VK_RETURN] Enter key.
		// 0x0E Undefined
		// 0x0F Undefined
		SHIFT					= 0x10,			// [VK_SHIFT] Shift key.
		CTRL					= 0x11,			// [VK_CONTROL] CTRL key.
		ALT						= 0x12,			// [VK_MENU] Alt key.
		PAUSE					= 0x13,			// [VK_PAUSE] PAUSE key.
		CAPS_LOCK				= 0x14,			// [VK_CAPITAL] CAPS LOCK key.
		// IME
		// IME
		// IME
		// Undefined
		// IME
		// IME
		// IME
		// IME
		// Undefined
		ESC						= 0x1B,			// [VK_ESCAPE] ESC key.
		// IME
		// IME
		// IME
		// IME
		SPACE					= 0x20,			// [VK_SPACE] SPACEBAR.
		PAGE_UP					= 0x21,			// [VK_PRIOR] PAGE UP key.
		PAGE_DOWN				= 0x22,			// [VK_NEXT] PAGE DOWN key.
		END						= 0x23,			// [VK_END] END key.
		HOME					= 0x24,			// [VK_HOME] HOME key.
		LEFT_ARROW				= 0x25,			// [VK_LEFT] LEFT ARROW key.
		UP_ARROW				= 0x26,			// [VK_UP] UP ARROW key.
		RIGHT_ARROW				= 0x27,			// [VK_RIGHT] RIGHT ARROW key.
		DOWN_ARROW				= 0x28,			// [VK_DOWN] DOWN ARROW key.
		SELECT					= 0x29,			// [VK_SELECT] SELECT key.
		PRINT					= 0x2A,			// [VK_PRINT] PRINT key.
		EXECUTE					= 0x2B,			// [VK_EXECUTE] EXECUTE key.
		PRINT_SCREEN			= 0x2C,			// [VK_SNAPSHOT] PRINT SCREEN key.
		INSERT					= 0x2D,			// [VK_INSERT] INS key.
		DEL						= 0x2E,			// [VK_DELETE] DEL key.
		HELP					= 0x2F,			// [VK_HELP] HELP key.

		NUM0					= 0x30,			// 0 key
		NUM1					= 0x31,			// 1 key
		NUM2					= 0x32,			// 2 key
		NUM3					= 0x33,			// 3 key
		NUM4					= 0x34,			// 4 key
		NUM5					= 0x35,			// 5 key
		NUM6					= 0x36,			// 6 key
		NUM7					= 0x37,			// 7 key
		NUM8					= 0x38,			// 8 key
		NUM9					= 0x39,			// 9 key

		// Undefined
		// Undefined
		// Undefined
		// Undefined
		// Undefined
		// Undefined

		A						= 0x41,			// A key
		B						= 0x42,			// B key
		C						= 0x43,			// C key
		D						= 0x44,			// D key
		E						= 0x45,			// E key
		F						= 0x46,			// F key
		G						= 0x47,			// G key
		H						= 0x48,			// H key
		I						= 0x49,			// I key
		J						= 0x4A,			// J key
		K						= 0x4B,			// K key
		L						= 0x4C,			// L key
		M						= 0x4D,			// M key
		N						= 0x4E,			// N key
		O						= 0x4F,			// O key
		P						= 0x50,			// P key
		Q						= 0x51,			// Q key
		R						= 0x52,			// R key
		S						= 0x53,			// S key
		T						= 0x54,			// T key
		U						= 0x55,			// U key
		V						= 0x56,			// V key
		W						= 0x57,			// W key
		X						= 0x58,			// X key
		Y						= 0x59,			// Y key
		Z						= 0x5A,			// Z key

		// Unknown
		// Unknown
		// Unknown
		// 0x5E Reserved
		// Unknown

		NUMPAD_0				= 0x60,			// [VK_NUMPAD0] Numeric keypad 0 key.
		NUMPAD_1				= 0x61,			// [VK_NUMPAD1] Numeric keypad 1 key.
		NUMPAD_2				= 0x62,			// [VK_NUMPAD2] Numeric keypad 2 key.
		NUMPAD_3				= 0x63,			// [VK_NUMPAD3] Numeric keypad 3 key.
		NUMPAD_4				= 0x64,			// [VK_NUMPAD4] Numeric keypad 4 key.
		NUMPAD_5				= 0x65,			// [VK_NUMPAD5] Numeric keypad 5 key.
		NUMPAD_6				= 0x66,			// [VK_NUMPAD6] Numeric keypad 6 key.
		NUMPAD_7				= 0x67,			// [VK_NUMPAD7] Numeric keypad 7 key.
		NUMPAD_8				= 0x68,			// [VK_NUMPAD8] Numeric keypad 8 key.
		NUMPAD_9				= 0x69,			// [VK_NUMPAD9] Numeric keypad 9 key.
		NUMPAD_MULTIPLY			= 0x6A,			// [VK_MULTIPLY] Multiply key.
		NUMPAD_ADD				= 0x6B,			// [VK_ADD] Add key.
		NUMPAD_SEPARATOR		= 0x6C,			// [VK_SEPARATOR] Separator key.
		NUMPAD_SUBTRACT			= 0x6D,			// [VK_SUBTRACT] Subtract key.
		NUMPAD_DECIMAL			= 0x6E,			// [VK_DECIMAL] Decimal key.
		NUMPAD_DIVIDE			= 0x6F,			// [VK_DIVIDE] Divide key.

		F1						= 0x70,			// [VK_F1] F1 key.
		F2						= 0x71,			// [VK_F2] F2 key.
		F3						= 0x72,			// [VK_F3] F3 key.
		F4						= 0x73,			// [VK_F4] F4 key.
		F5						= 0x74,			// [VK_F5] F5 key.
		F6						= 0x75,			// [VK_F6] F6 key.
		F7						= 0x76,			// [VK_F7] F7 key.
		F8						= 0x77,			// [VK_F8] F8 key.
		F9						= 0x78,			// [VK_F9] F9 key.
		F10						= 0x79,			// [VK_F10] F10 key.
		F11						= 0x7A,			// [VK_F11] F11 key.
		F12						= 0x7B,			// [VK_F12] F12 key.
		F13						= 0x7C,			// [VK_F13] F13 key.
		F14						= 0x7D,			// [VK_F14] F14 key.
		F15						= 0x7E,			// [VK_F15] F15 key.
		F16						= 0x7F,			// [VK_F16] F16 key.
		F17						= 0x80,			// [VK_F17] F17 key.
		F18						= 0x81,			// [VK_F18] F18 key.
		F19						= 0x82,			// [VK_F19] F19 key.
		F20						= 0x83,			// [VK_F20] F20 key.
		F21						= 0x84,			// [VK_F21] F21 key.
		F22						= 0x85,			// [VK_F22] F22 key.
		F23						= 0x86,			// [VK_F23] F23 key.
		F24						= 0x87,			// [VK_F24] F24 key.

		// Unassigned
		// Unassigned
		// Unassigned
		// Unassigned
		// Unassigned
		// Unassigned
		// Unassigned
		// Unassigned
		// NUM LOCK key
		// SCROLL LOCK key
		// 0x92-96 OEM specific
		// 0x97-9F Unassigned

		LEFT_SHIFT				= 0xA0,			// [VK_LSHIFT] Left SHIFT key.
		RIGHT_SHIFT				= 0xA1,			// [VK_RSHIFT] Right SHIFT key.
		LEFT_CTRL				= 0xA2,			// [VK_LCONTROL] Left CONTROL key.
		RIGHT_CTRL				= 0xA3,			// [VK_RCONTROL] Right CONTROL key.
		LEFT_ALT				= 0xA4,			// [VK_LMENU] Left MENU key.
		RIGHT_ALT				= 0xA5,			// [VK_RMENU] Right MENU key.

		OEM_1					= 0xBA,			// [VK_OEM_1] For the US standard keyboard, the ';:' key.
		OEM_PLUS				= 0xBB,			// [VK_OEM_PLUS] For any country/region, the '+' key.
		OEM_COMMA				= 0xBC,			// [VK_OEM_COMMA] For any country/region, the ',' key.
		OEM_MINUS				= 0xBD,			// [VK_OEM_MINUS] For any country/region, the '-' key.
		OEM_PERIOD				= 0xBE,			// [VK_OEM_PERIOD] For any country/region, the '.' key.
		
		OEM_4					= 0xDB,			// [VK_OEM_4] For the US standard keyboard, the '[{' key.
		OEM_5					= 0xDC,			// [VK_OEM_5] For the US standard keyboard, the '\|' key.
		OEM_6					= 0xDD,			// [VK_OEM_6] For the US standard keyboard, the ']}' key.
		OEM_7					= 0xDE,			// [VK_OEM_7] For the US standard keyboard, the 'single-quote/double-quote' key.
		OEM_8					= 0xDF,			// [VK_OEM_8] Used for miscellaneous characters; it can vary by keyboard.
	};

	/*
	struct KeyState
	{
		Key key;
		bool wasUp;
	};
	*/

	class Win32App;
	class Input
	{
		friend Win32App;

	private:
		//static list<KeyState> inputBuffer;
		static set<Key> _inputDownBuffer;
		static set<Key> _inputUpBuffer;

	private:
		static void InitRawInput( HWND hWnd );
		static void RegisterInput( WPARAM wParam, LPARAM lParam );
		static void ClearInputBuffer();

	public:
		static bool KeyDown( Key key );
		static bool KeyPressed( Key key );
		static bool KeyUp( Key key );
	};
}

#endif
