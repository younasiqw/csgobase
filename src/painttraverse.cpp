#include "stdafx.h"
#include "csgo.h"

void __stdcall csgo::hooks::hook_painttraverse(vgui::VPANEL vpanel, bool forcerepaint, bool allowforce)
{
	static auto panelId = vgui::VPANEL{ 0 };
	static auto ofunc = panel.get_original<painttraverse>(indices::painttraverse);
	const auto panelName = interfaces::panel->GetName(vpanel);

	ofunc(interfaces::panel, vpanel, forcerepaint, allowforce);

	if (!panelId)
	{
		if (!strcmp(panelName, "FocusOverlayPanel"))
		{
			panelId = vpanel;
		}
	} else if (panelId == vpanel)
	{
		// Draw Here
	}
}
