#pragma once

#include "sdk.h"
#include "vmt.h"

namespace csgo
{
	namespace core
	{
		DWORD WINAPI attach(LPVOID base);
		BOOL WINAPI detach();
		extern bool die;
		void init();
		void kill();

		typedef void(__cdecl *MsgFn)(const char *, ...);
		typedef void(__cdecl *WarningFn)(const char *, ...);
		extern MsgFn msg;
		extern WarningFn warning;
	}

	namespace hooks
	{
		namespace indices
		{
			constexpr auto EndScene = 42;
			constexpr auto Reset = 16;
			constexpr auto DrawModelExecute = 21;
		};

		extern vmt dx9;
		using EndScene = long(__stdcall *)(IDirect3DDevice9*);
		long __stdcall hook_endscene(IDirect3DDevice9* device);
		using Reset = long(__stdcall *)(IDirect3DDevice9*, D3DPRESENT_PARAMETERS*);
		long __stdcall hook_reset(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* pPresentationParameters);

		extern vmt mdlrender;
		using DrawModelExecute = void(__thiscall*)(IVModelRender*, IMatRenderContext*, const DrawModelState_t&, const ModelRenderInfo_t&, matrix3x4_t*);
		void __stdcall hook_drawmodelexecute(IMatRenderContext * ctx, const DrawModelState_t &state, const ModelRenderInfo_t &info, matrix3x4_t *matrix);

		void init();
		void kill();
	}

	class C_LocalPlayer
	{
		friend bool operator==(const C_LocalPlayer& lhs, void* rhs);
	public:
		C_LocalPlayer() : m_local(nullptr) {}

		operator bool() const { return *m_local != nullptr; }
		operator C_BasePlayer*() const { return *m_local; }

		C_BasePlayer* operator->() { return *m_local; }

	private:
		C_BasePlayer * * m_local;
	};

	namespace globals
	{
		extern CGlobalVarsBase* globalvars;
		extern IDirect3DDevice9* d3d9;
		extern CClientState* client_state;
		extern C_LocalPlayer localplayer;
	}

	namespace interfaces
	{
		extern IBaseClientDLL* client;
		extern IClientEntityList* elist;
		extern IVEngineClient* engine_client;
		extern IVModelInfoClient* model_info;
		extern IVModelRender* model_render;
		extern ISurface* surface;
		extern IMaterialSystem* matsystem;
		extern IVRenderView* renderview;
	}
}
