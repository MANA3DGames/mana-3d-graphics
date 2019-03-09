#include "Debug.h"
#include "Input.h"
using namespace MANA3D;

#include "Win32App.h"


bool Debug::IsEnabled;
bool Debug::IsDisplayed;
HWND Debug::hWndDebug;
MString Debug::mStr;

bool Debug::keyIsNotUp = false;


void Debug::Enable()
{
	IsEnabled = true;

	if ( hWndDebug == NULL )
		Init();
}

void Debug::Disable()
{
	IsEnabled = false;
	HideDebug();
}



void Debug::Init()
{
	HWND hWnd = Win32App::GetHWND();
	hWndDebug = CreateWindowA(	"STATIC", "",
								WS_CHILD, // WS_VISIBLE | SS_RIGHT,
								10, 10, 300, 100, 
								hWnd, NULL, NULL, NULL );
}

void Debug::Update()
{
	if ( Input::KeyPressed( Key::TAB ) )
	{
		if ( IsDisplayed )
			HideDebug();
		else
			ShowDebug();
	}
}

void Debug::ShowDebug()
{
	SetWindowTextA( hWndDebug, mStr.GetCStr() );
	ShowWindow( hWndDebug, true );
	IsDisplayed = true;
}

void Debug::HideDebug()
{
	ShowWindow( hWndDebug, false );
	IsDisplayed = false;
}

