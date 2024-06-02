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

#pragma once
#define IVY_FS_MAX 10
#define IVY_ENGN_MAX 3
#include "Ivy.h"

// OS X: we use this to convert our file path.
#if APL
#include <Carbon/Carbon.h>
#endif

// Your include paths for OpenAL may vary by platform.
#include "al.h"
#include "alc.h"

class MyIvyConfigAircraft
{
public:

	std::string m_config_path = "";
	int m_aircraft_number = 0;

	
	

	bool m_vspeeds_enabled = false;
	XPLMDataRef m_x_v1 = 0;
	XPLMDataRef m_x_vr = 0;
	XPLMDataRef m_x_v2 = 0;

	XPLMDataRef m_x_slats = 0;
	XPLMDataRef m_x_flaps = 0;

	XPLMDataTypeID m_t_v1 = 0;
	XPLMDataTypeID m_t_vr = 0;
	XPLMDataTypeID m_t_v2 = 0;

	XPLMDataTypeID m_t_slats = 0;
	XPLMDataTypeID m_t_flaps = 0;


	int m_li_v1 = 0;
	int m_li_vr = 0;
	int m_li_v2 = 0;

	float m_eng_egt_lightup = 300;
	float m_eng_n2_startup = 18;
	float m_eng_stab_n1_min = 20;
	float m_eng_stab_n1_delta_s = 0.2;

	float m_v1_offset = 0;
	float m_v1_k = 1;

	float m_vr_offset = 0;
	float m_vr_k = 1;

	float m_v2_offset = 0;
	float m_v2_k = 1;

	std::string m_lx_v1_data_ref = "";
	std::string m_lx_vr_data_ref = "";
	std::string m_lx_v2_data_ref = "";

	bool m_v_array = false;
	int m_v_array_size = 0;
	int m_v1_pos = 0;
	int m_v2_pos = 0;
	int m_vr_pos = 0;


	float m_n1_th[IVY_ENGN_MAX];
	float m_n1_sec[IVY_ENGN_MAX];
	float m_torque_th[IVY_ENGN_MAX];
	float m_torque_sec[IVY_ENGN_MAX];
	float m_itt_th[IVY_ENGN_MAX];
	float m_itt_sec[IVY_ENGN_MAX];
	float m_oil_press_th[IVY_ENGN_MAX];
	float m_oil_press_sec[IVY_ENGN_MAX];
	float m_oil_temp_th[IVY_ENGN_MAX];
	float m_oil_temp_sec[IVY_ENGN_MAX];
	float m_oil_temp_lo_th[IVY_ENGN_MAX];
	float m_oil_temp_lo_sec[IVY_ENGN_MAX];






	bool m_slats_enabled = false;
	bool m_flaps_enabled = false;
	float m_lf_flaps = 0;
	float m_lf_slats = 0;
	float m_flaps_tolerance = 0.05f;
	float m_slats_tolerance = 0.05f;
	std::string m_lf_flaps_data_ref = "sim/cockpit2/controls/flap_handle_request_ratio";
	std::string m_lf_slats_data_ref = "sim/cockpit2/controls/flap_handle_request_ratio";

	float m_flaps_deploy_value[IVY_FS_MAX];
	int m_flaps_deploy_pos[IVY_FS_MAX];
	int m_flaps_count_max = 10;
	int m_slats_count_max = 10;

	bool m_military = false;
	bool m_steep_descent = false;
	bool m_steep_bank = false;
	bool m_max_alt_enable = false;
	int m_max_alt_value = 100;
	bool m_localizer_enable = false;
	float m_to_power_lever = 0.7f;
	float m_to_power_tolerance = 1.0f;

	float m_slats_deploy_value[IVY_FS_MAX];
	int m_slats_deploy_pos[IVY_FS_MAX];

	bool m_dataref_init = false;

	bool m_is_206B3 = false;
	float m_206B3_MTBF = 1;

	bool m_is_206L3 = false;
	float m_206L3_MTBF = 1;

	char m_name[1024] = "UnconfiguredIvyAircraft";

	MyIvyConfigAircraft(const char * aircraft_config_path, int aircraft_number = 0, std::string m_acf_name = "");
	~MyIvyConfigAircraft();

	float GetSingleDataref(XPLMDataRef x_dataref, XPLMDataTypeID t_dataref, int array_pos = 0);

	void InitDataRefs();
	void UpdateData();
	void ReadConfigFile(const char * aircraft_config_path);
	void WriteConfigFile();
};

