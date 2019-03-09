#ifndef MANA3D_GAMEFRAMEWORK_H
#define MANA3D_GAMEFRAMEWORK_H

namespace MANA3D
{
	class Win32App;
	class GameFramework
	{
		friend Win32App;

	private:
		GameFramework() {}
		~GameFramework() {}

		static void OnCreate();
		static void OnStartup();
		static void OnUpdate();
		static void OnDestroy();
	};
}

#endif
