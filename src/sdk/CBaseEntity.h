#pragma once
#include "IClientEntity.h"
#include "netvars.h"
#include "vfunc.h"

class C_BaseEntity : public IClientEntity
{
public:
	NETVAR(m_iTeamNum, int32_t, "CBaseEntity->m_iTeamNum");
	NETVAR(m_nModelIndex, int32_t, "CBaseEntity->m_nModelIndex");
	NETVAR(m_flSimulationTime, float, "CBaseEntity->m_flSimulationTime");
	NETVAR(m_fFlags, int32_t, "CBaseEntity->m_fFlags");
	bool isPlayer() { return vfunc<bool(__thiscall*)(C_BaseEntity*)>(this, 152)(this); };
};

class C_BasePlayer : public C_BaseEntity
{
public:
	NETVAR(m_lifeState, int32_t, "CBasePlayer->m_lifeState");
	bool isAlive() { return m_lifeState() == LIFE_ALIVE; };
};
