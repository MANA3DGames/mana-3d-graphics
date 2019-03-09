#include "Win32App.h"
#include "MANA3DEngine.h"
using namespace MANA3D;


void Win32App::CreateApp( HINSTANCE hInstance, int cmdShow )
{
	HWND hWnd;
	LPCWSTR className = APP_NAME;

	// Create the main window.
	CreateApp32Window( hWnd, className, hInstance, cmdShow );

	// Initialization.
	InitOpenGL( hWnd );
	Input::InitRawInput( hWnd );
	InitFBXImporter();
	InitDevIL();

	// Call Startup function.
	GameFramework::OnStartup();
}

void Win32App::EnterMainLoop( WPARAM &wParam )
{
	MSG msg;
	HWND hWnd = GetHWND();
	MainLoop( msg, hWnd );

	// Return the exit code for the application (Set by the PostQuitMessage() in the WndProc).
	wParam = msg.wParam;
}

void Win32App::DestroyApp( HINSTANCE hInstance )
{
	LPCWSTR className = APP_NAME;
	UnregisterClass( className, hInstance );
}


LRESULT CALLBACK Win32App::WndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	// Process received message.
	switch ( msg )
	{
		case WM_CREATE:
			OnCreate();
			break;
		case WM_INPUT:
			OnInput( wParam, lParam );
			break;
		case WM_SIZE:
			OnSize( lParam );
			break; 
		case WM_PAINT:
			OnPaint( hWnd );
			break;
		case WM_SETCURSOR:
			OnSetCursor();
			break;
		case WM_DESTROY:
			OnDestroy();
			break;
		default:
			return DefWindowProc( hWnd, msg, wParam, lParam );
	}

	return 0;
}

void Win32App::MainLoop( MSG& msg, HWND& hWnd )
{
	// Enter the main loop.
	while ( true )
	{
		// Peek at the operating systems message queue and return true for every message it finds.
		if ( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
		{
			// if the message is quit message quit the game and break the main loop.
			if ( msg.message == WM_QUIT )
				break;

			// Translate windows virtual key codes into character messages.
			TranslateMessage( &msg );

			// Send the message that we received from the PeekMessage() to the WndProc.
			DispatchMessage( &msg );
		}
		else
		{
			// Make sure we have only 60 FPS.
			if ( Time::NextFrame( 60 ) )
			{
				// We calculate our frame rate and set our frame interval for time based movement.
				Time::CalculateFrameRate( hWnd );
				GameFramework::OnUpdate();
				GameObject::UpdateAll();
				Camera::RenderScene();

				if ( Debug::IsEnabled )
					Debug::Update();

				// Always clear buffer at the end of the frame.
				Input::ClearInputBuffer();
			}
			else
			{
				// Sleep for 1 millisecond to let other processes work.
				Sleep( 1 );
			}
		}
	}
}

void Win32App::CreateApp32Window( HWND& hWnd, LPCWSTR className, HINSTANCE hInstance, GLint cmdShow )
{
	DWORD dwStyle;
	if( FULL_SCREEN ) 									// Check if we wanted full screen mode
	{													// Set the window properties for full screen mode
		dwStyle = WS_POPUP | 
				  WS_CLIPSIBLINGS | 
				  WS_CLIPCHILDREN;
		Screen::SetFullScreenMode();					// Go to full screen
		ShowCursor( FALSE );							// Hide the cursor
	}
	else												// Assign styles to the window depending on the choice
		dwStyle = WS_OVERLAPPEDWINDOW | 
				  WS_CLIPSIBLINGS | 
				  WS_CLIPCHILDREN;
	
	// Below, we need to adjust the window to it's true requested size.  If we say we
	// want a window that is 800 by 600, that means we want the client rectangle to
	// be that big, not the entire window.  If we go into window mode, it will cut off
	// some of the client rect and stretch the remaining which causes slow down.  We fix this below.

	RECT rWindow;
	rWindow.left	= 0;								// Set Left Value To 0
	rWindow.right	= SCREEN_WIDTH;						// Set Right Value To Requested Width
	rWindow.top	    = 0;								// Set Top Value To 0
	rWindow.bottom	= SCREEN_HEIGHT;					// Set Bottom Value To Requested Height

	AdjustWindowRect( &rWindow, dwStyle, false );		// Adjust Window To True Requested Size



	// Create game window class.
	WNDCLASSEX wndClass;
	wndClass = { 0 };
	wndClass.lpszClassName = className;
	wndClass.hInstance = hInstance;
	wndClass.lpfnWndProc = WndProc;
	wndClass.cbSize = sizeof( wndClass );
	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.hbrBackground = (HBRUSH)GetStockObject( BLACK_BRUSH );
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = sizeof( long );
	wndClass.hIcon = LoadIcon( hInstance, IDI_APPLICATION );
	wndClass.hCursor = LoadCursor( hInstance, IDC_ARROW );
	wndClass.lpszMenuName = NULL;

	// Register game window class.
	RegisterClassEx( &wndClass );

	// Create the actual game window.
	hWnd = CreateWindowEx( NULL, wndClass.lpszClassName, TEXT( "GameWindow" ), dwStyle, 0, 0, 800, 600, NULL, NULL, hInstance, NULL );

	// Display game window.
	ShowWindow( hWnd, cmdShow );

	// Update the game window for the first time so we can see it.
	UpdateWindow( hWnd );
}

void Win32App::InitOpenGL( HWND hWnd )
{
	// Get the handle to the device context for the game window (openGl window).
	HDC hdc = GetDC( hWnd );

	// Set the pixel/information for the opengl window.
	if ( !SetupPixelFormat( hdc ) )
		// There is an error so quit the game.
		PostQuitMessage( 0 );

	// *** IMPORTANT! ***
	// We need now to create a rendering context AFTER we setup the pixel format.
	// A rendering context is different than a device context (hdc), but that is
	// what OpenGL uses to draw/render to.  Because OpenGL can be used on
	// Macs/Linux/Windows/etc.. It has it's on type of rendering context that is
	// The same for EACH operating system, but it piggy backs our HDC information.

	// Create a rendering context from our hdc.
	HGLRC hrc = wglCreateContext( hdc );

	// Make the rendering context we just create the one that we want to use.
	wglMakeCurrent( hdc, hrc );

	// Enable depth testing.
	glEnable( GL_DEPTH_TEST );

	glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );
	glEnable( GL_PERSPECTIVE_CORRECTION_HINT );
}

bool Win32App::SetupPixelFormat( HDC hdc )
{
	// Filling the information for the pixel format struct.
	PIXELFORMATDESCRIPTOR pfd = { 0 };
	pfd.nSize = sizeof( PIXELFORMATDESCRIPTOR );		// Set the size of the structure.
	pfd.nVersion = 1;									// Always set this to 1.
	pfd.dwFlags =	PFD_DRAW_TO_WINDOW 
				  | PFD_SUPPORT_OPENGL 
				  | PFD_DOUBLEBUFFER;					// Pass in the appropriate OpenGL flags.
	pfd.dwLayerMask = PFD_MAIN_PLANE;					// We want the standard mask (this is ignored anyway).
	pfd.iPixelType = PFD_TYPE_RGBA;						// We want RGB and Alpha pixel type.
	pfd.cColorBits = SCREEN_DEPTH;						// Screen depth for the color bits.
	pfd.cDepthBits = SCREEN_DEPTH;						// Depth bits is ignored for RGBA, but we do it anyway.
	pfd.cAccumBits = 0;									// No special bitplanes needed.
	pfd.cStencilBits = 0;								// We desire no stencil bits.

	int pixelformat;
	// This gets us a pixel format that best matches the one passed in from the device.
	if ( ( pixelformat = ChoosePixelFormat( hdc, &pfd ) ) == FALSE )
	{
		MessageBox( NULL, TEXT( "ChoosePixelFormat failed" ), TEXT( "Error" ), MB_OK );
		return FALSE;
	}

	// This sets the pixel format that we extracted from above
	if ( SetPixelFormat( hdc, pixelformat, &pfd ) == FALSE )
	{
		MessageBox( NULL, TEXT( "SetPixelFormat failed" ), TEXT( "Error" ), MB_OK );
		return FALSE;
	}

	return TRUE;
}


void Win32App::OnCreate()
{
	GameFramework::OnCreate();
}

void Win32App::OnDestroy()
{
	// Call custom Destroy function.
	GameFramework::OnDestroy();
	// Free memory.
	GameObject::DestroyAll();
	Material::DestroyAll();
	Texture::DestroyAll();
	Mesh::DestroyAll();


	HINSTANCE hInstance = GetModuleHandle( NULL );
	HGLRC hrc = wglGetCurrentContext();
	HDC hdc = wglGetCurrentDC();
	HWND hWnd = GetHWND();

	if ( hrc )											
	{
		wglMakeCurrent( NULL, NULL );					// This frees our rendering memory and sets everything back to normal
		wglDeleteContext( hrc );						// Delete our OpenGL Rendering Context	
	}
	
	if ( hdc ) 
		ReleaseDC( hWnd, hdc );							// Release our HDC from memory
		
	if ( Screen::IsFullScreen( hWnd ) )
	{
		ChangeDisplaySettings( NULL, 0 );				// Switch Back To The Desktop
		ShowCursor( TRUE );	
	}

	LPCWSTR className = APP_NAME;
	UnregisterClass( className, hInstance );			// Free the window class

	// PostQuitMessage() sends the WM_QUIT message.  The parameter we pass is the exit code we want to report to the operating system.  
	// By passing zero we're saying "Zero errors occurred in the application."
	PostQuitMessage( 0 );
}

void Win32App::OnSetCursor()
{
	SetCursor( LoadCursor( NULL, IDC_ARROW ) );
}

void Win32App::OnPaint( HWND hWnd )
{
	HDC     hdc;
	PAINTSTRUCT ps;
	hdc = BeginPaint( hWnd, &ps );
	EndPaint( hWnd, &ps );
}

void Win32App::OnSize( LPARAM lParam )
{
	if( !Screen::IsFullScreen() )
		Camera::UpdateGLView( LOWORD(lParam), HIWORD(lParam) );
}

void Win32App::OnInput( WPARAM wParam, LPARAM lParam )
{
	Input::RegisterInput( wParam, lParam );
}


HWND Win32App::GetHWND()
{
	HWND hWnd = GetActiveWindow();
	HWND temp = GetParent( hWnd );
	while ( temp != NULL )
	{
		hWnd = temp;
		temp = GetParent( hWnd );
	}
	return hWnd;
}
