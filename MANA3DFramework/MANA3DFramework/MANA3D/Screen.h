#ifndef MANA3D_SCREEN_H
#define MANA3D_SCREEN_H

#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>

#define SCREEN_WIDTH	800;
#define SCREEN_HEIGHT	600;
#define SCREEN_DEPTH	16;
#define FULL_SCREEN		0

namespace MANA3D
{
	class Screen
	{
	private:
		Screen() {}
		~Screen() {}

	public:
		static void GetWidthAndHeight( HWND hWnd, int& width, int& height );
		static void SetFullScreenMode();
		static bool IsFullScreen(void);
		static bool IsFullScreen(HWND hWnd);
	};
}

#endif
