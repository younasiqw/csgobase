#include "stdafx.h"
#include "csgo.h"

namespace csgo
{
	namespace hooks
	{
		vmt dx9;
		vmt mdlrender;
		vmt vclient;

		void init()
		{
			core::msg("hooking vfuncs...");
			dx9.setup(globals::d3d9);
			dx9.hook_index(indices::endscene, hook_endscene);
			dx9.hook_index(indices::reset, hook_reset);

			mdlrender.setup(interfaces::model_render);
			mdlrender.hook_index(indices::drawmodelexecute, hook_drawmodelexecute);

			vclient.setup(interfaces::client);
			vclient.hook_index(indices::createmove, hook_createmove_proxy);
			core::msg("done!\n");
		}

		void kill()
		{
			dx9.unhook_all();
			mdlrender.unhook_all();
			vclient.unhook_all();
		}
	}
}