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

#include "MyIvyConfigAircraft.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>





MyIvyConfigAircraft::MyIvyConfigAircraft(const char* aircraft_config_path, int aircraft_number, std::string m_acf_name)
{
	for (int index_number = 0; index_number < IVY_ENGN_MAX; index_number++)
	{
		m_n1_th[index_number] = 0;
		m_n1_sec[index_number] = 0;
		m_torque_th[index_number] = 0;
		m_torque_sec[index_number] = 0;
		m_itt_th[index_number] = 0;
		m_itt_sec[index_number] = 0;
		m_oil_press_th[index_number] = 0;
		m_oil_press_sec[index_number] = 0;
		m_oil_temp_th[index_number] = 0;
		m_oil_temp_sec[index_number] = 0;
		m_oil_temp_lo_th[index_number] = 0;
		m_oil_temp_lo_sec[index_number] = 0;
	}

	for (int index_number = 0; index_number < IVY_FS_MAX; index_number++)
	{
		m_flaps_deploy_value[index_number] = -100;
		m_flaps_deploy_pos[index_number] = 0;

		m_slats_deploy_value[index_number] = -100;
		m_slats_deploy_pos[index_number] = 0;
	}

	if (aircraft_config_path != NULL)
	{
		m_config_path = aircraft_config_path;
		m_aircraft_number = aircraft_number;

		std::string aircraft_file_name = "";

		// Get new Aircraft Number
		if (m_aircraft_number == 0)
		{
			strcpy(m_name, m_acf_name.c_str());

			bool found = false;
			for (int index = 1; (index < IVY_MAX_AIRCRAFT_CONFIG) && (!found); index++)
			{
				aircraft_file_name = m_config_path + "IvyAircraft_" + std::to_string(index) + ".ini";
				if (!file_exists(aircraft_file_name))
				{
					m_aircraft_number = index;
					found = true;
					WriteConfigFile();
				}

			}

		}
		else
		{
			std::string aircraft_file_name = m_config_path + "IvyAircraft_" + std::to_string(m_aircraft_number) + ".ini";

			try
			{
				ivy_output_file << "Reading aircraft:" << aircraft_file_name << std::endl;
				ReadConfigFile(aircraft_file_name.c_str());
			}
			catch (...)
			{
				ivy_output_file << "Could not read aircraft:" << aircraft_file_name << std::endl;
				ivy_output_file.flush();
			}
		}
	}
		
}


MyIvyConfigAircraft::~MyIvyConfigAircraft()
{
}

void MyIvyConfigAircraft::InitDataRefs()
{
	m_dataref_init = true;

	m_x_v1 = 0;
	m_x_vr = 0;
	m_x_v2 = 0;

	m_t_v1 = 0;
	m_t_vr = 0;
	m_t_v2 = 0;

	m_x_slats = 0;
	m_x_flaps = 0;

	m_t_slats = 0;
	m_t_flaps = 0;

	//if (m_vspeeds_enabled == true)
	{
		m_x_v1 = XPLMFindDataRef(m_lx_v1_data_ref.c_str());
		m_x_vr = XPLMFindDataRef(m_lx_vr_data_ref.c_str());
		m_x_v2 = XPLMFindDataRef(m_lx_v2_data_ref.c_str());

		if (m_x_v1 != NULL) m_t_v1 = XPLMGetDataRefTypes(m_x_v1);
		if (m_x_vr != NULL) m_t_vr = XPLMGetDataRefTypes(m_x_vr);
		if (m_x_v2 != NULL) m_t_v2 = XPLMGetDataRefTypes(m_x_v2);
	}

	//if (m_slats_enabled == true)
	{
		m_x_slats = XPLMFindDataRef(m_lf_slats_data_ref.c_str());

		if (m_x_slats != NULL) m_t_slats = XPLMGetDataRefTypes(m_x_slats);
	}

	//if (m_flaps_enabled == true)
	{
		m_x_flaps = XPLMFindDataRef(m_lf_flaps_data_ref.c_str());
		if (m_x_flaps != NULL) m_t_flaps = XPLMGetDataRefTypes(m_x_flaps);
	}
}

float MyIvyConfigAircraft::GetSingleDataref(XPLMDataRef x_dataref, XPLMDataTypeID t_dataref, int array_pos)
{
	if ((x_dataref == NULL) || (array_pos >= 100) || (array_pos < 0)) return 0;

	float value;
	
	if (t_dataref & xplmType_Int)			return XPLMGetDatai(x_dataref);
	else if (t_dataref & xplmType_Float)	return XPLMGetDataf(x_dataref);
	else if (t_dataref & xplmType_Double)	return XPLMGetDatad(x_dataref);
	else if (t_dataref & xplmType_FloatArray)
	{
		float v_array[100];
		memset(v_array, 0, sizeof(v_array));
		XPLMGetDatavf(x_dataref, v_array, 0, 100);
		return v_array[array_pos];
	}
	else if (t_dataref & xplmType_IntArray)
	{
		int v_array[100];
		memset(v_array, 0, sizeof(v_array));
		XPLMGetDatavi(x_dataref, v_array, 0, 100);
		return v_array[array_pos];
	}
	return 0;
}

void MyIvyConfigAircraft::UpdateData()
{
	if (m_dataref_init == false) 
		InitDataRefs();

	if (m_vspeeds_enabled == true)
	{
		m_li_v1 = (int)GetSingleDataref(m_x_v1, m_t_v1, m_v1_pos);
		m_li_vr = (int)GetSingleDataref(m_x_vr, m_t_vr, m_vr_pos);
		m_li_v2 = (int)GetSingleDataref(m_x_v2, m_t_v2, m_v2_pos);
	}

	if (m_slats_enabled == true)
		if (m_lf_slats_data_ref != "")	m_lf_slats = GetSingleDataref(m_x_slats, m_t_slats);

	if (m_flaps_enabled == true)
		if (m_lf_flaps_data_ref != "")	m_lf_flaps = GetSingleDataref(m_x_flaps, m_t_flaps);
}

void MyIvyConfigAircraft::WriteConfigFile()
{
	if ((m_aircraft_number == 0) || (m_config_path.size() == 0)) return;

	std::string aircraft_file_name = m_config_path + "IvyAircraft_" + std::to_string(m_aircraft_number) + ".ini";

	boost::property_tree::ptree pt;
	
	pt.put("IVY_AIRCRAFT.aircraft_name", m_name);

	pt.put("IVY_AIRCRAFT.military", m_military);
	pt.put("IVY_AIRCRAFT.206B3", m_is_206B3);
	pt.put("IVY_AIRCRAFT.206B3_mtbf", m_206B3_MTBF);
	pt.put("IVY_AIRCRAFT.206L3", m_is_206L3);
	pt.put("IVY_AIRCRAFT.206L3_mtbf", m_206L3_MTBF);

	pt.put("IVY_AIRCRAFT.vspeeds_enabled", m_vspeeds_enabled);
	pt.put("IVY_AIRCRAFT.v1_data_ref", m_lx_v1_data_ref);
	pt.put("IVY_AIRCRAFT.vr_data_ref", m_lx_vr_data_ref);
	pt.put("IVY_AIRCRAFT.v2_data_ref", m_lx_v2_data_ref);
	pt.put("IVY_AIRCRAFT.v1_pos", m_v1_pos);
	pt.put("IVY_AIRCRAFT.vr_pos", m_vr_pos);
	pt.put("IVY_AIRCRAFT.v2_pos", m_v2_pos);

	pt.put("IVY_AIRCRAFT.v1_static", m_li_v1);
	pt.put("IVY_AIRCRAFT.vr_static", m_li_vr);
	pt.put("IVY_AIRCRAFT.v2_static", m_li_v2);

	pt.put("IVY_AIRCRAFT.steep_descent", m_steep_descent);
	pt.put("IVY_AIRCRAFT.steep_bank", m_steep_bank);
	pt.put("IVY_AIRCRAFT.max_alt_enable", m_max_alt_enable);
	pt.put("IVY_AIRCRAFT.max_alt_value", m_max_alt_value);
	pt.put("IVY_AIRCRAFT.localizer_enable", m_localizer_enable);
	pt.put("IVY_AIRCRAFT.to_power_lever", m_to_power_lever);
	pt.put("IVY_AIRCRAFT.to_power_tolerance", m_to_power_tolerance);


	pt.put("IVY_AIRCRAFT.eng_egt_lightup", m_eng_egt_lightup);
	pt.put("IVY_AIRCRAFT.eng_n2_startup", m_eng_n2_startup);
	pt.put("IVY_AIRCRAFT.eng_stab_n1_min", m_eng_stab_n1_min);
	pt.put("IVY_AIRCRAFT.eng_stab_n1_delta_s", m_eng_stab_n1_delta_s);

	pt.put("IVY_AIRCRAFT.slats_enabled", m_slats_enabled);
	pt.put("IVY_AIRCRAFT.slats_data_ref", m_lf_slats_data_ref);
	pt.put("IVY_AIRCRAFT.slats_tolerance", m_slats_tolerance);

	pt.put("IVY_AIRCRAFT.flaps_enabled", m_flaps_enabled);
	pt.put("IVY_AIRCRAFT.flaps_data_ref", m_lf_flaps_data_ref);
	pt.put("IVY_AIRCRAFT.flaps_tolerance", m_flaps_tolerance);

	for (int index_number = 0; index_number < IVY_FS_MAX; index_number++)
	{
		char buffer[1024];

		sprintf(buffer, "IVY_AIRCRAFT.slats_value_%i", index_number + 1);
		pt.put(buffer, m_slats_deploy_value[index_number]);


		sprintf(buffer, "IVY_AIRCRAFT.slats_position_%i", index_number + 1);
		pt.put(buffer, m_slats_deploy_pos[index_number]);
	}

	for (int index_number = 0; index_number < IVY_FS_MAX; index_number++)
	{
		char buffer[1024];

		sprintf(buffer, "IVY_AIRCRAFT.flaps_value_%i", index_number + 1);
		pt.put(buffer, m_flaps_deploy_value[index_number]);


		sprintf(buffer, "IVY_AIRCRAFT.flaps_position_%i", index_number + 1);
		pt.put(buffer, m_flaps_deploy_pos[index_number]);
	}

	for (int index_number = 0; index_number < IVY_ENGN_MAX; index_number++)
	{
		char buffer[1024];

		sprintf(buffer, "IVY_AIRCRAFT.m_n1_th_%i", index_number + 1);
		pt.put(buffer, m_n1_th[index_number]);
		sprintf(buffer, "IVY_AIRCRAFT.m_n1_sec_%i", index_number + 1);
		pt.put(buffer, m_n1_sec[index_number]);
		sprintf(buffer, "IVY_AIRCRAFT.m_torque_th_%i", index_number + 1);
		pt.put(buffer, m_torque_th[index_number]);
		sprintf(buffer, "IVY_AIRCRAFT.m_torque_sec_%i", index_number + 1);
		pt.put(buffer, m_torque_sec[index_number]);
		sprintf(buffer, "IVY_AIRCRAFT.m_itt_th_%i", index_number + 1);
		pt.put(buffer, m_itt_th[index_number]);
		sprintf(buffer, "IVY_AIRCRAFT.m_itt_sec_%i", index_number + 1);
		pt.put(buffer, m_itt_sec[index_number]);
		sprintf(buffer, "IVY_AIRCRAFT.m_oil_press_th_%i", index_number + 1);
		pt.put(buffer, m_oil_press_th[index_number]);
		sprintf(buffer, "IVY_AIRCRAFT.m_oil_press_sec_%i", index_number + 1);
		pt.put(buffer, m_oil_press_sec[index_number]);
		sprintf(buffer, "IVY_AIRCRAFT.m_oil_temp_hi_th_%i", index_number + 1);
		pt.put(buffer, m_oil_temp_th[index_number]);
		sprintf(buffer, "IVY_AIRCRAFT.m_oil_temp_hi_sec_%i", index_number + 1);
		pt.put(buffer, m_oil_temp_sec[index_number]);
		sprintf(buffer, "IVY_AIRCRAFT.m_oil_temp_lo_th_%i", index_number + 1);
		pt.put(buffer, m_oil_temp_lo_th[index_number]);
		sprintf(buffer, "IVY_AIRCRAFT.m_oil_temp_lo_sec_%i", index_number + 1);
		pt.put(buffer, m_oil_temp_lo_sec[index_number]);


	}



	write_ini(aircraft_file_name, pt);


}

void MyIvyConfigAircraft::ReadConfigFile(const char * aircraft_config_path)
{
	m_dataref_init = false;

	boost::property_tree::ptree pt;
	boost::property_tree::ini_parser::read_ini(aircraft_config_path, pt);

	try { strcpy(m_name, pt.get<std::string>("IVY_AIRCRAFT.aircraft_name").c_str()); }
	catch(...) { IvyDebugString("IvyAircraftConfig: No aircraft_name.\n"); }

	try { m_military = pt.get<bool>("IVY_AIRCRAFT.military"); }
	catch (...) { IvyDebugString("IvyAircraftConfig: No military.\n"); }

	try { m_is_206B3 = pt.get<bool>("IVY_AIRCRAFT.206B3"); }
	catch (...) { IvyDebugString("IvyAircraftConfig: No 206B3.\n"); }

	try { m_206B3_MTBF = pt.get<float>("IVY_AIRCRAFT.206B3_mtbf"); }
	catch (...) { IvyDebugString("IvyAircraftConfig: No 206B3 MTBF.\n"); }

	try { m_is_206L3 = pt.get<bool>("IVY_AIRCRAFT.206L3"); }
	catch (...) { IvyDebugString("IvyAircraftConfig: No 206L3.\n"); }

	try { m_206L3_MTBF = pt.get<float>("IVY_AIRCRAFT.206L3_mtbf"); }
	catch (...) { IvyDebugString("IvyAircraftConfig: No 206L3 MTBF.\n"); }

	try { m_vspeeds_enabled = pt.get<bool>("IVY_AIRCRAFT.vspeeds_enabled"); }
	catch (...) { IvyDebugString("IvyAircraftConfig: No vspeeds_enabled.\n"); }

	try { m_lx_v1_data_ref = pt.get<std::string>("IVY_AIRCRAFT.v1_data_ref"); }
	catch (...) { IvyDebugString("IvyAircraftConfig: No v1_data_ref.\n"); }

	try { m_lx_vr_data_ref = pt.get<std::string>("IVY_AIRCRAFT.vr_data_ref"); }
	catch (...) { IvyDebugString("IvyAircraftConfig: No vr_data_ref.\n"); }

	try { m_lx_v2_data_ref = pt.get<std::string>("IVY_AIRCRAFT.v2_data_ref"); }
	catch (...) { IvyDebugString("IvyAircraftConfig: No v2_data_ref.\n"); }

	try { m_v_array = pt.get<bool>("IVY_AIRCRAFT.v_array"); }
	catch (...) { IvyDebugString("IvyAircraftConfig: No v_array.\n"); }

	try { m_v_array_size = pt.get<int>("IVY_AIRCRAFT.v_array_size"); }
	catch (...) { IvyDebugString("IvyAircraftConfig: No v_array_size.\n"); }

	try { m_v1_pos = pt.get<int>("IVY_AIRCRAFT.v1_pos"); }
	catch (...) { IvyDebugString("IvyAircraftConfig: No v1_pos.\n"); }

	try { m_v2_pos = pt.get<int>("IVY_AIRCRAFT.v2_pos"); }
	catch (...) { IvyDebugString("IvyAircraftConfig: No v2_pos.\n"); }

	try { m_vr_pos = pt.get<int>("IVY_AIRCRAFT.vr_pos"); }
	catch (...) { IvyDebugString("IvyAircraftConfig: No vr_pos.\n"); }

	try { m_li_v1 = pt.get<int>("IVY_AIRCRAFT.v1_static"); }
	catch (...) { IvyDebugString("IvyAircraftConfig: No v1_static.\n"); }

	try { m_li_v2 = pt.get<int>("IVY_AIRCRAFT.v2_static"); }
	catch (...) { IvyDebugString("IvyAircraftConfig: No v2_static.\n"); }

	try { m_li_vr = pt.get<int>("IVY_AIRCRAFT.vr_static"); }
	catch (...) { IvyDebugString("IvyAircraftConfig: No vr_static.\n"); }

	try { m_steep_descent = pt.get<bool>("IVY_AIRCRAFT.steep_descent"); }
	catch (...) { IvyDebugString("IvyAircraftConfig: No steep_descent.\n"); }

	try { m_steep_bank = pt.get<bool>("IVY_AIRCRAFT.steep_bank"); }
	catch (...) { IvyDebugString("IvyAircraftConfig: No steep_bank.\n"); }

	//pt.put("IVY_AIRCRAFT.steep_bank", m_steep_bank);

	try { m_max_alt_enable = pt.get<bool>("IVY_AIRCRAFT.max_alt_enable"); }
	catch (...) { IvyDebugString("IvyAircraftConfig: No max_alt_enable.\n"); }

	try { m_localizer_enable = pt.get<bool>("IVY_AIRCRAFT.localizer_enable"); }
	catch (...) { IvyDebugString("IvyAircraftConfig: No localizer_enable.\n"); }

	try { m_max_alt_value = pt.get<int>("IVY_AIRCRAFT.max_alt_value"); }
	catch (...) { IvyDebugString("IvyAircraftConfig: No max_alt_value.\n"); }

	try { m_to_power_lever = pt.get<float>("IVY_AIRCRAFT.to_power_lever"); }
	catch (...) { IvyDebugString("IvyAircraftConfig: No to_power_lever.\n"); }

	try { m_to_power_tolerance = pt.get<float>("IVY_AIRCRAFT.to_power_tolerance"); }
	catch (...) { IvyDebugString("IvyAircraftConfig: No to_power_tolerance.\n"); }

	try { m_eng_egt_lightup = pt.get<float>("IVY_AIRCRAFT.eng_egt_lightup"); }
	catch (...) { IvyDebugString("IvyAircraftConfig: No eng_egt_lightup.\n"); }

	try { m_eng_n2_startup = pt.get<float>("IVY_AIRCRAFT.eng_n2_startup"); }
	catch (...) { IvyDebugString("IvyAircraftConfig: No eng_n2_startup.\n"); }

	try { m_eng_stab_n1_min = pt.get<float>("IVY_AIRCRAFT.eng_stab_n1_min"); }
	catch (...) { IvyDebugString("IvyAircraftConfig: No eng_stab_n1_min.\n"); }

	try { m_eng_stab_n1_delta_s = pt.get<float>("IVY_AIRCRAFT.eng_stab_n1_delta_s"); }
	catch (...) { IvyDebugString("IvyAircraftConfig: No eng_stab_n1_delta_s.\n"); }

	try { m_slats_enabled = pt.get<bool>("IVY_AIRCRAFT.slats_enabled"); }
	catch (...) { IvyDebugString("IvyAircraftConfig: No slats_enabled.\n"); }

	//try { m_lf_slats_data_ref = pt.get<std::string>("IVY_AIRCRAFT.slats_data_ref"); }
	//catch (...) { IvyDebugString("IvyAircraftConfig: No slats_data_ref.\n"); }

	try { m_slats_tolerance = pt.get<float>("IVY_AIRCRAFT.slats_tolerance"); }
	catch (...) { IvyDebugString("IvyAircraftConfig: No slats_tolerance.\n"); }

	try { m_flaps_enabled = pt.get<bool>("IVY_AIRCRAFT.flaps_enabled"); }
	catch (...) { IvyDebugString("IvyAircraftConfig: No flaps_enabled.\n"); }

	//try { m_lf_flaps_data_ref = pt.get<std::string>("IVY_AIRCRAFT.flaps_data_ref"); }
	//catch (...) { IvyDebugString("IvyAircraftConfig: No flaps_data_ref.\n"); }

	try { m_flaps_tolerance = pt.get<float>("IVY_AIRCRAFT.flaps_tolerance"); }
	catch (...) { IvyDebugString("IvyAircraftConfig: No flaps_tolerance.\n"); }

	for (int index_number = 0; index_number < IVY_FS_MAX; index_number++)
	{
		char buffer[1024];

		sprintf(buffer,"IVY_AIRCRAFT.slats_value_%i", index_number+1);
		try { m_slats_deploy_value[index_number] = pt.get<float>(buffer); }
		catch (...) {}

		sprintf(buffer, "IVY_AIRCRAFT.slats_position_%i", index_number+1);
		try { m_slats_deploy_pos[index_number] = pt.get<float>(buffer); }
		catch (...) {}

	}

	for (int index_number = 0; index_number < IVY_FS_MAX; index_number++)
	{
		char buffer[1024];

		sprintf(buffer, "IVY_AIRCRAFT.flaps_value_%i", index_number+1);
		try { m_flaps_deploy_value[index_number] = pt.get<float>(buffer); }
		catch (...) {}

		sprintf(buffer, "IVY_AIRCRAFT.flaps_position_%i", index_number+1);
		try { m_flaps_deploy_pos[index_number] = pt.get<int>(buffer); }
		catch (...) {}

	}

	for (int index_number = 0; index_number < IVY_ENGN_MAX; index_number++)
	{
		char buffer[1024];

		sprintf(buffer, "IVY_AIRCRAFT.m_n1_th_%i", index_number + 1);
		try { m_n1_th[index_number] = pt.get<float>(buffer); }
		catch (...) {}
		sprintf(buffer, "IVY_AIRCRAFT.m_n1_sec_%i", index_number + 1);
		try { m_n1_sec[index_number] = pt.get<float>(buffer); }
		catch (...) {}
		sprintf(buffer, "IVY_AIRCRAFT.m_torque_th_%i", index_number + 1);
		try { m_torque_th[index_number] = pt.get<float>(buffer); }
		catch (...) {}
		sprintf(buffer, "IVY_AIRCRAFT.m_torque_sec_%i", index_number + 1);
		try { m_torque_sec[index_number] = pt.get<float>(buffer); }
		catch (...) {}
		sprintf(buffer, "IVY_AIRCRAFT.m_itt_th_%i", index_number + 1);
		try { m_itt_th[index_number] = pt.get<float>(buffer); }
		catch (...) {}
		sprintf(buffer, "IVY_AIRCRAFT.m_itt_sec_%i", index_number + 1);
		try { m_itt_sec[index_number] = pt.get<float>(buffer); }
		catch (...) {}
		sprintf(buffer, "IVY_AIRCRAFT.m_oil_press_th_%i", index_number + 1);
		try { m_oil_press_th[index_number] = pt.get<float>(buffer); }
		catch (...) {}
		sprintf(buffer, "IVY_AIRCRAFT.m_oil_press_sec_%i", index_number + 1);
		try { m_oil_press_sec[index_number] = pt.get<float>(buffer); }
		catch (...) {}
		sprintf(buffer, "IVY_AIRCRAFT.m_oil_temp_hi_th_%i", index_number + 1);
		try { m_oil_temp_th[index_number] = pt.get<float>(buffer); }
		catch (...) {}
		sprintf(buffer, "IVY_AIRCRAFT.m_oil_temp_hi_sec_%i", index_number + 1);
		try { m_oil_temp_sec[index_number] = pt.get<float>(buffer); }
		catch (...) {}
		sprintf(buffer, "IVY_AIRCRAFT.m_oil_temp_lo_th_%i", index_number + 1);
		try { m_oil_temp_lo_th[index_number] = pt.get<float>(buffer); }
		catch (...) {}
		sprintf(buffer, "IVY_AIRCRAFT.m_oil_temp_lo_sec_%i", index_number + 1);
		try { m_oil_temp_lo_sec[index_number] = pt.get<float>(buffer); }
		catch (...) {}
	}

}
