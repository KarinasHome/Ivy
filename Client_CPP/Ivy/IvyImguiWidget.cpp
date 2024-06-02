/*
 * This file is part of the Ivy distribution (https://github.com/KarinasHome/Ivy).
 * Copyright (c) 2019 Karina A.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "IvyImguiWidget.h"
#include "MyIvy.h"
#include <boost/range/adaptor/reversed.hpp>

#include "MyIvyConfigAircraft.h"

#include <iomanip> // setprecision
#include <sstream> // stringstream

#include <misc/cpp/imgui_stdlib.h>


IvyImguiWidget::IvyImguiWidget(MyIvy *pIvyNew, int left, int top, int right, int bot, int decoration) :
	ImgWindow(left, top, right, bot, decoration)
{
	pIvy = pIvyNew;
	SetWindowTitle("The Nagging Co-Pilot");
	SetVisible(false);
	configureImguiContext();
}


IvyImguiWidget::~IvyImguiWidget()
{
}

void IvyImguiWidget::configureImguiContext()
{
	/*ImGuiIO& io = ImGui::GetIO();
	io.Fonts->AddFontDefault();
	font2 = io.Fonts->AddFontFromFileTTF("./Resources/fonts/DejaVuSans.ttf", 8.0f);
	font3 = io.Fonts->AddFontFromFileTTF("./Resources/fonts/DejaVuSans.ttf", 10.0f);
	font4 = io.Fonts->AddFontFromFileTTF("./Resources/fonts/DejaVuSans.ttf", 12.0f);
	font5 = io.Fonts->AddFontFromFileTTF("./Resources/fonts/DejaVuSansMono.ttf", 8.0f);
	font6 = io.Fonts->AddFontFromFileTTF("./Resources/fonts/DejaVuSansMono.ttf", 16.0f);
	font7 = io.Fonts->AddFontFromFileTTF("./Resources/fonts/DejaVuSansMono.ttf", 18.0f);
	font8 = io.Fonts->AddFontFromFileTTF("./Resources/fonts/Inconsolata.ttf", 8.0f);
	font9 = io.Fonts->AddFontFromFileTTF("./Resources/fonts/Inconsolata.ttf", 22.0f);
	font10 = io.Fonts->AddFontFromFileTTF("./Resources/fonts/Inconsolata.ttf", 24.0f);*/

}

void IvyImguiWidget::Visible(bool visible)
{
	SetVisible(visible);
	if (visible)
	{
		output_dh = XPLMGetDataf(pIvy->m_f_decision_height);

		selected_radio = 0;

		for (int index = 0; index < pIvy->m_ivyConfig->m_audio_dirs.size(); index++)
		{
			if (pIvy->m_ivyConfig->m_audio_dirs[index].compare(pIvy->m_ivyConfig->m_mp3_dir) == 0)
				selected_radio = index;
		}
	}
}

void IvyImguiWidget::buildInterface() 
{
	
	win_width = ImGui::GetWindowWidth();
	win_height = ImGui::GetWindowHeight();

	ImGui::Spacing();
	ImGui::Columns(6, 0, false);
	ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5.f);
	if (ImGui::Button("MyFlight", ImVec2(150, 20))) selected_tab = 1;
	ImGui::NextColumn();
	if (ImGui::Button("Graphs", ImVec2(150, 20))) selected_tab = 2;
	ImGui::NextColumn();
	if (ImGui::Button("Areas of Concern", ImVec2(150, 20))) selected_tab = 3;
	ImGui::NextColumn();
	if (ImGui::Button("Logbook", ImVec2(150, 20))) selected_tab = 4;
	ImGui::NextColumn();
	if (ImGui::Button("Aircraft Configuration", ImVec2(180, 20))) selected_tab = 6;
	ImGui::NextColumn();
	if (ImGui::Button("Settings", ImVec2(150, 20))) selected_tab = 5;
	ImGui::Columns(1);
	ImGui::PopStyleVar();
	ImGui::Separator();
	if (selected_tab == 1) {
		ImGui::Columns(2, 0, true);
		ImGui::SetColumnWidth(-1, 150);

		
		ImGui::Text("Volume:");
		ImGui::Spacing();
		ImGui::SliderFloat("##Volume", &(pIvy->m_ivyConfig->m_ivyVolume), 0.1, 1, "%.1f");

		ImGui::Text("Call-Outs:");
		//for (int i = 0; i < (cols - 1); i++) ImGui::NextColumn();
		ImGui::Checkbox(" 60 kt", &(pIvy->m_ivyConfig->m_kt60_enabled));
		ImGui::Checkbox(" 80 kt", &(pIvy->m_ivyConfig->m_kt80_enabled));
		ImGui::Checkbox("100 kt", &(pIvy->m_ivyConfig->m_kt100_enabled));
		ImGui::Checkbox(" 60 kt Landing", &(pIvy->m_ivyConfig->m_kt60__landing_enabled));

		//ImGui::NextColumn();
		ImGui::Text(" ");
		ImGui::InputInt("V1", &(pIvy->m_ivyAircraft->m_li_v1), 1, 1);
		
		
		ImGui::InputInt("VR", &(pIvy->m_ivyAircraft->m_li_vr), 1, 1);
		
		
		
		ImGui::InputInt("V2", &(pIvy->m_ivyAircraft->m_li_v2), 1, 1);

		

		output_dh = XPLMGetDataf(pIvy->m_f_decision_height);
		
		//output_dh = XPLMGetDataf(pIvy->m_f_decision_height);
		ImGui::InputFloat("DH", &output_dh,1,1,0,0);
		XPLMSetDataf(pIvy->m_f_decision_height, output_dh);

		//ImGui::Text("Transition Alt.:");
		//ImGui::InputFloat("##Transition", &(pIvy->m_ivyConfig->m_trans_alt), 1000, 1000, 0, 0);

		ImGui::Spacing();
		ImGui::Checkbox("NOE Enable", &(pIvy->m_ivyAircraft->m_max_alt_enable));

		if (pIvy->m_ivyAircraft->m_is_206B3 == true)
		{
			ImGui::InputFloat("MTBF", &(pIvy->m_ivyAircraft->m_206B3_MTBF), 0.1, 1, 1, 0);
		}
		if (pIvy->m_ivyAircraft->m_is_206L3 == true)
		{
			ImGui::InputFloat("MTBF", &(pIvy->m_ivyAircraft->m_206L3_MTBF), 0.1, 1, 1, 0);
		}
		
		ImGui::NextColumn();
		//const char* listbox_items[1000];

		ImGui::Text("Events :");

		for (auto line : pIvy->m_error_list)
		{
			ImGui::Text(line.c_str());
		}
		
		//static int listbox_item_current = 0;
		//listbox_item_current = 0;
		
		//ImGui::ListBox("", &listbox_item_current, listbox_items, IM_ARRAYSIZE(listbox_items), 20);
	}

	// Show Graphs
	if (selected_tab == 2)
	{
		ImGui::Columns(2, 0, true);
		ImGui::SetColumnWidth(-1, 200);
		ImGui::Spacing();

		if (ImGui::Button("Altitude", ImVec2(180, 20))) selected_graph = 1;
		if (ImGui::Button("Climb Rate", ImVec2(180, 20))) selected_graph = 2;
		if (ImGui::Button("G-Forces Vertical", ImVec2(180, 20))) selected_graph = 3;
		if (ImGui::Button("G-Forces Forward", ImVec2(180, 20))) selected_graph = 4;
		if (ImGui::Button("G-Forces Side", ImVec2(180, 20))) selected_graph = 5;
		if (pIvy->m_ivyHVCheck.m_hv_diagram_found == true)
		{
			if (ImGui::Button("HV Diagram", ImVec2(180, 20))) selected_graph = 6;
		}
		else
		{
			ImGui::Text("No HV Diagram Found");
		}


		ImGui::NextColumn();

		// Altitude
		if (selected_graph == 1)
		{
			float *p_values = pIvy->m_graph_altitude;

			float max_value = 0;
			float min_value = p_values[0];
			for (int index = 0; index < MAX_GRAPH_DATA; index++)
			{
				if (max_value < p_values[index])
					max_value = p_values[index];

				if (min_value > p_values[index])
					min_value = p_values[index];
			}

			ImGui::PlotLines("", p_values, MAX_GRAPH_DATA, 0, "Altitude", min_value - 1, max_value + 1, ImVec2(win_width - 300, win_height - 100));

			std::string text = std::to_string((int)max_value) + " ft";
			ImGui::SetCursorScreenPos(ImVec2(win_width - 90, 40));
			ImGui::Text(text.c_str());

			text = std::to_string((int)min_value) + " ft";
			ImGui::SetCursorScreenPos(ImVec2(win_width - 90, win_height - 75));
			ImGui::Text(text.c_str());
		}
		// Climb Rate
		else if (selected_graph == 2)
		{
			float *p_values = pIvy->m_graph_climb;

			float max_value = 0;
			float min_value = p_values[0];

			float abs_max = 0;

			for (int index = 0; index < MAX_GRAPH_DATA; index++)
			{
				if (max_value < p_values[index])
					max_value = p_values[index];

				if (min_value > p_values[index])
					min_value = p_values[index];
			}

			abs_max = abs(max_value);
			if (abs_max < abs(min_value))
				abs_max = abs(min_value);

			max_value = abs_max;
			min_value = -1 * abs_max;

			ImGui::PlotLines("", p_values, MAX_GRAPH_DATA, 0, "Climb Rate", min_value - 1, max_value + 1, ImVec2(win_width - 300, win_height - 100));

			std::string text = std::to_string((int)max_value) + " ft/min";
			ImGui::SetCursorScreenPos(ImVec2(win_width - 90, 40));
			ImGui::Text(text.c_str());

			text = std::to_string((int)min_value) + " ft/min";
			ImGui::SetCursorScreenPos(ImVec2(win_width - 90, win_height - 75));
			ImGui::Text(text.c_str());
		}
		// G Vertical
		else if (selected_graph == 3)
		{
			float *p_values = pIvy->m_graph_g_vert;

			float max_value = 0;
			float min_value = p_values[0];

			float abs_max = 0;

			for (int index = 0; index < MAX_GRAPH_DATA; index++)
			{
				if (max_value < p_values[index])
					max_value = p_values[index];

				if (min_value > p_values[index])
					min_value = p_values[index];
			}

			abs_max = abs(max_value);
			if (abs_max < abs(min_value))
				abs_max = abs(min_value);

			max_value = abs_max + 0.1;
			min_value = -1 * abs_max - 0.1;

			ImGui::PlotLines("", p_values, MAX_GRAPH_DATA, 0, "G Forces Vertical", min_value, max_value, ImVec2(win_width - 300, win_height - 100));

			std::stringstream text;
			text << std::fixed << std::setprecision(2) << max_value << " g";
			ImGui::SetCursorScreenPos(ImVec2(win_width - 90, 40));
			ImGui::Text(text.str().c_str());

			text.str("");
			text << std::fixed << std::setprecision(2) << min_value << " g";
			ImGui::SetCursorScreenPos(ImVec2(win_width - 90, win_height - 75));
			ImGui::Text(text.str().c_str());
		}
		// Forward G
		else if (selected_graph == 4)
		{
			float *p_values = pIvy->m_graph_g_horiz;

			float max_value = 0;
			float min_value = p_values[0];

			float abs_max = 0;

			for (int index = 0; index < MAX_GRAPH_DATA; index++)
			{
				if (max_value < p_values[index])
					max_value = p_values[index];

				if (min_value > p_values[index])
					min_value = p_values[index];
			}

			abs_max = abs(max_value);
			if (abs_max < abs(min_value))
				abs_max = abs(min_value);

			max_value = abs_max + 0.1;
			min_value = -1 * abs_max - 0.1;

			ImGui::PlotLines("", p_values, MAX_GRAPH_DATA, 0, "G Forces Forward", min_value, max_value, ImVec2(win_width - 300, win_height - 100));

			std::stringstream text;
			text << std::fixed << std::setprecision(2) << max_value << " g";
			ImGui::SetCursorScreenPos(ImVec2(win_width - 90, 40));
			ImGui::Text(text.str().c_str());

			text.str("");
			text << std::fixed << std::setprecision(2) << min_value << " g";
			ImGui::SetCursorScreenPos(ImVec2(win_width - 90, win_height - 75));
			ImGui::Text(text.str().c_str());
		}
		// Side G
		else if (selected_graph == 5)
		{
			float *p_values = pIvy->m_graph_g_side;

			float max_value = 0;
			float min_value = p_values[0];

			float abs_max = 0;

			for (int index = 0; index < MAX_GRAPH_DATA; index++)
			{
				if (max_value < p_values[index])
					max_value = p_values[index];

				if (min_value > p_values[index])
					min_value = p_values[index];
			}

			abs_max = abs(max_value);
			if (abs_max < abs(min_value))
				abs_max = abs(min_value);

			max_value = abs_max + 0.1;
			min_value = -1 * abs_max - 0.1;

			ImGui::PlotLines("", p_values, MAX_GRAPH_DATA, 0, "G Forces Sideways", min_value, max_value, ImVec2(win_width - 300, win_height - 100));
			

			std::stringstream text;
			text << std::fixed << std::setprecision(2) << max_value << " g left";
			ImGui::SetCursorScreenPos(ImVec2(win_width - 90, 40));
			ImGui::Text(text.str().c_str());

			text.str("");
			text << std::fixed << std::setprecision(2) << min_value << " g right";
			ImGui::SetCursorScreenPos(ImVec2(win_width - 90, win_height - 75));
			ImGui::Text(text.str().c_str());
		}
		// HV Diagram
		else if (selected_graph == 6)
		{

			float start_x = 250;
			float start_y = 330;
			
			float scale_x = (550.0 - 250.0) / ((float) HV_VELOCITY_MAX);
			float scale_y = (50.0 - 330.0) / ((float) HV_HEIGHT_MAX);



			ImDrawList* draw_list = ImGui::GetWindowDrawList();
			ImVec4 col = ImColor(255, 255, 255);
			ImU32 col32 = ImColor(col);

			draw_list->AddText(ImVec2(200, 50), col32, "500ft");
			draw_list->AddLine(ImVec2(250, 50), ImVec2(250, 330), col32);
			draw_list->AddLine(ImVec2(250, 330), ImVec2(550, 330), col32);
			draw_list->AddText(ImVec2(540, 335), col32, "150kt");

			//col = ImColor(255, 255, 255);
			//col32 = ImColor(col);

			/*draw_list->AddLine(ImVec2(251, 331), ImVec2(251, 331), col32);
			draw_list->AddLine(ImVec2(252, 331), ImVec2(252, 331), col32);
			draw_list->AddLine(ImVec2(253, 331), ImVec2(253, 331), col32);
			draw_list->AddLine(ImVec2(254, 331), ImVec2(254, 331), col32);
			draw_list->AddLine(ImVec2(255, 331), ImVec2(255, 331), col32);
			draw_list->AddLine(ImVec2(256, 331), ImVec2(256, 331), col32);*/

			draw_list->AddCircle(ImVec2(250 + 1, 330), 0.1, col32);

			for (int poly_index = 0; poly_index < HV_POLYGONS_MAX; poly_index++)
			{
				if (pIvy->m_ivyHVCheck.hv_polygon[poly_index].size() > 1)
				{
					for (int index = 0; index < (pIvy->m_ivyHVCheck.hv_polygon[poly_index].size() - 1); index++)
					{
						int x1 = ((int)(start_x + scale_x * pIvy->m_ivyHVCheck.hv_polygon[poly_index][index].velocity));
						int x2 = ((int)(start_x + scale_x * pIvy->m_ivyHVCheck.hv_polygon[poly_index][index + 1].velocity));

						int y1 = ((int)(start_y + scale_y * pIvy->m_ivyHVCheck.hv_polygon[poly_index][index].height));
						int y2 = ((int)(start_y + scale_y * pIvy->m_ivyHVCheck.hv_polygon[poly_index][index + 1].height));

						draw_list->AddLine(ImVec2(x1, y1), ImVec2(x2, y2), col32);

						
					}
				}
			}

			col = ImColor(255, 0, 0);
			ImU32 col32_r = ImColor(col);

			col = ImColor(0, 255, 0);
			ImU32 col32_g = ImColor(col);

			for (int v_index = 0; v_index < HV_VELOCITY_MAX; v_index++)
			{
				for (int h_index = 0; h_index < HV_HEIGHT_MAX; h_index++)
				{
					if (pIvy->m_ivyHVCheck.m_hv_tracking[v_index + (h_index * HV_VELOCITY_MAX)] > 0)
					{
						int x = (int)(start_x + scale_x * ((float) v_index));
						int y = (int)(start_y + scale_y * ((float) h_index));

						ImU32 col_x = col32_g;
						if (pIvy->m_ivyHVCheck.m_hv_tracking[v_index + (h_index * HV_VELOCITY_MAX)] == 2)
							col_x = col32_r;

						draw_list->AddCircle(ImVec2(x + 1, y), 0.1, col_x);
					}
						
				}
			}

				//for (pIvy->m_ivyHVCheck->hv_polygon.size())
			
			
		}



	}
	// Logbook
	if (selected_tab == 4)
	{
		for (auto line : boost::adaptors::reverse(pIvy->m_logbook_entries))
		{
			ImGui::Text(line.c_str());
		}
		
	}

	// Areas of Concern
	if (selected_tab == 3)
	{
		ImGui::Columns(2, 0, true);
		ImGui::SetColumnWidth(-1, 350);

		ImGui::Text("Areas of Concern:");
		ImGui::Separator();

		ImVec4 col = ImColor(255, 0, 0);
		ImGui::PushStyleColor(ImGuiCol_Text, col);
		for (auto ivyObj : pIvy->m_aoc_list_high)
			ImGui::Text(ivyObj->m_error_string.c_str());
		ImGui::PopStyleColor();

		col = ImColor(255, 255, 0);
		ImGui::PushStyleColor(ImGuiCol_Text, col);
		for (auto ivyObj : pIvy->m_aoc_list_med)
			ImGui::Text(ivyObj->m_error_string.c_str());
		ImGui::PopStyleColor();

		col = ImColor(0, 255, 0);
		ImGui::PushStyleColor(ImGuiCol_Text, col);
		for (auto ivyObj : pIvy->m_aoc_list_low)
			ImGui::Text(ivyObj->m_error_string.c_str());
		ImGui::PopStyleColor();

		ImGui::NextColumn();

		ImGui::PlotHistogram("", pIvy->m_ivy_error_count_history, MAX_ERROR_HISTORY, 0, "Number of Errors per Flight", 0, pIvy->m_ivy_error_count_history_max, ImVec2(win_width - 400, win_height - 100));
		ImGui::SetCursorPosX(362);
		ImGui::Text("0");
		ImGui::SameLine();
		ImGui::SetCursorPosX(win_width - 60);
		ImGui::Text(std::to_string(MAX_ERROR_HISTORY - 1).c_str());



	}

	// Settings
	if (selected_tab == 5)
	{
		ImGui::Columns(3, 0, true);
		ImGui::SetColumnWidth(-1, 350);
		ImGui::Text("Audio Settings:");

		ImGui::Spacing();

		for (int index = 0; index < pIvy->m_ivyConfig->m_audio_names.size(); index++)
		{
			if (ImGui::RadioButton(pIvy->m_ivyConfig->m_audio_names[index].c_str(), selected_radio == index)) {
				selected_radio = index;
			}
		}
		ImGui::Spacing();

		if (ImGui::Button("Apply Audio Settings", ImVec2(250, 20)))
		{
			pIvy->m_ivyConfig->m_mp3_dir = pIvy->m_ivyConfig->m_audio_dirs[selected_radio];
			pIvy->m_ivyConfig->SetAudioDirectory();
			pIvy->IvyLoadSoundFiles(true);
		}

		ImGui::NextColumn();

		if (ImGui::Checkbox("Enable Ivy", &(pIvy->m_ivyConfig->m_ivy_enable))) pIvy->MenuCheck();
		if (ImGui::Checkbox("Enable Log", &(pIvy->m_ivyConfig->m_log_enable))) pIvy->MenuCheck();
		ImGui::Checkbox("Enable Callouts", &(pIvy->m_ivyConfig->m_callouts_enable));
		ImGui::Checkbox("Enable Errors", &(pIvy->m_ivyConfig->m_errors_enable));
		ImGui::Checkbox("Enable Pre-Warnings", &(pIvy->m_ivyConfig->m_pre_warnings));
		ImGui::Checkbox("Enable Ouch", &(pIvy->m_ivyConfig->m_ouch_enabled));

		ImGui::Spacing();

		ImGui::Checkbox("Enable Screaming", &(pIvy->m_ivyConfig->m_passengers_screaming));
		ImGui::Checkbox("Enable Applause", &(pIvy->m_ivyConfig->m_passengers_applause));
		ImGui::Spacing();
		ImGui::Checkbox("Enable HRM", &(pIvy->m_ivyConfig->m_hrm_enabled));

		ImGui::Spacing();

		ImGui::SliderFloat("Ivy Volume", &(pIvy->m_ivyConfig->m_ivyVolume), 0.1, 1, "%.1f");
		

		//ImGui::Checkbox("Enable Errors", &(pIvy->m_ivyConfig->m_p));


	}

	// Aircraft Configuration
	if (selected_tab == 6)
	{
		if (pIvy->m_ivyAircraft->m_aircraft_number == 0)
		{
			static bool create_failed = false;

			ImGui::Spacing();
			ImGui::Text("Unconfigured Aircraft");

			if (ImGui::Button("Create New Aircraft Configuration", ImVec2(250, 20)))
			{
				MyIvyConfigAircraft *p_ivyAircraft = new MyIvyConfigAircraft(pIvy->m_ivyConfig->m_config_path.c_str(),0, pIvy->m_ls_acf_descrip);

				if (p_ivyAircraft->m_aircraft_number == 0)
				{
					delete p_ivyAircraft;
					create_failed = true;
				}
				else
				{
					pIvy->m_ivy_aircraft_list->push_back(p_ivyAircraft);
					p_ivyAircraft->WriteConfigFile();
					pIvy->m_ivyAircraft = p_ivyAircraft;
				}
			}

			if (create_failed)
			{
				ImGui::Spacing();
				ImGui::Text("Aircraft Creation Failed");
			}
		}
		else
		{
			ImGui::Columns(4, 0, true);
			ImGui::SetColumnWidth(-1, 200);
			ImGui::Spacing();

			//Debug:
			//ImGui::Text("Debug: %f", pIvy->m_n1_deviation);

			ImGui::Text("Aircraft Configuration #");
			ImGui::SameLine();
			ImGui::Text(std::to_string(pIvy->m_ivyAircraft->m_aircraft_number).c_str());
			ImGui::Text("Aircraft Name: ");
			
			ImGui::TextWrapped(pIvy->m_ivyAircraft->m_name);
			//ImGui::Separator();
			ImGui::Spacing();
			
			
			ImGui::PushItemWidth(80);

			ImGui::InputInt("V1 Static         ", &(pIvy->m_ivyAircraft->m_li_v1),1,1);
			ImGui::InputInt("VR Static         ", &(pIvy->m_ivyAircraft->m_li_vr), 1, 1);
			ImGui::InputInt("V2 Static         ", &(pIvy->m_ivyAircraft->m_li_v2), 1, 1);

			ImGui::Checkbox("Dynamic V-Speeds", &(pIvy->m_ivyAircraft->m_vspeeds_enabled));
			ImGui::InputText("V1 Dataref", &(pIvy->m_ivyAircraft->m_lx_v1_data_ref));
			ImGui::InputText("VR Dataref", &(pIvy->m_ivyAircraft->m_lx_vr_data_ref));
			ImGui::InputText("V2 Dataref", &(pIvy->m_ivyAircraft->m_lx_v2_data_ref));

			ImGui::PopItemWidth();

			ImGui::PushItemWidth(150);

			if (ImGui::Button("Update Datarefs",ImVec2(180,25)))
			{
				pIvy->m_ivyAircraft->InitDataRefs();
			}

			if (ImGui::Button("Write Configuration File", ImVec2(180, 25)))
			{
				pIvy->m_ivyAircraft->WriteConfigFile();
			}
			ImGui::PopItemWidth();

			ImGui::NextColumn();

			ImGui::SetColumnWidth(-1, 200);

			ImGui::Checkbox("Say Localizer", &(pIvy->m_ivyAircraft->m_localizer_enable));
			ImGui::PushItemWidth(80);
			ImGui::InputFloat("TO Power Lever", &(pIvy->m_ivyAircraft->m_to_power_lever), 0.01, 0.01, 2, 0);
			ImGui::InputFloat("TO Power Tolerance", &(pIvy->m_ivyAircraft->m_to_power_tolerance), 0.01, 0.01, 2, 0);

			ImGui::InputFloat("SU EGN", &(pIvy->m_ivyAircraft->m_eng_egt_lightup), 1, 1, 0, 0);
			ImGui::InputFloat("SU N2", &(pIvy->m_ivyAircraft->m_eng_n2_startup), 1, 1, 0, 0);
			ImGui::InputFloat("SU Stab N1", &(pIvy->m_ivyAircraft->m_eng_stab_n1_min), 1, 1, 0, 0);
			ImGui::InputFloat("SU Stab dN1", &(pIvy->m_ivyAircraft->m_eng_stab_n1_delta_s), 0.01, 0.01, 2, 0);
			ImGui::PopItemWidth();
			ImGui::Spacing();

			ImGui::Checkbox("No Lights or Transp.", &(pIvy->m_ivyAircraft->m_military));
			ImGui::Checkbox("Steep Descent", &(pIvy->m_ivyAircraft->m_steep_descent));
			ImGui::Checkbox("Steep Pitch/Bank", &(pIvy->m_ivyAircraft->m_steep_bank));
			ImGui::Checkbox("NOE Enable", &(pIvy->m_ivyAircraft->m_max_alt_enable));

			ImGui::PushItemWidth(100);
			ImGui::InputInt("Max Alt AGL", &(pIvy->m_ivyAircraft->m_max_alt_value), 1, 1);
			ImGui::PopItemWidth();
			//ImGui::Spacing();

			

			//ImGui::Spacing();

			ImGui::NextColumn();

			ImGui::SetColumnWidth(-1, 450);
			
			
			ImGui::PushItemWidth(100);
			ImGui::Checkbox("Enable Slats", &(pIvy->m_ivyAircraft->m_slats_enabled));
			//ImGui::SameLine();
			ImGui::InputFloat("Slats Tolerance", &(pIvy->m_ivyAircraft->m_slats_tolerance), 0.01, 0.01, 2, 0);
			ImGui::SameLine();
			ImGui::InputFloat("Slats Value", &(pIvy->m_ivyAircraft->m_lf_slats), 0.01, 0.01, 2, ImGuiInputTextFlags_ReadOnly);

			ImGui::PopItemWidth();
			ImGui::PushItemWidth(350);
			ImGui::InputText("SDataRef", &(pIvy->m_ivyAircraft->m_lf_slats_data_ref));
			ImGui::PopItemWidth();
			
			//ImGui::Spacing();
						
			for (int index = 0; index < IVY_FS_MAX; index++)
			{
				char buffer[1024];

				ImGui::Text("Pos S%2i: ", index + 1);
				ImGui::SameLine();
				ImGui::PushItemWidth(100);
				sprintf(buffer, "##Pos S%2i", index + 1);
				ImGui::InputInt(buffer, &(pIvy->m_ivyAircraft->m_slats_deploy_pos[index]), 1, 1, 0);

				

				ImGui::SameLine();

				ImGui::PopItemWidth();

				sprintf(buffer, "SET S%2i", index + 1);
				if (ImGui::Button(buffer)) pIvy->m_ivyAircraft->m_slats_deploy_value[index] = pIvy->m_ivyAircraft->m_lf_slats;
				ImGui::SameLine();
				sprintf(buffer, "DEL S%2i", index + 1);
				if (ImGui::Button(buffer)) pIvy->m_ivyAircraft->m_slats_deploy_value[index] = -100;

				ImGui::SameLine();

				ImGui::Text("Val: %.2f", pIvy->m_ivyAircraft->m_slats_deploy_value[index]);

				//sprintf(buffer, "##Val S%2i", index + 1);
				//ImGui::InputFloat(buffer, &(pIvy->m_ivyAircraft->m_slats_deploy_value[index]), 0.01, 0.01, "%.2f", ImGuiInputTextFlags_ReadOnly );
				

				
			}
			



			ImGui::NextColumn();

			ImGui::SetColumnWidth(-1, 450);

			ImGui::PushItemWidth(100);
			ImGui::Checkbox("Enable Flaps", &(pIvy->m_ivyAircraft->m_flaps_enabled));
			//ImGui::SameLine();
			ImGui::InputFloat("Flaps Tolerance", &(pIvy->m_ivyAircraft->m_flaps_tolerance), 0.01, 0.01, 2, 0);
			ImGui::SameLine();
			ImGui::InputFloat("Flaps Value", &(pIvy->m_ivyAircraft->m_lf_flaps), 0.01, 0.01, 2, ImGuiInputTextFlags_ReadOnly);

			ImGui::PopItemWidth();
			ImGui::PushItemWidth(350);
			ImGui::InputText("FDataRef", &(pIvy->m_ivyAircraft->m_lf_flaps_data_ref));
			ImGui::PopItemWidth();

			//ImGui::Spacing();

			for (int index = 0; index < IVY_FS_MAX; index++)
			{
				char buffer[1024];

				ImGui::Text("Pos F%2i: ", index + 1);

				ImGui::SameLine();

				ImGui::PushItemWidth(100);
				sprintf(buffer, "##Pos F%2i", index + 1);
				ImGui::InputInt(buffer, &(pIvy->m_ivyAircraft->m_flaps_deploy_pos[index]), 1, 1, 0);

				ImGui::PopItemWidth();

				ImGui::SameLine();

				sprintf(buffer, "SET F%2i", index + 1);
				if (ImGui::Button(buffer)) pIvy->m_ivyAircraft->m_flaps_deploy_value[index] = pIvy->m_ivyAircraft->m_lf_flaps;
				ImGui::SameLine();
				sprintf(buffer, "DEL F%2i", index + 1);
				if (ImGui::Button(buffer)) pIvy->m_ivyAircraft->m_flaps_deploy_value[index] = -100;

				ImGui::SameLine();

				ImGui::Text("Val: %.2f", pIvy->m_ivyAircraft->m_flaps_deploy_value[index]);

				//sprintf(buffer, "F Val #%2i", index + 1);
				//ImGui::InputFloat(buffer, &(pIvy->m_ivyAircraft->m_flaps_deploy_value[index]), 0.01, 0.01, "%.2f", ImGuiInputTextFlags_ReadOnly);
				//ImGui::SameLine();

				


			}

			

			
			
		}
	}

	
	//ImGui::TextUnformatted("Hello, World!");

	//ImGui::Text("Window size: width = %f  height = %f", win_width, win_height);

	//ImGui::TextUnformatted("Two Widgets");
}
