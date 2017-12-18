#include "stdafx.h"
#include "csgo.h"

long __stdcall csgo::hooks::hook_reset(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* pPresentationParameters)
{
	auto ofunc = dx9.get_original<Reset>(indices::Reset);
	auto hr = ofunc(device, pPresentationParameters);

	if (hr >= 0)
	{}

	return hr;
}