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

#include "MyIvyConfiguration.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>


MyIvyConfiguration::MyIvyConfiguration()
{
	char buffer[2048];
	XPLMGetSystemPath(buffer);
	m_system_path = buffer;
	
	ivy_output_file << "Ivy System Path" << m_system_path  <<  std::endl;
	ivy_output_file.flush();

	m_dir_sep = XPLMGetDirectorySeparator();

	ivy_output_file << "Ivy directory separator" << m_dir_sep << std::endl;
	ivy_output_file.flush();
	//m_dir_sep = buffer;

	m_mp3_dir = "IvyFunny1";
	SetAudioDirectory();
	m_ini_path = m_system_path + m_dir_sep + "Resources" + m_dir_sep + "plugins" + m_dir_sep + "Ivy" + m_dir_sep + "Ivy.ini";
	m_config_path = m_system_path + m_dir_sep + "Resources" + m_dir_sep + "plugins" + m_dir_sep + "Ivy" + m_dir_sep + "IvyConfig" + m_dir_sep;
	m_logbook_path = m_system_path + m_dir_sep + "Resources" + m_dir_sep + "plugins" + m_dir_sep + "Ivy" + m_dir_sep + "IvyConfig" + m_dir_sep + "IvyLogbook.txt";
	m_errorlog_path = m_system_path + m_dir_sep + "Resources" + m_dir_sep + "plugins" + m_dir_sep + "Ivy" + m_dir_sep + "IvyConfig" + m_dir_sep + "IvyErrorLog.xml";
	m_output_path = m_config_path + "IvyLog.txt";

	m_audio_names.push_back("Ivy (US) - Funny");
	m_audio_dirs.push_back("IvyFunny1");

	m_audio_names.push_back("Ivy (US) - Funny, Normal Announcements");
	m_audio_dirs.push_back("IvyFunny2");

	m_audio_names.push_back("Brian (UK) - Funny");
	m_audio_dirs.push_back("BrianFunny");

	m_audio_names.push_back("Ivy (US)");
	m_audio_dirs.push_back("IvySerious");

	m_audio_names.push_back("Matthew (US)");
	m_audio_dirs.push_back("MatthewSerious");

	m_audio_names.push_back("Salli (US)");
	m_audio_dirs.push_back("SalliSerious");

	m_audio_names.push_back("Joey (US)");
	m_audio_dirs.push_back("JoeySerious");

	m_audio_names.push_back("Emma (UK)");
	m_audio_dirs.push_back("EmmaSerious");

	m_audio_names.push_back("Brian (UK)");
	m_audio_dirs.push_back("BrianSerious");

	m_audio_names.push_back("Nicole (AUS)");
	m_audio_dirs.push_back("NicoleSerious");

	m_audio_names.push_back("Russel (AUS)");
	m_audio_dirs.push_back("RusselSerious");

}


MyIvyConfiguration::~MyIvyConfiguration()
{
}

void MyIvyConfiguration::WriteConfig()
{
	boost::property_tree::ptree pt;

	pt.put("IVY_SETTINGS.mp3_dir", m_mp3_dir);
	pt.put("IVY_SETTINGS.ivy_volume", m_ivyVolume);

	pt.put("IVY_SETTINGS.ivy_enable", m_ivy_enable);
	pt.put("IVY_SETTINGS.log_enable", m_log_enable);
	pt.put("IVY_SETTINGS.callouts_enable", m_callouts_enable);
	pt.put("IVY_SETTINGS.errors_enable", m_errors_enable);
	pt.put("IVY_SETTINGS.baro_is_error", m_baro_is_error);
	pt.put("IVY_SETTINGS.pre_warnings", m_pre_warnings);

	pt.put("IVY_SETTINGS.passengers_screaming", m_passengers_screaming);
	pt.put("IVY_SETTINGS.passengers_applause", m_passengers_applause);


	pt.put("IVY_SETTINGS.passengers_enabled", m_passengers_enabled);
	pt.put("IVY_SETTINGS.ouch_enabled", m_ouch_enabled);

	pt.put("IVY_SETTINGS.pos_rate_climb", m_pos_rate_climb);
	pt.put("IVY_SETTINGS.ivy_ouch_g", m_ivy_ouch_g);
	pt.put("IVY_SETTINGS.brake_max_forward_g", m_brake_max_forward_g);
	pt.put("IVY_SETTINGS.alt_landing_lights_low", m_alt_landing_lights_low);
	pt.put("IVY_SETTINGS.alt_landing_lights_high", m_alt_landing_lights_high);
	pt.put("IVY_SETTINGS.night_world_light_precent", m_night_world_light_precent);
	pt.put("IVY_SETTINGS.taxi_ground_speed_min", m_taxi_ground_speed_min);
	pt.put("IVY_SETTINGS.vis_is_fog", m_vis_is_fog);
	pt.put("IVY_SETTINGS.cab_rate_low", m_cab_rate_low);
	pt.put("IVY_SETTINGS.cab_rate_high", m_cab_rate_high);
	pt.put("IVY_SETTINGS.cab_rate_reset_hysteresis", m_cab_rate_reset_hysteresis);
	pt.put("IVY_SETTINGS.bank_reset_low", m_bank_reset_low);
	pt.put("IVY_SETTINGS.bank_low", m_bank_low);
	pt.put("IVY_SETTINGS.bank_high", m_bank_high);
	pt.put("IVY_SETTINGS.bank_xhigh", m_bank_xhigh);
	pt.put("IVY_SETTINGS.pitch_reset_low", m_pitch_reset_low);
	pt.put("IVY_SETTINGS.pitch_low", m_pitch_low);
	pt.put("IVY_SETTINGS.pitch_high", m_pitch_high);
	pt.put("IVY_SETTINGS.max_g_down_low_reset", m_max_g_down_low_reset);
	pt.put("IVY_SETTINGS.max_g_down_low", m_max_g_down_low);
	pt.put("IVY_SETTINGS.max_g_down_high", m_max_g_down_high);
	pt.put("IVY_SETTINGS.max_g_down_xhigh", m_max_g_down_xhigh);
	pt.put("IVY_SETTINGS.trans_alt", m_trans_alt);
	pt.put("IVY_SETTINGS.trans_hysteresis", m_trans_hysteresis);
	pt.put("IVY_SETTINGS.baro_tolerance", m_baro_tolerance);
	pt.put("IVY_SETTINGS.baro_alt_low", m_baro_alt_low);
	pt.put("IVY_SETTINGS.ice_low", m_ice_low);
	pt.put("IVY_SETTINGS.ice_high", m_ice_high);
	pt.put("IVY_SETTINGS.cab_press_low", m_cab_press_low);
	pt.put("IVY_SETTINGS.cab_press_high", m_cab_press_high);
	pt.put("IVY_SETTINGS.non_smoking_annoucetime", m_non_smoking_annoucetime);
	pt.put("IVY_SETTINGS.decition_height_arm", m_decition_height_arm);
	pt.put("IVY_SETTINGS.decition_height_plus", m_decition_height_plus);

	pt.put("IVY_SETTINGS.kt60_enabled", m_kt60_enabled);
	pt.put("IVY_SETTINGS.kt80_enabled", m_kt80_enabled);
	pt.put("IVY_SETTINGS.kt100_enabled", m_kt100_enabled);
	pt.put("IVY_SETTINGS.kt60_landing_enabled", m_kt60__landing_enabled);

	pt.put("IVY_SETTINGS.log_window_pos_x", m_log_window_pos_x);
	pt.put("IVY_SETTINGS.log_window_pos_y", m_log_window_pos_y);
	pt.put("IVY_SETTINGS.log_window_height", m_log_window_height);
	pt.put("IVY_SETTINGS.log_window_width", m_log_window_width);
	pt.put("IVY_SETTINGS.log_window_entries", m_log_window_entries);
	pt.put("IVY_SETTINGS.log_afc_name_length", m_log_afc_name_length);


	pt.put("IVY_SETTINGS.aoc_count_total", m_aoc_count_total);
	pt.put("IVY_SETTINGS.aoc_level_high", m_aoc_level_high);
	pt.put("IVY_SETTINGS.aoc_level_med", m_aoc_level_med);
	pt.put("IVY_SETTINGS.aoc_level_low", m_aoc_level_low);

	pt.put("IVY_SETTINGS.hrm_enabled", m_hrm_enabled);

	pt.put("IVY_SETTINGS.airspeed_alive", m_airspeed_alive);
	pt.put("IVY_SETTINGS.to_power_speed", m_to_power_speed);
	


	write_ini(m_ini_path, pt);
}

void MyIvyConfiguration::ReadConfig()
{

	ivy_output_file << "Ivy Read Config: " << m_ini_path << std::endl;
	ivy_output_file.flush();
	
	boost::property_tree::ptree pt;
	try
	{
		boost::property_tree::ini_parser::read_ini(m_ini_path, pt);
	}
	catch (...) 
	{
		ivy_output_file << "Could not read config file" << std::endl;
		ivy_output_file.flush();
		return;
	}
	

	try { m_mp3_dir = pt.get<std::string>("IVY_SETTINGS.mp3_dir"); }
	catch (...) { IvyDebugString("IvyConfiguration: Entry not found.\n"); }


	try { m_ivy_enable = pt.get<bool>("IVY_SETTINGS.ivy_enable"); }
	catch (...) { IvyDebugString("IvyConfiguration: Entry not found.\n"); }

	try { m_log_enable = pt.get<bool>("IVY_SETTINGS.log_enable"); }
	catch (...) { IvyDebugString("IvyConfiguration: Entry not found.\n"); }

	try { m_callouts_enable = pt.get<bool>("IVY_SETTINGS.callouts_enable"); }
	catch (...) { IvyDebugString("IvyConfiguration: Entry not found.\n"); }
	try { m_errors_enable = pt.get<bool>("IVY_SETTINGS.errors_enable"); }
	catch (...) { IvyDebugString("IvyConfiguration: Entry not found.\n"); }
	try { m_baro_is_error = pt.get<bool>("IVY_SETTINGS.baro_is_error"); }
	catch (...) { IvyDebugString("IvyConfiguration: Entry not found.\n"); }

	try { m_pre_warnings = pt.get<bool>("IVY_SETTINGS.pre_warnings"); }
	catch (...) { IvyDebugString("IvyConfiguration: Entry not found.\n"); }

	try { m_passengers_screaming = pt.get<bool>("IVY_SETTINGS.passengers_screaming"); }
	catch (...) { IvyDebugString("IvyConfiguration: Entry not found.\n"); }

	try { m_passengers_applause = pt.get<bool>("IVY_SETTINGS.passengers_applause"); }
	catch (...) { IvyDebugString("IvyConfiguration: Entry not found.\n"); }

	try { m_ouch_enabled = pt.get<bool>("IVY_SETTINGS.ouch_enabled"); }
	catch (...) { IvyDebugString("IvyConfiguration: Entry not found.\n"); }



	try { m_passengers_enabled = pt.get<bool>("IVY_SETTINGS.passengers_enabled"); }
	catch (...) { IvyDebugString("IvyConfiguration: Entry not found.\n"); }

	try { m_pos_rate_climb = pt.get<float>("IVY_SETTINGS.pos_rate_climb"); }
	catch (...) { IvyDebugString("IvyConfiguration: Entry not found.\n"); }
	try { m_ivy_ouch_g = pt.get<float>("IVY_SETTINGS.ivy_ouch_g"); }
	catch (...) { IvyDebugString("IvyConfiguration: Entry not found.\n"); }
	try { m_brake_max_forward_g = pt.get<float>("IVY_SETTINGS.brake_max_forward_g"); }
	catch (...) { IvyDebugString("IvyConfiguration: Entry not found.\n"); }
	try { m_alt_landing_lights_low = pt.get<float>("IVY_SETTINGS.alt_landing_lights_low"); }
	catch (...) { IvyDebugString("IvyConfiguration: Entry not found.\n"); }
	try { m_alt_landing_lights_high = pt.get<float>("IVY_SETTINGS.alt_landing_lights_high"); }
	catch (...) { IvyDebugString("IvyConfiguration: Entry not found.\n"); }
	try { m_night_world_light_precent = pt.get<float>("IVY_SETTINGS.night_world_light_precent"); }
	catch (...) { IvyDebugString("IvyConfiguration: Entry not found.\n"); }
	try { m_taxi_ground_speed_min = pt.get<float>("IVY_SETTINGS.taxi_ground_speed_min"); }
	catch (...) { IvyDebugString("IvyConfiguration: Entry not found.\n"); }
	try { m_vis_is_fog = pt.get<float>("IVY_SETTINGS.vis_is_fog"); }
	catch (...) { IvyDebugString("IvyConfiguration: Entry not found.\n"); }
	try { m_cab_rate_low = pt.get<float>("IVY_SETTINGS.cab_rate_low"); }
	catch (...) { IvyDebugString("IvyConfiguration: Entry not found.\n"); }
	try { m_cab_rate_high = pt.get<float>("IVY_SETTINGS.cab_rate_high"); }
	catch (...) { IvyDebugString("IvyConfiguration: Entry not found.\n"); }
	try { m_cab_rate_reset_hysteresis = pt.get<float>("IVY_SETTINGS.cab_rate_reset_hysteresis"); }
	catch (...) { IvyDebugString("IvyConfiguration: Entry not found.\n"); }

	try { m_bank_reset_low = pt.get<float>("IVY_SETTINGS.bank_reset_low"); }
	catch (...) { IvyDebugString("IvyConfiguration: Entry not found.\n"); }
	try { m_bank_low = pt.get<float>("IVY_SETTINGS.bank_low"); }
	catch (...) { IvyDebugString("IvyConfiguration: Entry not found.\n"); }
	try { m_bank_high = pt.get<float>("IVY_SETTINGS.bank_high"); }
	catch (...) { IvyDebugString("IvyConfiguration: Entry not found.\n"); }
	try { m_bank_xhigh = pt.get<float>("IVY_SETTINGS.bank_xhigh"); }
	catch (...) { IvyDebugString("IvyConfiguration: Entry not found.\n"); }

	try { m_pitch_reset_low = pt.get<float>("IVY_SETTINGS.pitch_reset_low"); }
	catch (...) { IvyDebugString("IvyConfiguration: Entry not found.\n"); }
	try { m_pitch_low = pt.get<float>("IVY_SETTINGS.pitch_low"); }
	catch (...) { IvyDebugString("IvyConfiguration: Entry not found.\n"); }
	try { m_pitch_high = pt.get<float>("IVY_SETTINGS.pitch_high"); }
	catch (...) { IvyDebugString("IvyConfiguration: Entry not found.\n"); }

	try { m_max_g_down_low_reset = pt.get<float>("IVY_SETTINGS.max_g_down_low_reset"); }
	catch (...) { IvyDebugString("IvyConfiguration: Entry not found.\n"); }
	try { m_max_g_down_low = pt.get<float>("IVY_SETTINGS.max_g_down_low"); }
	catch (...) { IvyDebugString("IvyConfiguration: Entry not found.\n"); }
	try { m_max_g_down_high = pt.get<float>("IVY_SETTINGS.max_g_down_high"); }
	catch (...) { IvyDebugString("IvyConfiguration: Entry not found.\n"); }
	try { m_max_g_down_xhigh = pt.get<float>("IVY_SETTINGS.max_g_down_xhigh"); }
	catch (...) { IvyDebugString("IvyConfiguration: Entry not found.\n"); }


	try { m_trans_alt = pt.get<float>("IVY_SETTINGS.trans_alt"); }
	catch (...) { IvyDebugString("IvyConfiguration: Entry not found.\n"); }
	try { m_trans_hysteresis = pt.get<float>("IVY_SETTINGS.trans_hysteresis"); }
	catch (...) { IvyDebugString("IvyConfiguration: Entry not found.\n"); }
	try { m_baro_tolerance = pt.get<float>("IVY_SETTINGS.baro_tolerance"); }
	catch (...) { IvyDebugString("IvyConfiguration: Entry not found.\n"); }
	try { m_baro_alt_low = pt.get<float>("IVY_SETTINGS.baro_alt_low"); }
	catch (...) { IvyDebugString("IvyConfiguration: Entry not found.\n"); }

	try { m_ice_low = pt.get<float>("IVY_SETTINGS.ice_low"); }
	catch (...) { IvyDebugString("IvyConfiguration: Entry not found.\n"); }
	try { m_ice_high = pt.get<float>("IVY_SETTINGS.ice_high"); }
	catch (...) { IvyDebugString("IvyConfiguration: Entry not found.\n"); }

	try { m_cab_press_low = pt.get<float>("IVY_SETTINGS.cab_press_low"); }
	catch (...) { IvyDebugString("IvyConfiguration: Entry not found.\n"); }
	try { m_cab_press_high = pt.get<float>("IVY_SETTINGS.cab_press_high"); }
	catch (...) { IvyDebugString("IvyConfiguration: Entry not found.\n"); }

	try { m_non_smoking_annoucetime = pt.get<float>("IVY_SETTINGS.non_smoking_annoucetime"); }
	catch (...) { IvyDebugString("IvyConfiguration: Entry not found.\n"); }

	try { m_decition_height_arm = pt.get<float>("IVY_SETTINGS.decition_height_arm"); }
	catch (...) { IvyDebugString("IvyConfiguration: Entry not found.\n"); }
	try { m_decition_height_plus = pt.get<float>("IVY_SETTINGS.decition_height_plus"); }
	catch (...) { IvyDebugString("IvyConfiguration: Entry not found.\n"); }

	try { m_kt60_enabled = pt.get<bool>("IVY_SETTINGS.kt60_enabled"); }
	catch (...) { IvyDebugString("IvyConfiguration: Entry not found.\n"); }
	try { m_kt80_enabled = pt.get<bool>("IVY_SETTINGS.kt80_enabled"); }
	catch (...) { IvyDebugString("IvyConfiguration: Entry not found.\n"); }
	try { m_kt100_enabled = pt.get<bool>("IVY_SETTINGS.kt100_enabled"); }
	catch (...) { IvyDebugString("IvyConfiguration: Entry not found.\n"); }

	try { m_kt60__landing_enabled = pt.get<bool>("IVY_SETTINGS.kt60_landing_enabled"); }
	catch (...) { IvyDebugString("IvyConfiguration: Entry not found.\n"); }


	try { m_log_window_pos_x = pt.get<float>("IVY_SETTINGS.log_window_pos_x"); }
	catch (...) { IvyDebugString("IvyConfiguration: Entry not found.\n"); }
	try { m_log_window_pos_y = pt.get<float>("IVY_SETTINGS.log_window_pos_y"); }
	catch (...) { IvyDebugString("IvyConfiguration: Entry not found.\n"); }
	try { m_log_window_height = pt.get<float>("IVY_SETTINGS.log_window_height"); }
	catch (...) { IvyDebugString("IvyConfiguration: Entry not found.\n"); }
	try { m_log_window_width = pt.get<float>("IVY_SETTINGS.log_window_width"); }
	catch (...) { IvyDebugString("IvyConfiguration: Entry not found.\n"); }
	try { m_log_window_entries = pt.get<float>("IVY_SETTINGS.log_window_entries"); }
	catch (...) { IvyDebugString("IvyConfiguration: Entry not found.\n"); }
	try { m_log_afc_name_length = pt.get<float>("IVY_SETTINGS.log_afc_name_length"); }
	catch (...) { IvyDebugString("IvyConfiguration: Entry not found.\n"); }

	try { m_aoc_count_total = pt.get<int>("IVY_SETTINGS.aoc_count_total"); }
	catch (...) { IvyDebugString("IvyConfiguration: Entry not found.\n"); }
	try { m_aoc_level_high = pt.get<int>("IVY_SETTINGS.aoc_level_high"); }
	catch (...) { IvyDebugString("IvyConfiguration: Entry not found.\n"); }
	try { m_aoc_level_med = pt.get<int>("IVY_SETTINGS.aoc_level_med"); }
	catch (...) { IvyDebugString("IvyConfiguration: Entry not found.\n"); }
	try { m_aoc_level_low = pt.get<int>("IVY_SETTINGS.aoc_level_low"); }
	catch (...) { IvyDebugString("IvyConfiguration: Entry not found.\n"); }

	try { m_hrm_enabled = pt.get<bool>("IVY_SETTINGS.hrm_enabled"); }
	catch (...) { IvyDebugString("IvyConfiguration: Entry not found.\n"); }

	try { m_airspeed_alive = pt.get<bool>("IVY_SETTINGS.airspeed_alive"); }
	catch (...) { IvyDebugString("IvyConfiguration: Entry not found.\n"); }
	try { m_to_power_speed = pt.get<bool>("IVY_SETTINGS.to_power_speed"); }
	catch (...) { IvyDebugString("IvyConfiguration: Entry not found.\n"); }

	try { m_ivyVolume = pt.get<float>("IVY_SETTINGS.ivy_volume"); }
	catch (...) { IvyDebugString("IvyConfiguration: Entry not found.\n"); }


	SetAudioDirectory();
}

void MyIvyConfiguration::SetAudioDirectory()
{
	m_mp3_path = m_system_path + m_dir_sep + "Resources" + m_dir_sep + "plugins" + m_dir_sep + "Ivy" + m_dir_sep + m_mp3_dir + m_dir_sep;
	m_number_path = m_mp3_path + "numbers" + m_dir_sep;
}
