#include "stdafx.h"
#include "csgo.h"

BOOL WINAPI DllMain(
	_In_      HINSTANCE hinstDll,
	_In_      DWORD     fdwReason,
	_In_opt_  LPVOID    lpvReserved
)
{
	switch (fdwReason)
	{
		case DLL_PROCESS_ATTACH:
			DisableThreadLibraryCalls(hinstDll);
			CreateThread(nullptr, 0, csgo::core::attach, hinstDll, 0, nullptr);
			return TRUE;
		case DLL_PROCESS_DETACH:
			if (lpvReserved == nullptr)
				return csgo::core::detach();
			return TRUE;
		default:
			return TRUE;
	}
}
