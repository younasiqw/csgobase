#include "stdafx.h"
#include "csgo.h"

namespace csgo
{
	namespace hooks
	{
		vmt dx9;
		vmt mdlrender;

		void init()
		{
			core::msg("hooking vfuncs...");
			dx9.setup(globals::d3d9);
			dx9.hook_index(indices::EndScene, hook_endscene);
			dx9.hook_index(indices::Reset, hook_reset);

			mdlrender.setup(interfaces::model_render);
			mdlrender.hook_index(indices::DrawModelExecute, hook_drawmodelexecute);
			core::msg("done!\n");
		}

		void kill()
		{
			dx9.unhook_all();
			mdlrender.unhook_all();
		}
	}
}