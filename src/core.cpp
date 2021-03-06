#include "stdafx.h"
#include "csgo.h"
#include "utils.h"

namespace csgo
{
	namespace globals
	{
		CGlobalVarsBase* globalvars;
		IDirect3DDevice9* d3d9;
		CClientState* client_state;
		CInput* input;
		C_LocalPlayer localplayer;
	}

	namespace interfaces
	{
		IBaseClientDLL* client;
		IClientEntityList* elist;
		IVEngineClient* engine_client;
		IVModelInfoClient* model_info;
		IVModelRender* model_render;
		IPanel* panel;
		ISurface* surface;
		IMaterialSystem* matsystem;
		IVRenderView* renderview;
	}

	namespace core
	{
		MsgFn msg;
		WarningFn warning;
		bool die = false;

		DWORD WINAPI attach(LPVOID base)
		{
			// Wait at most 10s for the main game modules to be loaded.
			if (utils::waitonmodules(10000, { L"client.dll", L"engine.dll", L"shaderapidx9.dll" }) == WAIT_TIMEOUT)
			{
				// One or more modules were not loaded in time
				return FALSE;
			}

			try
			{
				csgo::core::init();

				while (!die && !GetAsyncKeyState(VK_DELETE))
					Sleep(1000);

				FreeLibraryAndExitThread(static_cast<HMODULE>(base), 1);
			} catch (const std::exception& ex)
			{
				MessageBoxA(NULL, ex.what(), "An error occured", MB_OK);
				FreeLibraryAndExitThread(static_cast<HMODULE>(base), 1);
			}
		}

		BOOL WINAPI detach()
		{
			kill();
			return TRUE;
		}

		typedef void* (*CreateInterfaceFn)(const char *pName, int *pReturnCode);
		CreateInterfaceFn modulefactory(const char* name)
		{
			return reinterpret_cast<CreateInterfaceFn>(GetProcAddress(GetModuleHandleA(name), "CreateInterface"));
		}

		template<typename I>
		inline I createinterface(const char *dll, const char *name)
		{
			return (I) (((void *(*)(const char *, void *))GetProcAddress(GetModuleHandleA(dll), "CreateInterface"))(name, 0));
		}

		template<typename Ex>
		inline Ex getexport(const char *dll, const char *name)
		{
			return (Ex) ((void *(*)(const char *, const char *))GetProcAddress(GetModuleHandleA(dll), name));
		}

		void init()
		{
			msg = getexport<MsgFn>("tier0.dll", "Msg");
			warning = getexport<WarningFn>("tier0.dll", "Warning");

			msg("getting interfaces...");

			interfaces::client          = createinterface<IBaseClientDLL*>("client.dll", "VClient018");
			interfaces::elist           = createinterface<IClientEntityList*>("client.dll", "VClientEntityList003");
			interfaces::engine_client   = createinterface<IVEngineClient*>("engine.dll", "VEngineClient014");
			interfaces::model_info      = createinterface<IVModelInfoClient*>("engine.dll", "VModelInfoClient004");
			interfaces::model_render    = createinterface<IVModelRender*>("engine.dll", "VEngineModel016");
			interfaces::panel	        = createinterface<IPanel*>("vgui2.dll", "VGUI_Panel009");
			interfaces::surface         = createinterface<ISurface*>("vguimatsurface.dll", "VGUI_Surface031");
			interfaces::matsystem       = createinterface<IMaterialSystem*>("materialsystem.dll", "VMaterialSystem080");
			interfaces::renderview      = createinterface<IVRenderView*>("engine.dll", "VEngineRenderView014");

			msg("done!\n");
			msg("scanning for globals...");

			globals::globalvars   = **(CGlobalVarsBase***) (utils::patternscan("client.dll", "A1 ? ? ? ? 5E 8B 40 10") + 1);
			globals::d3d9         = **(IDirect3DDevice9***) (utils::patternscan("shaderapidx9.dll", "A1 ? ? ? ? 50 8B 08 FF 51 0C") + 1);
			globals::client_state = **(CClientState***) (utils::patternscan("engine.dll", "A1 ? ? ? ? 8B 80 ? ? ? ? C3") + 1);
			globals::input        = *(CInput**)(utils::patternscan("client.dll", "B9 ? ? ? ? 8B 40 38 FF D0 84 C0 0F 85") + 1);
			globals::localplayer  = *(C_LocalPlayer*) (utils::patternscan("client.dll", "8B 0D ? ? ? ? 83 FF FF 74 07") + 2);

			msg("done!\n");

			hooks::init();
		}

		void kill()
		{
			msg("killing!!\n");
			hooks::kill();
		}
	}
}
