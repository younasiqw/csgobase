#pragma once
#include "vfunc.h"

class ClientClass;

class IBaseClientDLL
{
public:
	ClientClass* GetAllClasses() {
		return vfunc<ClientClass*(__thiscall *)(IBaseClientDLL*)>(this, 8)(this);
	}
};