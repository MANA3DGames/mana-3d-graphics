#include "Input.h"
using namespace MANA3D;


//list<KeyState> Input::inputBuffer;
set<Key> Input::_inputDownBuffer;
set<Key> Input::_inputUpBuffer;


void Input::InitRawInput( HWND hWnd )
{
	RAWINPUTDEVICE device;
	device.usUsagePage = 0x01;
	device.usUsage = 0x06;
	device.dwFlags = RIDEV_NOLEGACY;        // do not generate legacy messages such as WM_KEYDOWN
	device.hwndTarget = hWnd;
	RegisterRawInputDevices( &device, 1, sizeof(device) );
}

void Input::RegisterInput( WPARAM wParam, LPARAM lParam )
{
	char buffer[sizeof(RAWINPUT)] = {};
	UINT size = sizeof(RAWINPUT);
	GetRawInputData( reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, buffer, &size, sizeof(RAWINPUTHEADER) );
 
	// extract keyboard raw input data
	RAWINPUT* raw = reinterpret_cast<RAWINPUT*>( buffer );
	if ( raw->header.dwType == RIM_TYPEKEYBOARD )
	{
		const RAWKEYBOARD& rawKB = raw->data.keyboard;
		
		UINT virtualKey = rawKB.VKey;
		UINT scanCode = rawKB.MakeCode;
		UINT flags = rawKB.Flags;

		if ( virtualKey == 255 )
		{
			// Discard "fake keys" which are part of an escaped sequence
			return;
		}
		else if ( virtualKey == VK_SHIFT )
		{
			// Correct left-hand / right-hand SHIFT
			virtualKey = MapVirtualKey( scanCode, MAPVK_VSC_TO_VK_EX );
		}
		else if ( virtualKey == VK_NUMLOCK )
		{
			// Correct PAUSE/BREAK and NUM LOCK silliness, and set the extended bit
			scanCode = ( MapVirtualKey( virtualKey, MAPVK_VK_TO_VSC ) | 0x100 );
		}

		// e0 and e1 are escape sequences used for certain special keys, such as PRINT and PAUSE/BREAK.
		// see http://www.win.tue.nl/~aeb/linux/kbd/scancodes-1.html
		const bool isE0 = ( ( flags & RI_KEY_E0 ) != 0 );
		const bool isE1 = ( ( flags & RI_KEY_E1 ) != 0 );

		if ( isE1 )
		{
			// For escaped sequences, turn the virtual key into the correct scan code using MapVirtualKey.
			// however, MapVirtualKey is unable to map VK_PAUSE (this is a known bug), hence we map that by hand.
			if ( virtualKey == VK_PAUSE )
				scanCode = 0x45;
			else
				scanCode = MapVirtualKey( virtualKey, MAPVK_VK_TO_VSC );
		}

		switch ( virtualKey )
		{
			// right-hand CONTROL and ALT have their e0 bit set
		case VK_CONTROL:
			if ( isE0 )
				virtualKey = Key::RIGHT_CTRL;
			else
				virtualKey = Key::LEFT_CTRL;
			break;

		case VK_MENU:
			if ( isE0 )
				virtualKey = Key::RIGHT_ALT;
			else
				virtualKey = Key::LEFT_ALT;
			break;

			// NUMPAD ENTER has its e0 bit set
			//case VK_RETURN:
			//	if ( isE0 )
			//		virtualKey = Keys::NUMPAD_ENTER;
			//	break;

			// the standard INSERT, DELETE, HOME, END, PRIOR and NEXT keys will always have their e0 bit set, but the
			// corresponding keys on the NUMPAD will not.
		case VK_INSERT:
			if ( !isE0 )
				virtualKey = Key::NUMPAD_0;
			break;

		case VK_DELETE:
			if ( !isE0 )
				virtualKey = Key::NUMPAD_DECIMAL;
			break;

		case VK_HOME:
			if ( !isE0 )
				virtualKey = Key::NUMPAD_7;
			break;

		case VK_END:
			if ( !isE0 )
				virtualKey = Key::NUMPAD_1;
			break;

		case VK_PRIOR:
			if ( !isE0 )
				virtualKey = Key::NUMPAD_9;
			break;

		case VK_NEXT:
			if ( !isE0 )
				virtualKey = Key::NUMPAD_3;
			break;

			// the standard arrow keys will always have their e0 bit set, but the
			// corresponding keys on the NUMPAD will not.
		case VK_LEFT:
			if ( !isE0 )
				virtualKey = Key::NUMPAD_4;
			break;

		case VK_RIGHT:
			if ( !isE0 )
				virtualKey = Key::NUMPAD_6;
			break;

		case VK_UP:
			if ( !isE0 )
				virtualKey = Key::NUMPAD_8;
			break;

		case VK_DOWN:
			if ( !isE0 )
				virtualKey = Key::NUMPAD_2;
			break;

			// NUMPAD 5 doesn't have its e0 bit set
		case VK_CLEAR:
			if ( !isE0 )
				virtualKey = Key::NUMPAD_5;
			break;
		}

		// A key can either produce a "make" or "break" scancode. this is used to differentiate between down-presses and releases
		// see http://www.win.tue.nl/~aeb/linux/kbd/scancodes-1.html
		const bool wasUp = ( ( flags & RI_KEY_BREAK ) != 0 );

		// Getting a human-readable string
		UINT key = ( scanCode << 16 ) | ( isE0 << 24 );
		//char buffer[512] = {};
		LPWSTR buffer = {};
		GetKeyNameText( (LONG)key, buffer, 512 );


		/*
		KeyState state;
		state.key = (Key)virtualKey;
		state.wasUp = wasUp;
		inputBuffer.push_back( state );
		*/


		Key ckey = (Key)virtualKey;
		if ( wasUp )
		{
			std::set<Key>::iterator it = _inputUpBuffer.find( ckey );
			if ( it == _inputUpBuffer.end() )
				_inputUpBuffer.insert( ckey );
		}
		else
		{
			std::set<Key>::iterator it = _inputDownBuffer.find( ckey );
			if ( it == _inputDownBuffer.end() )
				_inputDownBuffer.insert( ckey );
		}

	}
}

void Input::ClearInputBuffer()
{
	_inputDownBuffer.clear();
	_inputUpBuffer.clear();
}


bool Input::KeyPressed( Key key )
{
	for ( std::set<Key>::iterator it = _inputDownBuffer.begin(); it != _inputDownBuffer.end(); ++it )
	{
		if ( *it == key )
			return true;
	}

	return false;
}

bool Input::KeyUp( Key key )
{
	for ( std::set<Key>::iterator it = _inputUpBuffer.begin(); it != _inputUpBuffer.end(); ++it )
	{
		if ( *it == key )
			return true;
	}

	return false;
}

bool Input::KeyDown( Key key )
{
	// Test high bit - if set, key was down when GetAsyncKeyState was called.
	return ( 0x8000 & GetAsyncKeyState( key ) ); // ( 1 << 16 )
}
