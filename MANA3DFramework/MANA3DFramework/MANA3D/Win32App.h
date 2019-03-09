#ifndef MANA3D_WIN32APP_H
#define MANA3D_WIN32APP_H

#pragma comment( lib, "opengl32.lib" )
#pragma comment( lib, "glu32.lib" )
#pragma comment( lib, "jpeg.lib" )
#pragma comment( lib, "DevIL.lib" )
#pragma comment( lib, "ILU.lib" )
#pragma comment( lib, "ILUT.lib" )
#pragma comment( lib, "libfbxsdk.lib" )

#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <stdio.h>
#include <cwchar>

#define APP_NAME TEXT( "Win32AppClass" );

namespace MANA3D
{
	class Win32App
	{
	public:
		static void CreateApp( HINSTANCE hInstance, int cmdShow );
		static void DestroyApp( HINSTANCE hInstance );
		static void EnterMainLoop( WPARAM &wParam );

		static HWND GetHWND();

	private:
		static void CreateApp32Window( HWND& hWnd, LPCWSTR className, HINSTANCE hInstance, GLint cmdShow );
		static void InitOpenGL( HWND hWnd );
		static bool SetupPixelFormat( HDC hdc );

		static LRESULT CALLBACK WndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
		static void OnDestroy();
		static void OnSetCursor();
		static void OnPaint( HWND hWnd );
		static void OnSize( LPARAM lParam );
		static void OnInput( WPARAM wParam, LPARAM lParam );
		static void OnCreate();

		static void MainLoop( MSG &msg, HWND &hWnd );

	private:
		Win32App() {}
		~Win32App() {}
	};
}

#endif