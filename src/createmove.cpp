#include "stdafx.h"
#include "csgo.h"

__declspec(naked) void __stdcall csgo::hooks::hook_createmove_proxy(int sequence, float frametime, bool active)
{
	__asm
	{
		push ebp
		mov  ebp, esp
		push ebx
		lea  ecx, [esp]
		push ecx
		push dword ptr[active]
		push dword ptr[frametime]
		push dword ptr[sequence]
		call csgo::hooks::hook_createmove
		pop  ebx
		pop  ebp
		retn 0Ch
	}
}

//--------------------------------------------------------------------------------

void __stdcall csgo::hooks::hook_createmove(int sequence, float frametime, bool active, bool& sendpacket)
{
	auto ofunc = vclient.get_original<createmove>(indices::createmove);
	ofunc(interfaces::client, sequence, frametime, active);

	auto cmd = globals::input->GetUserCmd(sequence);
	auto verified = globals::input->GetVerifiedCmd(sequence);

	if (!cmd || !cmd->command_number)
		return;

	verified->m_cmd = *cmd;
	verified->m_crc = cmd->GetChecksum();
}