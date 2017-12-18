#include "stdafx.h"
#include "csgo.h"

void __stdcall csgo::hooks::hook_drawmodelexecute(IMatRenderContext * ctx, const DrawModelState_t &state, const ModelRenderInfo_t &info, matrix3x4_t *matrix)
{
	const auto model = info.pModel;

	if (strstr(model->szName, "models/player") != nullptr)
	{
		auto pEntity = static_cast<C_BasePlayer*>(interfaces::elist->GetClientEntity(info.entity_index));

		if (pEntity
			&& pEntity->isPlayer()
			&& !pEntity->isDormantt()
			&& pEntity->isAlive()
			&& pEntity->m_iTeamNum() != globals::localplayer->m_iTeamNum())
		{
			interfaces::renderview->SetColorModulation(160, 10, 0);
		}
	} else if (strstr(model->szName, "v_sleeve") != nullptr)
	{
		auto material = interfaces::matsystem->FindMaterial(model->szName, TEXTURE_GROUP_MODEL);
		if (!material) return;

		material->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, true);
		interfaces::model_render->ForcedMaterialOverride(material);
	}

	auto ofunc = mdlrender.get_original<DrawModelExecute>(indices::DrawModelExecute);
	return ofunc(interfaces::model_render, ctx, state, info, matrix);
}
