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

#include "Ivy.h"

// OS X: we use this to convert our file path.
#if APL
#include <Carbon/Carbon.h>
#endif

// Your include paths for OpenAL may vary by platform.
#include "al.h"
#include "alc.h"

class MyIvyConfiguration
{
public:
	std::string m_system_path = "";
	std::string m_mp3_dir = "IvyAudio";
	std::string m_mp3_path = "";
	std::string m_number_path = "";
	std::string m_ini_path = "";
	std::string m_config_path = "";
	std::string m_logbook_path = "";
	std::string m_errorlog_path = "";
	std::string m_dir_sep = "/";
	std::string m_output_path = "";

	std::vector<std::string> m_audio_names;
	std::vector<std::string> m_audio_dirs;

	bool m_ivy_enable = true;
	bool m_log_enable = true;

	bool m_callouts_enable = true;
	bool m_errors_enable = true;
	bool m_baro_is_error = false;

	bool m_pre_warnings = false;
	bool m_ouch_enabled = true;

	bool m_passengers_screaming = true;
	bool m_passengers_applause = true;

	float m_data_rate = 0.1;
	int  m_disable_after_loading = 10; //debug, 20 = normal
	int m_deact_after_queue = 0;

	bool m_passengers_enabled = true;

	float m_pos_rate_climb = 100;
	float m_ivy_ouch_g = 1.5;
	float m_brake_max_forward_g = 0.5;
	float m_alt_landing_lights_low = 1000;
	float m_alt_landing_lights_high = 12000;
	float m_night_world_light_precent = 0.5;
	float m_taxi_ground_speed_min = 5;
	float m_vis_is_fog = 5000;
	float m_cab_rate_low = -1500;
	float m_cab_rate_high = -2500;
	float m_cab_rate_reset_hysteresis = 500;

	bool m_kt60__landing_enabled = true;
	bool m_kt60_enabled = false;
	bool m_kt80_enabled = true;
	bool m_kt100_enabled = false;

	float m_bank_reset_low = 15;
	float m_bank_low = 28;
	float m_bank_high = 35;
	float m_bank_xhigh = 45;

	float m_pitch_reset_low = -5;
	float m_pitch_low = -10;
	float m_pitch_high = -20;

	float m_max_g_down_low_reset = 1.5;
	float m_max_g_down_low = 2;
	float m_max_g_down_high = 3;
	float m_max_g_down_xhigh = 5;

	float m_trans_alt = 18000;
	float m_trans_hysteresis = 1000;
	float m_baro_tolerance = 3;
	float m_baro_alt_low = 3000;

	float m_ice_low = 0.05;
	float m_ice_high = 0.2;

	//float m_carb_ice_low 				= 0.02;
	//float m_carb_ice_high 				= 0.10;

	float m_cab_press_low = 13000;
	float m_cab_press_high = 20000;

	float m_non_smoking_annoucetime = 3;
	float m_decition_height_arm = 500;
	float m_decition_height_plus = 100;

	int m_log_window_pos_x = 300;
	int m_log_window_pos_y = 550;
	int m_log_window_height = 350;
	int m_log_window_width = 1300;
	int m_log_window_entries = 15;
	int m_log_afc_name_length = 40;

	int m_aoc_count_total = 20;
	int m_aoc_level_high = 10;
	int m_aoc_level_med = 7;
	int m_aoc_level_low = 3;

	bool m_hrm_enabled = true;

	float m_airspeed_alive = 35;
	float m_to_power_speed = 20;
	int m_to_power_cycles = 10;

	float m_ivyVolume = 1.0;

	MyIvyConfiguration();
	~MyIvyConfiguration();

	void WriteConfig();
	void ReadConfig();

	void SetAudioDirectory();

};

