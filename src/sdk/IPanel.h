#pragma once

#include "vfunc.h"

class IPanel
{
public:
	const char *GetName(unsigned int vguiPanel)
	{
		typedef const char *(__thiscall* tGetName)(void*, unsigned int);
		return vfunc<tGetName>(this, 36)(this, vguiPanel);
	}
#ifdef GetClassName
#undef GetClassName
#endif
	const char *GetClassName(unsigned int vguiPanel)
	{
		typedef const char *(__thiscall* tGetClassName)(void*, unsigned int);
		return vfunc<tGetClassName>(this, 37)(this, vguiPanel);
	}
};

