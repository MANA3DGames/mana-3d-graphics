#include "Screen.h"
#include "Win32App.h"
using namespace MANA3D;


void Screen::GetWidthAndHeight( HWND hWnd, int& width, int& height )
{
	RECT    rect;
	GetClientRect( hWnd, &rect );
	width = rect.right - rect.left;
	height = rect.bottom - rect.top;
}

void Screen::SetFullScreenMode()
{
	DEVMODE dmSettings;									// Device Mode variable

	memset( &dmSettings, 0, sizeof( dmSettings ) );			// Makes Sure Memory's Cleared

	// Get current settings -- This function fills in our settings.
	// This makes sure NT and Win98 machines change correctly.
	if ( !EnumDisplaySettings( NULL, ENUM_CURRENT_SETTINGS, &dmSettings ) )
	{
		// Display error message if we couldn't get display settings
		MessageBox(NULL, TEXT( "Could Not Enum Display Settings" ), TEXT( "Error" ), MB_OK );
		return;
	}

	dmSettings.dmPelsWidth	= SCREEN_WIDTH;				// Selected Screen Width
	dmSettings.dmPelsHeight	= SCREEN_HEIGHT;			// Selected Screen Height
	
	// This function actually changes the screen to full screen.
	// CDS_FULLSCREEN gets rid of the start Bar.
	// We always want to get a result from this function to check if we failed.
	int result = ChangeDisplaySettings( &dmSettings, CDS_FULLSCREEN );	

	// Check if we didn't receive a good return message from the function
	if ( result != DISP_CHANGE_SUCCESSFUL )
	{
		// Display the error message and quit the program
		MessageBox(NULL, TEXT( "Display Mode Not Compatible" ), TEXT( "Error" ), MB_OK );
		PostQuitMessage(0);
	}
}

bool Screen::IsFullScreen( void )
{
	return IsFullScreen( Win32App::GetHWND() );
}

bool Screen::IsFullScreen( HWND hWnd )
{
	// Check if fullscreen mode.
	RECT appBounds;
	RECT rc;
	GetWindowRect( GetDesktopWindow(), &rc );
	if ( hWnd != GetDesktopWindow() && hWnd != GetShellWindow() )
	{
		GetWindowRect( hWnd, &appBounds );
		return  rc.bottom == appBounds.bottom &&
				rc.left == appBounds.left &&
				rc.right == appBounds.right &&
				rc.top == appBounds.top;
	}

	return false;
}