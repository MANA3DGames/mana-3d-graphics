#include "GUI.h"
#include "MString.h"
#include "Win32App.h"
using namespace MANA3D;


void GUI::CreateButton()
{
	HWND hWnd = Win32App::GetHWND();
	HWND hwndButton = CreateWindow( L"BUTTON",												// Predefined class; Unicode assumed 
									L"OK",													// Button text 
									WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
									10,														// x position 
									10,														// y position 
									100,													// Button width
									50,														// Button height
									hWnd,													// Parent window
									NULL,													// No menu.
									// (HMENU) ID_BUTTON1, // 0x8801
									(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),			// ID
									NULL);													// Pointer not needed.

	//if ( msg.hwnd == hwndButton ) 
	//{
	//	switch (msg.message) 
	//	{
	//	case WM_LBUTTONDOWN:
	//		// start sound
	//		continue;
	//	case WM_LBUTTONUP:
	//		// stop sound
	//		continue;
	//	}
	//	// Allow other messages to fall through
	//}


	//case WM_COMMAND:
	//	switch (wParam)
	//	{
	//	case BTN_LEFT:
	//		//		handle your button click here
	//	}



	SetWindowText( hwndButton, L"User:" );
	
}


HWND GUI::hwndLabel;

void GUI::CreateLabel()
{
	HWND hWnd = Win32App::GetHWND();
	hwndLabel = CreateWindow(	L"STATIC", L"Label",
								WS_VISIBLE | WS_CHILD,// | SS_RIGHT,
								10, 10, 200, 100, hWnd, NULL, NULL, NULL );

}

void GUI::SetLabelText( string str )
{
	SetWindowText( hwndLabel, MString::ToLPWSTR( str.c_str() ) );
}