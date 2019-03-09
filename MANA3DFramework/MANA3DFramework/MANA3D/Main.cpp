#include "Win32App.h"
using namespace MANA3D;

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevious, PSTR szCmdLine, int cmdShow )
{ 
	WPARAM wParam;

	// Create the main app.
	Win32App::CreateApp( hInstance, cmdShow );
	// Enter the main loop of the app.
	Win32App::EnterMainLoop( wParam );
	// Now we exist the main loop so we can destroy the main app.
	Win32App::DestroyApp( hInstance );

	// Return the exit code for the application (Set by the PostQuitMessage() in the WndProc).
	return wParam;
}