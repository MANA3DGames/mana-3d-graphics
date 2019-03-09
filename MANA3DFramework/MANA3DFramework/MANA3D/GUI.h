#ifndef MANA3D_GUI_H
#define MANA3D_GUI_H

#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>

#include <string>
using std::string;

namespace MANA3D
{
	class GUI
	{
	private:
		GUI() {}
		~GUI() {}

	public:

		static HWND hwndLabel;
		static void CreateLabel();
		static void SetLabelText( string str );

		static void CreateButton();

	};
}


#endif