#include "stdafx.h"
#include "csgo.h"

long __stdcall csgo::hooks::hook_endscene(IDirect3DDevice9* device)
{
	DWORD OldD3DRSCOLORWRITEENABLE;
	device->GetRenderState(D3DRS_COLORWRITEENABLE, &OldD3DRSCOLORWRITEENABLE); // Lets us draw without Net_Graph
	device->SetRenderState(D3DRS_COLORWRITEENABLE, 0xffffffff);
	device->SetRenderState(D3DRS_COLORWRITEENABLE, OldD3DRSCOLORWRITEENABLE);

	auto ofunc = dx9.get_original<EndScene>(indices::EndScene);
	return ofunc(device);
}