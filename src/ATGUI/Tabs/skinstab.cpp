#include "skinstab.h"

#include "../../settings.h"
#include "../../Utils/xorstring.h"
#include "../../ImGUI/imgui_internal.h"
#include "../../Utils/skins.h"
#include "../../Hacks/skinchanger.h"

#pragma GCC diagnostic ignored "-Wformat-security"

void SplitSkins()
{
	static int modelCT = 1;
	static int modelSkinCT = 1;
	static float skinWearCT = 0.0001f;
	static int skinSeedCT = -1;
	static int skinStatTrakCT = -1;
	static char skinNameCT[18];
	static char filterModelsCT[18];
	static char filterModelSkinsCT[18];
	static int modelT = 1;
	static int modelSkinT = 1;
	static float skinWearT = 0.0001f;
	static int skinSeedT = -1;
	static int skinStatTrakT = -1;
	static char skinNameT[18];
	static char filterModelsT[18];
	static char filterModelSkinsT[18];

	ImGui::Columns(2, nullptr, false);

	ImGui::Text(XORSTR("Counter Terrorist Skins"));
	ImGui::NextColumn();

	ImGui::Text(XORSTR("Terrorist Skins"));
	ImGui::Columns();
	ImGui::Separator();

	ImGui::Columns(4, nullptr, false);

	ImGui::Text(XORSTR("Model"));
	ImGui::NextColumn();

	ImGui::Text(XORSTR("Skin"));
	ImGui::NextColumn();

	ImGui::Text(XORSTR("Model"));
	ImGui::NextColumn();

	ImGui::Text(XORSTR("Skin"));
	ImGui::Columns();

	ImGui::Columns(4, nullptr, false);

	ImGui::PushItemWidth(-1);
	ImGui::InputText(XORSTR("##filterModelsCT"), filterModelsCT, IM_ARRAYSIZE(filterModelsCT));
	ImGui::PopItemWidth();
	ImGui::NextColumn();

	ImGui::PushItemWidth(-1);
	ImGui::InputText(XORSTR("##filterModelSkinsCT"), filterModelSkinsCT, IM_ARRAYSIZE(filterModelSkinsCT));
	ImGui::PopItemWidth();
	ImGui::NextColumn();

	ImGui::PushItemWidth(-1);
	ImGui::InputText(XORSTR("##filterModelsT"), filterModelsT, IM_ARRAYSIZE(filterModelsT));
	ImGui::PopItemWidth();
	ImGui::NextColumn();

	ImGui::PushItemWidth(-1);
	ImGui::InputText(XORSTR("##filterModelSkinsT"), filterModelSkinsT, IM_ARRAYSIZE(filterModelSkinsT));
	ImGui::PopItemWidth();
	ImGui::Columns();

	ImGui::Columns(4, nullptr, false);

	ImGui::GetCurrentContext()->FontBaseSize = 14.0f;
	ImGui::ListBoxHeader(XORSTR("##modelsCT"), ImVec2(-1, 300));
	ImGui::Columns(2, nullptr, false);
	for (auto model : ItemDefinitionIndexMap) {
		if (!(Util::Contains(Util::ToLower(std::string(filterModelsCT)),
		                     Util::ToLower(Util::Items::GetItemDisplayName(model.first))) ||
		      Util::Contains(Util::ToLower(std::string(filterModelsCT)),
		                     Util::ToLower(Util::Items::GetItemEntityName(model.first))))) {
			continue;
		}
		if (model.first == ItemDefinitionIndex::INVALID || Util::Items::IsUtility(model.first) ||
		    model.first == ItemDefinitionIndex::GLOVE_CT_SIDE || model.first == ItemDefinitionIndex::GLOVE_T_SIDE ||
		    Util::Items::IsTWeapon(model.first) || model.first == ItemDefinitionIndex::WEAPON_KNIFE ||
		    model.first == ItemDefinitionIndex::WEAPON_KNIFE_T) {
			continue;
		}
		const bool item_selected = ((int) model.first == modelCT);
		ImGui::PushID((int) model.first);
		if (ImGui::Selectable(Util::Items::GetItemDisplayName(model.first).c_str(), item_selected,
		                      ImGuiSelectableFlags_SpanAllColumns)) {
			modelCT = (int) model.first;
			if (Settings::Skinchanger::skinsCT.find((ItemDefinitionIndex) modelCT) !=
			    Settings::Skinchanger::skinsCT.end()) {
				modelSkinCT = Settings::Skinchanger::skinsCT.at(model.first).fallbackPaintKit;
				skinWearCT = Settings::Skinchanger::skinsCT.at(model.first).fallbackWear;
				skinSeedCT = Settings::Skinchanger::skinsCT.at(model.first).fallbackSeed;
				skinStatTrakCT = Settings::Skinchanger::skinsCT.at(model.first).fallbackStatTrak;
				std::fill(std::begin(skinNameCT), std::end(skinNameCT), 0);
				std::copy(std::begin(Settings::Skinchanger::skinsCT.at(model.first).customName),
				          std::end(Settings::Skinchanger::skinsCT.at(model.first).customName), std::begin(skinNameCT));
			}
		}
		ImGui::NextColumn();

		ImGui::Text("%s", Util::Items::GetItemEntityName(model.first).c_str());
		ImGui::NextColumn();

		ImGui::Separator();
		ImGui::PopID();
	}
	ImGui::ListBoxFooter();
	ImGui::NextColumn();

	ImGui::ListBoxHeader(XORSTR("##modelSkinsCT"), ImVec2(-1, 300));
	ImGui::Columns(2, nullptr, false);;
	for (auto skin : itemSkins) {
		if (skin.second.paintName != nullptr ? !(Util::Contains(Util::ToLower(std::string(filterModelSkinsCT)),
		                                                        Util::ToLower(skin.second.displayName)) ||
		                                         Util::Contains(Util::ToLower(std::string(filterModelSkinsCT)),
		                                                        skin.second.paintName)) : !Util::Contains(
				Util::ToLower(std::string(filterModelSkinsCT)), Util::ToLower(skin.second.displayName))) {
			continue;
		}
		if (Util::Items::IsGlove((ItemDefinitionIndex) modelCT) && skin.first < 10064) {
			continue;
		}
		const bool item_selected = (skin.first == modelSkinCT);
		ImGui::PushID(skin.first);
		if (ImGui::Selectable(skin.first == -1 ? skin.second.displayName : skin.second.displayName, item_selected,
		                      ImGuiSelectableFlags_SpanAllColumns)) {
			modelSkinCT = skin.first;
		}
		ImGui::NextColumn();

		ImGui::Text("%s", skin.second.paintName != nullptr ? skin.second.paintName : "");
		ImGui::NextColumn();

		ImGui::Separator();
		ImGui::PopID();
	}
	ImGui::ListBoxFooter();
	ImGui::NextColumn();

	ImGui::ListBoxHeader(XORSTR("##modelsT"), ImVec2(-1, 300));
	ImGui::Columns(2, nullptr, false);;
	for (auto model : ItemDefinitionIndexMap) {
		if (!(Util::Contains(Util::ToLower(std::string(filterModelsT)),
		                     Util::ToLower(Util::Items::GetItemDisplayName(model.first))) ||
		      Util::Contains(Util::ToLower(std::string(filterModelsT)),
		                     Util::ToLower(Util::Items::GetItemEntityName(model.first))))) {
			continue;
		}
		if (model.first == ItemDefinitionIndex::INVALID
		    || Util::Items::IsUtility(model.first)
		    || model.first == ItemDefinitionIndex::GLOVE_CT_SIDE
		    || model.first == ItemDefinitionIndex::GLOVE_T_SIDE
		    || Util::Items::IsCTWeapon(model.first)
		    || model.first == ItemDefinitionIndex::WEAPON_KNIFE
		    || model.first == ItemDefinitionIndex::WEAPON_KNIFE_T) {
			continue;
		}
		const bool item_selected = ((int) model.first == modelT);
		ImGui::PushID((int) model.first);
		if (ImGui::Selectable(Util::Items::GetItemDisplayName(model.first).c_str(), item_selected,
		                      ImGuiSelectableFlags_SpanAllColumns)) {
			modelT = (int) model.first;
			if (Settings::Skinchanger::skinsT.find((ItemDefinitionIndex) modelT) !=
			    Settings::Skinchanger::skinsT.end()) {
				modelSkinT = Settings::Skinchanger::skinsT.at(model.first).fallbackPaintKit;
				skinWearT = Settings::Skinchanger::skinsCT.at(model.first).fallbackWear;
				skinSeedT = Settings::Skinchanger::skinsCT.at(model.first).fallbackSeed;
				skinStatTrakT = Settings::Skinchanger::skinsCT.at(model.first).fallbackStatTrak;
				std::fill(std::begin(skinNameT), std::end(skinNameT), 0);
				std::copy(std::begin(Settings::Skinchanger::skinsT.at(model.first).customName),
				          std::end(Settings::Skinchanger::skinsT.at(model.first).customName), std::begin(skinNameT));
			}
		}
		ImGui::NextColumn();

		ImGui::Text("%s", Util::Items::GetItemEntityName(model.first).c_str());
		ImGui::NextColumn();

		ImGui::Separator();
		ImGui::PopID();
	}
	ImGui::ListBoxFooter();
	ImGui::NextColumn();

	ImGui::ListBoxHeader(XORSTR("##modelSkinsT"), ImVec2(-1, 300));
	ImGui::Columns(2, nullptr, false);;
	for (auto skin : itemSkins) {
		if (skin.second.paintName != nullptr ? !(
				Util::Contains(Util::ToLower(std::string(filterModelSkinsT)), Util::ToLower(skin.second.displayName)) ||
				Util::Contains(Util::ToLower(std::string(filterModelSkinsT)), skin.second.paintName)) : !Util::Contains(
				Util::ToLower(std::string(filterModelSkinsT)), Util::ToLower(skin.second.displayName))) {
			continue;
		}
		if (Util::Items::IsGlove((ItemDefinitionIndex) modelT) && skin.first < 10006) {
			continue;
		}
		const bool item_selected = (skin.first == modelSkinT);
		ImGui::PushID(skin.first);
		if (ImGui::Selectable(skin.first == -1 ? skin.second.displayName : skin.second.displayName, item_selected,
		                      ImGuiSelectableFlags_SpanAllColumns)) {
			modelSkinT = skin.first;
		}
		ImGui::NextColumn();

		ImGui::Text("%s", skin.second.paintName != nullptr ? skin.second.paintName : "");
		ImGui::NextColumn();

		ImGui::Separator();
		ImGui::PopID();
	}
	ImGui::GetCurrentContext()->FontBaseSize = 18.0f;
	ImGui::ListBoxFooter();
	ImGui::Columns();

	ImGui::Columns(2, nullptr, false);;

	ImGui::BeginChild(XORSTR("CT Settings"), ImVec2(0, 0), true);
	{
		ImGui::Columns(3, nullptr, false);

		ImGui::InputInt(XORSTR("ID##CT"), &modelSkinCT);
		ImGui::NextColumn();

		ImGui::InputInt(XORSTR("Seed##CT"), &skinSeedCT);
		ImGui::NextColumn();

		ImGui::PushItemWidth(-1);
		ImGui::SliderFloat(XORSTR("##WearCT"), &skinWearCT, 0.0005f, 1.0f, XORSTR("Wear: %0f"));
		ImGui::PopItemWidth();
		ImGui::Columns();

		if (!Util::Items::IsGlove((ItemDefinitionIndex) modelCT)) {
			ImGui::Columns(2, nullptr, false);

			ImGui::InputInt(XORSTR("StatTrak##CT"), &skinStatTrakCT);
			ImGui::NextColumn();

			ImGui::InputText(XORSTR("Name##CT"), skinNameCT, IM_ARRAYSIZE(skinNameCT));
			ImGui::Columns();
		}

		if (ImGui::Button(XORSTR("Apply##skinCT"), ImVec2(-1, 0))) {
			if (Settings::Skinchanger::skinsCT.find((ItemDefinitionIndex) modelCT) ==
			    Settings::Skinchanger::skinsCT.end()) {
				Settings::Skinchanger::skinsCT[(ItemDefinitionIndex) modelCT] = AttribItem_t();
			}

			Settings::Skinchanger::skinsCT.at((ItemDefinitionIndex) modelCT) = {ItemDefinitionIndex::INVALID,
			                                                                    modelSkinCT, skinWearCT, skinSeedCT,
			                                                                    skinStatTrakCT, -1, skinNameCT};

			SkinChanger::forceFullUpdate = true;

			if (Util::Items::IsGlove((ItemDefinitionIndex) modelCT)) {
				SkinChanger::glovesUpdated = true;
			}
		}

		ImGui::EndChild();
	}
	ImGui::NextColumn();

	ImGui::BeginChild(XORSTR("T Settings"), ImVec2(0, 0), true);
	{
		ImGui::Columns(3, nullptr, false);

		ImGui::InputInt(XORSTR("ID##T"), &modelSkinT);
		ImGui::NextColumn();

		ImGui::InputInt(XORSTR("Seed##T"), &skinSeedT);
		ImGui::NextColumn();

		ImGui::PushItemWidth(-1);
		ImGui::SliderFloat(XORSTR("##WearT"), &skinWearT, 0.0005f, 1.0f, XORSTR("Wear: %0f"));
		ImGui::PopItemWidth();
		ImGui::Columns();

		if (!Util::Items::IsGlove((ItemDefinitionIndex) modelT)) {
			ImGui::Columns(2, nullptr, false);

			ImGui::InputInt(XORSTR("StatTrak##T"), &skinStatTrakT);
			ImGui::NextColumn();

			ImGui::InputText(XORSTR("Name##T"), skinNameT, IM_ARRAYSIZE(skinNameT));
			ImGui::Columns();
		}

		if (ImGui::Button(XORSTR("Apply##skinT"), ImVec2(-1, 0))) {
			if (Settings::Skinchanger::skinsT.find((ItemDefinitionIndex) modelT) ==
			    Settings::Skinchanger::skinsT.end()) {
				Settings::Skinchanger::skinsT[(ItemDefinitionIndex) modelT] = AttribItem_t();
			}

			Settings::Skinchanger::skinsT.at((ItemDefinitionIndex) modelT) = {ItemDefinitionIndex::INVALID, modelSkinT,
			                                                                  skinWearT, skinSeedT, skinStatTrakT, -1,
			                                                                  skinNameT};

			SkinChanger::forceFullUpdate = true;

			if (Util::Items::IsGlove((ItemDefinitionIndex) modelT)) {
				SkinChanger::glovesUpdated = true;
			}
		}

		ImGui::EndChild();
	}
	ImGui::Columns();
}

void CombinedSkins()
{
	static int selectedModel = 1;
	static int selectedModelSkin = 1;
	static float skinWear = 0.0001f;
	static int skinSeed = -1;
	static int skinStatTrak = -1;
	static char skinName[18];
	static char filterModels[18];
	static char filterModelSkins[18];

	ImGui::Columns(2, nullptr, false);;

	ImGui::Text(XORSTR("Models"));
	ImGui::NextColumn();

	ImGui::Text(XORSTR("Skins"));
	ImGui::Columns();

	ImGui::Columns(2, nullptr, false);;

	ImGui::PushItemWidth(-1);
	ImGui::InputText(XORSTR("##filterModels"), filterModels, IM_ARRAYSIZE(filterModels));
	ImGui::PopItemWidth();
	ImGui::NextColumn();

	ImGui::PushItemWidth(-1);
	ImGui::InputText(XORSTR("##filterModelSkins"), filterModelSkins, IM_ARRAYSIZE(filterModelSkins));
	ImGui::PopItemWidth();
	ImGui::Columns();

	ImGui::Columns(2, nullptr, false);;

	ImGui::ListBoxHeader(XORSTR("##models"), ImVec2(-1, 300));
	ImGui::Columns(2, nullptr, false);;
	for (auto model : ItemDefinitionIndexMap) {
		if (!(Util::Contains(Util::ToLower(std::string(filterModels)),
		                     Util::ToLower(Util::Items::GetItemDisplayName(model.first))) ||
		      Util::Contains(Util::ToLower(std::string(filterModels)),
		                     Util::ToLower(Util::Items::GetItemEntityName(model.first))))) {
			continue;
		}
		if (Util::Items::IsUtility(model.first) || model.first == ItemDefinitionIndex::GLOVE_CT_SIDE ||
		    model.first == ItemDefinitionIndex::GLOVE_T_SIDE || model.first == ItemDefinitionIndex::WEAPON_KNIFE ||
		    model.first == ItemDefinitionIndex::WEAPON_KNIFE_T) {
			continue;
		}
		const bool item_selected = ((int) model.first == selectedModel);
		ImGui::PushID((int) model.first);
		if (ImGui::Selectable(Util::Items::GetItemDisplayName(model.first).c_str(), item_selected,
		                      ImGuiSelectableFlags_SpanAllColumns)) {
			selectedModel = (int) model.first;
			if (Settings::Skinchanger::skinsCT.find((ItemDefinitionIndex) selectedModel) !=
			    Settings::Skinchanger::skinsCT.end()) {
				selectedModelSkin = Settings::Skinchanger::skinsCT.at(model.first).fallbackPaintKit;
				skinWear = Settings::Skinchanger::skinsCT.at(model.first).fallbackWear;
				skinSeed = Settings::Skinchanger::skinsCT.at(model.first).fallbackSeed;
				skinStatTrak = Settings::Skinchanger::skinsCT.at(model.first).fallbackStatTrak;
				std::fill(std::begin(skinName), std::end(skinName), 0);
				std::copy(std::begin(Settings::Skinchanger::skinsCT.at(model.first).customName),
				          std::end(Settings::Skinchanger::skinsCT.at(model.first).customName), std::begin(skinName));
			}
		}
		ImGui::NextColumn();

		ImGui::Text("%s", Util::Items::GetItemEntityName(model.first).c_str());
		ImGui::NextColumn();

		ImGui::Separator();
		ImGui::PopID();
	}
	ImGui::ListBoxFooter();
	ImGui::NextColumn();

	ImGui::ListBoxHeader(XORSTR("##modelSkins"), ImVec2(-1, 300));
	ImGui::Columns(2, nullptr, false);;
	for (auto skin : itemSkins) {
		if (skin.second.paintName != nullptr ? !(
				Util::Contains(Util::ToLower(std::string(filterModelSkins)), Util::ToLower(skin.second.displayName)) ||
				Util::Contains(Util::ToLower(std::string(filterModelSkins)), skin.second.paintName)) : !Util::Contains(
				Util::ToLower(std::string(filterModelSkins)), Util::ToLower(skin.second.displayName))) {
			continue;
		}
		if (Util::Items::IsGlove((ItemDefinitionIndex) selectedModel) && skin.first < 10006) {
			continue;
		}
		const bool item_selected = (skin.first == selectedModelSkin);
		ImGui::PushID(skin.first);
		if (ImGui::Selectable(skin.first == -1 ? skin.second.displayName : skin.second.displayName, item_selected,
		                      ImGuiSelectableFlags_SpanAllColumns)) {
			selectedModelSkin = skin.first;
		}
		ImGui::NextColumn();

		ImGui::Text("%s", skin.second.paintName != nullptr ? skin.second.paintName : "");
		ImGui::NextColumn();

		ImGui::Separator();
		ImGui::PopID();
	}
	ImGui::ListBoxFooter();
	ImGui::Columns();

	ImGui::BeginChild(XORSTR("Settings"), ImVec2(0, 0), true);
	{
		ImGui::Columns(3, nullptr, false);;

		ImGui::InputInt(XORSTR("ID##CT"), &selectedModelSkin);
		ImGui::NextColumn();

		ImGui::InputInt(XORSTR("Seed##CT"), &skinSeed);
		ImGui::NextColumn();

		ImGui::PushItemWidth(-1);
		ImGui::SliderFloat(XORSTR("##WearCT"), &skinWear, 0.0005f, 1.0f, XORSTR("Wear: %0f"));
		ImGui::PopItemWidth();
		ImGui::Columns();

		if (!Util::Items::IsGlove((ItemDefinitionIndex) selectedModel)) {
			ImGui::Columns(2, nullptr, false);

			ImGui::InputInt(XORSTR("StatTrak##Combined"), &skinStatTrak);
			ImGui::NextColumn();

			ImGui::InputText(XORSTR("Name##Combined"), skinName, IM_ARRAYSIZE(skinName));
			ImGui::Columns();
		}

		if (ImGui::Button(XORSTR("Apply##Combined"), ImVec2(-1, 0))) {
			if (Settings::Skinchanger::skinsCT.find((ItemDefinitionIndex) selectedModel) ==
			    Settings::Skinchanger::skinsCT.end()) {
				Settings::Skinchanger::skinsCT[(ItemDefinitionIndex) selectedModel] = AttribItem_t();
			}

			Settings::Skinchanger::skinsCT.at((ItemDefinitionIndex) selectedModel) = {ItemDefinitionIndex::INVALID,
			                                                                          selectedModelSkin, skinWear,
			                                                                          skinSeed, skinStatTrak, -1,
			                                                                          skinName};

			SkinChanger::forceFullUpdate = true;

			if (Util::Items::IsGlove((ItemDefinitionIndex) selectedModel)) {
				SkinChanger::glovesUpdated = true;
			}
		}

		ImGui::EndChild();
	}
}

void Skins::RenderTab()
{
	ImGui::Columns(2, nullptr, false);

	if (ImGui::CheckboxFill(XORSTR("Enabled"), &Settings::Skinchanger::Skins::enabled)) {
		SkinChanger::forceFullUpdate = true;
		SkinChanger::glovesUpdated = true;
	}
	ImGui::NextColumn();

	if (ImGui::CheckboxFill(XORSTR("Per-Team"), &Settings::Skinchanger::Skins::perTeam)) {
		SkinChanger::forceFullUpdate = true;
		SkinChanger::glovesUpdated = true;
	}
	ImGui::NextColumn();

	ImGui::Columns();

	ImGui::Separator();

	if (Settings::Skinchanger::Skins::perTeam) {
		SplitSkins();
	} else {
		CombinedSkins();
	}
}
