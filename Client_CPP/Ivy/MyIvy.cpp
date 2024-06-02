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

#include "MyIvy.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

MyIvy::MyIvy() :
	m_error_list(),
	m_ivy_flighttime_history(),
	m_aoc_list_low(),
	m_aoc_list_med(),
	m_aoc_list_high()
{
	m_play_mp3_queue = new std::queue<FMOD_SOUND*>;
	m_ivy_object_list = new std::vector<MyIvyResponse*>;
	m_ivy_landing_list = new std::vector<IvyLandingDetection*>;
	m_ivy_aircraft_list = new std::vector<MyIvyConfigAircraft*>;

	for (int index = 0; index < MAX_ERROR_HISTORY; index++)
		m_ivy_error_count_history[index] = 0;

	for (int index = 0; index < MAX_GRAPH_DATA; index++)
	{
		m_graph_altitude[index] = 0;
		m_graph_climb[index] = 0;
		m_graph_g_vert[index] = 0;
		m_graph_g_horiz[index] = 0;
		m_graph_g_side[index] = 0;
	}

	for (int index = 0; index < 8; index++)
	{
		m_lf_n1_percent_old[index] = 0;
		m_lia_engine_start_old[index] = 0;
	}

	for (int index = 0; index < IVY_ENGN_MAX; index++)
	{
		m_n1_failure[index] = false;
		m_n1_start[index] = 0;
		m_torque_failure[index] = false;
		m_torque_start[index] = 0;
		m_itt_failure[index] = false;
		m_itt_start[index] = 0;
		m_oil_press_failure[index] = false;
		m_oil_press_start[index] = 0;
		m_oil_temp_failure[index] = false;
		m_oil_temp_start[index] = 0;
		m_rotor_failure[index] = false;
		m_rotor_start[index] = 0;
	}
}


MyIvy::~MyIvy()
{
	if (m_play_mp3_queue)   delete m_play_mp3_queue;
	if (m_ivy_object_list)  delete m_ivy_object_list;
	if (m_ivy_landing_list) delete m_ivy_landing_list;
	if (m_ivy_aircraft_list)delete m_ivy_aircraft_list;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////;
// ResetIvy;
//;
// Resets all datasets && reloads the IvyAircraft depending upon the aircraft name given in the acf file;

void MyIvy::ResetIvy()
{
	ivy_output_file << "Resetting Ivy" << std::endl;
	ivy_output_file.flush();
	if (m_initialized == false) return;

	if (m_ivyChannel != NULL)
	{
		m_IvySound->InitSound();
		m_ivyChannel = m_IvySound->CreateSound(xplm_AudioRadioCopilot, "IvyMain");
		m_ivyChannelPassengersScreaming = m_IvySound->CreateSound(xplm_AudioInterior, "IvyPassengersScreaming");
		m_ivyChannelPassengersGeneral = m_IvySound->CreateSound(xplm_AudioInterior, "IvyPassengersGeneral");

		ivy_output_file << "Got new ChannelGroups" << std::endl;
		ivy_output_file.flush();

		m_ivyPassengers->m_passengerChannel = m_ivyChannelPassengersScreaming;

		IvyLoadSoundFiles(true);
	}


	

	m_error_list.clear();
	m_i_206_br_adf = NULL;
	m_i_206L3_br_adf = NULL;

	for (int index = 0; index < MAX_GRAPH_DATA; index++)
	{
		m_graph_altitude[index] = 0;
		m_graph_climb[index] = 0;
		m_graph_g_vert[index] = 0;
		m_graph_g_horiz[index] = 0;
		m_graph_g_side[index] = 0;
	}

	m_time = 0;
	m_landing_detected = 0;
	m_landing_rated = 0;
	m_landing_bounces = 0;
	m_landing_g_normal = 0;
	m_landing_sink_rate = 0;
	m_aircraft_crashed = 0;
	m_pressure_said = 0;
	m_non_smoking_old = 0;
	m_non_smoking_event = -100; // Needed for Startup with Non-Smoking enabled.

	m_airport_departure = "NONE";
	m_airport_departure_temp = "NONE";
	m_airport_arrival = "NONE";
	m_time_departure = 0;

	m_206_caution_test = false;
	m_206_tot_test = false;
	m_i_206_br_adf = NULL;
	m_engine_on_old = 0;
	m_precond = true;
	m_starter_switch_old = 0;
	m_starter_off_early = false;
	m_206_switches_set = false;

	m_206L3_caution_test = false;
	m_206L3_tot_test = false;
	m_i_206L3_br_adf = NULL;
	m_206L3_switches_set = false;

	for (int index = 0; index < 8; index++)
	{
		m_lf_n1_percent_old[index] = 0;
		m_lia_engine_start_old[index] = 0;
	}

	for (int index = 0; index < IVY_ENGN_MAX; index++)
	{
		m_n1_failure[index] = false;
		m_n1_start[index] = 0;
		m_torque_failure[index] = false;
		m_torque_start[index] = 0;
		m_itt_failure[index] = false;
		m_itt_start[index] = 0;
		m_oil_press_failure[index] = false;
		m_oil_press_start[index] = 0;
		m_oil_temp_failure[index] = false;
		m_oil_temp_start[index] = 0;
		m_rotor_failure[index] = false;
		m_rotor_start[index] = 0;
	}


	if (m_play_mp3_queue)   delete m_play_mp3_queue;
	if (m_ivy_landing_list) delete m_ivy_landing_list;

	m_play_mp3_queue = new std::queue<FMOD_SOUND*>;
	m_ivy_landing_list = new std::vector<IvyLandingDetection*>;

	for (int obj_number = 0; obj_number < m_ivy_object_list->size(); ++obj_number)
	{
		m_ivy_object_list->at(obj_number)->m_error_count = 0;
		m_ivy_object_list->at(obj_number)->m_active = m_ivy_object_list->at(obj_number)->m_active_on_load;
		m_ivy_object_list->at(obj_number)->m_played = m_ivy_object_list->at(obj_number)->m_active_on_load;
	}
	
	char lba_acf_descrip[1024];
	char lba_acf_tailnumber[1024];

	XPLMGetDatab(m_s_acf_descrip, lba_acf_descrip, 0, 240);
	XPLMGetDatab(m_s_acf_tailnumber, lba_acf_tailnumber, 0, 40);

	m_ls_acf_descrip = lba_acf_descrip;
	m_ls_acf_descrip += lba_acf_tailnumber;

	m_ivyAircraft = m_ivy_aircraft_list->at(0);

	for (int index = 0; index < m_ivy_aircraft_list->size(); index++)
		if (m_ls_acf_descrip.find(m_ivy_aircraft_list->at(index)->m_name) != std::string::npos)
			m_ivyAircraft = m_ivy_aircraft_list->at(index);

	m_ivyHVCheck.ReadConfig();


}

void MyIvy::IvyStart()
{
	ivy_output_file << "Ivy Startup" << std::endl;
	ivy_output_file.flush();

	m_initialized = true;

	m_ds = XPLMGetDirectorySeparator();

	ivy_output_file << "Ivy Creating Config" << std::endl;
	ivy_output_file.flush();

	m_ivyConfig = new MyIvyConfiguration();

	ivy_output_file << "Ivy Reading Config" << std::endl;
	ivy_output_file.flush();

	m_ivyConfig->ReadConfig();

	ivy_output_file << "Ivy Config loaded" << std::endl;
	ivy_output_file.flush();

	// Create Default Aircraft
	m_ivyAircraft = new MyIvyConfigAircraft(NULL);
	m_ivy_aircraft_list->push_back(m_ivyAircraft);

	ivy_output_file << "Ivy Creating Aircraft" << std::endl;
	ivy_output_file.flush();

	for (int index = 1; index < IVY_MAX_AIRCRAFT_CONFIG; index++)
	{
		std::string aircraft_ini_path = m_ivyConfig->m_config_path + "IvyAircraft_" + std::to_string(index) + ".ini";
		if (file_exists(aircraft_ini_path)) m_ivy_aircraft_list->push_back(new MyIvyConfigAircraft(m_ivyConfig->m_config_path.c_str(), index));
	}

	ivy_output_file << "Ivy aircrafts created" << std::endl;
	ivy_output_file.flush();

	m_s_acf_descrip = XPLMFindDataRef("sim/aircraft/view/acf_descrip");
	m_s_acf_tailnumber = XPLMFindDataRef("sim/aircraft/view/acf_tailnum");

	m_ivyChannel = NULL;

	ResetIvy();

	ivy_output_file << "Ivy starting sound" << std::endl;
	ivy_output_file.flush();

	m_IvySound = new XPSound();
	m_IvySound->InitSound();

	ivy_output_file << "Sound Initialized" << std::endl;
	ivy_output_file.flush();

	m_ivyChannel = m_IvySound->CreateSound(xplm_AudioRadioCopilot, "IvyMain");
	m_ivyChannelPassengersScreaming = m_IvySound->CreateSound(xplm_AudioInterior, "IvyPassengersScreaming");
	m_ivyChannelPassengersGeneral = m_IvySound->CreateSound(xplm_AudioInterior, "IvyPassengersGeneral");

	m_ivyPassengers = new IvyPassengers(m_ivyConfig, m_ivyChannelPassengersScreaming, m_IvySound);

	srand(time(NULL));

	IvyLoadSoundFiles();

	ivy_output_file << "Sound Files Loaded" << std::endl;
	ivy_output_file.flush();

	////////////////////////////////////////////////////////////////////////// DEBUG;
	for (int obj_number = 0; obj_number < m_ivy_object_list->size(); obj_number++)
	{
		std::string buf;
		buf =  "IvyObject: " + m_ivy_object_list->at(obj_number)->m_event_name + " Number of Soundfiles: " + std::to_string(m_ivy_object_list->at(obj_number)->m_sounds.size());
		ivy_output_file << buf << std::endl;
		ivy_output_file.flush();
	}
	////////////////////////////////////////////////////////////////////////// END DEBUG;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////;
	// Register Flight Callback

	XPLMRegisterFlightLoopCallback(WrapIvyFlightLoopCallback, m_ivyConfig->m_data_rate, 0);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////;
	// Register Command Callbacks;

	m_CmdSayBaro = XPLMCreateCommand("Ivy/say_baro", "Ask Ivy to tell you the barometric pressure");
	m_CmdSayWind = XPLMCreateCommand("Ivy/say_wind", "Ivy holds out a finger && tells you the current wind direction plus speed");
	m_CmdToogleWindow = XPLMCreateCommand("Ivy/show_output", "Toggles the Control Window");
	m_CmdResetIvy = XPLMCreateCommand("Ivy/reset_ivy", "Reset Ivy");
	m_CmdAnnouncement = XPLMCreateCommand("Ivy/cabin_announcement", "Tell Ivy to make a cabin announcement");

	XPLMRegisterCommandHandler(m_CmdSayBaro, WrapSayBaroCallback, 0, 0);
	XPLMRegisterCommandHandler(m_CmdResetIvy, WrapResetIvyCallback, 0, 0);
	XPLMRegisterCommandHandler(m_CmdSayWind, WrapSayWindCallback, 0, 0);
	XPLMRegisterCommandHandler(m_CmdToogleWindow, WrapToogleWindowCallback, 0, 0);
	XPLMRegisterCommandHandler(m_CmdAnnouncement, WrapAnnouncementCallback, 0, 0);
	
	
	ivy_output_file << "Commands Registered" << std::endl;
	ivy_output_file.flush();

	// Menu;
	m_IvyMenu = XPLMAppendMenuItem(XPLMFindPluginsMenu(), "Nagging Copilot", 0, 1);
	m_MenuId = XPLMCreateMenu("Nagging Copilot", XPLMFindPluginsMenu(), m_IvyMenu, WrapIvyMenuHandler, 0);
	XPLMAppendMenuItem(m_MenuId, "Toggle Control Window", "Item1", 1);
	XPLMAppendMenuItem(m_MenuId, "Make Announcement", "Item3", 1);
	XPLMAppendMenuItem(m_MenuId, "Barometric Pressure", "Item4", 1);
	XPLMAppendMenuItem(m_MenuId, "Wind Situtation", "Item5", 1);
	XPLMAppendMenuItem(m_MenuId, "Reset Copilot", "Item7", 1);
	m_menuindex_ivy_enable = XPLMAppendMenuItem(m_MenuId, "Copilot Enable", "Item8", 1);
	m_menuindex_log_enable = XPLMAppendMenuItem(m_MenuId, "Logging Enable", "Item9", 1);

	// Flag to tell us if theLogbook is shown;
	m_MenuLogbookShow = 0;

	ivy_output_file << "Menu Created" << std::endl;
	ivy_output_file.flush();

	// Get our Datarefs
	IvyAssignDataRefs();

	ivy_output_file << "Datarefs Assigned" << std::endl;
	ivy_output_file.flush();

	m_data_read_valid = false;

	m_WindowId = XPLMCreateWindow(10, 200, 500, 500, 1, WrapIvyDrawOutputWindow, WrapKeyCallback, WrapMouseClickCallback, 0);

	ivy_output_file << "Output Window Created" << std::endl;
	ivy_output_file.flush();

	// Make sure logbook file exists;
	FILE *logbook_file;
	ivy_output_file << "LogBook File: " << m_ivyConfig->m_logbook_path << std::endl;
	ivy_output_file.flush();
	try
	{
		logbook_file = fopen(m_ivyConfig->m_logbook_path.c_str(), "a+");
		fclose(logbook_file);
	}
	catch (...)
	{
		ivy_output_file << "Could not create Logbook File" << std::endl;
	}

	RefreshLogbook();

	int left, top, right, bot;
	XPLMGetScreenBoundsGlobal(&left, &top, &right, &bot);

	int width = 1000;
	int height = 550;
	int left_pad = 175;
	int top_pad = 75;
	int x = left + left_pad;
	int y = top - top_pad;
	// WindowDecoration decorate
	// WindowDecorationNone = 0
	// WindowDecorationRoundRectangle = 1
	// WindowDecorationSelfDecorated = 2
	// WindowDecorationSelfDecoratedResizable = 3
	int decorate = 1;
	imguiPtr = std::make_shared<IvyImguiWidget>(this, 
		                                        m_ivyConfig->m_log_window_pos_x, 
												m_ivyConfig->m_log_window_pos_y, 
												m_ivyConfig->m_log_window_pos_x + m_ivyConfig->m_log_window_width, 
												m_ivyConfig->m_log_window_pos_y - m_ivyConfig->m_log_window_height, 
												decorate);  // calls constructor
	//imguiPtr-> // calls ImgWindow::init from the base class which in turn virtually calls the overridden function

	ivy_output_file << "Reading Error Log" << std::endl;
	ivy_output_file.flush();
	
	ReadErrorLog();

	MenuCheck();
	
	ivy_output_file << "Init Finished" << std::endl;
	ivy_output_file.flush();
	
}



void MyIvy::IvyStop()
{
	
	ivy_output_file << "Stopping Ivy" << std::endl;
	ivy_output_file.flush();
	
	if (m_initialized == false) return;

	//XPLMUnregisterFlightLoopCallback(m_FlightLoopCB, 0);
	XPLMUnregisterCommandHandler(m_CmdSayBaro, m_SayBaroCB, 0, 0);
	XPLMUnregisterCommandHandler(m_CmdResetIvy, m_ResetIvyCB, 0, 0);
	XPLMUnregisterCommandHandler(m_CmdSayWind, m_SayWindCB, 0, 0);
	XPLMUnregisterCommandHandler(m_CmdToogleWindow, m_ToogleWindowCB, 0, 0);
	XPLMUnregisterCommandHandler(m_CmdAnnouncement, m_AnnouncementCB, 0, 0);
	XPLMDestroyWindow(m_WindowId);
	XPLMDestroyMenu(m_MenuId);
	//XPLMDestroyMenu(self, m_IvyMenu);

	//if (m_MenuLogbookShow == 1) XPDestroyWidget(m_LogbookWidget, 1);
	//if (m_MenuVSpeedsShow == 1) XPDestroyWidget(m_VSpeedsWidget, 1);
	m_ivyConfig->WriteConfig();

	if (m_ivyConfig) delete m_ivyConfig;

	ivy_output_file << "Ivy Stopped" << std::endl;
	ivy_output_file.flush();

}

void MyIvy::IvyEnable()
{
	ivy_output_file << "Ivy Enabled" << std::endl;
	ivy_output_file.flush();
	m_plugin_enabled = 1;
}

void MyIvy::IvyDisable()
{
	ivy_output_file << "Ivy Disabled" << std::endl;
	ivy_output_file.flush();
	m_plugin_enabled = 0;
}

void MyIvy::IvyLoadSoundFiles(bool reload)
{
	if (reload == true)
	{
		//m_IvySound->RemoveAllBuffers();



		m_ivyPassengers->LoadSound(m_ivyConfig, m_ivyChannelPassengersScreaming, m_IvySound);

		for (auto my_obj : *m_ivy_object_list)
		{
			my_obj->m_ivyChannel = m_ivyChannel;
			my_obj->LoadSound(m_ivyConfig->m_mp3_path, m_IvySound);
		}
		
		
	}
	else
	{
		m_ivyPassengers->LoadSound(m_ivyConfig, m_ivyChannelPassengersScreaming, m_IvySound);
//														  Name						   PATH						DEACT_ON_LOAD		MINIMUM_OCC_TIME		DEACT_TIME				IS_ERROR			         IVY_OBJECT_LIST		CHANNEL       XPSound
        m_ivyOuch =                 new MyIvyResponse(    "ouch",                      m_ivyConfig->m_mp3_path,    0,                0,                         0,                         0,                    m_ivy_object_list,     m_ivyChannel, m_IvySound, m_error_list, &(m_ivyConfig->m_errors_enable),      "");
        m_ivyPosRateClimb =         new MyIvyResponse(    "pos_climb",                 m_ivyConfig->m_mp3_path,    0,                1,                         20,                        0,                    m_ivy_object_list,     m_ivyChannel, m_IvySound, m_error_list, &(m_ivyConfig->m_errors_enable),       "");
        m_ivyTyre =                 new MyIvyResponse(    "tyre",                      m_ivyConfig->m_mp3_path,    0,                0,                         0,                         1,                    m_ivy_object_list,     m_ivyChannel, m_IvySound, m_error_list, &(m_ivyConfig->m_errors_enable),       "Tyre Damaged");
        m_ivyBrake =                new MyIvyResponse(    "brake",                     m_ivyConfig->m_mp3_path,    0,                2,                         10,                        1,                    m_ivy_object_list,     m_ivyChannel, m_IvySound, m_error_list, &(m_ivyConfig->m_errors_enable),       "Strong Braking");
        m_ivyTransponder =          new MyIvyResponse(    "transponder",               m_ivyConfig->m_mp3_path,    0,                120,                       0,                         1,                    m_ivy_object_list,     m_ivyChannel, m_IvySound, m_error_list, &(m_ivyConfig->m_errors_enable),       "Transponder OFF");
        m_ivyCockpitLights =        new MyIvyResponse(    "cockpit_lights",            m_ivyConfig->m_mp3_path,    0,                5,                         0,                         0,                    m_ivy_object_list,     m_ivyChannel, m_IvySound, m_error_list, &(m_ivyConfig->m_errors_enable),       "Cockpit Lights OFF");
        m_ivyLandingLights =        new MyIvyResponse(    "landing_lights",            m_ivyConfig->m_mp3_path,    0,                0,                         0,                         1,                    m_ivy_object_list,     m_ivyChannel, m_IvySound, m_error_list, &(m_ivyConfig->m_errors_enable),       "Landing Lights OFF");
		m_ivyAirSpeedAlive =        new MyIvyResponse(    "airspeed",                  m_ivyConfig->m_mp3_path,    0,                0,                         0,                         0,                    m_ivy_object_list,     m_ivyChannel, m_IvySound, m_error_list, &(m_ivyConfig->m_errors_enable),       "");
		m_ivyPowerSet =             new MyIvyResponse(    "power_set",                 m_ivyConfig->m_mp3_path,    0,                0,                         0,                         0,                    m_ivy_object_list,     m_ivyChannel, m_IvySound, m_error_list, &(m_ivyConfig->m_errors_enable),       "");
 
        m_ivyGearUp =               new MyIvyResponse(    "gear_up",                   m_ivyConfig->m_mp3_path,    1,                1,                         0,                         0,                    m_ivy_object_list,     m_ivyChannel, m_IvySound, m_error_list, &(m_ivyConfig->m_callouts_enable),     "");
        m_ivyGearDown =             new MyIvyResponse(    "gear_down",                 m_ivyConfig->m_mp3_path,    1,                1,                         0,                         0,                    m_ivy_object_list,     m_ivyChannel, m_IvySound, m_error_list, &(m_ivyConfig->m_callouts_enable),     "");
		m_ivyGearSelectedUp =       new MyIvyResponse(    "gear_selected_up",          m_ivyConfig->m_mp3_path,    1,                0,                         0,                         0,                    m_ivy_object_list,     m_ivyChannel, m_IvySound, m_error_list, &(m_ivyConfig->m_callouts_enable),     "");
        m_ivyGearSelectedDown =     new MyIvyResponse(    "gear_selected_down",        m_ivyConfig->m_mp3_path,    1,                0,                         0,                         0,                    m_ivy_object_list,     m_ivyChannel, m_IvySound, m_error_list, &(m_ivyConfig->m_callouts_enable),     "");
 
        m_ivyBeaconLights =         new MyIvyResponse(    "beacon",                    m_ivyConfig->m_mp3_path,    0,                20,                        0,                         1,                    m_ivy_object_list,     m_ivyChannel, m_IvySound, m_error_list, &(m_ivyConfig->m_errors_enable),       "Beacon OFF");
        m_ivyNavLights =            new MyIvyResponse(    "nav_lights",                m_ivyConfig->m_mp3_path,    0,                50,                        0,                         1,                    m_ivy_object_list,     m_ivyChannel, m_IvySound, m_error_list, &(m_ivyConfig->m_errors_enable),       "NAV Lights OFF");
        m_ivyStrobes =              new MyIvyResponse(    "strobes",                   m_ivyConfig->m_mp3_path,    0,                200,                       0,                         1,                    m_ivy_object_list,     m_ivyChannel, m_IvySound, m_error_list, &(m_ivyConfig->m_errors_enable),       "Strobes OFF");
        m_ivySkidTyres =            new MyIvyResponse(    "skid_tyres",                m_ivyConfig->m_mp3_path,    0,                5,                         0,                         0,                    m_ivy_object_list,     m_ivyChannel, m_IvySound, m_error_list, &(m_ivyConfig->m_errors_enable),       "");
        m_ivyBatteryOut =           new MyIvyResponse(    "battery_out",               m_ivyConfig->m_mp3_path,    0,                0,                         0,                         1,                    m_ivy_object_list,     m_ivyChannel, m_IvySound, m_error_list, &(m_ivyConfig->m_errors_enable),       "Battery LOW");
        m_ivyEngineFire =           new MyIvyResponse(    "engine_fire",               m_ivyConfig->m_mp3_path,    0,                0,                         0,                         1,                    m_ivy_object_list,     m_ivyChannel, m_IvySound, m_error_list, &(m_ivyConfig->m_errors_enable),       "Engine Fire");
        m_ivyEngineFlameout =       new MyIvyResponse(    "engine_flameout",           m_ivyConfig->m_mp3_path,    0,                0,                         0,                         1,                    m_ivy_object_list,     m_ivyChannel, m_IvySound, m_error_list, &(m_ivyConfig->m_errors_enable),       "Engine Flameout");
        m_ivyEngineFailureGround =  new MyIvyResponse(    "engine_failure_ground",     m_ivyConfig->m_mp3_path,    1,                0,                         0,                         1,                    m_ivy_object_list,     m_ivyChannel, m_IvySound, m_error_list, &(m_ivyConfig->m_errors_enable),       "Engine Failure Ground");
        m_ivyEngineFailureAir =     new MyIvyResponse(    "engine_failure_air",        m_ivyConfig->m_mp3_path,    0,                0,                         0,                         1,                    m_ivy_object_list,     m_ivyChannel, m_IvySound, m_error_list, &(m_ivyConfig->m_errors_enable),       "Engine Failure Air");
        m_ivyEngineHotStart =       new MyIvyResponse(    "engine_hot",                m_ivyConfig->m_mp3_path,    0,                0,                         0,                         1,                    m_ivy_object_list,     m_ivyChannel, m_IvySound, m_error_list, &(m_ivyConfig->m_errors_enable),       "Engine Hot-Start");
        m_ivyBirdStrike =           new MyIvyResponse(    "bird",                      m_ivyConfig->m_mp3_path,    0,                0,                         0,                         1,                    m_ivy_object_list,     m_ivyChannel, m_IvySound, m_error_list, &(m_ivyConfig->m_errors_enable),       "Bird Strike");
 
        m_ivyOverspeedFlaps =       new MyIvyResponse(    "overspeed_flaps",           m_ivyConfig->m_mp3_path,    1,                0,                         1,                         1,                    m_ivy_object_list,     m_ivyChannel, m_IvySound, m_error_list, &(m_ivyConfig->m_errors_enable),       "Flaps Overspeed");
        m_ivyOverspeedGear =        new MyIvyResponse(    "overspeed_gear",            m_ivyConfig->m_mp3_path,    1,                0,                         1,                         1,                    m_ivy_object_list,     m_ivyChannel, m_IvySound, m_error_list, &(m_ivyConfig->m_errors_enable),       "Landing Gear Overspeed");
        m_ivyOverspeedAircraft =    new MyIvyResponse(    "overspeed_aircraft",        m_ivyConfig->m_mp3_path,    1,                1,                         1,                         1,                    m_ivy_object_list,     m_ivyChannel, m_IvySound, m_error_list, &(m_ivyConfig->m_errors_enable),       "Aircraft Overspeed");
        m_ivyStall =                new MyIvyResponse(    "stall",                     m_ivyConfig->m_mp3_path,    1,                0,                         5,                         1,                    m_ivy_object_list,     m_ivyChannel, m_IvySound, m_error_list, &(m_ivyConfig->m_errors_enable),       "STALL");
 
        m_ivyNoBatt =               new MyIvyResponse(    "no_batt",                   m_ivyConfig->m_mp3_path,    0,                180,                       0,                         0,                    m_ivy_object_list,     m_ivyChannel, m_IvySound, m_error_list, &(m_ivyConfig->m_errors_enable),       "");
        m_ivyHelloSun =             new MyIvyResponse(    "hello_normal",              m_ivyConfig->m_mp3_path,    0,                15,                        0,                         0,                    m_ivy_object_list,     m_ivyChannel, m_IvySound, m_error_list, &(m_ivyConfig->m_errors_enable),       "");
        m_ivyHellorain =            new MyIvyResponse(    "hello_normal",              m_ivyConfig->m_mp3_path,    0,                15,                        0,                         0,                    m_ivy_object_list,     m_ivyChannel, m_IvySound, m_error_list, &(m_ivyConfig->m_errors_enable),       "");
        m_ivyHelloFog =             new MyIvyResponse(    "hello_normal",              m_ivyConfig->m_mp3_path,    0,                15,                        0,                         0,                    m_ivy_object_list,     m_ivyChannel, m_IvySound, m_error_list, &(m_ivyConfig->m_errors_enable),       "");
        m_ivyHelloNormal =          new MyIvyResponse(    "hello_normal",              m_ivyConfig->m_mp3_path,    0,                15,                        0,                         0,                    m_ivy_object_list,     m_ivyChannel, m_IvySound, m_error_list, &(m_ivyConfig->m_errors_enable),       "");
        m_ivyCabinDownNormal =      new MyIvyResponse(    "cabin_down_normal",         m_ivyConfig->m_mp3_path,    0,                2,                         0,                         1,                    m_ivy_object_list,     m_ivyChannel, m_IvySound, m_error_list, &(m_ivyConfig->m_errors_enable),       "Cabin Pressure Raising Fast");
        m_ivyCabinDownFast =        new MyIvyResponse(    "cabin_down_fast",           m_ivyConfig->m_mp3_path,    0,                2,                         0,                         1,                    m_ivy_object_list,     m_ivyChannel, m_IvySound, m_error_list, &(m_ivyConfig->m_errors_enable),       "Cabin Pressure Raising Extremely Fast");
        m_ivyBankNormal =           new MyIvyResponse(    "bank_normal",               m_ivyConfig->m_mp3_path,    0,                0,                         0,                         0,                    m_ivy_object_list,     m_ivyChannel, m_IvySound, m_error_list, &(m_ivyConfig->m_pre_warnings),        "");
        m_ivyBankHigh =             new MyIvyResponse(    "bank_high",                 m_ivyConfig->m_mp3_path,    0,                2,                         0,                         1,                    m_ivy_object_list,     m_ivyChannel, m_IvySound, m_error_list, &(m_ivyConfig->m_errors_enable),       "Bank Angle High");
        m_ivyBankXHigh =            new MyIvyResponse(    "bank_xhigh",                m_ivyConfig->m_mp3_path,    0,                2,                         0,                         1,                    m_ivy_object_list,     m_ivyChannel, m_IvySound, m_error_list, &(m_ivyConfig->m_errors_enable),       "Bank Angle Extremely High");
        m_ivyPitchDownNormal =      new MyIvyResponse(    "pitch_down_normal",         m_ivyConfig->m_mp3_path,    0,                0,                         0,                         0,                    m_ivy_object_list,     m_ivyChannel, m_IvySound, m_error_list, &(m_ivyConfig->m_pre_warnings),        "");
        m_ivyPitchDownHigh =        new MyIvyResponse(    "pitch_down_high",           m_ivyConfig->m_mp3_path,    0,                2,                         0,                         1,                    m_ivy_object_list,     m_ivyChannel, m_IvySound, m_error_list, &(m_ivyConfig->m_errors_enable),       "Pitch Low");
        m_ivyGNormalFlightNormal =  new MyIvyResponse(    "g_normal_flight_normal",    m_ivyConfig->m_mp3_path,    0,                2,                         0,                         0,                    m_ivy_object_list,     m_ivyChannel, m_IvySound, m_error_list, &(m_ivyConfig->m_pre_warnings),       "");
        m_ivyGNormalFlightHigh =    new MyIvyResponse(    "g_normal_flight_high",      m_ivyConfig->m_mp3_path,    0,                2,                         0,                         1,                    m_ivy_object_list,     m_ivyChannel, m_IvySound, m_error_list, &(m_ivyConfig->m_errors_enable),       "G-Forces High");
        m_ivyGNormalFlightXHigh =   new MyIvyResponse(    "g_normal_flight_xhigh",     m_ivyConfig->m_mp3_path,    0,                2,                         0,                         1,                    m_ivy_object_list,     m_ivyChannel, m_IvySound, m_error_list, &(m_ivyConfig->m_errors_enable),       "G-Forces Extremely High");
        m_ivyGNormalNegativeLow =   new MyIvyResponse(    "g_normal_negative_low",     m_ivyConfig->m_mp3_path,    0,                0.5,                       0,                         1,                    m_ivy_object_list,     m_ivyChannel, m_IvySound, m_error_list, &(m_ivyConfig->m_errors_enable),       "G-Forces Low");
        m_ivyGNormalNegativeHigh =  new MyIvyResponse(    "g_normal_negative_high",    m_ivyConfig->m_mp3_path,    0,                0.5,                       0,                         1,                    m_ivy_object_list,     m_ivyChannel, m_IvySound, m_error_list, &(m_ivyConfig->m_errors_enable),       "G-Forces Negative");
        m_ivyTurbulenceNormal =     new MyIvyResponse(    "turbulence_normal",         m_ivyConfig->m_mp3_path,    0,                20,                        0,                         0,                    m_ivy_object_list,     m_ivyChannel, m_IvySound, m_error_list, &(m_ivyConfig->m_errors_enable),       "");
        m_ivyTurbolenceHigh =       new MyIvyResponse(    "turbulence_high",           m_ivyConfig->m_mp3_path,    0,                20,                        0,                         0,                    m_ivy_object_list,     m_ivyChannel, m_IvySound, m_error_list, &(m_ivyConfig->m_errors_enable),       "");
 
        m_ivyLandingXGood =         new MyIvyResponse(    "landing_xgood",             m_ivyConfig->m_mp3_path,    0,                5,                         0,                         0,                    m_ivy_object_list,     m_ivyChannel, m_IvySound, m_error_list, &(m_ivyConfig->m_errors_enable),       "");
        m_ivyLandingGood =          new MyIvyResponse(    "landing_good",              m_ivyConfig->m_mp3_path,    0,                5,                         0,                         0,                    m_ivy_object_list,     m_ivyChannel, m_IvySound, m_error_list, &(m_ivyConfig->m_errors_enable),       "");
        m_ivyLandingNormal =        new MyIvyResponse(    "landing_normal",            m_ivyConfig->m_mp3_path,    0,                5,                         0,                         0,                    m_ivy_object_list,     m_ivyChannel, m_IvySound, m_error_list, &(m_ivyConfig->m_errors_enable),       "");
        m_ivyLandingBad =           new MyIvyResponse(    "landing_bad",               m_ivyConfig->m_mp3_path,    0,                5,                         0,                         0,                    m_ivy_object_list,     m_ivyChannel, m_IvySound, m_error_list, &(m_ivyConfig->m_errors_enable),       "");
        m_ivyLandingXBad =          new MyIvyResponse(    "landing_xbad",              m_ivyConfig->m_mp3_path,    0,                5,                         0,                         1,                    m_ivy_object_list,     m_ivyChannel, m_IvySound, m_error_list, &(m_ivyConfig->m_errors_enable),       "Heavy Landing");
 
        m_ivyBaroLow =              new MyIvyResponse(    "baro_low",                  m_ivyConfig->m_mp3_path,    0,                5,                         0,                         1,                    m_ivy_object_list,     m_ivyChannel, m_IvySound, m_error_list, &(m_ivyConfig->m_errors_enable),       "Barometer not set correctly");
        m_ivyBaroGround =           new MyIvyResponse(    "baro_low",                  m_ivyConfig->m_mp3_path,    0,                60,                        0,                         0,                    m_ivy_object_list,     m_ivyChannel, m_IvySound, m_error_list, &(m_ivyConfig->m_errors_enable),       "");
        m_ivyBaroHigh =             new MyIvyResponse(    "baro_high",                 m_ivyConfig->m_mp3_path,    0,                120,                       0,                         1,                    m_ivy_object_list,     m_ivyChannel, m_IvySound, m_error_list, &(m_ivyConfig->m_errors_enable),       "Barometer not on Standard above Transition Level");
        m_ivyLandingLightsHigh =    new MyIvyResponse(    "landing_lights_high",       m_ivyConfig->m_mp3_path,    0,                300,                       0,                         1,                    m_ivy_object_list,     m_ivyChannel, m_IvySound, m_error_list, &(m_ivyConfig->m_errors_enable),       "Landing Lights ON at High Altitude");
        m_ivyRotate =               new MyIvyResponse(    "rotate",                    m_ivyConfig->m_mp3_path,    0,                0,                         0,                         0,                    m_ivy_object_list,     m_ivyChannel, m_IvySound, m_error_list, &(m_ivyConfig->m_errors_enable),       "");
		m_ivy60ktLanding =          new MyIvyResponse(    "60kt",                      m_ivyConfig->m_mp3_path,    0,                0,                         5,                         0,                    m_ivy_object_list,     m_ivyChannel, m_IvySound, m_error_list, &(m_ivyConfig->m_callouts_enable),       "");
        m_ivy60kt =                 new MyIvyResponse(    "60kt",                      m_ivyConfig->m_mp3_path,    0,                0,                         5,                         0,                    m_ivy_object_list,     m_ivyChannel, m_IvySound, m_error_list, &(m_ivyConfig->m_callouts_enable),       "");
        m_ivy80kt =                 new MyIvyResponse(    "80kt",                      m_ivyConfig->m_mp3_path,    0,                0,                         5,                         0,                    m_ivy_object_list,     m_ivyChannel, m_IvySound, m_error_list, &(m_ivyConfig->m_callouts_enable),       "");
        m_ivy100kt =                new MyIvyResponse(    "100kt",                     m_ivyConfig->m_mp3_path,    0,                0,                         5,                         0,                    m_ivy_object_list,     m_ivyChannel, m_IvySound, m_error_list, &(m_ivyConfig->m_callouts_enable),       "");
        m_ivyV1 =                   new MyIvyResponse(    "v1",                        m_ivyConfig->m_mp3_path,    0,                0,                         5,                         0,                    m_ivy_object_list,     m_ivyChannel, m_IvySound, m_error_list, &(m_ivyConfig->m_callouts_enable),       "");
        m_ivyVR =                   new MyIvyResponse(    "vr",                        m_ivyConfig->m_mp3_path,    0,                0,                         5,                         0,                    m_ivy_object_list,     m_ivyChannel, m_IvySound, m_error_list, &(m_ivyConfig->m_callouts_enable),       "");
        m_ivyBelowV2 =              new MyIvyResponse(    "below_v2",                  m_ivyConfig->m_mp3_path,    0,                5,                         5,                         1,                    m_ivy_object_list,     m_ivyChannel, m_IvySound, m_error_list, &(m_ivyConfig->m_errors_enable),       "V2 Not Achieved");
        m_ivyAboveV2 =              new MyIvyResponse(    "above_v2",                  m_ivyConfig->m_mp3_path,    0,                0,                         5,                         0,                    m_ivy_object_list,     m_ivyChannel, m_IvySound, m_error_list, &(m_ivyConfig->m_callouts_enable),       "");
        m_ivyFlapsRetracted =       new MyIvyResponse(    "flaps_retracted",           m_ivyConfig->m_mp3_path,    1,                0,                         0,                         0,                    m_ivy_object_list,     m_ivyChannel, m_IvySound, m_error_list, &(m_ivyConfig->m_callouts_enable),       "");
        m_ivySlatsRetracted =       new MyIvyResponse(    "slats_retracted",           m_ivyConfig->m_mp3_path,    1,                0,                         0,                         0,                    m_ivy_object_list,     m_ivyChannel, m_IvySound, m_error_list, &(m_ivyConfig->m_callouts_enable),       "");
        m_ivyFlapsPosition =        new MyIvyResponse(    "flaps",                     m_ivyConfig->m_mp3_path,    1,                1,                         0,                         0,                    m_ivy_object_list,     m_ivyChannel, m_IvySound, m_error_list, &(m_ivyConfig->m_callouts_enable),       "");
        m_ivySlatsPosition =        new MyIvyResponse(    "slats",                     m_ivyConfig->m_mp3_path,    1,                1,                         0,                         0,                    m_ivy_object_list,     m_ivyChannel, m_IvySound, m_error_list, &(m_ivyConfig->m_callouts_enable),       "");
 
		m_ivyLocalizerAlive =       new MyIvyResponse(    "localizer_alive",           m_ivyConfig->m_mp3_path,    1,                0,                         5,                         0,                    m_ivy_object_list,     m_ivyChannel, m_IvySound, m_error_list, &(m_ivyConfig->m_callouts_enable),       "");
		m_ivyGlideSlopeAlive =      new MyIvyResponse(    "glide_slope_alive",         m_ivyConfig->m_mp3_path,    1,                0,                         5,                         0,                    m_ivy_object_list,     m_ivyChannel, m_IvySound, m_error_list, &(m_ivyConfig->m_callouts_enable),       "");
		m_ivySpoilersArmed =        new MyIvyResponse(    "spoilers_armed",            m_ivyConfig->m_mp3_path,    1,                0,                         0,                         0,                    m_ivy_object_list,     m_ivyChannel, m_IvySound, m_error_list, &(m_ivyConfig->m_callouts_enable),       "");
		m_ivySpoilersDisarmed =     new MyIvyResponse(    "spoilers_disarmed",         m_ivyConfig->m_mp3_path,    1,                0,                         0,                         0,                    m_ivy_object_list,     m_ivyChannel, m_IvySound, m_error_list, &(m_ivyConfig->m_callouts_enable),       "");
		m_ivySpeedBrakeRetracted =  new MyIvyResponse(    "speed_brake_retracted",     m_ivyConfig->m_mp3_path,    1,                0,                         0,                         0,                    m_ivy_object_list,     m_ivyChannel, m_IvySound, m_error_list, &(m_ivyConfig->m_callouts_enable),       "");
		m_ivySpeedBrakeExtended =   new MyIvyResponse(    "speed_brake_on",            m_ivyConfig->m_mp3_path,    1,                0,                         0,                         0,                    m_ivy_object_list,     m_ivyChannel, m_IvySound, m_error_list, &(m_ivyConfig->m_callouts_enable),       "");
		
		
		
		m_ivyStartEngine1 =         new MyIvyResponse(    "start_1",                   m_ivyConfig->m_mp3_path,    1,                0,                         0,                         0,                    m_ivy_object_list,     m_ivyChannel, m_IvySound, m_error_list, &(m_ivyConfig->m_callouts_enable),       "");
		m_ivyStartEngine2 =         new MyIvyResponse(    "start_2",                   m_ivyConfig->m_mp3_path,    1,                0,                         0,                         0,                    m_ivy_object_list,     m_ivyChannel, m_IvySound, m_error_list, &(m_ivyConfig->m_callouts_enable),       "");
		m_ivyStartEngine3 =         new MyIvyResponse(    "start_3",                   m_ivyConfig->m_mp3_path,    1,                0,                         0,                         0,                    m_ivy_object_list,     m_ivyChannel, m_IvySound, m_error_list, &(m_ivyConfig->m_callouts_enable),       "");
		m_ivyStartEngine4 =         new MyIvyResponse(    "start_4",                   m_ivyConfig->m_mp3_path,    1,                0,                         0,                         0,                    m_ivy_object_list,     m_ivyChannel, m_IvySound, m_error_list, &(m_ivyConfig->m_callouts_enable),       "");
		m_ivyStartAPU =             new MyIvyResponse(    "start_apu",                 m_ivyConfig->m_mp3_path,    1,                0,                         0,                         0,                    m_ivy_object_list,     m_ivyChannel, m_IvySound, m_error_list, &(m_ivyConfig->m_callouts_enable),       "");

		m_ivyLightUp1 =              new MyIvyResponse(    "light_up",                  m_ivyConfig->m_mp3_path,    1,                1.5,                      0,                         0,                    m_ivy_object_list,     m_ivyChannel, m_IvySound, m_error_list, &(m_ivyConfig->m_callouts_enable),       "");
		m_ivyLightUp2 =              new MyIvyResponse(    "light_up",                  m_ivyConfig->m_mp3_path,    1,                1.5,                      0,                         0,                    m_ivy_object_list,     m_ivyChannel, m_IvySound, m_error_list, &(m_ivyConfig->m_callouts_enable),       "");
		m_ivyLightUp3 =              new MyIvyResponse(    "light_up",                  m_ivyConfig->m_mp3_path,    1,                1.5,                      0,                         0,                    m_ivy_object_list,     m_ivyChannel, m_IvySound, m_error_list, &(m_ivyConfig->m_callouts_enable),       "");
		m_ivyLightUp4 =              new MyIvyResponse(    "light_up",                  m_ivyConfig->m_mp3_path,    1,                1.5,                      0,                         0,                    m_ivy_object_list,     m_ivyChannel, m_IvySound, m_error_list, &(m_ivyConfig->m_callouts_enable),       "");

		m_ivyFuelOn1 =               new MyIvyResponse(    "fuel_on",                   m_ivyConfig->m_mp3_path,    1,                0,                         0,                         0,                    m_ivy_object_list,     m_ivyChannel, m_IvySound, m_error_list, &(m_ivyConfig->m_callouts_enable),       "");
		m_ivyFuelOn2 =               new MyIvyResponse(    "fuel_on",                   m_ivyConfig->m_mp3_path,    1,                0,                         0,                         0,                    m_ivy_object_list,     m_ivyChannel, m_IvySound, m_error_list, &(m_ivyConfig->m_callouts_enable),       "");
		m_ivyFuelOn3 =               new MyIvyResponse(    "fuel_on",                   m_ivyConfig->m_mp3_path,    1,                0,                         0,                         0,                    m_ivy_object_list,     m_ivyChannel, m_IvySound, m_error_list, &(m_ivyConfig->m_callouts_enable),       "");
		m_ivyFuelOn4 =               new MyIvyResponse(    "fuel_on",                   m_ivyConfig->m_mp3_path,    1,                0,                         0,                         0,                    m_ivy_object_list,     m_ivyChannel, m_IvySound, m_error_list, &(m_ivyConfig->m_callouts_enable),       "");

		m_ivyN21 =                   new MyIvyResponse(    "n2",                        m_ivyConfig->m_mp3_path,    1,                0,                         0,                         0,                    m_ivy_object_list,     m_ivyChannel, m_IvySound, m_error_list, &(m_ivyConfig->m_callouts_enable),       "");
		m_ivyN22 =                   new MyIvyResponse(    "n2",                        m_ivyConfig->m_mp3_path,    1,                0,                         0,                         0,                    m_ivy_object_list,     m_ivyChannel, m_IvySound, m_error_list, &(m_ivyConfig->m_callouts_enable),       "");
		m_ivyN23 =                   new MyIvyResponse(    "n2",                        m_ivyConfig->m_mp3_path,    1,                0,                         0,                         0,                    m_ivy_object_list,     m_ivyChannel, m_IvySound, m_error_list, &(m_ivyConfig->m_callouts_enable),       "");
		m_ivyN24 =                   new MyIvyResponse(    "n2",                        m_ivyConfig->m_mp3_path,    1,                0,                         0,                         0,                    m_ivy_object_list,     m_ivyChannel, m_IvySound, m_error_list, &(m_ivyConfig->m_callouts_enable),       "");

		m_ivyEngineStab1 =           new MyIvyResponse(    "engine_stab",               m_ivyConfig->m_mp3_path,    1,                0,                         0,                         0,                    m_ivy_object_list,     m_ivyChannel, m_IvySound, m_error_list, &(m_ivyConfig->m_callouts_enable),       "");
		m_ivyEngineStab2 =           new MyIvyResponse(    "engine_stab",               m_ivyConfig->m_mp3_path,    1,                0,                         0,                         0,                    m_ivy_object_list,     m_ivyChannel, m_IvySound, m_error_list, &(m_ivyConfig->m_callouts_enable),       "");
		m_ivyEngineStab3 =           new MyIvyResponse(    "engine_stab",               m_ivyConfig->m_mp3_path,    1,                0,                         0,                         0,                    m_ivy_object_list,     m_ivyChannel, m_IvySound, m_error_list, &(m_ivyConfig->m_callouts_enable),       "");
		m_ivyEngineStab4 =           new MyIvyResponse(    "engine_stab",               m_ivyConfig->m_mp3_path,    1,                0,                         0,                         0,                    m_ivy_object_list,     m_ivyChannel, m_IvySound, m_error_list, &(m_ivyConfig->m_callouts_enable),       "");




        m_ivyCrash =                new MyIvyResponse(    "crash",                     m_ivyConfig->m_mp3_path,    0,                3,                         0,                         1,                    m_ivy_object_list,     m_ivyChannel, m_IvySound, m_error_list, &(m_ivyConfig->m_errors_enable),       "Aircraft Crashed");
        // TODO;
 
        m_ivyPressureLow =          new MyIvyResponse(    "pressure_low",              m_ivyConfig->m_mp3_path,    0,                5,                         0,                         1,                    m_ivy_object_list,     m_ivyChannel, m_IvySound, m_error_list, &(m_ivyConfig->m_errors_enable),       "Cabin Pressure Low");
        m_ivyPressureXLow =         new MyIvyResponse(    "pressure_xlow",             m_ivyConfig->m_mp3_path,    0,                1,                         0,                         1,                    m_ivy_object_list,     m_ivyChannel, m_IvySound, m_error_list, &(m_ivyConfig->m_errors_enable),       "Cabin Pressure Extremely Low");
 
        m_ivyIceWindowLow =         new MyIvyResponse(    "ice_window_low",            m_ivyConfig->m_mp3_path,    0,                20,                        0,                         1,                    m_ivy_object_list,     m_ivyChannel, m_IvySound, m_error_list, &(m_ivyConfig->m_errors_enable),       "Ice Window Low");
        m_ivyIceWindowHigh =        new MyIvyResponse(    "ice_window_high",           m_ivyConfig->m_mp3_path,    0,                20,                        0,                         1,                    m_ivy_object_list,     m_ivyChannel, m_IvySound, m_error_list, &(m_ivyConfig->m_errors_enable),       "Ice Window High");
        m_ivyIcePropellerLow =      new MyIvyResponse(    "ice_propeller_low",         m_ivyConfig->m_mp3_path,    0,                40,                        0,                         1,                    m_ivy_object_list,     m_ivyChannel, m_IvySound, m_error_list, &(m_ivyConfig->m_errors_enable),       "Ice Propeller Low"); // Inlet ice
        m_ivyIcePropellerHigh =     new MyIvyResponse(    "ice_propeller_high",        m_ivyConfig->m_mp3_path,    0,                40,                        0,                         1,                    m_ivy_object_list,     m_ivyChannel, m_IvySound, m_error_list, &(m_ivyConfig->m_errors_enable),       "Ice Propeller High");
        m_ivyIcePitotLow =          new MyIvyResponse(    "ice_pitot_low",             m_ivyConfig->m_mp3_path,    0,                60,                        0,                         1,                    m_ivy_object_list,     m_ivyChannel, m_IvySound, m_error_list, &(m_ivyConfig->m_errors_enable),       "Ice Pitot Low"); // Ice low bei 0.05
        m_ivyIcePitotHigh =         new MyIvyResponse(    "ice_pitot_high",            m_ivyConfig->m_mp3_path,    0,                60,                        0,                         1,                    m_ivy_object_list,     m_ivyChannel, m_IvySound, m_error_list, &(m_ivyConfig->m_errors_enable),       "Ice Pitot High");
        m_ivyIceAirframeLow =       new MyIvyResponse(    "ice_airframe_low",          m_ivyConfig->m_mp3_path,    0,                100,                       0,                         1,                    m_ivy_object_list,     m_ivyChannel, m_IvySound, m_error_list, &(m_ivyConfig->m_errors_enable),       "Ice Airframe Low"); // Ice high bei 0.15
        m_ivyIceAirframeHigh =      new MyIvyResponse(    "ice_airframe_high",         m_ivyConfig->m_mp3_path,    0,                100,                       0,                         1,                    m_ivy_object_list,     m_ivyChannel, m_IvySound, m_error_list, &(m_ivyConfig->m_errors_enable),       "Ice Airframe High");

		m_ivyNOE             =      new MyIvyResponse(    "noe",                       m_ivyConfig->m_mp3_path,    1,                1,                         1,                         1,                    m_ivy_object_list,     m_ivyChannel, m_IvySound, m_error_list, &(m_ivyConfig->m_errors_enable),       "Above NOE Altitude");
 
        m_ivyAnnounceTakeOff =      new MyIvyResponse(    "takeoff",                   m_ivyConfig->m_mp3_path,    0,                0,                         0,                         0,                    m_ivy_object_list,     m_ivyChannel, m_IvySound, m_error_list, &(m_ivyConfig->m_errors_enable),       "");
        m_ivyAnnounceLanding =      new MyIvyResponse(    "landing",                   m_ivyConfig->m_mp3_path,    0,                0,                         0,                         0,                    m_ivy_object_list,     m_ivyChannel, m_IvySound, m_error_list, &(m_ivyConfig->m_errors_enable),       "");
 
        m_ivySeatBelts =            new MyIvyResponse(    "seatbelts",                 m_ivyConfig->m_mp3_path,    0,                1,                         0,                         0,                    m_ivy_object_list,     m_ivyChannel, m_IvySound, m_error_list, &(m_ivyConfig->m_errors_enable),       "");

		m_ivyOverweight =           new MyIvyResponse(    "overweight",                m_ivyConfig->m_mp3_path,    0,               30,                         0,                         1,                    m_ivy_object_list,     m_ivyChannel, m_IvySound, m_error_list, &(m_ivyConfig->m_errors_enable),       "Aircraft Overweight");
		m_ivyHeightVelocity =       new MyIvyResponse(    "height_velocity",           m_ivyConfig->m_mp3_path,    0,                1,                         1,                         0,                    m_ivy_object_list,     m_ivyChannel, m_IvySound, m_error_list, &(m_ivyConfig->m_errors_enable),       "HV Violation");

 
        m_ivyMinimums =             new MyIvyResponse(    "minimums",                  m_ivyConfig->m_mp3_path,    0,                0,                         10,                        0,                    m_ivy_object_list,     m_ivyChannel, m_IvySound, m_error_list, &(m_ivyConfig->m_errors_enable),       "");
        m_ivyApplause =             new MyIvyResponse(    "passenger_applause",        m_ivyConfig->m_mp3_path,    1,                5,                         30,                        0,                    m_ivy_object_list,     m_ivyChannelPassengersGeneral, m_IvySound, m_error_list, &(m_ivyConfig->m_passengers_enabled),       "");
 
        // No Callout Events;
        m_ivyArmLanding =           new MyIvyResponse(    "arm_landing",               m_ivyConfig->m_mp3_path,    0,                1,                         0,                         0,                    m_ivy_object_list,     m_ivyChannel, m_IvySound, m_error_list, &(m_ivyConfig->m_errors_enable),       "");
        m_ivyArmMinimums =          new MyIvyResponse(    "arm_descent",               m_ivyConfig->m_mp3_path,    0,                1,                         10,                        0,                    m_ivy_object_list,     m_ivyChannel, m_IvySound, m_error_list, &(m_ivyConfig->m_errors_enable),       "");
		m_ivyLogWriter =            new MyIvyResponse(    "log_writer",                m_ivyConfig->m_mp3_path,    0,                0,                         0,                         1,                    m_ivy_object_list,     m_ivyChannel, m_IvySound, m_error_list, &(m_ivyConfig->m_errors_enable),       "");

		// 206 Events
		m_ivyRpmLow =               new MyIvyResponse(    "rpm_low",                   m_ivyConfig->m_mp3_path,    0,                0,                         0,                         1,                    m_ivy_object_list,     m_ivyChannel, m_IvySound, m_error_list, &(m_ivyConfig->m_errors_enable),       "Rotor RPM Low");
		m_ivyRpmHigh =              new MyIvyResponse(    "rpm_high",                   m_ivyConfig->m_mp3_path,    0,                0,                         0,                         1,                    m_ivy_object_list,     m_ivyChannel, m_IvySound, m_error_list, &(m_ivyConfig->m_errors_enable),       "Rotor RPM High");
		m_ivyBattDrain =            new MyIvyResponse(    "batt_drain",                m_ivyConfig->m_mp3_path,    0,                0,                         0,                         1,                    m_ivy_object_list,     m_ivyChannel, m_IvySound, m_error_list, &(m_ivyConfig->m_errors_enable),       "Draining Battery");
		m_ivyStarterPrecond =       new MyIvyResponse(    "start_precond",             m_ivyConfig->m_mp3_path,    0,                0,                         0,                         1,                    m_ivy_object_list,     m_ivyChannel, m_IvySound, m_error_list, &(m_ivyConfig->m_errors_enable),       "Engine Startup Precond");
		m_ivyStarterOffEarly =      new MyIvyResponse(    "starter_off_early",         m_ivyConfig->m_mp3_path,    0,                0,                         0,                         1,                    m_ivy_object_list,     m_ivyChannel, m_IvySound, m_error_list, &(m_ivyConfig->m_errors_enable),       "Starter Off Early");
		m_ivyStarterOffLate =       new MyIvyResponse(    "starter_off_late",          m_ivyConfig->m_mp3_path,    0,                0,                         0,                         1,                    m_ivy_object_list,     m_ivyChannel, m_IvySound, m_error_list, &(m_ivyConfig->m_errors_enable),       "Starter Off Late");
		m_ivyCyclicNotCentered =    new MyIvyResponse(    "cyclic_not_centered",       m_ivyConfig->m_mp3_path,    0,                0,                         0,                         1,                    m_ivy_object_list,     m_ivyChannel, m_IvySound, m_error_list, &(m_ivyConfig->m_errors_enable),       "Cyclic Not Centered");
		m_ivyCollectiveHigh =       new MyIvyResponse(    "collective_high",           m_ivyConfig->m_mp3_path,    0,                0,                         0,                         1,                    m_ivy_object_list,     m_ivyChannel, m_IvySound, m_error_list, &(m_ivyConfig->m_errors_enable),       "Collective Not Lowered");
		m_ivyPitotHot =             new MyIvyResponse(    "pitot_heating_hot",         m_ivyConfig->m_mp3_path,    0,                300,                       0,                         1,                    m_ivy_object_list,     m_ivyChannel, m_IvySound, m_error_list, &(m_ivyConfig->m_errors_enable),       "Pitot Overheating");
		m_ivyAntiIceHot =           new MyIvyResponse(    "anti_ice_hot",              m_ivyConfig->m_mp3_path,    0,                300,                       0,                         1,                    m_ivy_object_list,     m_ivyChannel, m_IvySound, m_error_list, &(m_ivyConfig->m_errors_enable),       "Anti-Ice Overheating");
		m_ivyIdleTime =             new MyIvyResponse(    "engine_idle_time",          m_ivyConfig->m_mp3_path,    0,                0,                         0,                         1,                    m_ivy_object_list,     m_ivyChannel, m_IvySound, m_error_list, &(m_ivyConfig->m_errors_enable),       "Engine OFF IDLE Time Violation");
		m_ivyVRS =                  new MyIvyResponse(    "vrs_danger",                m_ivyConfig->m_mp3_path,    0,                0,                         0,                         1,                    m_ivy_object_list,     m_ivyChannel, m_IvySound, m_error_list, &(m_ivyConfig->m_errors_enable),       "VRS Conditions");
		m_ivyCircuitBreakers =      new MyIvyResponse(    "circuit_breakers",          m_ivyConfig->m_mp3_path,    0,                0,                         1,                         1,                    m_ivy_object_list,     m_ivyChannel, m_IvySound, m_error_list, &(m_ivyConfig->m_errors_enable),       "Circuit Breakers");
		m_ivyEngineWear =           new MyIvyResponse(    "engine_wear",               m_ivyConfig->m_mp3_path,    0,                0,                         0,                         1,                    m_ivy_object_list,     m_ivyChannel, m_IvySound, m_error_list, &(m_ivyConfig->m_errors_enable),       "High Engine Wear");
		m_ivyServiceCeiling =       new MyIvyResponse(    "above_max_alt",             m_ivyConfig->m_mp3_path,    0,                0,                         0,                         1,                    m_ivy_object_list,     m_ivyChannel, m_IvySound, m_error_list, &(m_ivyConfig->m_errors_enable),       "Above Service Ceiling");
		m_ivyWindAzimuth =          new MyIvyResponse(    "azimuth_area",              m_ivyConfig->m_mp3_path,    0,                0,                         0,                         1,                    m_ivy_object_list,     m_ivyChannel, m_IvySound, m_error_list, &(m_ivyConfig->m_errors_enable),       "Critical Wind Direction");
		m_ivyGenLowRPM =            new MyIvyResponse(    "gen_early",                 m_ivyConfig->m_mp3_path,    0,                0,                         0,                         1,                    m_ivy_object_list,     m_ivyChannel, m_IvySound, m_error_list, &(m_ivyConfig->m_errors_enable),       "Generator Low RPM");
		

	}

	m_s_low.clear();
	m_s_hundreds.clear();
	m_s_hrm.clear();

	for (int index = 0; index < 100; index++)
		m_s_low.push_back(IvyLoadSingleSoundFile(std::to_string(index), true));

	for (int index = 0; index < 1000; index+=100)
		m_s_hundreds.push_back(IvyLoadSingleSoundFile(std::to_string(index), true));

	for (int index = 0; index < 100; index++)
	{
		std::string mp3_name = "hrm_";
		mp3_name += std::to_string(index);
		m_s_hrm.push_back(IvyLoadSingleSoundFile(mp3_name, false));
	}

	m_s_million = IvyLoadSingleSoundFile("1000000", true);
	m_s_thousand = IvyLoadSingleSoundFile("1000", true);

	m_s_error_zero = IvyLoadSingleSoundFile("error_zero");
	m_s_error_good = IvyLoadSingleSoundFile("error_good");
	m_s_error_bad = IvyLoadSingleSoundFile("error_bad");
	m_s_error_xbad = IvyLoadSingleSoundFile("error_xbad");
	m_s_error_a = IvyLoadSingleSoundFile("error_a");
	m_s_error_b = IvyLoadSingleSoundFile("error_b");
	m_s_landing_rate = IvyLoadSingleSoundFile("landing_rate");
	m_s_landing_feet = IvyLoadSingleSoundFile("landing_feet");
	m_s_landing_g = IvyLoadSingleSoundFile("landing_g");
	m_s_dot = IvyLoadSingleSoundFile("dot");
	m_s_wind1 = IvyLoadSingleSoundFile("wind1");
	m_s_wind2 = IvyLoadSingleSoundFile("wind2");
	m_s_knots = IvyLoadSingleSoundFile("knots");
	m_s_bounce1 = IvyLoadSingleSoundFile("bounce1");
	m_s_bounce2s = IvyLoadSingleSoundFile("bounce2s");
	m_s_bounce2 = IvyLoadSingleSoundFile("bounce2");
	m_s_no_bounce = IvyLoadSingleSoundFile("no_bounce");
	m_s_baro_press_1 = IvyLoadSingleSoundFile("baro_press_1");
	m_s_ouch = IvyLoadSingleSoundFile("ouch_1");

	m_s_fse_start1 = IvyLoadSingleSoundFile("fse_start1");
	m_s_fse_start2 = IvyLoadSingleSoundFile("fse_start2");
	m_s_fse_start3 = IvyLoadSingleSoundFile("fse_start3");
	m_s_fse_start4 = IvyLoadSingleSoundFile("fse_start4");
}

FMOD_SOUND* MyIvy::IvyLoadSingleSoundFile(std::string sound_file, bool is_number)
{
	std::string sound_file_path = m_ivyConfig->m_mp3_path;
	if (is_number)
		sound_file_path = m_ivyConfig->m_number_path;

	sound_file_path += sound_file + ".mp3";

	if (file_exists(sound_file_path))
	{
		ivy_output_file << "Loading: " << sound_file_path << std::endl;
		ivy_output_file.flush();
		return m_IvySound->CreateBuffer(sound_file_path);
	}

	ivy_output_file << "File not found: " << sound_file_path << std::endl;
	ivy_output_file.flush();
	
	return NULL;
}

void MyIvy::IvyAssignDataRefs()
{


	m_i_paused = XPLMFindDataRef("sim/time/paused");
	m_i_fse_flying = XPLMFindDataRef("fse/status/flying");

	m_f_fuel_tot = XPLMFindDataRef("sim/flightmodel/weight/m_fuel_total");
	m_f_payload_tot = XPLMFindDataRef("sim/flightmodel/weight/m_fixed");
	m_f_weight_max = XPLMFindDataRef("sim/aircraft/weight/acf_m_max");

	m_f_weight_tot = XPLMFindDataRef("sim/flightmodel/weight/m_total");

	m_i_on_ground = XPLMFindDataRef("sim/flightmodel/failures/onground_any");
	m_f_climb_rate = XPLMFindDataRef("sim/flightmodel/position/vh_ind_fpm");

	m_f_gear1_ratio = XPLMFindDataRef("sim/flightmodel/movingparts/gear1def");
	m_f_gear2_ratio = XPLMFindDataRef("sim/flightmodel/movingparts/gear2def");
	m_f_gear3_ratio = XPLMFindDataRef("sim/flightmodel/movingparts/gear3def");
	m_f_gear4_ratio = XPLMFindDataRef("sim/flightmodel/movingparts/gear4def");
	m_f_gear5_ratio = XPLMFindDataRef("sim/flightmodel/movingparts/gear5def");

	m_i_gear_handle = XPLMFindDataRef("sim/cockpit/switches/gear_handle_status");
	m_f_flap_ratio = XPLMFindDataRef("sim/cockpit2/controls/flap_handle_request_ratio");

	m_f_ground_speed = XPLMFindDataRef("sim/flightmodel/position/groundspeed");
	m_f_ias = XPLMFindDataRef("sim/flightmodel/position/indicated_airspeed");
	m_f_sun_pitch = XPLMFindDataRef("sim/graphics/scenery/sun_pitch_degrees");
	m_f_airport_light = XPLMFindDataRef("sim/graphics/scenery/airport_light_level"); //0-1;
	m_f_world_light_precent = XPLMFindDataRef("sim/graphics/scenery/percent_lights_on");
	m_i_has_skid = XPLMFindDataRef("sim/aircraft/gear/acf_gear_is_skid");
	m_i_transponder_mode = XPLMFindDataRef("sim/cockpit/radios/transponder_mode"); // on = 3;
	m_i_sim_ground_speed = XPLMFindDataRef("sim/time/ground_speed");

	m_i_temp_sl = XPLMFindDataRef("sim/weather/region/sealevel_temperature_c"); // Td ~ T - (100-RH)/5 where Td is the dew point, T is temperature, RH is relative humidity;
	m_i_dew_sl = XPLMFindDataRef("sim/weather/region/dewpoint_deg_c");

	m_i_landing_lights = XPLMFindDataRef("sim/cockpit/electrical/landing_lights_on");
	m_i_beacon_lights = XPLMFindDataRef("sim/cockpit/electrical/beacon_lights_on");
	m_i_nav_lights = XPLMFindDataRef("sim/cockpit/electrical/nav_lights_on");
	m_i_strobe_lights = XPLMFindDataRef("sim/cockpit/electrical/strobe_lights_on");
	m_i_taxi_lights = XPLMFindDataRef("sim/cockpit/electrical/taxi_light_on");
	m_i_cockpit_lights = XPLMFindDataRef("sim/cockpit/electrical/cockpit_lights_on");
	m_f_radio_alt = XPLMFindDataRef("sim/cockpit2/gauges/indicators/radio_altimeter_height_ft_pilot");
	m_f_decision_height = XPLMFindDataRef("sim/cockpit/misc/radio_altimeter_minimum");
	m_f8_batter_charge = XPLMFindDataRef("sim/cockpit/electrical/battery_charge_watt_hr");
	m_i_battery_on = XPLMFindDataRef("sim/cockpit/electrical/battery_on");
	m_i_gpu_on = XPLMFindDataRef("sim/cockpit/electrical/gpu_on");

	m_i_flaps_overspeed = XPLMFindDataRef("sim/flightmodel/failures/over_vfe");
	m_i_gear_overspeed = XPLMFindDataRef("sim/flightmodel/failures/over_vle");
	m_f_aircraft_vne = XPLMFindDataRef("sim/aircraft/view/acf_Vne");
	m_i_aircraft_overspeed = XPLMFindDataRef("sim/flightmodel/failures/over_vne");
	m_i_stall = XPLMFindDataRef("sim/flightmodel/failures/stallwarning");

	m_i_cloud_0 = XPLMFindDataRef("sim/weather/aircraft/cloud_type[0]");
	m_i_cloud_1 = XPLMFindDataRef("sim/weather/aircraft/cloud_type[0]");
	m_i_cloud_2 = XPLMFindDataRef("sim/weather/aircraft/cloud_type[0]");
	m_f_visibility = XPLMFindDataRef("sim/weather/region/visibility_reported_sm");
	m_i_rain = XPLMFindDataRef("sim/weather/region/rain_percent");
	m_i_thunder = XPLMFindDataRef("sim/weather/thunderstorm_percent");
	m_i_turbulence = XPLMFindDataRef("sim/weather/region/turbulence");

	m_i_batt1 = XPLMFindDataRef("sim/operation/failures/rel_bat0_lo");
	m_i_batt2 = XPLMFindDataRef("sim/operation/failures/rel_bat1_lo");

	m_i_tire1 = XPLMFindDataRef("sim/operation/failures/rel_tire1"); //eight;
	m_i_tire2 = XPLMFindDataRef("sim/operation/failures/rel_tire2");
	m_i_tire3 = XPLMFindDataRef("sim/operation/failures/rel_tire3");
	m_i_tire4 = XPLMFindDataRef("sim/operation/failures/rel_tire4");
	m_i_tire5 = XPLMFindDataRef("sim/operation/failures/rel_tire5");

	m_i_fire1 = XPLMFindDataRef("sim/operation/failures/rel_engfir0"); //multiple;
	m_i_fire2 = XPLMFindDataRef("sim/operation/failures/rel_engfir1");
	m_i_fire3 = XPLMFindDataRef("sim/operation/failures/rel_engfir2");
	m_i_fire4 = XPLMFindDataRef("sim/operation/failures/rel_engfir3");
	m_i_fire5 = XPLMFindDataRef("sim/operation/failures/rel_engfir4");
	m_i_fire6 = XPLMFindDataRef("sim/operation/failures/rel_engfir5");
	m_i_fire7 = XPLMFindDataRef("sim/operation/failures/rel_engfir6");
	m_i_fire8 = XPLMFindDataRef("sim/operation/failures/rel_engfir7");

	m_i_flameout1 = XPLMFindDataRef("sim/operation/failures/rel_engfla0"); //multiple;
	m_i_flameout2 = XPLMFindDataRef("sim/operation/failures/rel_engfla1");
	m_i_flameout3 = XPLMFindDataRef("sim/operation/failures/rel_engfla2");
	m_i_flameout4 = XPLMFindDataRef("sim/operation/failures/rel_engfla3");
	m_i_flameout5 = XPLMFindDataRef("sim/operation/failures/rel_engfla4");
	m_i_flameout6 = XPLMFindDataRef("sim/operation/failures/rel_engfla5");
	m_i_flameout7 = XPLMFindDataRef("sim/operation/failures/rel_engfla6");
	m_i_flameout8 = XPLMFindDataRef("sim/operation/failures/rel_engfla7");



	m_i_engine_failure1 = XPLMFindDataRef("sim/operation/failures/rel_engfai0"); //multiple;
	m_i_engine_failure2 = XPLMFindDataRef("sim/operation/failures/rel_engfai1");
	m_i_engine_failure3 = XPLMFindDataRef("sim/operation/failures/rel_engfai2");
	m_i_engine_failure4 = XPLMFindDataRef("sim/operation/failures/rel_engfai3");
	m_i_engine_failure5 = XPLMFindDataRef("sim/operation/failures/rel_engfai4");
	m_i_engine_failure6 = XPLMFindDataRef("sim/operation/failures/rel_engfai5");
	m_i_engine_failure7 = XPLMFindDataRef("sim/operation/failures/rel_engfai6");
	m_i_engine_failure8 = XPLMFindDataRef("sim/operation/failures/rel_engfai7");

	m_i_hot1 = XPLMFindDataRef("sim/operation/failures/rel_hotsta0"); //multiple;
	m_i_hot2 = XPLMFindDataRef("sim/operation/failures/rel_hotsta1");
	m_i_hot3 = XPLMFindDataRef("sim/operation/failures/rel_hotsta2");
	m_i_hot4 = XPLMFindDataRef("sim/operation/failures/rel_hotsta3");
	m_i_hot5 = XPLMFindDataRef("sim/operation/failures/rel_hotsta4");
	m_i_hot6 = XPLMFindDataRef("sim/operation/failures/rel_hotsta5");
	m_i_hot7 = XPLMFindDataRef("sim/operation/failures/rel_hotsta6");
	m_i_hot8 = XPLMFindDataRef("sim/operation/failures/rel_hotsta7");

	m_f_ice_frame = XPLMFindDataRef("sim/flightmodel/failures/frm_ice");
	m_f_ice_pitot = XPLMFindDataRef("sim/flightmodel/failures/pitot_ice");
	m_f_ice_propeller = XPLMFindDataRef("sim/flightmodel/failures/prop_ice");
	m_f_ice_window = XPLMFindDataRef("sim/flightmodel/failures/window_ice");

	m_f_g_normal = XPLMFindDataRef("sim/flightmodel/forces/g_nrml");
	m_f_g_forward = XPLMFindDataRef("sim/flightmodel/forces/g_axil");
	m_f_g_side = XPLMFindDataRef("sim/flightmodel/forces/g_side");

	m_i_bird = XPLMFindDataRef("sim/operation/failures/rel_bird_strike");


	m_f_pitch = XPLMFindDataRef("sim/flightmodel/position/theta");
	m_f_roll = XPLMFindDataRef("sim/flightmodel/position/phi");
	m_f_yaw = XPLMFindDataRef("sim/flightmodel/position/beta");

	//cab press;
	m_f_cab_press = XPLMFindDataRef("sim/cockpit2/pressurization/indicators/cabin_altitude_ft"); // meter?; Not anymore!
	//cab climb rate;
	m_f_cab_rate = XPLMFindDataRef("sim/cockpit/pressure/cabin_vvi_actual_fpm"); // meter per second? 1000ft/min = 5m/s;
	//cab humidity;
	//cab temp;

	m_f_outside_temp1 = XPLMFindDataRef("sim/weather/aircraft/temperature_ambient_deg_c");
	m_f_outside_temp2 = XPLMFindDataRef("sim/cockpit2/temperature/outside_air_temp_degc");
	m_f_outside_temp3 = XPLMFindDataRef("sim/cockpit2/temperature/outside_air_LE_temp_degc");

	m_f_baro_set = XPLMFindDataRef("sim/cockpit/misc/barometer_setting");
	m_f_baro_sea_level = XPLMFindDataRef("sim/weather/region/sealevel_pressure_pas"); // Pascal
	m_f_baro_alt = XPLMFindDataRef("sim/flightmodel/misc/h_ind");

	m_f_wind_direction = XPLMFindDataRef("sim/weather/aircraft/wind_now_direction_degt");
	m_f_wind_speed_kt = XPLMFindDataRef("sim/weather/aircraft/wind_now_speed_msc");

	m_f_slats_1 = XPLMFindDataRef("sim/flightmodel2/controls/slat1_deploy_ratio");
	m_f_flaps_1 = XPLMFindDataRef("sim/flightmodel2/controls/flap1_deploy_ratio");

	m_d_latitude = XPLMFindDataRef("sim/flightmodel/position/latitude");
	m_d_longitude = XPLMFindDataRef("sim/flightmodel/position/longitude");

	m_i_nonsmoking = XPLMFindDataRef("sim/cockpit/switches/no_smoking");
	m_i_fastenseatbelt = XPLMFindDataRef("sim/cockpit/switches/fasten_seat_belts");

	m_i_replay = XPLMFindDataRef("sim/operation/prefs/replay_mode");


	m_f_hsi_hdef_dot = XPLMFindDataRef("sim/cockpit2/radios/indicators/hsi_hdef_dots_pilot");
	m_f_hsi_vdef_dot = XPLMFindDataRef("sim/cockpit2/radios/indicators/hsi_vdef_dots_pilot");

	m_i_hsi_display_h = XPLMFindDataRef("sim/cockpit2/radios/indicators/hsi_display_horizontal_pilot");
	m_i_hsi_display_v = XPLMFindDataRef("sim/cockpit2/radios/indicators/hsi_display_vertical_pilot");
	m_f_acf_heading = XPLMFindDataRef("sim/flightmodel/position/mag_psi");



	m_i_hsi_source_select = XPLMFindDataRef("sim/cockpit2/radios/actuators/HSI_source_select_pilot");
	m_f_speed_brake = XPLMFindDataRef("sim/flightmodel/controls/sbrkrqst");
	m_ia_engine_start = XPLMFindDataRef("sim/cockpit2/engine/actuators/starter_hit");
	m_ia_engine_running = XPLMFindDataRef("sim/flightmodel/engine/ENGN_running");
	m_ia_generator_running = XPLMFindDataRef("sim/cockpit2/electrical/generator_on");
	m_i_apu_starter_switch = XPLMFindDataRef("sim/cockpit/engine/APU_switch");


	m_f_throttle_ratio = XPLMFindDataRef("sim/cockpit2/engine/actuators/throttle_beta_rev_ratio_all");

	m_f_n1_percent = XPLMFindDataRef("sim/cockpit2/engine/indicators/N1_percent");
	m_f_n2_percent = XPLMFindDataRef("sim/cockpit2/engine/indicators/N2_percent");

	m_f_egt_c = XPLMFindDataRef("sim/flightmodel2/engines/EGT_deg_cel");
	m_f_fuel_flow = XPLMFindDataRef("sim/flightmodel/engine/ENGN_FF_");


	

	m_f_cyclic_ail = XPLMFindDataRef("sim/cockpit2/engine/actuators/cyclic_aileron_deg");
	m_f_cyclic_elev = XPLMFindDataRef("sim/cockpit2/engine/actuators/cyclic_elevator_deg");
	m_fa_torque = XPLMFindDataRef("sim/flightmodel/engine/ENGN_TRQ");
	m_fa_itt = XPLMFindDataRef("sim/flightmodel2/engines/ITT_deg_cel");
	m_i_pitot_heat = XPLMFindDataRef("sim/cockpit/switches/pitot_heat_on");
	m_ia_eng_anti_ice = XPLMFindDataRef("sim/cockpit2/ice/cowling_thermal_anti_ice_per_engine");
	m_f_vertical_speed = XPLMFindDataRef("sim/flightmodel/position/vh_ind_fpm");

	m_i_avionics = XPLMFindDataRef("sim/cockpit/electrical/avionics_on");

	//m_f_cyclic_elev = XPLMFindDataRef("sim/cockpit2/engine/actuators/cyclic_elevator_deg");
	//m_f_cyclic_ail = XPLMFindDataRef("sim/cockpit2/engine/actuators/cyclic_aileron_deg");

}

void MyIvy::CreateVSpeedsWidget()
{
	int x = 300;
	int y = 300;
	int x2 = x + 200;
	int y2 = y - 200;

	m_VSpeedsWidget = XPCreateWidget(x, y, x2, y2, 1, "Ivy VSpeeds", 1, 0, xpWidgetClass_MainWindow);


	// Add Close Box decorations to the Main Widget;
	XPSetWidgetProperty(m_VSpeedsWidget, xpProperty_MainWindowHasCloseBoxes, 1);

	m_v1_label = XPCreateWidget(x + 10, y - 25, x + 50, y - 40, 1, "V1:", 0, m_VSpeedsWidget, xpWidgetClass_Caption);
	m_vr_label = XPCreateWidget(x + 10, y - 45, x + 50, y - 60, 1, "VR:", 0, m_VSpeedsWidget, xpWidgetClass_Caption);
	m_v2_label = XPCreateWidget(x + 10, y - 65, x + 50, y - 80, 1, "V2:", 0, m_VSpeedsWidget, xpWidgetClass_Caption);
	m_dh_label = XPCreateWidget(x + 10, y - 85, x + 50, y - 100, 1, "DH:", 0, m_VSpeedsWidget, xpWidgetClass_Caption);

	m_kt60_label = XPCreateWidget(x + 10, y - 105, x + 50, y - 120, 1, "60kt:", 0, m_VSpeedsWidget, xpWidgetClass_Caption);
	m_kt80_label = XPCreateWidget(x + 10, y - 125, x + 50, y - 140, 1, "80kt:", 0, m_VSpeedsWidget, xpWidgetClass_Caption);
	m_kt100_label = XPCreateWidget(x + 10, y - 145, x + 50, y - 160, 1, "100kt:", 0, m_VSpeedsWidget, xpWidgetClass_Caption);

	m_v1_label_val = XPCreateWidget(x + 50, y - 25, x + 80, y - 40, 1, std::to_string(m_ivyAircraft->m_li_v1).c_str(), 0, m_VSpeedsWidget, xpWidgetClass_Caption);
	m_vr_label_val = XPCreateWidget(x + 50, y - 45, x + 80, y - 60, 1, std::to_string(m_ivyAircraft->m_li_vr).c_str(), 0, m_VSpeedsWidget, xpWidgetClass_Caption);
	m_v2_label_val = XPCreateWidget(x + 50, y - 65, x + 80, y - 80, 1, std::to_string(m_ivyAircraft->m_li_v2).c_str(), 0, m_VSpeedsWidget, xpWidgetClass_Caption);
	m_dh_label_val = XPCreateWidget(x + 50, y - 85, x + 80, y - 100, 1, std::to_string(int(m_lf_decision_height)).c_str(), 0, m_VSpeedsWidget, xpWidgetClass_Caption);

	m_v1_textbox = XPCreateWidget(x + 80, y - 30, x + 120, y - 40, 1, std::to_string(m_ivyAircraft->m_li_v1).c_str(), 0, m_VSpeedsWidget, xpWidgetClass_TextField);
	m_vr_textbox = XPCreateWidget(x + 80, y - 50, x + 120, y - 60, 1, std::to_string(m_ivyAircraft->m_li_vr).c_str(), 0, m_VSpeedsWidget, xpWidgetClass_TextField);
	m_v2_textbox = XPCreateWidget(x + 80, y - 70, x + 120, y - 80, 1, std::to_string(m_ivyAircraft->m_li_v2).c_str(), 0, m_VSpeedsWidget, xpWidgetClass_TextField);
	m_dh_textbox = XPCreateWidget(x + 80, y - 90, x + 120, y - 100, 1, std::to_string(int(m_lf_decision_height)).c_str(), 0, m_VSpeedsWidget, xpWidgetClass_TextField);

	XPSetWidgetProperty(m_v1_textbox, xpProperty_TextFieldType, xpTextEntryField);
	XPSetWidgetProperty(m_vr_textbox, xpProperty_TextFieldType, xpTextEntryField);
	XPSetWidgetProperty(m_v2_textbox, xpProperty_TextFieldType, xpTextEntryField);
	XPSetWidgetProperty(m_dh_textbox, xpProperty_TextFieldType, xpTextEntryField);

	XPSetWidgetProperty(m_v1_textbox, xpProperty_Enabled, 1);
	XPSetWidgetProperty(m_vr_textbox, xpProperty_Enabled, 1);
	XPSetWidgetProperty(m_v2_textbox, xpProperty_Enabled, 1);
	XPSetWidgetProperty(m_dh_textbox, xpProperty_Enabled, 1);

	m_v1_button = XPCreateWidget(x + 140, y - 30, x + 180, y - 40, 1, "Set", 0, m_VSpeedsWidget, xpWidgetClass_Button);
	m_vr_button = XPCreateWidget(x + 140, y - 50, x + 180, y - 60, 1, "Set", 0, m_VSpeedsWidget, xpWidgetClass_Button);
	m_v2_button = XPCreateWidget(x + 140, y - 70, x + 180, y - 80, 1, "Set", 0, m_VSpeedsWidget, xpWidgetClass_Button);
	m_dh_button = XPCreateWidget(x + 140, y - 90, x + 180, y - 100, 1, "Set", 0, m_VSpeedsWidget, xpWidgetClass_Button);

	m_kt60_button = XPCreateWidget(x + 55, y - 110, x + 65, y - 120, 1, "", 0, m_VSpeedsWidget, xpWidgetClass_Button);
	XPSetWidgetProperty(m_kt60_button, xpProperty_ButtonType, xpRadioButton);
	XPSetWidgetProperty(m_kt60_button, xpProperty_ButtonBehavior, xpButtonBehaviorCheckBox);
	if (m_ivyConfig->m_kt60_enabled == true)
		XPSetWidgetProperty(m_kt60_button, xpProperty_ButtonState, 1);
	else
		XPSetWidgetProperty(m_kt60_button, xpProperty_ButtonState, 0);

	m_kt80_button = XPCreateWidget(x + 55, y - 130, x + 65, y - 140, 1, "", 0, m_VSpeedsWidget, xpWidgetClass_Button);
	XPSetWidgetProperty(m_kt80_button, xpProperty_ButtonType, xpRadioButton);
	XPSetWidgetProperty(m_kt80_button, xpProperty_ButtonBehavior, xpButtonBehaviorCheckBox);
	if (m_ivyConfig->m_kt80_enabled == true)
		XPSetWidgetProperty(m_kt80_button, xpProperty_ButtonState, 1);
	else
		XPSetWidgetProperty(m_kt80_button, xpProperty_ButtonState, 0);

	m_kt100_button = XPCreateWidget(x + 55, y - 150, x + 65, y - 160, 1, "", 0, m_VSpeedsWidget, xpWidgetClass_Button);
	XPSetWidgetProperty(m_kt100_button, xpProperty_ButtonType, xpRadioButton);
	XPSetWidgetProperty(m_kt100_button, xpProperty_ButtonBehavior, xpButtonBehaviorCheckBox);
	if (m_ivyConfig->m_kt100_enabled == true)
		XPSetWidgetProperty(m_kt100_button, xpProperty_ButtonState, 1);
	else
		XPSetWidgetProperty(m_kt100_button, xpProperty_ButtonState, 0);

	XPAddWidgetCallback(m_VSpeedsWidget, WrapIvyVSpeedHandler);

	ivy_output_file << "VSpeed Widget created" << std::endl;
	ivy_output_file.flush();

}

int MyIvy::IvyVSpeedHandler(XPWidgetMessage inMessage, XPWidgetID inWidget, intptr_t inParam1, intptr_t inParam2)
{

	if (m_initialized == false) return 0;

	if (inMessage == xpMessage_CloseButtonPushed)
	{
		if (m_MenuVSpeedsShow == 1)
		{
			XPDestroyWidget(m_VSpeedsWidget, 1);
			m_MenuVSpeedsShow = 0;
		}
		return 1;
	}

	if (inMessage == xpMsg_PushButtonPressed)
	{
		try
		{
			if (inParam1 == (intptr_t) m_v1_button)
			{
				char buffer[1024];
				XPGetWidgetDescriptor(m_v1_textbox, buffer, 256);
				std::string text = buffer;
				m_ivyAircraft->m_li_v1 = std::stoi(text);
				XPSetWidgetDescriptor(m_v1_textbox, std::to_string(m_ivyAircraft->m_li_v1).c_str());
				XPSetWidgetDescriptor(m_v1_label_val, std::to_string(m_ivyAircraft->m_li_v1).c_str());
			}

			if (inParam1 == (intptr_t)m_vr_button)
			{
				char buffer[1024];
				XPGetWidgetDescriptor(m_vr_textbox, buffer, 256);
				std::string text = buffer;
				m_ivyAircraft->m_li_vr = std::stoi(text);
				XPSetWidgetDescriptor(m_vr_textbox, std::to_string(m_ivyAircraft->m_li_vr).c_str());
				XPSetWidgetDescriptor(m_vr_label_val, std::to_string(m_ivyAircraft->m_li_vr).c_str());
			}

			if (inParam1 == (intptr_t)m_v2_button)
			{
				char buffer[1024];
				XPGetWidgetDescriptor(m_v2_textbox, buffer, 256);
				std::string text = buffer;
				m_ivyAircraft->m_li_v2 = std::stoi(text);
				XPSetWidgetDescriptor(m_v2_textbox, std::to_string(m_ivyAircraft->m_li_v2).c_str());
				XPSetWidgetDescriptor(m_v2_label_val, std::to_string(m_ivyAircraft->m_li_v2).c_str());
			}

			if (inParam1 == (intptr_t)m_dh_button)
			{
				char buffer[1024];
				XPGetWidgetDescriptor(m_dh_textbox, buffer, 256);
				std::string text = buffer;
				float dh_new = (float)std::stoi(text);
				XPLMSetDataf(m_f_decision_height, dh_new);
				m_lf_decision_height = XPLMGetDataf(m_f_decision_height);

				XPSetWidgetDescriptor(m_dh_textbox, std::to_string((int)m_lf_decision_height).c_str());
				XPSetWidgetDescriptor(m_dh_label_val, std::to_string((int)m_lf_decision_height).c_str());
			}
		}
		// If conversion failed, do nothing
		catch (...) {}
	}

	if (inMessage == xpMsg_ButtonStateChanged)
	{
		if (inParam1 == (intptr_t) m_kt60_button)
		{
			if (XPGetWidgetProperty(m_kt60_button, xpProperty_ButtonState, NULL) == 1)			m_ivyConfig->m_kt60_enabled = true;
			else																				m_ivyConfig->m_kt60_enabled = false;
		}

		if (inParam1 == (intptr_t) m_kt80_button)
		{
			if (XPGetWidgetProperty(m_kt80_button, xpProperty_ButtonState, NULL) == 1)			m_ivyConfig->m_kt80_enabled = true;
			else																				m_ivyConfig->m_kt80_enabled = false;
		}

		if (inParam1 == (intptr_t) m_kt100_button)
		{
			if (XPGetWidgetProperty(m_kt100_button, xpProperty_ButtonState, NULL) == 1)			m_ivyConfig->m_kt100_enabled = true;
			else																				m_ivyConfig->m_kt100_enabled = false;
		}
	}

	return 0;
}

void MyIvy::RefreshLogbook()
{
	std::ifstream logbook_file;
	std::string line;

	logbook_file.open(m_ivyConfig->m_logbook_path.c_str(), std::ifstream::in);
	while (getline(logbook_file, line))
		m_logbook_entries.push_back(line);
	logbook_file.close();
}

void MyIvy::CreateLogbookWidget(int x, int y, int w, int h)
{
	ivy_output_file << "CreateLogbookWidget" << std::endl;
	ivy_output_file.flush();

	if (m_initialized == false) return;

	int x2 = x + w;
	int y2 = y - h;

	// Create the Main Widget window;
	m_LogbookWidget = XPCreateWidget(x, y, x2, y2, 1, "Ivy Loogbook", 1, 0, xpWidgetClass_MainWindow);
	XPAddWidgetCallback(m_LogbookWidget, WrapIvyLogbookHandler);

	// Add Close Box decorations to the Main Widget;
	XPSetWidgetProperty(m_LogbookWidget, xpProperty_MainWindowHasCloseBoxes, 1);
	//std::string logbookstring = "test1; test2\n test3";

	m_logbook_lines = int(m_ivyConfig->m_log_window_entries);
	m_logbook_index = 0;
	m_text_field_array.clear();
	m_logbook_entries.clear();



	std::ifstream logbook_file;
	std::string line;
	logbook_file.open(m_ivyConfig->m_logbook_path.c_str(), std::ifstream::in);
	while (getline(logbook_file, line))
		m_logbook_entries.push_back(line);
	logbook_file.close();




	m_LogbookScrollBar = XPCreateWidget(x2 - 10, y - 20, x2 - 5, y2, 1, "", 0, m_LogbookWidget, xpWidgetClass_ScrollBar);
	XPSetWidgetProperty(m_LogbookScrollBar, xpProperty_ScrollBarMin, 0);
	XPSetWidgetProperty(m_LogbookScrollBar, xpProperty_ScrollBarMax, max(m_logbook_entries.size() + 2, 0));
	XPSetWidgetProperty(m_LogbookScrollBar, xpProperty_ScrollBarPageAmount, m_logbook_lines);
	XPSetWidgetProperty(m_LogbookScrollBar, xpProperty_ScrollBarSliderPosition, min(m_logbook_lines, m_logbook_entries.size())); // Set page to show last flight max(len(m_logbook_entries), m_logbook_lines);

	XPAddWidgetCallback(m_LogbookScrollBar, WrapIvyLogbookScrollHandler);

	for (int index = 0; index < m_logbook_lines; index++)
	{
		m_text_field_array.push_back(XPCreateWidget(x + 5, y - (30 + (index * 20)), x2 - 15, y - (40 + (index * 20)), 1, std::to_string(index).c_str(), 0, m_LogbookWidget, xpWidgetClass_TextField));
		//subwindow =                  XPCreateWidget(x+5, y-(50 + (index*20)), x2-15, y-(60 + (index*20)),1, "",	0, m_LogbookWidget, xpWidgetClass_SubWindow);
		//XPSetWidgetProperty(subwindow, xpProperty_SubWindowType, xpSubWindowStyle_SubWindow);



		// Text Draw;
		IvyFillLogbook();
	}
}

int MyIvy::IvyLogbookHandler(XPWidgetMessage inMessage, XPWidgetID inWidget, intptr_t inParam1, intptr_t inParam2)
{
	if (m_initialized == false) return 0;

	if (inMessage == xpMessage_CloseButtonPushed) 
	{
		if (m_MenuLogbookShow == 1)
		{
			XPDestroyWidget(m_LogbookWidget, 1);
			m_MenuLogbookShow = 0;
		}
		return 1;
	}

	return 0;
}

int MyIvy::IvyLogbookScrollHandler(XPWidgetMessage inMessage, XPWidgetID inWidget, intptr_t inParam1, intptr_t inParam2)
{

	if (m_initialized == false) return 0;

	if (inMessage == xpMsg_ScrollBarSliderPositionChanged)
	{
		IvyFillLogbook();
		return 1;
	}
	return 0;
}

void MyIvy::IvyFillLogbook()
{
	if (m_initialized == false) return;

	m_logbook_index = max(m_logbook_entries.size() - XPGetWidgetProperty(m_LogbookScrollBar, xpProperty_ScrollBarSliderPosition, NULL), 0);
	
	for (int index = 0; index < m_text_field_array.size(); index++)
	{
		int text_index = index + m_logbook_index;
		if (text_index < m_logbook_entries.size())
			XPSetWidgetDescriptor(m_text_field_array[index], m_logbook_entries[text_index].c_str());
		else
			XPSetWidgetDescriptor(m_text_field_array[index], "");
	}
}

void MyIvy::IvyReceiveMessage(XPLMPluginID inFromWho, int inMessage, void * inParam)
{
	ivy_output_file << "Message Received" << std::endl;
	ivy_output_file.flush();

	if (m_initialized == false) return;

	if (inFromWho == XPLM_PLUGIN_XPLANE)
	{
		if (inMessage == XPLM_MSG_PLANE_LOADED)
		{
			ivy_output_file << "Plane Loaded" << std::endl;
			ivy_output_file.flush();

			m_aircraft_loaded = 1;
			m_no_aircraft = false;
			ResetIvy();
		}
		if (inMessage == XPLM_MSG_AIRPORT_LOADED)
		{
			ivy_output_file << "Airport Loaded" << std::endl;
			ivy_output_file.flush();

			m_aircraft_loaded = 1;
			m_no_aircraft = false;
			ResetIvy();
		}
		if (inMessage == XPLM_MSG_PLANE_CRASHED)
		{
			ivy_output_file << "Plane Crashed" << std::endl;
			ivy_output_file.flush();

			m_aircraft_crashed = 1;
		}

		if (inMessage == XPLM_MSG_SCENERY_LOADED)
		{
			

		}
		// On new FMOD Bank
		if (inMessage == XPLM_MSG_FMOD_BANK_LOADED)
		{
			ivy_output_file << "New Bank Loaded" << std::endl;
			ivy_output_file.flush();

			if (reinterpret_cast<uintptr_t>(inParam) == xplm_RadioBank)
			{
				ivy_output_file << "New Radio Bank - Resetting" << std::endl;
				ivy_output_file.flush();
				ResetIvy();
			}
			
		}
	}
	else
	{
		if (inMessage == HRM_MESSAGE_SOUND_1)
		{
			HRM_Sound *p_hrm_sound = (HRM_Sound *) inParam;

			if (m_ivyConfig->m_hrm_enabled == true)
			{

				if ((p_hrm_sound->sound_before > 0) && (p_hrm_sound->sound_before < m_s_hrm.size()))
					m_play_mp3_queue->push(m_s_hrm[p_hrm_sound->sound_before]);

				if (p_hrm_sound->say_value > 0) SayNumber(p_hrm_sound->say_value);

				if ((p_hrm_sound->sound_after > 0) && (p_hrm_sound->sound_after < m_s_hrm.size()))
					m_play_mp3_queue->push(m_s_hrm[p_hrm_sound->sound_after]);
			}


		}
	}
}

void MyIvy::IvyDrawOutputWindow(XPLMWindowID inWindowID, void * inRefcon)
{
	if (m_initialized == false) return;

	int		left, top, right, bottom;
	float	color[] = { 1.0, 1.0, 1.0 }; 	/* RGB White */

	/* First we get the location of the window passed in to us. */
	XPLMGetWindowGeometry(inWindowID, &left, &top, &right, &bottom);

	if ((m_data_read_valid == true) && (m_draw_window == 1))
	{
		char buf[2048];

		XPLMDrawTranslucentDarkBox(left, top, right, bottom);
		sprintf(buf, "Aircraft Name    %s", m_ls_acf_descrip.c_str());
		XPLMDrawString(color, left + 5, top - 10, buf, 0, xplmFont_Basic);
		sprintf(buf, "IvyAircraft Name %s", m_ivyAircraft->m_name);
		XPLMDrawString(color, left + 5, top - 20, buf, 0, xplmFont_Basic);
		sprintf(buf, "Slats position   %f", m_ivyAircraft->m_lf_slats);
		XPLMDrawString(color, left + 5, top - 30, buf, 0, xplmFont_Basic);
		sprintf(buf, "Flaps position   %f", m_ivyAircraft->m_lf_flaps);
		XPLMDrawString(color, left + 5, top - 40, buf, 0, xplmFont_Basic);
		sprintf(buf, "V1               %i", m_ivyAircraft->m_li_v1);
		XPLMDrawString(color, left + 5, top - 50, buf, 0, xplmFont_Basic);
		sprintf(buf, "VR               %i", m_ivyAircraft->m_li_vr);
		XPLMDrawString(color, left + 5, top - 60, buf, 0, xplmFont_Basic);
		sprintf(buf, "V2               %i", m_ivyAircraft->m_li_v2);
		XPLMDrawString(color, left + 5, top - 70, buf, 0, xplmFont_Basic);
		sprintf(buf, "Decision Height  %i", int(m_lf_decision_height));
		XPLMDrawString(color, left + 5, top - 80, buf, 0, xplmFont_Basic);

		sprintf(buf, "FSE Flying  %i", int(m_li_fse_flying));
		XPLMDrawString(color, left + 5, top - 90, buf, 0, xplmFont_Basic);

		// For Debug;
//		if ((m_li_on_ground == 0) && (m_lf_radio_alt > (m_lf_decision_height + m_ivyConfig->m_decition_height_arm))){
//			XPLMDrawString(color, left + 5, top - 100, "Statement true", 0, xplmFont_Basic);
//		else{
//			XPLMDrawString(color, left + 5, top - 100, "Statement false", 0, xplmFont_Basic);
	}


}

void MyIvy::IvyMenuHandler(void * in_menu_ref, void * inItemRef)
{
	ivy_output_file << "MenuHandler" << std::endl;
	ivy_output_file.flush();

	if (m_initialized == false) return;

	if (strcmp((char *) inItemRef, "Item1") == NULL)
	{
		if (!imguiPtr->GetVisible())
		{
			imguiPtr->Visible(true);

			//CreateLogbookWidget(int(m_ivyConfig->m_log_window_pos_x), int(m_ivyConfig->m_log_window_pos_y), int(m_ivyConfig->m_log_window_width), int(m_ivyConfig->m_log_window_height));
			//m_MenuLogbookShow = 1;
		}
		else
		{
			imguiPtr->Visible(false);

			//m_MenuLogbookShow = 0;
			//XPDestroyWidget(m_LogbookWidget, 1);
		}
	}
	/*else if (strcmp((char *)inItemRef, "Item2") == NULL)
	{
		if (m_MenuLogbookShow == 0)
		{
			CreateVSpeedsWidget();
			m_MenuVSpeedsShow = 1;
		}
		else
		{
			m_MenuVSpeedsShow = 0;
			XPDestroyWidget(m_VSpeedsWidget, 1);
		}
	}*/
	else if (strcmp((char *)inItemRef, "Item3") == NULL)
		AnnouncementCallback(0, 0, 0);
	else if (strcmp((char *)inItemRef, "Item4") == NULL)
		SayBaroCallback(0, 0, 0);
	else if (strcmp((char *)inItemRef, "Item5") == NULL)
		SayWindCallback(0, 0, 0);
	/*else if (strcmp((char *)inItemRef, "Item6") == NULL)
		ToogleWindowCallback(0, 0, 0);*/
	else if (strcmp((char *)inItemRef, "Item7") == NULL)
		ResetIvyCallback(0, 0, 0);
	else if (strcmp((char *)inItemRef, "Item8") == NULL)
	{
		m_ivyConfig->m_ivy_enable = !m_ivyConfig->m_ivy_enable;
		MenuCheck();
	}
	else if (strcmp((char *)inItemRef, "Item9") == NULL)
	{
		m_ivyConfig->m_log_enable = !m_ivyConfig->m_log_enable;
		MenuCheck();
	}
}

int MyIvy::SayBaroCallback(XPLMCommandRef cmd, XPLMCommandPhase phase, void * refcon)
{
	if (phase == 0)
		SayBaro();
	return 1;
}

int MyIvy::SayWindCallback(XPLMCommandRef cmd, XPLMCommandPhase phase, void * refcon)
{
	if (phase == 0)
		SayWind();
	return 1;
}

int MyIvy::AnnouncementCallback(XPLMCommandRef cmd, XPLMCommandPhase phase, void * refcon)
{
	if (phase == 0)
	{
		if (m_li_on_ground == 1)
			m_ivyAnnounceTakeOff->Activate(m_time);
		else
			m_ivyAnnounceLanding->Activate(m_time);
	}
	return 1;
}

int MyIvy::ResetIvyCallback(XPLMCommandRef cmd, XPLMCommandPhase phase, void * refcon)
{
	if (phase == 0)
		ResetIvy();
	return 1;
}

int MyIvy::ToogleWindowCallback(XPLMCommandRef cmd, XPLMCommandPhase phase, void * refcon)
{
	if (phase == 0)
	{
		if (!imguiPtr->GetVisible())
		{
			imguiPtr->Visible(true);
		}
		else
		{
			imguiPtr->Visible(false);
		}
	}
		//m_draw_window = 1 - m_draw_window;
	return 1;
}

void MyIvy::KeyCallback(XPLMWindowID inWindowID, char inKey, XPLMKeyFlags inFlags, char inVirtualKey, void * inRefcon, int losingFocus)
{
}

int MyIvy::MouseClickCallback(XPLMWindowID inWindowID, int x, int y, XPLMMouseStatus inMouse, void * inRefcon)
{
	return 0;
}

void MyIvy::SayBaro()
{
	if (m_ivyConfig->m_ivy_enable == false) return;

	ivy_output_file << "SayBaro" << std::endl;
	ivy_output_file.flush();

	m_play_mp3_queue->push(m_s_baro_press_1);
	SpellOutDigits(m_li_baro_sea_level);
}

void MyIvy::SayWind()
{
	if (m_ivyConfig->m_ivy_enable == false) return;

	ivy_output_file << "SayWind" << std::endl;
	ivy_output_file.flush();

	m_play_mp3_queue->push(m_s_wind1);
	SayNumber(int(m_lf_wind_direction));
	m_play_mp3_queue->push(m_s_wind2);
	SayNumber(int(m_lf_wind_speed_kt));
	m_play_mp3_queue->push(m_s_knots);
}

void MyIvy::MenuCheck()
{
	XPLMMenuCheck m_check = m_ivyConfig->m_ivy_enable == true ? xplm_Menu_Checked : xplm_Menu_NoCheck;
	XPLMCheckMenuItem(m_MenuId, m_menuindex_ivy_enable, m_check);

	m_check = m_ivyConfig->m_log_enable == true ? xplm_Menu_Checked : xplm_Menu_NoCheck;
	XPLMCheckMenuItem(m_MenuId, m_menuindex_log_enable, m_check);
}

void MyIvy::BoardingCompleted()
{
	if (m_ivyConfig->m_ivy_enable == false) return;
	if (m_ivyConfig->m_callouts_enable == false) return;

	m_play_mp3_queue->push(m_s_fse_start1);
	//m_play_mp3_queue->push(m_s_fse_start2);
	int payload = (int) (m_lf_payload_tot  * 2.20462);
	
	// Round Values if greater 100
	if (payload > 100)
		payload = ((int)(payload / 100)) * 100;

	SayNumber(payload);

	m_play_mp3_queue->push(m_s_fse_start2);

	payload = (int) (m_lf_fuel_tot * 2.20462);

	// Round Values if greater 100
	if (payload > 100)
		payload = ((int)(payload / 100)) * 100;

	SayNumber(payload);

	m_play_mp3_queue->push(m_s_fse_start3);

}

void MyIvy::SpellOutDigits(int spell_number)
{
	if (m_ivyConfig->m_ivy_enable == false) return;

	ivy_output_file << "SpellOutDigits" << spell_number << std::endl;
	ivy_output_file.flush();

	// 1000;
	int digit = int((spell_number % 10000) / 1000);
	m_play_mp3_queue->push(m_s_low[digit]);

	// 100;
	digit = int((spell_number % 1000) / 100);
	m_play_mp3_queue->push(m_s_low[digit]);

	// 10;
	digit = int((spell_number % 100) / 10);
	m_play_mp3_queue->push(m_s_low[digit]);

	// 1;
	digit = int(spell_number % 10);
	m_play_mp3_queue->push(m_s_low[digit]);
}

void MyIvy::SayNumber(int spell_number)
{
	if (m_ivyConfig->m_ivy_enable == false) return;

	ivy_output_file << "SpellOutNumber" << spell_number << std::endl;
	ivy_output_file.flush();


	// Zero is a special case
	if (spell_number == 0)
	{
		m_play_mp3_queue->push(m_s_low[0]);
	}
	else
	{
		
		// 1000;
		int digit = int((spell_number % 1000000) / 1000);
		if (digit > 0)
		{
			SayNumber(digit);
			m_play_mp3_queue->push(m_s_thousand);
		}
		// 100;
		digit = int((spell_number % 1000) / 100);
		if (digit > 0)
		{
			m_play_mp3_queue->push(m_s_hundreds[digit]);
		}

		// 2 lowest digits directly;
		digit = int((spell_number % 100));

		// Single digit < 100;
		if (digit > 0)
		{
			m_play_mp3_queue->push(m_s_low[digit]);
		}
	}
}

void MyIvy::CheckAnnouncement()
{
	if (m_non_smoking_old != m_li_nonsmoking)
	{
		if ((m_time - m_non_smoking_event) < m_ivyConfig->m_non_smoking_annoucetime)
		{

			if (m_li_on_ground == 1)
				m_ivyAnnounceTakeOff->Activate(m_time);
			else
				m_ivyAnnounceLanding->Activate(m_time);
		}
		m_non_smoking_event = m_time;
	}
	m_non_smoking_old = m_li_nonsmoking;
}

void MyIvy::DetectLanding()
{
	// Detect potential depature airport first{
		// Need to store temporarily, because helicopters on bouncing might trigger this;
	if ((m_li_on_ground == 1) && (m_lf_ground_speed < m_ivyConfig->m_taxi_ground_speed_min))
	{
		float f_lat = (float) m_ld_latitude;
		float f_long = (float) m_ld_longitude;

		XPLMNavRef m_airport_departure_ref = XPLMFindNavAid(NULL, NULL, &f_lat, &f_long, NULL, xplm_Nav_Airport);
		char airport_name[2048];
		XPLMGetNavAidInfo(m_airport_departure_ref, NULL, NULL, NULL, NULL, NULL, NULL, airport_name, NULL, NULL);
		m_airport_departure_temp = airport_name;
	}



	if (m_li_on_ground == 0)
	{
		m_landing_detected = 0;
		m_landing_rated = 0;

		// Store the potential departure airport within a 10s window after positive climb callout;
		// Pos climb has a 20s cooldown, which means that we only take the new departure after 20s on ground;
		float flight_time = m_time - m_ivyPosRateClimb->m_time_activated;
		if ((flight_time > 0) && (flight_time < 10)) 
			m_airport_departure = m_airport_departure_temp;
	}

	else if ((m_li_on_ground_old == 0) && (m_li_on_ground == 1))
	{
		m_landing_detected = 1;

		ivy_output_file << "Landing detected{: " << m_time << " Sinkrate: " << m_lf_climb_rate << " G-Force: " << m_lf_g_normal << std::endl;
		ivy_output_file.flush();


		///////////////////// Add to online log

		std::string text = "";

		std::string s_value = std::to_string((int)(m_time / 3600));
		if (s_value.size() < 2) s_value = "0" + s_value;
		text += s_value + ":";

		s_value = std::to_string((int)((((int) m_time) % 3600) / 60));
		if (s_value.size() < 2) s_value = "0" + s_value;
		text += s_value + ":";

		s_value = std::to_string(((int) m_time) % 60);
		if (s_value.size() < 2) s_value = "0" + s_value;
		text += s_value + " - Touch Down: " + std::to_string(m_lf_climb_rate) + " ft/min, " + std::to_string(m_lf_g_normal) + " g";

		m_error_list.push_back(text);

		//////////////////////////////////////


		IvyLandingDetection *landing_object = new IvyLandingDetection(m_time, m_lf_climb_rate, m_lf_g_normal, m_lf_g_side, m_lf_g_forward);
		m_ivy_landing_list->push_back(landing_object);

		m_landing_rated = 0;
		m_landing_sink_rate = 0;
		m_landing_g_normal = 0;
		m_landing_bounces = 0;

		// Check all touch downs. Rating is 0 if not in window;
		for (int obj_number = 0; obj_number < m_ivy_landing_list->size(); obj_number++)
		{
			int act_rating = ((*m_ivy_landing_list)[obj_number])->GetCurrentRate(m_time, 10);

			if (act_rating > 0)
			{
				m_landing_sink_rate = max(m_landing_sink_rate, abs(((*m_ivy_landing_list)[obj_number])->m_sink_rate));
				m_landing_g_normal = max(m_landing_g_normal, abs(((*m_ivy_landing_list)[obj_number])->m_g_normal));
				m_landing_bounces = m_landing_bounces + 1;
			}


			if (act_rating > m_landing_rated)  m_landing_rated = act_rating;
			ivy_output_file << "Landing T=" << ((*m_ivy_landing_list)[obj_number])->m_time << " Sink Rate " << abs(((*m_ivy_landing_list)[obj_number])->m_sink_rate) << " g: " << abs(((*m_ivy_landing_list)[obj_number])->m_g_normal) << " Grade: " << act_rating << " | ";
		}
		ivy_output_file << std::endl;
		ivy_output_file.flush();
	}
	m_li_on_ground_old = m_li_on_ground;
}

void MyIvy::EndOfFlightEvaluation()
{
	ivy_output_file << "EndOfFlightEvaluation" << std::endl;
	ivy_output_file.flush();

	int error_rate = 0;
	m_landing_detected = 0;

	// Get the proper values to speak (before && after decimal point for g forces);
	int sink_rate = (int)(m_landing_sink_rate);
	int g_force_int = (int)(m_landing_g_normal);
	int g_force_dec_2 = (int)(((m_landing_g_normal - g_force_int) * 100));

	std::string s_g_force_dec_2 = std::to_string(g_force_dec_2);

	if (s_g_force_dec_2.size() <= 1) s_g_force_dec_2 = "0" + s_g_force_dec_2;

	// Count all the errors that occurred;
	for (int obj_number = 0; obj_number < m_ivy_object_list->size(); obj_number++)
		if ((*m_ivy_object_list)[obj_number]->m_is_error != 0)
			error_rate = error_rate + (*m_ivy_object_list)[obj_number]->m_error_count;

	// Evaluate Flight;
	if (error_rate == 0)
		m_play_mp3_queue->push(m_s_error_zero);
	else if (error_rate < 5)
		m_play_mp3_queue->push(m_s_error_good);
	else if (error_rate < 10)
		m_play_mp3_queue->push(m_s_error_bad);
	else
		m_play_mp3_queue->push(m_s_error_xbad);


	if (error_rate > 0) SayNumber(error_rate);

	// Singular - Plural;
	if (error_rate == 1)
		m_play_mp3_queue->push(m_s_error_a);
	else if (error_rate > 1)
		m_play_mp3_queue->push(m_s_error_b);

	// Tell landing sinkrate && g forces;
	m_play_mp3_queue->push(m_s_landing_rate);
	SayNumber(sink_rate);
	m_play_mp3_queue->push(m_s_landing_feet);

	m_play_mp3_queue->push(m_s_landing_g);
	SayNumber(g_force_int);
	m_play_mp3_queue->push(m_s_dot);
	SayNumber(g_force_dec_2);

	// Tell bounces;
	if (m_landing_bounces <= 1)
	{
		m_play_mp3_queue->push(m_s_no_bounce);
	}
	else
	{
		m_play_mp3_queue->push(m_s_bounce1);
		SayNumber(m_landing_bounces - 1);
		if (m_landing_bounces == 2)
			m_play_mp3_queue->push(m_s_bounce2s); // singular;
		else
			m_play_mp3_queue->push(m_s_bounce2);
	}

	int flight_time = (int)(m_time - m_ivyPosRateClimb->m_time_activated);
	std::string flight_hours = std::to_string((int)(flight_time / 3600));
	std::string flight_minutes = std::to_string((int)((flight_time % 3600) / 60));
	std::string flight_seconds = std::to_string(flight_time % 60);

	if (flight_hours.size() <= 1) 		flight_hours = "0" + flight_hours;
	if (flight_minutes.size() <= 1) 	flight_minutes = "0" + flight_minutes;
	if (flight_seconds.size() <= 1) 	flight_seconds = "0" + flight_seconds;

	// Logbook;

	char lba_acf_descrip[2048];
	XPLMGetDatab(m_s_acf_descrip, lba_acf_descrip, 0, 240);
	std::string aircraft_name = lba_acf_descrip;

	float f_lat = (float)m_ld_latitude;
	float f_long = (float)m_ld_longitude;

	XPLMNavRef m_airport_departure_ref = XPLMFindNavAid(NULL, NULL, &f_lat, &f_long, NULL, xplm_Nav_Airport);
	char airport_name[2048];
	XPLMGetNavAidInfo(m_airport_departure_ref, NULL, NULL, NULL, NULL, NULL, NULL, airport_name, NULL, NULL);

	m_airport_arrival = airport_name;

	// Do not write in replay mode;
	if ((m_li_replay == 0) && (m_ivyConfig->m_log_enable == true))
	{
		std::ofstream  logbook_file;
		logbook_file.open(m_ivyConfig->m_logbook_path, std::ofstream::out | std::ofstream::app);
		logbook_file.precision(2);

		time_t curr_time = time(0);
		struct tm * now = localtime(&curr_time);

		logbook_file << std::setw(4) << 1900 + now->tm_year << "/" << std::setw(2) << now->tm_mon + 1 << "/" << now->tm_mday << " ";

		std::string aircraft_short = aircraft_name;
		aircraft_short.resize(m_ivyConfig->m_log_afc_name_length, ' ');

		logbook_file << "Aircraft: " << aircraft_short << ", ";

		std::string resized = m_airport_departure;
		if (resized.length() < 6) resized.insert(resized.begin(), 6 - resized.length(), ' ');

		logbook_file << "Dep: " << resized << ", ";

		resized = m_airport_arrival;
		if (resized.length() < 6) resized.insert(resized.begin(), 6 - resized.length(), ' ');

		logbook_file << "Arr: " << resized << ", ";

		logbook_file << "Flight Time: " << flight_hours << ":" << flight_minutes << ":" << flight_seconds << ", ";

		logbook_file << "Errors: " << std::setw(3) << error_rate << ", ";

		std::string grade;

		if (m_landing_rated == 1)	grade = "A";
		else if (m_landing_rated == 2)	grade = "B";
		else if (m_landing_rated == 3)	grade = "C";
		else if (m_landing_rated == 4)	grade = "D";
		else
		{
			// Set IvyResponse here
			grade = "F";
		}

		if (m_landing_bounces >= 1)   m_landing_bounces = m_landing_bounces - 1;

		logbook_file << "Landing: " << grade << ", " << std::setw(5) << sink_rate << " ft/min, " << std::setw(2) << g_force_int << "." << s_g_force_dec_2 << "g, " << std::setw(3) << m_landing_bounces << " bounce(s)";
		logbook_file << std::endl;

		logbook_file.close();
		
		// Reset error counters;
		for (int obj_number = 0; obj_number < m_ivy_object_list->size(); obj_number++)
			(*m_ivy_object_list)[obj_number]->Archive();

		m_ivy_flighttime_history.push_back(flight_time);

		WriteErrorLog();

	}
}

float MyIvy::IvyFlightLoopCallback(float elapsedMe, float elapsedSim, int counter, void * refcon)
{
	if ((m_ivyConfig->m_ivy_enable == false) && (m_ivy_enabled == true)) ResetIvy();

	m_ivy_enabled = m_ivyConfig->m_ivy_enable;

	if (m_ivy_enabled == false) return m_ivyConfig->m_data_rate;


	m_li_replay = XPLMGetDatai(m_i_replay);
	m_li_paused = XPLMGetDatai(m_i_paused);

	// Do nothing if replay or paused, do not even count the time
	if ((m_li_replay > 0) || (m_li_paused > 0)) return m_ivyConfig->m_data_rate;


	m_time = m_time + m_ivyConfig->m_data_rate;

	m_passengersScreaming = false;
	m_passengerVolume = 0.3;

	// We reset the aircraft loaded situation after 60 seconds;
	if (m_time > (30 + m_ivyConfig->m_disable_after_loading))
		m_aircraft_loaded = 0;

	if (m_i_fse_flying == NULL)
	{
		m_i_fse_flying = XPLMFindDataRef("fse/status/flying");
	}

	// Get all the fresh data from the datarefs;
	if (m_plugin_enabled == 1)
		ReadData();

	// If started to play queue file, we deactivate for X cycles;
	// Currently deactivated, as it seems we do not need this;
	//if (m_deact_queue > 0)
	//	m_deact_queue = m_deact_queue - 1;

	// Playlist. Here we can queue text that is longer than a single mp3;
	// If we still have to say something, error detection is disabled. We would not have time to say it anyways.;
	if (m_plugin_enabled == 0)
	{
	}

	// Sound Queue
	else if (m_play_mp3_queue->size() > 0)
	{
		
		if (m_IvySound->IsPlayingSound(m_ivyChannel) == false)
		{
			if (m_IvySound->PlaySingleSound(m_ivyChannel, m_play_mp3_queue->front()) == true)
			{
				m_play_mp3_queue->pop();
			}
			else
			{
				IvyLoadSoundFiles(true);
			}
		}
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////;
	// NOT after Load && NOT after Crash;

	else if ((m_time > m_ivyConfig->m_disable_after_loading) && (m_aircraft_crashed == 0) && (m_li_replay == 0))
	{
		

		if (m_lf_g_normal > m_f_g_normal_max)
			m_f_g_normal_max = m_lf_g_normal;
		if (m_lf_g_side > m_f_g_side_max)
			m_f_g_side_max = m_lf_g_side;
		if (m_lf_g_forward > m_f_g_forward_max)
			m_f_g_forward_max = m_lf_g_forward;

		if (m_lf_g_normal < m_f_g_normal_min)
			m_f_g_normal_min = m_lf_g_normal;
		if (m_lf_g_side < m_f_g_side_min)
			m_f_g_side_min = m_lf_g_side;
		if (m_lf_g_forward < m_f_g_forward_min)
			m_f_g_forward_min = m_lf_g_forward;



		m_f_g_normal_delta = (m_lf_g_normal - m_f_g_normal_old) / m_ivyConfig->m_data_rate;
		m_f_g_side_delta = (m_lf_g_side - m_f_g_side_old) / m_ivyConfig->m_data_rate;
		m_f_g_forward_delta = (m_lf_g_forward - m_f_g_forward_old) / m_ivyConfig->m_data_rate;

		m_f_g_normal_old = m_lf_g_normal;
		m_f_g_side_old = m_lf_g_side;
		m_f_g_forward_old = m_lf_g_forward;

		if (m_cab_press_old != 0)
			m_cab_press_rate = 60 * (m_lf_cab_press - m_cab_press_old) / (m_li_sim_ground_speed * m_ivyConfig->m_data_rate);
		m_cab_press_old = m_lf_cab_press;

		// Store N1 Values
		n1_values.push_front(m_lf_n1_percent[0]);
		if (n1_values.size() > m_ivyConfig->m_to_power_cycles)	
			n1_values.pop_back();


		// Calculate average N1 values
		float n1_max = 0;
		float n1_min = 1000;
		for (auto n1_val : n1_values)
		{
			n1_max = max(n1_val, n1_max);
			n1_min = min(n1_val, n1_min);
		}
		m_n1_deviation = abs(n1_max - n1_min);
		

		DetectLanding();


		// Ouch when bumping on ground;
		if ((m_li_on_ground == 1) && ((m_lf_g_normal) > m_ivyConfig->m_ivy_ouch_g) && (m_ivyConfig->m_ouch_enabled == true)) //play ouch;
		{
			if (m_IvySound->IsPlayingSound(m_ivyChannel) == false)
			{
				m_IvySound->PlaySingleSound(m_ivyChannel, m_s_ouch);
			}
		}
		// Check for announcemnt to make;
		CheckAnnouncement();

		if ((m_li_fse_flying > 0) && (m_li_fse_flying != m_li_fse_flying_old))
		{
			BoardingCompleted();
		}

		m_li_fse_flying_old = m_li_fse_flying;

		if (m_ivyHVCheck.CheckHV(m_lf_radio_alt, m_lf_ias) == false)																			m_ivyHeightVelocity->Activate(m_time);
		else																																	m_ivyHeightVelocity->Deactivate(m_time);

		// Announcement deactivation only. Either activated by command || by CheckAnnouncement;
		if (m_li_on_ground == 1)																												m_ivyAnnounceLanding->Deactivate(m_time);
		if (m_li_on_ground == 0)																												m_ivyAnnounceTakeOff->Deactivate(m_time);

		if ((m_li_on_ground == 0) && (m_lf_climb_rate > m_ivyConfig->m_pos_rate_climb))															m_ivyPosRateClimb->Activate(m_time);
		else if (m_li_on_ground == 1)																											m_ivyPosRateClimb->Deactivate(m_time);

		// Decision Height Arm;
		if ((m_li_on_ground == 0) && (m_lf_radio_alt > (m_lf_decision_height + m_ivyConfig->m_decition_height_arm)))							m_ivyArmMinimums->Activate(m_time);
		else if (m_li_on_ground == 1)																											m_ivyArmMinimums->Deactivate(m_time);

		if ((m_li_on_ground == 0) &&
			(m_ivyArmMinimums->m_played == 1) &&
			(m_lf_decision_height > 0) &&
			(m_lf_radio_alt < (m_lf_decision_height + m_ivyConfig->m_decition_height_plus)))													m_ivyMinimums->Activate(m_time);
		else if (m_lf_radio_alt > (m_lf_decision_height + m_ivyConfig->m_decition_height_arm))													m_ivyMinimums->Deactivate(m_time);


		// Fasten Seatbelts;
		if (m_li_fastenseatbelts > 0)																											m_ivySeatBelts->Activate(m_time);
		else																																	m_ivySeatBelts->Deactivate(m_time);


		if ((m_lf_gear1_ratio == 1) &&
			(is_int(m_lf_gear2_ratio)) &&
			(is_int(m_lf_gear3_ratio)) &&
			(is_int(m_lf_gear4_ratio)) &&
			(is_int(m_lf_gear5_ratio)))																											m_ivyGearDown->Activate(m_time);
		else																																	m_ivyGearDown->Deactivate(m_time);


		if ((m_lf_gear1_ratio == 0) &&
			(is_int(m_lf_gear2_ratio)) &&
			(is_int(m_lf_gear3_ratio)) &&
			(is_int(m_lf_gear4_ratio)) &&
			(is_int(m_lf_gear5_ratio)))																											m_ivyGearUp->Activate(m_time);
		else 
																																				m_ivyGearUp->Deactivate(m_time);

		if (m_li_gear_handle == 0)																												m_ivyGearSelectedUp->Activate(m_time);
		else																																	m_ivyGearSelectedUp->Deactivate(m_time);

		if (m_li_gear_handle == 1)																												m_ivyGearSelectedDown->Activate(m_time);
		else																																	m_ivyGearSelectedDown->Deactivate(m_time);

		// Tire blown;
		if ((m_li_tire1 + m_li_tire2 + m_li_tire3 + m_li_tire4 + m_li_tire5) > 0)																m_ivyTyre->Activate(m_time);
		else																																	m_ivyTyre->Deactivate(m_time);

		// Hard braking;
		if ((m_li_on_ground == 1) && (m_lf_g_forward > m_ivyConfig->m_brake_max_forward_g))		 												m_ivyBrake->Activate(m_time);
		else																																	m_ivyBrake->Deactivate(m_time);

		// Transponder not active when airborne;
		if ((m_li_on_ground == 0) && (m_li_transponder_mode < 2) && (m_ivyAircraft->m_military == false))										m_ivyTransponder->Activate(m_time); // Dataref{ Mode2=ON, B407 4=ON;
		else if (m_li_on_ground == 1)																											m_ivyTransponder->Deactivate(m_time);

		// Landing lights not on when landing in the night;
		if ((m_li_on_ground == 0) && (m_lf_radio_alt < m_ivyConfig->m_alt_landing_lights_low) &&
			(m_lf_world_light_precent > m_ivyConfig->m_night_world_light_precent) && (m_li_landing_lights == 0) && (m_ivyAircraft->m_military == false))								
																																				m_ivyLandingLights->Activate(m_time);
		else																																	m_ivyLandingLights->Deactivate(m_time);

		// Landing lights not off on high altitude;
		if ((m_li_on_ground == 0) && (m_lf_radio_alt > 7000) && (m_lf_baro_alt > m_ivyConfig->m_alt_landing_lights_high) &&
			(m_li_landing_lights == 1))																											m_ivyLandingLightsHigh->Activate(m_time);
		else																																	m_ivyLandingLightsHigh->Deactivate(m_time);

		// Beacon lights not when taxiing;
		if ((m_li_on_ground == 1) && (m_lf_ground_speed > m_ivyConfig->m_taxi_ground_speed_min) && (m_li_beacon_lights == 0) && (m_ivyAircraft->m_military == false))					
																																				m_ivyBeaconLights->Activate(m_time);
		else if (m_li_beacon_lights != 0)																										m_ivyBeaconLights->Deactivate(m_time);

		// Nav lights lights not when airborne;
		if ((m_li_on_ground == 0) && (m_li_nav_lights == 0) && (m_ivyAircraft->m_military == false))											m_ivyNavLights->Activate(m_time);
		else																																	m_ivyNavLights->Deactivate(m_time);

		// Strobes not on when airborne;
		if ((m_li_on_ground == 0) && (m_li_strobe_lights == 0) && (m_ivyAircraft->m_military == false))											m_ivyStrobes->Activate(m_time);
		else																																	m_ivyStrobes->Deactivate(m_time);

		// Comment on X-Plane tire blown message when aircraft has skid;
		if (((m_li_tire1 + m_li_tire2 + m_li_tire3 + m_li_tire4 + m_li_tire5) > 0) && (m_li_has_skid == 1))										m_ivySkidTyres->Activate(m_time);
		else																																	m_ivySkidTyres->Deactivate(m_time);

		// Battery low;
		if ((m_li_batt1 + m_li_batt2) > 0)																										m_ivyBatteryOut->Activate(m_time);
		else																																	m_ivyBatteryOut->Deactivate(m_time);

		// Engine fire;
		if ((m_li_fire1 + m_li_fire2 + m_li_fire3 + m_li_fire4 + m_li_fire5 + m_li_fire6 + m_li_fire7 + m_li_fire8) > 0)						m_ivyEngineFire->Activate(m_time);
		else																																	m_ivyEngineFire->Deactivate(m_time);

		// Engine flameout;
		if ((m_li_flameout1 + m_li_flameout2 + m_li_flameout3 + m_li_flameout4 +
		m_li_flameout5 + m_li_flameout6 + m_li_flameout7 + m_li_flameout8) > 0)																	m_ivyEngineFlameout->Activate(m_time);
		else																																	m_ivyEngineFlameout->Deactivate(m_time);

		// Engine ground failure;
		if ((m_li_on_ground == 1) &&
			((m_li_engine_failure1 + m_li_engine_failure2 + m_li_engine_failure3 + m_li_engine_failure4 +
			m_li_engine_failure5 + m_li_engine_failure6 + m_li_engine_failure7 + m_li_engine_failure8) > 0))									m_ivyEngineFailureGround->Activate(m_time);
		else																																	m_ivyEngineFailureGround->Deactivate(m_time);

		// Engine airborne failure;
		if ((m_li_on_ground == 0) &&
			((m_li_engine_failure1 + m_li_engine_failure2 + m_li_engine_failure3 + m_li_engine_failure4 +
			m_li_engine_failure5 + m_li_engine_failure6 + m_li_engine_failure7 + m_li_engine_failure8) > 0))									m_ivyEngineFailureAir->Activate(m_time);
		else																																	m_ivyEngineFailureAir->Deactivate(m_time);

		// Engine hot start;
		if ((m_li_hot1 + m_li_hot2 + m_li_hot3 + m_li_hot4 + m_li_hot5 + m_li_hot6 + m_li_hot7 + m_li_hot8) > 0)								m_ivyEngineHotStart->Activate(m_time);
		else																																	m_ivyEngineHotStart->Deactivate(m_time);

		// Battery not on;
		if ((m_li_battery_on == 0) && (m_li_gpu_on == 0))																						m_ivyNoBatt->Activate(m_time);
		else																																	m_ivyNoBatt->Deactivate(m_time);

		// Flaps Overspeed;
		if (m_li_flaps_overspeed > 0)																											m_ivyOverspeedFlaps->Activate(m_time);
		else																																	m_ivyOverspeedFlaps->Deactivate(m_time);

		// Gear Overspeed;
		if (m_li_gear_overspeed > 0)																											m_ivyOverspeedGear->Activate(m_time);
		else																																	m_ivyOverspeedGear->Deactivate(m_time);

		// Stall;
		if ((m_li_on_ground == 0) && (m_li_stall > 0))																							m_ivyStall->Activate(m_time);
		else																																	m_ivyStall->Deactivate(m_time);

		// Aircraft Overspeed;
		if ((m_lf_ias > m_lf_aircraft_vne) && (m_lf_aircraft_vne > 1))																			m_ivyOverspeedAircraft->Activate(m_time);
		else																																	m_ivyOverspeedAircraft->Deactivate(m_time);

		// Hello - Depending on weather;
		if ((m_aircraft_loaded == 1) &&
			(m_li_cloud_0 < 1) && (m_li_cloud_1 < 1) && (m_li_cloud_2 < 1) &&
			(m_lf_visibility > m_ivyConfig->m_vis_is_fog) && (m_li_rain == 0) && (m_li_thunder == 0))											m_ivyHelloSun->Activate(m_time);
		else																																	m_ivyHelloSun->Deactivate(m_time);

		if ((m_aircraft_loaded == 1) &&
			(m_lf_visibility > m_ivyConfig->m_vis_is_fog) && (m_li_rain > 0) && (m_li_thunder == 0))											m_ivyHellorain->Activate(m_time);
		else																																	m_ivyHellorain->Deactivate(m_time);

		//ToDo{
		//if ((m_aircraft_loaded == 1) &&
		//	(m_lf_visibility > m_ivyConfig->m_vis_is_fog) && (m_li_thunder > 0))																m_ivyHelloThunder->Activate(m_time);
		//else																																	m_ivyHelloThunder->Deactivate(m_time);

		if ((m_aircraft_loaded == 1) &&
			(m_lf_visibility <= m_ivyConfig->m_vis_is_fog))																						m_ivyHelloFog->Activate(m_time);
		else																																	m_ivyHelloFog->Deactivate(m_time);

		if ((m_aircraft_loaded == 1) &&
			((m_li_cloud_0 >= 1) || (m_li_cloud_1 >= 1) || (m_li_cloud_2 >= 1)) &&
			(m_lf_visibility > m_ivyConfig->m_vis_is_fog) && (m_li_rain == 0) && (m_li_thunder == 0))											m_ivyHelloNormal->Activate(m_time);
		else																																	m_ivyHelloNormal->Deactivate(m_time);

		// Cabin pressure falling too fast;
		// Some aircraft do not get it right, when you increase the ground speed. Hence, I use both, my own && the Aircraft computation;
		if ((m_li_on_ground == 0) && (max(m_lf_cab_rate, m_lf_climb_rate) < m_ivyConfig->m_cab_rate_low) && (m_ivyAircraft->m_steep_descent==false))										
																																				m_ivyCabinDownNormal->Activate(m_time);
		else if (max(m_lf_cab_rate, m_lf_climb_rate) > (m_ivyConfig->m_cab_rate_low + m_ivyConfig->m_cab_rate_reset_hysteresis))				m_ivyCabinDownNormal->Deactivate(m_time);

		// Cabin pressure falling rapidely;
		if ((m_li_on_ground == 0) && (max(m_lf_cab_rate, m_lf_climb_rate) < m_ivyConfig->m_cab_rate_high) && (m_ivyAircraft->m_steep_descent == false))
		{
			m_ivyCabinDownFast->Activate(m_time);
			m_ivyCabinDownNormal->SetAsPlayed(m_time);
		}
		else if (max(m_lf_cab_rate, m_lf_climb_rate) > (m_ivyConfig->m_cab_rate_high + m_ivyConfig->m_cab_rate_reset_hysteresis))				m_ivyCabinDownFast->Deactivate(m_time);

		// Bank angle pre-warning;
		if ((m_li_on_ground == 0) && (abs(m_lf_roll) > m_ivyConfig->m_bank_low) && (m_ivyAircraft->m_steep_bank == false))						m_ivyBankNormal->Activate(m_time);
		else if (abs(m_lf_roll) < m_ivyConfig->m_bank_reset_low)																				m_ivyBankNormal->Deactivate(m_time);

		// Bank angle too high;
		if ((m_li_on_ground == 0) && (abs(m_lf_roll) > m_ivyConfig->m_bank_high) && (m_ivyAircraft->m_steep_bank == false))
		{
			m_ivyBankHigh->Activate(m_time);
			m_ivyBankNormal->SetAsPlayed(m_time);
		}
		else if (abs(m_lf_roll) < m_ivyConfig->m_bank_low)																						m_ivyBankHigh->Deactivate(m_time);

		// Bank angle extremely high;
		if ((m_li_on_ground == 0) && (abs(m_lf_roll) > m_ivyConfig->m_bank_xhigh) && (m_ivyAircraft->m_steep_bank == false))
		{
			m_passengersScreaming = true;
			m_ivyBankXHigh->Activate(m_time);
			m_ivyBankHigh->SetAsPlayed(m_time);
			m_ivyBankNormal->SetAsPlayed(m_time);
		}
		else if (abs(m_lf_roll) < m_ivyConfig->m_bank_high)																						m_ivyBankXHigh->Deactivate(m_time);


		// Pitch down pre-warning;
		if ((m_li_on_ground == 0) && (m_lf_pitch < m_ivyConfig->m_pitch_low) && (m_ivyAircraft->m_steep_bank == false))							m_ivyPitchDownNormal->Activate(m_time);
		else if (m_lf_pitch > m_ivyConfig->m_pitch_reset_low)																					m_ivyPitchDownNormal->Deactivate(m_time);

		// Pitch too low;
		if ((m_li_on_ground == 0) && (m_lf_pitch <= m_ivyConfig->m_pitch_high) && (m_ivyAircraft->m_steep_bank == false))
		{
			m_passengersScreaming = true;
			m_ivyPitchDownHigh->Activate(m_time);
			m_ivyPitchDownNormal->SetAsPlayed(m_time);
		}
		else if (m_lf_pitch > m_ivyConfig->m_pitch_low)
			m_ivyPitchDownHigh->Deactivate(m_time);


		// Normal G Force high;
		if ((m_li_on_ground == 0) && (m_lf_g_normal >= m_ivyConfig->m_max_g_down_low))
		{
			m_passengersScreaming = true;
			m_passengerVolume = max(m_passengerVolume, abs(m_lf_g_normal) / 6);
			m_ivyGNormalFlightNormal->Activate(m_time);
		}
		else if (m_lf_g_normal <= m_ivyConfig->m_max_g_down_low_reset)																			m_ivyGNormalFlightNormal->Deactivate(m_time);

		// Normal G Force very high;
		if ((m_li_on_ground == 0) && (m_lf_g_normal >= m_ivyConfig->m_max_g_down_high))
		{
			m_ivyGNormalFlightHigh->Activate(m_time);
			m_ivyGNormalFlightNormal->SetAsPlayed(m_time);
		}
		else if (m_lf_g_normal <= m_ivyConfig->m_max_g_down_low_reset)																			m_ivyGNormalFlightHigh->Deactivate(m_time);

		// Normal G Force very, very high;
		if ((m_li_on_ground == 0) && (m_lf_g_normal >= m_ivyConfig->m_max_g_down_xhigh))
		{
			m_ivyGNormalFlightXHigh->Activate(m_time);
			m_ivyGNormalFlightHigh->SetAsPlayed(m_time);
			m_ivyGNormalFlightNormal->SetAsPlayed(m_time);
		}
		else if (m_lf_g_normal <= m_ivyConfig->m_max_g_down_low_reset)																			m_ivyGNormalFlightXHigh->Deactivate(m_time);


		// Normal G Force too low;
		if ((m_li_on_ground == 0) && (m_lf_g_normal <= 0.5))
		{
			m_passengersScreaming = true;
			m_passengerVolume = max(m_passengerVolume, abs(m_lf_g_normal - 0.5) / 2);
			m_ivyGNormalNegativeLow->Activate(m_time);
		}
		else if (m_lf_g_normal > 0.8)																											m_ivyGNormalNegativeLow->Deactivate(m_time);

		// Normal G Force negative;
		if ((m_li_on_ground == 0) && (m_lf_g_normal <= 0))
		{
			m_ivyGNormalNegativeHigh->Activate(m_time);
			m_ivyGNormalNegativeLow->SetAsPlayed(m_time);
		}
		else if (m_lf_g_normal > 0.5)																											m_ivyGNormalNegativeHigh->Deactivate(m_time);

		// TBD;
		//m_li_turbulence = XPLMGetDatai(m_i_turbulence);
		//if ((m_li_on_ground == 0) && (m_li_turbulence > 10)){																					m_ivyTurbulenceNormal->Activate(m_time);
		//else if (m_li_turbulence < 2){																										m_ivyTurbulenceNormal->Deactivate(m_time);

		//if ((m_li_on_ground == 0) && (m_li_turbulence > 30)){																					m_ivyTurbolenceHigh->Activate(m_time);
		//else if (m_li_turbulence < 5){																										m_ivyTurbolenceHigh->Deactivate(m_time);

		// Barometric pressure not set accordingly while close to ground || taxiing (within tolerance);
		if ((m_lf_radio_alt < m_ivyConfig->m_baro_alt_low) &&
			(abs(m_li_baro_set - m_li_baro_sea_level) > m_ivyConfig->m_baro_tolerance) &&
			(m_lf_ground_speed > m_ivyConfig->m_taxi_ground_speed_min))
		{
			m_ivyBaroGround->Activate(m_time);
			if ((m_ivyBaroGround->m_played == 1) && (m_pressure_said == 0))
			{
				m_pressure_said = 1;
				if (m_ivyConfig->m_errors_enable) SayBaro();
			}
		}
		else if ((abs(m_li_baro_set - m_li_baro_sea_level) <= m_ivyConfig->m_baro_tolerance) || (m_lf_baro_alt > m_ivyConfig->m_trans_alt))
		{
			m_ivyBaroGround->Deactivate(m_time);
			m_pressure_said = 0;
		}

		// Barometric pressure not set to standard above transition altitude;
		if ((m_lf_baro_alt > (m_ivyConfig->m_trans_alt + m_ivyConfig->m_trans_hysteresis)) &&
			(abs(2992 - m_li_baro_set) > m_ivyConfig->m_baro_tolerance))																		m_ivyBaroHigh->Activate(m_time);
		else																																	m_ivyBaroHigh->Deactivate(m_time);

		// 60 knots landing callout:
		if ((m_li_on_ground == 1) && (m_ivyConfig->m_kt60__landing_enabled == true) && (m_lf_ias > 40) && (m_lf_ias <= 60) && (m_ivyArmLanding->m_played == 1))
																																				m_ivy60kt->Activate(m_time);
		else																																	m_ivy60kt->Deactivate(m_time);

		// 60 knots callout;
		if ((m_li_on_ground == 1) && (m_ivyConfig->m_kt60_enabled == true) && (m_lf_ias > 58) && (m_lf_ias < 70) && (m_ivyArmLanding->m_played == 0))								
																																				m_ivy60kt->Activate(m_time);
		else																																	m_ivy60kt->Deactivate(m_time);

		// 80 knots callout;
		if ((m_li_on_ground == 1) && (m_ivyConfig->m_kt80_enabled == true) && (m_lf_ias > 78) && (m_lf_ias < 90) && (m_ivyArmLanding->m_played == 0))								
																																				m_ivy80kt->Activate(m_time);
		else																																	m_ivy80kt->Deactivate(m_time);

		// 100 knots callout;
		if ((m_li_on_ground == 1) && (m_ivyConfig->m_kt100_enabled == true) && (m_lf_ias > 98) && (m_lf_ias < 110) && (m_ivyArmLanding->m_played == 0))								
																																				m_ivy100kt->Activate(m_time);
		else																																	m_ivy100kt->Deactivate(m_time);

		// Not rotated;
		if ((m_li_on_ground == 1) && (m_lf_ias > 180) && (m_landing_detected == 0))																m_ivyRotate->Activate(m_time);
		else																																	m_ivyRotate->Deactivate(m_time);

		// Spoilers armed
		if (m_lf_speed_brake < 0)																												m_ivySpoilersArmed->Activate(m_time);
		else																																	m_ivySpoilersArmed->Deactivate(m_time);

		// Speed Brake Extended
		if (m_lf_speed_brake > 0)																												m_ivySpeedBrakeExtended->Activate(m_time);
		else																																	m_ivySpeedBrakeExtended->Deactivate(m_time);

		// Speed Brake Retracted
		if ((m_lf_speed_brake == 0)	&& (m_lf_speed_brake_old > 0))																				m_ivySpeedBrakeRetracted->Activate(m_time);
		else																																	m_ivySpeedBrakeRetracted->Deactivate(m_time);

		// Spoilers Disarmed
		if ((m_lf_speed_brake == 0) && (m_lf_speed_brake_old <= 0))																				m_ivySpoilersDisarmed->Activate(m_time);
		else																																	m_ivySpoilersDisarmed->Deactivate(m_time);

		// Keep last speed brake lever position that was not retracted
		if (m_lf_speed_brake != 0)
			m_lf_speed_brake_old = m_lf_speed_brake;


		// Start Engine 1
		if (m_lia_engine_start[0] > 0)																											
		{
																																				m_ivyStartEngine1->Activate(m_time);
																																				if (m_lia_engine_start_old[0] == 0)
																																				{
																																					m_lia_engine_start_old[0] = 1;
																																					m_ivyLightUp1->Deactivate(m_time);
																																					m_ivyFuelOn1->Deactivate(m_time);
																																					m_ivyN21->Deactivate(m_time);
																																					m_ivyEngineStab1->Deactivate(m_time);
																																				}
		}
		else																																	m_ivyStartEngine1->Deactivate(m_time);

		// Start Engine 2
		if (m_lia_engine_start[1] > 0)
		{
																																				m_ivyStartEngine2->Activate(m_time);
																																				if (m_lia_engine_start_old[1] == 0)
																																				{
																																					m_lia_engine_start_old[1] = 1;
																																					m_ivyLightUp2->Deactivate(m_time);
																																					m_ivyFuelOn2->Deactivate(m_time);
																																					m_ivyN22->Deactivate(m_time);
																																					m_ivyEngineStab2->Deactivate(m_time);
																																				}
		}
		else																																	m_ivyStartEngine2->Deactivate(m_time);

		// Start Engine 3
		if (m_lia_engine_start[2] > 0) {
																																				m_ivyStartEngine3->Activate(m_time);
																																				if (m_lia_engine_start_old[2] == 0)
																																				{
																																					m_lia_engine_start_old[2] = 1;
																																					m_ivyLightUp3->Deactivate(m_time);
																																					m_ivyFuelOn3->Deactivate(m_time);
																																					m_ivyN23->Deactivate(m_time);
																																					m_ivyEngineStab3->Deactivate(m_time);
																																				}
		}
		else																																	m_ivyStartEngine3->Deactivate(m_time);

		// Start Engine 4
		if (m_lia_engine_start[3] > 0) {
																																				m_ivyStartEngine4->Activate(m_time);
																																				if (m_lia_engine_start_old[3] == 0)
																																				{
																																					m_lia_engine_start_old[3] = 1;
																																					m_ivyLightUp4->Deactivate(m_time);
																																					m_ivyFuelOn4->Deactivate(m_time);
																																					m_ivyN24->Deactivate(m_time);
																																					m_ivyEngineStab4->Deactivate(m_time);
																																				}
		}
		else																																	m_ivyStartEngine4->Deactivate(m_time);

		// Start APU
		if (m_li_apu_starter_switch == 2)																										m_ivyStartAPU->Activate(m_time);
		else																																	m_ivyStartAPU->Deactivate(m_time);

		// N2
		if ((m_ivyAircraft->m_eng_n2_startup > 0) && (m_lf_n2_percent[0] >= m_ivyAircraft->m_eng_n2_startup))									m_ivyN21->Activate(m_time);
		if ((m_ivyAircraft->m_eng_n2_startup > 0) && (m_lf_n2_percent[1] >= m_ivyAircraft->m_eng_n2_startup))									m_ivyN22->Activate(m_time);
		if ((m_ivyAircraft->m_eng_n2_startup > 0) && (m_lf_n2_percent[2] >= m_ivyAircraft->m_eng_n2_startup))									m_ivyN23->Activate(m_time);
		if ((m_ivyAircraft->m_eng_n2_startup > 0) && (m_lf_n2_percent[3] >= m_ivyAircraft->m_eng_n2_startup))									m_ivyN24->Activate(m_time);

		// Fuel On
		if (m_lf_fuel_flow[0] > 0) 																												m_ivyFuelOn1->Activate(m_time);
		if (m_lf_fuel_flow[1] > 0) 																												m_ivyFuelOn2->Activate(m_time);
		if (m_lf_fuel_flow[2] > 0) 																												m_ivyFuelOn3->Activate(m_time);
		if (m_lf_fuel_flow[3] > 0) 																												m_ivyFuelOn4->Activate(m_time);

		//LightUp
		if ((m_ivyAircraft->m_eng_egt_lightup > 0) && (m_lf_egt_c[0] >= m_ivyAircraft->m_eng_egt_lightup))									    m_ivyLightUp1->Activate(m_time);
		if ((m_ivyAircraft->m_eng_egt_lightup > 0) && (m_lf_egt_c[1] >= m_ivyAircraft->m_eng_egt_lightup))									    m_ivyLightUp2->Activate(m_time);
		if ((m_ivyAircraft->m_eng_egt_lightup > 0) && (m_lf_egt_c[2] >= m_ivyAircraft->m_eng_egt_lightup))									    m_ivyLightUp3->Activate(m_time);
		if ((m_ivyAircraft->m_eng_egt_lightup > 0) && (m_lf_egt_c[3] >= m_ivyAircraft->m_eng_egt_lightup))									    m_ivyLightUp4->Activate(m_time);

		

		// Engine Stabilized
		if ((m_ivyAircraft->m_eng_stab_n1_min > 0) && (m_lf_n1_percent[0] >= m_ivyAircraft->m_eng_stab_n1_min) && (abs(m_lf_n1_percent[0] - m_lf_n1_percent_old[0]) <= (m_ivyAircraft->m_eng_stab_n1_delta_s * m_ivyConfig->m_data_rate)))									    
																																				m_ivyEngineStab1->Activate(m_time);
		if ((m_ivyAircraft->m_eng_stab_n1_min > 0) && (m_lf_n1_percent[1] >= m_ivyAircraft->m_eng_stab_n1_min) && (abs(m_lf_n1_percent[1] - m_lf_n1_percent_old[1]) <= (m_ivyAircraft->m_eng_stab_n1_delta_s * m_ivyConfig->m_data_rate)))
																																				m_ivyEngineStab2->Activate(m_time);
		if ((m_ivyAircraft->m_eng_stab_n1_min > 0) && (m_lf_n1_percent[2] >= m_ivyAircraft->m_eng_stab_n1_min) && (abs(m_lf_n1_percent[2] - m_lf_n1_percent_old[2]) <= (m_ivyAircraft->m_eng_stab_n1_delta_s * m_ivyConfig->m_data_rate)))
																																				m_ivyEngineStab3->Activate(m_time);
		if ((m_ivyAircraft->m_eng_stab_n1_min > 0) && (m_lf_n1_percent[3] >= m_ivyAircraft->m_eng_stab_n1_min) && (abs(m_lf_n1_percent[3] - m_lf_n1_percent_old[3]) <= (m_ivyAircraft->m_eng_stab_n1_delta_s * m_ivyConfig->m_data_rate)))
																																				m_ivyEngineStab4->Activate(m_time);

		for (int index = 0; index < 8; index++)
		{
			m_lf_n1_percent_old[index] = m_lf_n1_percent[index];
			if (m_lia_engine_start_old[index] == 1) m_lia_engine_start_old[index] = m_lia_engine_start[index];
		}

		// Localizer Alive
		if ((m_lf_hsi_hdef_dot < 2.4f) && (m_lf_hsi_hdef_dot > -2.4f) && (m_lf_hsi_hdef_dot != 0) && (m_li_hsi_display_h > 0) && (m_ivyAircraft->m_localizer_enable == true) && (m_li_on_ground == 0))
																																				m_ivyLocalizerAlive->Activate(m_time);
		else																																	m_ivyLocalizerAlive->Deactivate(m_time);

		// Glide Slope Alive
		if ((m_lf_hsi_vdef_dot < 2.4f) && (m_lf_hsi_vdef_dot > -2.4f) && (m_lf_hsi_vdef_dot != 0) && (m_li_hsi_display_h > 0) && (m_li_hsi_display_v > 0) && (m_ivyAircraft->m_localizer_enable == true) && (m_li_on_ground == 0))
																																											m_ivyGlideSlopeAlive->Activate(m_time);
		else																																								m_ivyGlideSlopeAlive->Deactivate(m_time);

		// Ice airframe low;
		if (m_lf_ice_frame > m_ivyConfig->m_ice_low)																							m_ivyIceAirframeLow->Activate(m_time);
		else																																	m_ivyIceAirframeLow->Deactivate(m_time);

		// Ice airframe high;
		if (m_lf_ice_frame > m_ivyConfig->m_ice_high)
		{
			m_ivyIceAirframeHigh->Activate(m_time);
			m_ivyIceAirframeLow->SetAsPlayed(m_time);
		}
		else																																	m_ivyIceAirframeHigh->Deactivate(m_time);

		// Ice pitot low;
		if (m_lf_ice_pitot > m_ivyConfig->m_ice_low)																							m_ivyIcePitotLow->Activate(m_time);
		else																																	m_ivyIcePitotLow->Deactivate(m_time);

		// Ice pitot high;
		if (m_lf_ice_pitot > m_ivyConfig->m_ice_high)
		{
			m_ivyIcePitotHigh->Activate(m_time);
			m_ivyIcePitotLow->SetAsPlayed(m_time);
		}
		else																																	m_ivyIcePitotHigh->Deactivate(m_time);

		// Ice propeller low;
		if (m_lf_ice_propeller > m_ivyConfig->m_ice_low)																						m_ivyIcePropellerLow->Activate(m_time);
		else																																	m_ivyIcePropellerLow->Deactivate(m_time);

		// Ice propeller high;
		if (m_lf_ice_propeller > m_ivyConfig->m_ice_high)
		{
			m_ivyIcePropellerHigh->Activate(m_time);
			m_ivyIcePropellerLow->SetAsPlayed(m_time);
		}
		else																																	m_ivyIcePropellerHigh->Deactivate(m_time);

		// Ice cockpit window low;
		if (m_lf_ice_window > m_ivyConfig->m_ice_low)																							m_ivyIceWindowLow->Activate(m_time);
		else																																	m_ivyIceWindowLow->Deactivate(m_time);

		// Ice cockpit window high;
		if (m_lf_ice_window > m_ivyConfig->m_ice_high)
		{
			m_ivyIceWindowHigh->Activate(m_time);
			m_ivyIceWindowLow->SetAsPlayed(m_time);
		}
		else																																	m_ivyIceWindowHigh->Deactivate(m_time);

		// Cabin pressure low;
		if (m_lf_cab_press > m_ivyConfig->m_cab_press_low)																						m_ivyPressureLow->Activate(m_time);
		else																																	m_ivyPressureLow->Deactivate(m_time);

		// Cabin pressure too low to breath;
		if (m_lf_cab_press > m_ivyConfig->m_cab_press_high)
		{
			m_ivyPressureXLow->Activate(m_time);
			m_ivyPressureLow->SetAsPlayed(m_time);
		}
		else																																	m_ivyPressureXLow->Deactivate(m_time);

		// Birdstrike;
		if (m_li_bird != 0)																														m_ivyBirdStrike->Activate(m_time);
		else																																	m_ivyBirdStrike->Deactivate(m_time);

		// Overweight;
		if (m_lf_weight_tot > m_lf_weight_max)																									m_ivyOverweight->Activate(m_time);
		else																																	m_ivyOverweight->Deactivate(m_time);

		// Airspeed Alive
		if ((m_li_on_ground == 1) && (m_lf_ground_speed > m_ivyConfig->m_taxi_ground_speed_min) && (m_lf_ias >= m_ivyConfig->m_airspeed_alive)) m_ivyAirSpeedAlive->Activate(m_time);
		else if ((m_li_on_ground == 1) && (m_lf_ground_speed < m_ivyConfig->m_taxi_ground_speed_min))											m_ivyAirSpeedAlive->Deactivate(m_time);

		// Power Set
		if ((m_li_on_ground == 1) && 
			(m_lf_ground_speed > m_ivyConfig->m_to_power_speed) &&
			(m_n1_deviation > 0) &&
			(m_n1_deviation <= m_ivyAircraft->m_to_power_tolerance) && 
			(m_lf_throttle_ratio >= m_ivyAircraft->m_to_power_lever))																			m_ivyPowerSet->Activate(m_time);
		else if ((m_li_on_ground == 1) && (m_lf_ground_speed < m_ivyConfig->m_taxi_ground_speed_min))											m_ivyAirSpeedAlive->Deactivate(m_time);

		// NOE Altitude
		if ((m_li_on_ground == 0) && (m_ivyAircraft->m_max_alt_enable == true) &&(m_lf_radio_alt > m_ivyAircraft->m_max_alt_value))				m_ivyNOE->Activate(m_time);
		else																																	m_ivyNOE->Deactivate(m_time);
		

		//
		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////;
		// V-Speed callouts if configured in IvyAircraft ini file;
		// V1;
		// VR;
		// V2;
		// V2 not achieved within 5 seconds after take off;

		if ((m_li_on_ground == 1) && (m_ivyAircraft->m_li_v1 > 0) &&
			(m_lf_ias >= m_ivyAircraft->m_li_v1) && (m_lf_ground_speed > m_ivyConfig->m_taxi_ground_speed_min))									m_ivyV1->Activate(m_time);
		else if ((m_li_on_ground == 1) && (m_lf_ground_speed < 10))																				m_ivyV1->Deactivate(m_time);

		if ((m_li_on_ground == 1) && (m_ivyAircraft->m_li_vr > 0) &&
			(m_lf_ias >= m_ivyAircraft->m_li_vr) && (m_lf_ground_speed > m_ivyConfig->m_taxi_ground_speed_min))									m_ivyVR->Activate(m_time);
		else if ((m_li_on_ground == 1) && (m_lf_ground_speed < 10))																				m_ivyVR->Deactivate(m_time);

		if ((m_li_on_ground == 0) && (m_ivyAircraft->m_li_v2 > 0) &&
			(m_lf_ias >= m_ivyAircraft->m_li_v2))																								m_ivyAboveV2->Activate(m_time);
		else if (m_li_on_ground == 1)																											m_ivyAboveV2->Deactivate(m_time);

		if ((m_li_on_ground == 0) && (m_ivyAircraft->m_li_v2 > 0) &&
			(m_lf_ias < m_ivyAircraft->m_li_v2) && (m_ivyAboveV2->m_played == 0))
		{
			m_ivyBelowV2->Activate(m_time);
			m_ivyAboveV2->Deactivate(m_time);
		}
		else if (m_li_on_ground == 1)																											m_ivyBelowV2->Deactivate(m_time);

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////;
		//                                                             206B3
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////;
		if (m_ivyAircraft->m_is_206B3 == true)
		{
			// Service Ceiling
			bool overweight = true;
			if ((m_lf_weight_tot <= 1360.0f) && (m_lf_baro_alt >= 20000.0f))																	m_ivyServiceCeiling->Activate(m_time);
			else if ((m_lf_weight_tot > 1360.0f) && (m_lf_density_alt > 13500.0f))																m_ivyServiceCeiling->Activate(m_time);
			else																																m_ivyServiceCeiling->Deactivate(m_time);

			// Aircraft Overspeed;
			if (m_lf_ias > 130)																													m_ivyOverspeedAircraft->Activate(m_time);
			else if (m_lf_ias > (130 - (((m_lf_density_alt - 3000) / 1000) * 3.5)))																m_ivyOverspeedAircraft->Activate(m_time);
			else if ((m_lfa_torque[0] > 6500.0f) && (m_lf_ias > 80))																			m_ivyOverspeedAircraft->Activate(m_time);
			else if ((m_lf_206_doors > 4) && (m_lf_ias > 69))																					m_ivyOverspeedAircraft->Activate(m_time);
			else																																m_ivyOverspeedAircraft->Deactivate(m_time);

			
			// Engine Overspeed - Could be generalized
			static bool n1_failure = false;
			static int n1_failure_start = 0;

			if (m_lf_n1_percent[0] > 106.0f)
			{
				WriteLogMessage("Engine N1 Overspeed");
				XPLMSetDatai(m_i_fire1, 6);
			}
			else if (m_lf_n1_percent[0] > 105.0f)
			{
				if (n1_failure == false)
					n1_failure_start = m_time;
				if ((m_time - n1_failure_start) > 5.0f)
				{
					WriteLogMessage("Engine N1 Overspeed");
					XPLMSetDatai(m_i_fire1, 6);
				}
			}
			else
			{
				n1_failure = false;
			}


			m_ivyAircraft->m_torque_th[0] = 6500; //85%
			m_ivyAircraft->m_torque_sec[0] = 300;
			m_ivyAircraft->m_torque_th[1] = 7100; //100%
			m_ivyAircraft->m_torque_sec[1] = 5;
			m_ivyAircraft->m_torque_th[2] = 7575; //110%
			m_ivyAircraft->m_torque_sec[2] = 0.1f;

			// Overtorque
			EngineLimitsCheck(m_lfa_torque[0], m_ivyAircraft->m_torque_th, m_ivyAircraft->m_torque_sec, m_torque_failure, m_torque_start, m_i_fire1, NULL, "Overtorque");

			m_ivyAircraft->m_itt_th[0] = 738;
			m_ivyAircraft->m_itt_sec[0] = 300;
			m_ivyAircraft->m_itt_th[1] = 810;
			m_ivyAircraft->m_itt_sec[1] = 6;
			m_ivyAircraft->m_itt_th[2] = 843;
			m_ivyAircraft->m_itt_sec[2] = 0.1f;

			// ITT in Air
			if (m_li_on_ground == 0) EngineLimitsCheck(m_lfa_itt[0], m_ivyAircraft->m_itt_th, m_ivyAircraft->m_itt_sec, m_itt_failure, m_itt_start, m_i_fire1, NULL, "Engine Temperature");

			m_ivyAircraft->m_itt_th[0] = 738;
			m_ivyAircraft->m_itt_sec[0] = 300;
			m_ivyAircraft->m_itt_th[1] = 810;
			m_ivyAircraft->m_itt_sec[1] = 10;
			m_ivyAircraft->m_itt_th[2] = 0;//927;
			m_ivyAircraft->m_itt_sec[2] = 0;// 0.1f;

			// ITT on Ground
			if (m_li_on_ground == 1) EngineLimitsCheck(m_lfa_itt[0], m_ivyAircraft->m_itt_th, m_ivyAircraft->m_itt_sec, m_itt_failure, m_itt_start, m_i_fire1, NULL, "Engine Temperature");

			m_ivyAircraft->m_oil_press_th[0] = 50;
			m_ivyAircraft->m_oil_press_sec[0] = 300;
			m_ivyAircraft->m_oil_press_th[1] = 70;
			m_ivyAircraft->m_oil_press_sec[1] = 0.1f;
			m_ivyAircraft->m_oil_press_th[2] = -30;
			m_ivyAircraft->m_oil_press_sec[2] = 600;

			// Oil Pressure High in Air
			if (m_li_on_ground == 0) EngineLimitsCheck(m_lf_206_oil_press, m_ivyAircraft->m_oil_press_th, m_ivyAircraft->m_oil_press_sec, m_oil_press_failure, m_oil_press_start, m_i_fire1, NULL, "Transmission Oil Pressure");

			m_ivyAircraft->m_oil_press_th[2] = 0;
			m_ivyAircraft->m_oil_press_sec[2] = 0;

			// Oil Pressure High on Ground
			if (m_li_on_ground == 1) EngineLimitsCheck(m_lf_206_oil_press, m_ivyAircraft->m_oil_press_th, m_ivyAircraft->m_oil_press_sec, m_oil_press_failure, m_oil_press_start, m_i_fire1, NULL, "Transmission Oil Pressure");

			m_ivyAircraft->m_oil_temp_th[0] = 110;
			m_ivyAircraft->m_oil_temp_sec[0] = 0;
			m_ivyAircraft->m_oil_temp_th[1] = -15;
			m_ivyAircraft->m_oil_temp_sec[1] = 600;
			m_ivyAircraft->m_oil_temp_th[2] = 0;
			m_ivyAircraft->m_oil_temp_sec[2] = 0;

			// Oil Temperature High in Air
			if (m_li_on_ground == 0) EngineLimitsCheck(m_lf_206_oil_press, m_ivyAircraft->m_oil_temp_th, m_ivyAircraft->m_oil_temp_sec, m_oil_temp_failure, m_oil_temp_start, m_i_fire1, NULL, "Transmission Oil Temperature");

			m_ivyAircraft->m_oil_temp_th[1] = 0;
			m_ivyAircraft->m_oil_temp_sec[1] = 0;

			// Oil Temperature High On Ground
			if (m_li_on_ground == 1) EngineLimitsCheck(m_lf_206_oil_press, m_ivyAircraft->m_oil_temp_th, m_ivyAircraft->m_oil_temp_sec, m_oil_temp_failure, m_oil_temp_start, m_i_fire1, NULL, "Transmission Oil Temperature");

			float m_rotor_th[IVY_ENGN_MAX];
			float m_rotor_sec[IVY_ENGN_MAX];

			// Rotor RPM
			if ((m_lia_engine_running[0] > 0) && (m_li_on_ground == 0))
			{
				m_rotor_th[0] = -95;
				m_rotor_sec[0] = 0;
				m_rotor_th[1] = -97;
				m_rotor_sec[1] = 5;
				m_rotor_th[2] = 0;
				m_rotor_sec[2] = 0;

				EngineLimitsCheck(m_lf_206_rotor_rpm, m_rotor_th, m_rotor_sec, m_rotor_failure, m_rotor_start, NULL, m_ivyRpmLow, "");

				m_rotor_th[0] = 0;
				m_rotor_sec[0] = 0;
				m_rotor_th[1] = 0;
				m_rotor_sec[1] = 0;
				m_rotor_th[2] = 105;
				m_rotor_sec[2] = 1;

				EngineLimitsCheck(m_lf_206_rotor_rpm, m_rotor_th, m_rotor_sec, m_rotor_failure, m_rotor_start, NULL, m_ivyRpmHigh, "");
			}
			else if ((m_lia_engine_running[0] == 0) && (m_li_on_ground == 0))
			{
				m_rotor_th[0] = -90;
				m_rotor_sec[0] = 1;
				m_rotor_th[1] = 0;
				m_rotor_sec[1] = 0;
				m_rotor_th[2] = 0;
				m_rotor_sec[2] = 0;

				EngineLimitsCheck(m_lf_206_rotor_rpm, m_rotor_th, m_rotor_sec, m_rotor_failure, m_rotor_start, NULL, m_ivyRpmLow, "");

				m_rotor_th[0] = 0;
				m_rotor_sec[0] = 0;
				m_rotor_th[1] = 0;
				m_rotor_sec[1] = 0;
				m_rotor_th[2] = 107;
				m_rotor_sec[2] = 1;

				EngineLimitsCheck(m_lf_206_rotor_rpm, m_rotor_th, m_rotor_sec, m_rotor_failure, m_rotor_start, NULL, m_ivyRpmHigh, "");
			}

			// Battery Draining
			if ((m_li_battery_on > 0) && ((m_lia_generator_running[0] == 0) || (m_lia_engine_running[0] == 0)))
			{
				bool draining = false;

				if (m_li_landing_lights > 0)		draining = true;
				if (m_li_nav_lights > 0)	    	draining = true;
				if (m_lia_eng_anti_ice[0] > 0)		draining = true;
				if (m_li_pitot_heat > 0)	    	draining = true;
				if (m_li_206_br_heat == 0)	    	draining = true;

				if (draining) m_ivyBattDrain->Activate(m_time);
				else m_ivyBattDrain->Deactivate(m_time);

			}

			// Generator Low RPM
			if ((m_lia_generator_running[0] > 0) && (m_lia_engine_running[0] > 0) && (m_lf_n1_percent[0] < 60.0f))
			{
				m_ivyGenLowRPM->Activate(m_time);
			}
			else
			{
				m_ivyGenLowRPM->Deactivate(m_time);
			}

			bool breakers_in = true;
			bool controls_neutral = true;

			if (m_lf_cyclic_ail[0]  < -2.0f) controls_neutral = false;
			if (m_lf_cyclic_ail[0]  >  2.0f) controls_neutral = false;
			if (m_lf_cyclic_elev[0] < -3.0f) controls_neutral = false;
			if (m_lf_cyclic_elev[0] >  4.0f) controls_neutral = false;

			int breakers = 0;

			breakers += m_li_206_br_adf;
			breakers += m_li_206_br_avionics;
			breakers += m_li_206_br_cable;
			breakers += m_li_206_br_cargo_hook;
			breakers += m_li_206_br_caution_light;
			breakers += m_li_206_br_cockpit_light;
			breakers += m_li_206_br_anitice;
			breakers += m_li_206_br_xmsn;
			breakers += m_li_206_br_floats;
			breakers += m_li_206_br_boost_f;
			breakers += m_li_206_br_boost_a;
			breakers += m_li_206_br_fuel_press;
			breakers += m_li_206_br_fuel_valve;
			breakers += m_li_206_br_gen_field;
			breakers += m_li_206_br_gen_reset;
			breakers += m_li_206_br_gov;
			breakers += m_li_206_br_gps;
			breakers += m_li_206_br_hmike;
			//breakers += m_li_206_br_heat;
			breakers += m_li_206_br_hoist;
			breakers += m_li_206_br_htr_cont;
			breakers += m_li_206_br_htr_pwr;
			breakers += m_li_206_br_htr_hyd_boost;
			breakers += m_li_206_br_htr_ics;
			breakers += m_li_206_br_htr_ign;
			breakers += m_li_206_br_htr_inst_light;
			breakers += m_li_206_br_htr_ldg_light;
			breakers += m_li_206_br_htr_sas;
			breakers += m_li_206_br_htr_starter;
			breakers += m_li_206_br_htr_tot_ind;
			breakers += m_li_206_br_htr_turn;
			breakers += m_li_206_br_htr_vhf;
			breakers += m_li_206_br_htr_xpdf;


			if (breakers > 0) breakers_in = false;


			if (m_li_206_caut_test > 0) m_206_caution_test = true;
			if (m_li_206_tot_test > 0) m_206_tot_test = true;

			// Starter Preconditions

			if ((m_lia_engine_start[0] == 1) && (m_starter_switch_old == 0))
			{
				m_starter_off_early = false;
				m_ivyStarterOffLate->Deactivate(m_time);
				m_ivyStarterOffEarly->Deactivate(m_time);

				

				if (m_li_landing_lights > 0)		m_precond = false;
				if (m_lia_eng_anti_ice[0] > 0)		m_precond = false;
				if (m_li_pitot_heat > 0)			m_precond = false;
				if (m_li_206_hydraulics == 0)		m_precond = false;
				if (breakers_in == false)			m_precond = false;
				if (m_lia_generator_running[0] > 0) m_precond = false;
				if (m_206_caution_test == false)	m_precond = false;
				if (m_206_tot_test == false)		m_precond = false;
				if (controls_neutral == false)		m_precond = false;
				if (m_li_beacon_lights == 0)		m_precond = false;
				if (m_lf_206_throttle > 0.1f)		m_precond = false;

				
			}

			// Start Off Early
			else if ((m_lia_engine_start[0] == 0) && (m_starter_switch_old == 1))
			{
				if (m_lf_n1_percent[0] < 55.0f) m_starter_off_early = true;
					
			}
			// Starter Off Late
			else if ((m_lia_engine_start[0] == 1) && (m_lf_n1_percent[0] > 65.0f))
			{
				m_ivyStarterOffLate->Activate(m_time);
			}
			else if (m_lia_engine_start[0] == 0)
			{
				m_ivyStarterPrecond->Deactivate(m_time);
				m_precond = true;
			}

			if (m_precond == false) m_ivyStarterPrecond->Activate(m_time);
			if (m_starter_off_early == true) m_ivyStarterOffEarly->Activate(m_time);

			m_starter_switch_old = m_lia_engine_start[0];

			// Circuit Breakers
			if ((breakers_in == false) && ((m_lia_engine_start[0] == 1) || (m_lia_engine_running[0] > 0)))
			{
				m_ivyCircuitBreakers->Activate(m_time);
			}
			else
			{
				m_ivyCircuitBreakers->Deactivate(m_time);
			}

			// Engine Wear
			if ((m_li_on_ground == 1) && (m_lf_206_rotor_rpm < 88.0f) && (m_lfa_torque[0] > 3000.0f))
			{
				m_ivyEngineWear->Activate(m_time);
			}
			else
			{
				m_ivyEngineWear->Deactivate(m_time);
			}

			// Cyclic not Centered
			if ((m_li_on_ground == 1) && (m_lf_206_rotor_rpm < 90.0f) && (m_lf_206_rotor_rpm > 20.0f) && (controls_neutral == false))
			{
				m_ivyCyclicNotCentered->Activate(m_time);
			}
			else if ((m_li_on_ground == 1) && (m_lia_engine_start[0] == 1) && (controls_neutral == false))
			{
				m_ivyCyclicNotCentered->Activate(m_time);
			}
			else
			{
				m_ivyCyclicNotCentered->Deactivate(m_time);
			}

			// VRS Conditions
			if ((m_lf_ias < 30.0f) && (m_lf_vertical_speed < -300.0f))
			{
				m_ivyVRS->Activate(m_time);
			}
			else
			{
				m_ivyVRS->Deactivate(m_time);
			}

			//Pitot Hot
			if ((m_lf_outside_temp1 > 4.4f) && (m_li_pitot_heat > 0))	m_ivyPitotHot->Activate(m_time);
			else m_ivyPitotHot->Deactivate(m_time);

			//Anti Ice Hot
			if ((m_lf_outside_temp1 > 4.4f) && (m_lia_eng_anti_ice[0] > 0))	m_ivyAntiIceHot->Activate(m_time);
			else m_ivyAntiIceHot->Deactivate(m_time);

			// Azimuth Conditions

			float wind_relative = m_lf_wind_direction - m_lf_acf_heading;

			if (wind_relative < 0) wind_relative += 360.0f;
			if (wind_relative > 360) wind_relative -= 360.0f;

			if ((m_lf_ias < 10.0f) && (wind_relative > 50.0f) && (wind_relative < 210.0f) && (m_lf_wind_speed_kt > 10.0f))
			{
				m_ivyWindAzimuth->Activate(m_time);
			}
			else
			{
				m_ivyWindAzimuth->Deactivate(m_time);
			}

			if (XPLMGetDatai(m_i_fire1) == 6) XPLMSetDatai(m_i_engine_failure1, 6);

			static float idle_time = m_time;

			if (m_lf_206_throttle > 0.3) idle_time = m_time;


			if ((m_engine_on_old == 1) && (m_lia_engine_running[0] == 0))
			{
				if ((m_time - idle_time) < 120)	m_ivyIdleTime->Activate(m_time);
			}
			else
			{
				m_ivyIdleTime->Deactivate(m_time);
			}

			m_engine_on_old = m_lia_engine_running[0];
			
			// Errors

			if (m_ivyAircraft->m_206B3_MTBF > 0)
			{
				static int time_old_206 = 0;
				int time = (int) m_time;

				// Once per second
				if (time > time_old_206)
				{
					

					int possibility = (int)(m_ivyAircraft->m_206B3_MTBF * 3600.0f);
					possibility = rand() % possibility;

					if (possibility == 0)
					{
						m_ivyCircuitBreakers->SetAsPlayed(m_time);

						std::vector<XPLMDataRef> breakers;

						breakers.push_back(m_i_206_br_adf);
						//breakers.push_back(m_i_206_br_avionics);
						breakers.push_back(m_i_206_br_cockpit_light);
						breakers.push_back(m_i_206_br_xmsn);
						breakers.push_back(m_i_206_br_fuel_press);
						breakers.push_back(m_i_206_br_gps);
						breakers.push_back(m_i_206_br_htr_inst_light);
						breakers.push_back(m_i_206_br_htr_sas);
						breakers.push_back(m_i_206_br_htr_tot_ind);
						//breakers.push_back(m_i_206_br_htr_turn);
						breakers.push_back(m_i_206_br_htr_vhf);

						int index = rand() % breakers.size();

						XPLMSetDatai(breakers[index], 1);

					}

				}
				time_old_206 = time;
			}


		}

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////;
		//                                                             206L3
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////;
		if (m_ivyAircraft->m_is_206L3 == true)
		{
			// Service Ceiling
			bool overweight = true;
			if ((m_lf_weight_tot <= 1360.0f) && (m_lf_baro_alt >= 20000.0f))																	m_ivyServiceCeiling->Activate(m_time);
			else if ((m_lf_weight_tot > 1882.4f) && (m_lf_density_alt > 10000.0f))																m_ivyServiceCeiling->Activate(m_time);
			else																																m_ivyServiceCeiling->Deactivate(m_time);

			// Aircraft Overspeed;
			if (m_lf_ias > 130)																													m_ivyOverspeedAircraft->Activate(m_time);
			else if ((m_lfa_torque[0] > 7000.0f) && (m_lf_ias > 84))																			m_ivyOverspeedAircraft->Activate(m_time);
			else if ((m_lf_206L3_doors > 4) && (m_lf_ias > 90))																					m_ivyOverspeedAircraft->Activate(m_time);
			else																																m_ivyOverspeedAircraft->Deactivate(m_time);


			// Engine Overspeed - Could be generalized
			static bool n1_failure = false;
			static int n1_failure_start = 0;

			if (m_lf_n1_percent[0] > 104.0f)
			{
				WriteLogMessage("Engine N1 Overspeed");
				XPLMSetDatai(m_i_fire1, 6);
			}
			else if (m_lf_n1_percent[0] > 101.0f)
			{
				if (n1_failure == false)
					n1_failure_start = m_time;
				if ((m_time - n1_failure_start) > 4.0f)
				{
					WriteLogMessage("Engine N1 Overspeed");
					XPLMSetDatai(m_i_fire1, 6);
				}
			}
			else
			{
				n1_failure = false;
			}


			m_ivyAircraft->m_torque_th[0] = 9200; //85%
			m_ivyAircraft->m_torque_sec[0] = 300;
			m_ivyAircraft->m_torque_th[1] = 11000; //100%
			m_ivyAircraft->m_torque_sec[1] = 5;
			m_ivyAircraft->m_torque_th[2] = 12500; //110%
			m_ivyAircraft->m_torque_sec[2] = 0.1f;

			// Overtorque
			EngineLimitsCheck(m_lfa_torque[0], m_ivyAircraft->m_torque_th, m_ivyAircraft->m_torque_sec, m_torque_failure, m_torque_start, m_i_fire1, NULL, "Overtorque");

			m_ivyAircraft->m_itt_th[0] = 716;
			m_ivyAircraft->m_itt_sec[0] = 300;
			m_ivyAircraft->m_itt_th[1] = 871;
			m_ivyAircraft->m_itt_sec[1] = 6;

			// ITT in Air
			if (m_li_on_ground == 0) EngineLimitsCheck(m_lfa_itt[0], m_ivyAircraft->m_itt_th, m_ivyAircraft->m_itt_sec, m_itt_failure, m_itt_start, m_i_fire1, NULL, "Engine Temperature");

			m_ivyAircraft->m_itt_th[0] = 716;
			m_ivyAircraft->m_itt_sec[0] = 300;
			m_ivyAircraft->m_itt_th[1] = 871;
			m_ivyAircraft->m_itt_sec[1] = 10;
			m_ivyAircraft->m_itt_th[2] = 0;//927;
			m_ivyAircraft->m_itt_sec[2] = 0;// 0.1f;

			// ITT on Ground
			if (m_li_on_ground == 1) EngineLimitsCheck(m_lfa_itt[0], m_ivyAircraft->m_itt_th, m_ivyAircraft->m_itt_sec, m_itt_failure, m_itt_start, m_i_fire1, NULL, "Engine Temperature");

			m_ivyAircraft->m_oil_press_th[0] = 70;
			m_ivyAircraft->m_oil_press_sec[0] = 0.1;
			m_ivyAircraft->m_oil_press_th[1] = 70;
			m_ivyAircraft->m_oil_press_sec[1] = 0.1f;
			m_ivyAircraft->m_oil_press_th[2] = -30;
			m_ivyAircraft->m_oil_press_sec[2] = 600;

			// Oil Pressure High in Air
			if (m_li_on_ground == 0) EngineLimitsCheck(m_lf_206L3_oil_press, m_ivyAircraft->m_oil_press_th, m_ivyAircraft->m_oil_press_sec, m_oil_press_failure, m_oil_press_start, m_i_fire1, NULL, "Transmission Oil Pressure");

			m_ivyAircraft->m_oil_press_th[2] = 0;
			m_ivyAircraft->m_oil_press_sec[2] = 0;

			// Oil Pressure High on Ground
			if (m_li_on_ground == 1) EngineLimitsCheck(m_lf_206L3_oil_press, m_ivyAircraft->m_oil_press_th, m_ivyAircraft->m_oil_press_sec, m_oil_press_failure, m_oil_press_start, m_i_fire1, NULL, "Transmission Oil Pressure");

			m_ivyAircraft->m_oil_temp_th[0] = 107;
			m_ivyAircraft->m_oil_temp_sec[0] = 0.1;
			m_ivyAircraft->m_oil_temp_th[1] = 0;
			m_ivyAircraft->m_oil_temp_sec[1] = 0;
			m_ivyAircraft->m_oil_temp_th[2] = 0;
			m_ivyAircraft->m_oil_temp_sec[2] = 0;

			// Oil Temperature High in Air
			if (m_li_on_ground == 0) EngineLimitsCheck(m_lf_206L3_oil_press, m_ivyAircraft->m_oil_temp_th, m_ivyAircraft->m_oil_temp_sec, m_oil_temp_failure, m_oil_temp_start, m_i_fire1, NULL, "Transmission Oil Temperature");

			m_ivyAircraft->m_oil_temp_th[1] = 0;
			m_ivyAircraft->m_oil_temp_sec[1] = 0;

			// Oil Temperature High On Ground
			if (m_li_on_ground == 1) EngineLimitsCheck(m_lf_206L3_oil_press, m_ivyAircraft->m_oil_temp_th, m_ivyAircraft->m_oil_temp_sec, m_oil_temp_failure, m_oil_temp_start, m_i_fire1, NULL, "Transmission Oil Temperature");

			float m_rotor_th[IVY_ENGN_MAX];
			float m_rotor_sec[IVY_ENGN_MAX];

			// Rotor RPM
			if ((m_lia_engine_running[0] > 0) && (m_li_on_ground == 0))
			{
				m_rotor_th[0] = -95;
				m_rotor_sec[0] = 0;
				m_rotor_th[1] = -97;
				m_rotor_sec[1] = 5;
				m_rotor_th[2] = 0;
				m_rotor_sec[2] = 0;

				EngineLimitsCheck(m_lf_206L3_rotor_rpm, m_rotor_th, m_rotor_sec, m_rotor_failure, m_rotor_start, NULL, m_ivyRpmLow, "");

				m_rotor_th[0] = 0;
				m_rotor_sec[0] = 0;
				m_rotor_th[1] = 0;
				m_rotor_sec[1] = 0;
				m_rotor_th[2] = 105;
				m_rotor_sec[2] = 1;

				EngineLimitsCheck(m_lf_206L3_rotor_rpm, m_rotor_th, m_rotor_sec, m_rotor_failure, m_rotor_start, NULL, m_ivyRpmHigh, "");
			}
			else if ((m_lia_engine_running[0] == 0) && (m_li_on_ground == 0))
			{
				m_rotor_th[0] = -90;
				m_rotor_sec[0] = 1;
				m_rotor_th[1] = 0;
				m_rotor_sec[1] = 0;
				m_rotor_th[2] = 0;
				m_rotor_sec[2] = 0;

				EngineLimitsCheck(m_lf_206L3_rotor_rpm, m_rotor_th, m_rotor_sec, m_rotor_failure, m_rotor_start, NULL, m_ivyRpmLow, "");

				m_rotor_th[0] = 0;
				m_rotor_sec[0] = 0;
				m_rotor_th[1] = 0;
				m_rotor_sec[1] = 0;
				m_rotor_th[2] = 107;
				m_rotor_sec[2] = 1;

				EngineLimitsCheck(m_lf_206L3_rotor_rpm, m_rotor_th, m_rotor_sec, m_rotor_failure, m_rotor_start, NULL, m_ivyRpmHigh, "");
			}

			// Battery Draining
			if ((m_li_battery_on > 0) && ((m_lia_generator_running[0] == 0) || (m_lia_engine_running[0] == 0)))
			{
				bool draining = false;

				if (m_li_landing_lights > 0)		draining = true;
				if (m_li_nav_lights > 0)	    	draining = true;
				if (m_lia_eng_anti_ice[0] > 0)		draining = true;
				if (m_li_pitot_heat > 0)	    	draining = true;
				if (m_li_206L3_br_heat == 0)	    	draining = true;

				if (draining) m_ivyBattDrain->Activate(m_time);
				else m_ivyBattDrain->Deactivate(m_time);

			}

			// Generator Low RPM
			if ((m_lia_generator_running[0] > 0) && (m_lia_engine_running[0] > 0) && (m_lf_n1_percent[0] < 60.0f))
			{
				m_ivyGenLowRPM->Activate(m_time);
			}
			else
			{
				m_ivyGenLowRPM->Deactivate(m_time);
			}

			bool breakers_in = true;
			bool controls_neutral = true;

			if (m_lf_cyclic_ail[0] < -2.0f) controls_neutral = false;
			if (m_lf_cyclic_ail[0] > 2.0f) controls_neutral = false;
			if (m_lf_cyclic_elev[0] < -3.0f) controls_neutral = false;
			if (m_lf_cyclic_elev[0] > 4.0f) controls_neutral = false;

			int breakers = 0;

			breakers += m_li_206L3_br_adf;
			breakers += m_li_206L3_br_avionics;
			breakers += m_li_206L3_br_cable;
			breakers += m_li_206L3_br_cargo_hook;
			breakers += m_li_206L3_br_caution_light;
			breakers += m_li_206L3_br_cockpit_light;
			breakers += m_li_206L3_br_anitice;
			breakers += m_li_206L3_br_xmsn;
			breakers += m_li_206L3_br_floats;
			breakers += m_li_206L3_br_boost_f;
			breakers += m_li_206L3_br_boost_a;
			breakers += m_li_206L3_br_fuel_press;
			breakers += m_li_206L3_br_fuel_valve;
			breakers += m_li_206L3_br_gen_field;
			breakers += m_li_206L3_br_gen_reset;
			breakers += m_li_206L3_br_gov;
			breakers += m_li_206L3_br_gps;
			breakers += m_li_206L3_br_hmike;
			//breakers += m_li_206L3_br_heat;
			breakers += m_li_206L3_br_hoist;
			breakers += m_li_206L3_br_htr_cont;
			breakers += m_li_206L3_br_htr_pwr;
			breakers += m_li_206L3_br_htr_hyd_boost;
			breakers += m_li_206L3_br_htr_ics;
			breakers += m_li_206L3_br_htr_ign;
			breakers += m_li_206L3_br_htr_inst_light;
			breakers += m_li_206L3_br_htr_ldg_light;
			breakers += m_li_206L3_br_htr_sas;
			breakers += m_li_206L3_br_htr_starter;
			breakers += m_li_206L3_br_htr_tot_ind;
			breakers += m_li_206L3_br_htr_turn;
			breakers += m_li_206L3_br_htr_vhf;
			breakers += m_li_206L3_br_htr_xpdf;


			if (breakers > 0) breakers_in = false;


			if (m_li_206L3_caut_test > 0) m_206L3_caution_test = true;
			if (m_li_206L3_tot_test > 0) m_206L3_tot_test = true;

			// Starter Preconditions

			if ((m_lia_engine_start[0] == 1) && (m_starter_switch_old == 0))
			{
				m_starter_off_early = false;
				m_ivyStarterOffLate->Deactivate(m_time);
				m_ivyStarterOffEarly->Deactivate(m_time);



				if (m_li_landing_lights > 0)		m_precond = false;
				if (m_lia_eng_anti_ice[0] > 0)		m_precond = false;
				if (m_li_pitot_heat > 0)			m_precond = false;
				if (m_li_206L3_hydraulics == 0)		m_precond = false;
				if (breakers_in == false)			m_precond = false;
				if (m_lia_generator_running[0] > 0) m_precond = false;
				if (m_206L3_caution_test == false)	m_precond = false;
				if (m_206L3_tot_test == false)		m_precond = false;
				if (controls_neutral == false)		m_precond = false;
				if (m_li_beacon_lights == 0)		m_precond = false;
				if (m_lf_206L3_throttle > 0.1f)		m_precond = false;


			}

			// Start Off Early
			else if ((m_lia_engine_start[0] == 0) && (m_starter_switch_old == 1))
			{
				if (m_lf_n1_percent[0] < 55.0f) m_starter_off_early = true;

			}
			// Starter Off Late
			else if ((m_lia_engine_start[0] == 1) && (m_lf_n1_percent[0] > 65.0f))
			{
				m_ivyStarterOffLate->Activate(m_time);
			}
			else if (m_lia_engine_start[0] == 0)
			{
				m_ivyStarterPrecond->Deactivate(m_time);
				m_precond = true;
			}

			if (m_precond == false) m_ivyStarterPrecond->Activate(m_time);
			if (m_starter_off_early == true) m_ivyStarterOffEarly->Activate(m_time);

			m_starter_switch_old = m_lia_engine_start[0];

			// Circuit Breakers
			if ((breakers_in == false) && ((m_lia_engine_start[0] == 1) || (m_lia_engine_running[0] > 0)))
			{
				m_ivyCircuitBreakers->Activate(m_time);
			}
			else
			{
				m_ivyCircuitBreakers->Deactivate(m_time);
			}

			// Engine Wear
			if ((m_li_on_ground == 1) && (m_lf_206L3_rotor_rpm < 88.0f) && (m_lfa_torque[0] > 3200.0f))
			{
				m_ivyEngineWear->Activate(m_time);
			}
			else
			{
				m_ivyEngineWear->Deactivate(m_time);
			}

			// Cyclic not Centered
			if ((m_li_on_ground == 1) && (m_lf_206L3_rotor_rpm < 90.0f) && (m_lf_206L3_rotor_rpm > 20.0f) && (controls_neutral == false))
			{
				m_ivyCyclicNotCentered->Activate(m_time);
			}
			else if ((m_li_on_ground == 1) && (m_lia_engine_start[0] == 1) && (controls_neutral == false))
			{
				m_ivyCyclicNotCentered->Activate(m_time);
			}
			else
			{
				m_ivyCyclicNotCentered->Deactivate(m_time);
			}

			// VRS Conditions
			if ((m_lf_ias < 30.0f) && (m_lf_vertical_speed < -300.0f))
			{
				m_ivyVRS->Activate(m_time);
			}
			else
			{
				m_ivyVRS->Deactivate(m_time);
			}

			//Pitot Hot
			if ((m_lf_outside_temp1 > 4.4f) && (m_li_pitot_heat > 0))	m_ivyPitotHot->Activate(m_time);
			else m_ivyPitotHot->Deactivate(m_time);

			//Anti Ice Hot
			if ((m_lf_outside_temp1 > 4.4f) && (m_lia_eng_anti_ice[0] > 0))	m_ivyAntiIceHot->Activate(m_time);
			else m_ivyAntiIceHot->Deactivate(m_time);


			if (XPLMGetDatai(m_i_fire1) == 6) XPLMSetDatai(m_i_engine_failure1, 6);

			static float idle_time = m_time;

			if (m_lf_206L3_throttle > 0.3) idle_time = m_time;


			if ((m_engine_on_old == 1) && (m_lia_engine_running[0] == 0))
			{
				if ((m_time - idle_time) < 120)	m_ivyIdleTime->Activate(m_time);
			}
			else
			{
				m_ivyIdleTime->Deactivate(m_time);
			}

			m_engine_on_old = m_lia_engine_running[0];

			// Errors

			if (m_ivyAircraft->m_206L3_MTBF > 0)
			{
				static int time_old_206L3 = 0;
				int time = (int)m_time;

				// Once per second
				if (time > time_old_206L3)
				{


					int possibility = (int)(m_ivyAircraft->m_206L3_MTBF * 3600.0f);
					possibility = rand() % possibility;

					if (possibility == 0)
					{
						m_ivyCircuitBreakers->SetAsPlayed(m_time);

						std::vector<XPLMDataRef> breakers;

						breakers.push_back(m_i_206L3_br_adf);
						//breakers.push_back(m_i_206L3_br_avionics);
						breakers.push_back(m_i_206L3_br_cockpit_light);
						breakers.push_back(m_i_206L3_br_xmsn);
						breakers.push_back(m_i_206L3_br_fuel_press);
						breakers.push_back(m_i_206L3_br_gps);
						breakers.push_back(m_i_206L3_br_htr_inst_light);
						breakers.push_back(m_i_206L3_br_htr_sas);
						breakers.push_back(m_i_206L3_br_htr_tot_ind);
						//breakers.push_back(m_i_206L3_br_htr_turn);
						breakers.push_back(m_i_206L3_br_htr_vhf);

						int index = rand() % breakers.size();

						XPLMSetDatai(breakers[index], 1);

					}

				}
				time_old_206L3 = time;
			}


		}



		if (m_ivyConfig->m_callouts_enable)
		{
			bool slats_activated = false;
			////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////;
			// Slats callout if configured in IvyAircraft ini file;
			if ((m_ivyAircraft->m_slats_enabled == true) && (m_lf_flap_ratio == 0))
			{
				m_ivySlatsRetracted->Activate(m_time);
				slats_activated = true;
			}
			else																																	m_ivySlatsRetracted->Deactivate(m_time);


			if ((m_ivyAircraft->m_slats_enabled == true) && (slats_activated == false))
			{

				for (int index = 0; index < IVY_FS_MAX; index++)
				{
					//if (abs(m_ivyAircraft->m_lf_flaps - m_ivyAircraft->m_slats_deploy_value[index]) < m_ivyAircraft->m_slats_tolerance)
					if (abs(m_lf_flap_ratio - m_ivyAircraft->m_slats_deploy_value[index]) < m_ivyAircraft->m_slats_tolerance)
					{
						slats_activated = true;
						m_ivySlatsPosition->Activate(m_time);

						if ((m_ivySlatsPosition->m_played == 1) && (m_ivySlatsPosition->m_internal_note == 0))
						{

							SayNumber(m_ivyAircraft->m_slats_deploy_pos[index]);
							m_ivySlatsPosition->m_internal_note = 1;
						}
					}
				}
			}
			if (slats_activated == false)																											m_ivySlatsPosition->Deactivate(m_time);

			////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////;
			// Flaps callout if configured in IvyAircraft ini file;
			bool flaps_activated = false;
			static float flaps_old = 0;

			if (flaps_old != m_lf_flap_ratio) m_ivyFlapsPosition->Deactivate(m_time);
			flaps_old = m_lf_flap_ratio;

			if (slats_activated == false)
			{

				if ((m_ivyAircraft->m_flaps_enabled == true) && (m_lf_flap_ratio == 0))
				{
					m_ivyFlapsRetracted->Activate(m_time);
					flaps_activated = true;
				}
				else																																	m_ivyFlapsRetracted->Deactivate(m_time);

				if ((m_ivyAircraft->m_flaps_enabled == true) && (flaps_activated == false))
				{

					for (int index = 0; index < IVY_FS_MAX; index++)
					{
						if (abs(m_lf_flap_ratio - m_ivyAircraft->m_flaps_deploy_value[index]) < m_ivyAircraft->m_flaps_tolerance)
						{
							flaps_activated = true;
							m_ivyFlapsPosition->Activate(m_time);


							if ((m_ivyFlapsPosition->m_played == 1) && (m_ivyFlapsPosition->m_internal_note == 0))
							{
								SayNumber(m_ivyAircraft->m_flaps_deploy_pos[index]);
								m_ivyFlapsPosition->m_internal_note = 1;
							}
						}
					}

				}
			}
			if (flaps_activated == false)																										m_ivyFlapsPosition->Deactivate(m_time);
		}
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////;
		// After landing{
		// Rating of the Landing;
		// End of flight evaluation;

		// We want to get airborne before landing evaluation - too many false alarms on load;
		if ((m_li_on_ground == 0) && (((m_lf_radio_alt > 100) && (m_lf_climb_rate > 100)) || (m_lf_radio_alt > 500)))
		{
																																				m_ivyArmLanding->Activate(m_time);
																																				//m_ivy60kt->SetAsPlayed(m_time);
																																				//m_ivy80kt->SetAsPlayed(m_time);
																																				//m_ivy100kt->SetAsPlayed(m_time);
																																				m_ivyRotate->SetAsPlayed(m_time);
																																				//m_ivyPosRateClimb->SetAsPlayed(m_time);
																																				m_ivyAirSpeedAlive->SetAsPlayed(m_time);
																																				m_ivyPowerSet->SetAsPlayed(m_time);
																																				m_ivyV1->SetAsPlayed(m_time);
																																				m_ivyVR->SetAsPlayed(m_time);
		}
		if ((m_li_on_ground == 0) && (m_lf_radio_alt > 2000))
		{
																																				m_ivyPosRateClimb->SetAsPlayed(m_time);
																																				m_ivyAboveV2->SetAsPlayed(m_time);
		}

		if ((m_landing_detected == 1) && (m_landing_rated > 0) &&
			(m_ivyArmLanding->m_played == 1) && (m_ivyConfig->m_passengers_applause == true))													
																																				m_ivyApplause->Activate(m_time);
		else if (m_li_on_ground == 0)																											m_ivyApplause->Deactivate(m_time);


		if ((m_landing_detected == 1) && (m_landing_rated == 1) && (m_lf_ground_speed < m_ivyConfig->m_taxi_ground_speed_min) && (m_ivyArmLanding->m_played == 1))
		{
			m_ivyLandingXGood->Activate(m_time);
			if (m_ivyLandingXGood->m_played == 1)
			{
				EndOfFlightEvaluation();
				m_ivyArmLanding->Deactivate(m_time);
			}
		}
		else																																		m_ivyLandingXGood->Deactivate(m_time);

		if ((m_landing_detected == 1) && (m_landing_rated == 2) && (m_lf_ground_speed < m_ivyConfig->m_taxi_ground_speed_min) && (m_ivyArmLanding->m_played == 1))
		{
			m_ivyLandingGood->Activate(m_time);
			if (m_ivyLandingGood->m_played == 1)
			{
				EndOfFlightEvaluation();
				m_ivyArmLanding->Deactivate(m_time);
			}
		}
		else																																	m_ivyLandingGood->Deactivate(m_time);

		if ((m_landing_detected == 1) && (m_landing_rated == 3) && (m_lf_ground_speed < m_ivyConfig->m_taxi_ground_speed_min) && (m_ivyArmLanding->m_played == 1))
		{
			m_ivyLandingNormal->Activate(m_time);
			if (m_ivyLandingNormal->m_played == 1)
			{
				EndOfFlightEvaluation();
				m_ivyArmLanding->Deactivate(m_time);
			}
		}
		else																																	m_ivyLandingNormal->Deactivate(m_time);

		if ((m_landing_detected == 1) && (m_landing_rated == 4) && (m_lf_ground_speed < m_ivyConfig->m_taxi_ground_speed_min) && (m_ivyArmLanding->m_played == 1))
		{
			m_ivyLandingBad->Activate(m_time);
			if (m_ivyLandingBad->m_played == 1)
			{
				EndOfFlightEvaluation();
				m_ivyArmLanding->Deactivate(m_time);
			}
		}
		else																																	m_ivyLandingBad->Deactivate(m_time);


		if ((m_landing_detected == 1) && (m_landing_rated == 5) && (m_lf_ground_speed < m_ivyConfig->m_taxi_ground_speed_min) && (m_ivyArmLanding->m_played == 1))
		{
			m_ivyLandingXBad->Activate(m_time);
			if (m_ivyLandingXBad->m_played == 1)
			{
				EndOfFlightEvaluation();
				m_ivyArmLanding->Deactivate(m_time);
			}
		}
		else																																	m_ivyLandingXBad->Deactivate(m_time);
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////;
	// After Load || after Crash;

	if ((m_aircraft_crashed == 1) && (m_time > m_ivyConfig->m_disable_after_loading))															m_ivyCrash->Activate(m_time);
	else																																		m_ivyCrash->Deactivate(m_time);

	// Here comes the screaming;
	m_passengerVolume = max(m_passengerVolume, abs(m_lf_roll) / 120);
	m_passengerVolume = max(m_passengerVolume, abs(m_lf_pitch) / 60);

	if (m_ivyConfig->m_passengers_screaming == false) m_passengersScreaming = false;

	m_ivyPassengers->MakeScream(m_passengersScreaming, m_passengerVolume);
	
	// Set Sound Gain
	m_IvySound->SetSoundGain(m_ivyChannel, m_ivyConfig->m_ivyVolume);

	return m_ivyConfig->m_data_rate;
}

void MyIvy::EngineLimitsCheck(float value, float* p_threshold, float* p_seconds, bool* p_failure, float* p_start, XPLMDataRef dr_error, MyIvyResponse* p_response, std::string log_entry)
{
	

	for (int index = 0; index < IVY_ENGN_MAX; index++)
	{
		if (p_threshold[index] != 0)
		{
			if (p_threshold[index] < 0)
				value *= -1;

			if (value > p_threshold[index])
			{
				if (p_failure[index] == false)
				{
					p_failure[index] = true;
					p_start[index] = m_time;
				}
				if ((m_time - p_start[index]) > p_seconds[index])
				{
					if (p_response == NULL)
					{
						WriteLogMessage(log_entry);
						XPLMSetDatai(dr_error, 6);
					}
					else p_response->Activate(m_time);
					
				}
			}
			else
			{
				if (p_response != NULL) p_response->Deactivate(m_time);
				p_failure[index] = false;
			}

			if (p_threshold[index] < 0)
				value *= -1;
		}
	}
}

void MyIvy::ReadData()
{
	
	LoadAircraftDatarefs();

	


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////;
	//;
	//						DATAREF to Local Variables;
	//;
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////;
 
	char lba_acf_descrip[1024];
	char lba_acf_tailnumber[1024];

	XPLMGetDatab(m_s_acf_descrip, lba_acf_descrip, 0, 240);
	XPLMGetDatab(m_s_acf_tailnumber, lba_acf_tailnumber, 0, 40);

	m_ls_acf_name = lba_acf_descrip;
	m_ls_acf_descrip = lba_acf_descrip;
	m_ls_acf_descrip += lba_acf_tailnumber;

	if (m_i_fse_flying != NULL)
	{
		m_li_fse_flying = XPLMGetDatai(m_i_fse_flying);
	}

	m_lf_fuel_tot  = XPLMGetDataf(m_f_fuel_tot);
	m_lf_payload_tot = XPLMGetDataf(m_f_payload_tot);
	m_lf_weight_max = XPLMGetDataf(m_f_weight_max);

	m_lf_weight_tot = XPLMGetDataf(m_f_weight_tot);
 
	m_li_on_ground= 			XPLMGetDatai(m_i_on_ground);
	m_lf_climb_rate= 			XPLMGetDataf(m_f_climb_rate);
 
	m_lf_gear1_ratio=			XPLMGetDataf(m_f_gear1_ratio);
	m_lf_gear2_ratio=			XPLMGetDataf(m_f_gear2_ratio);
	m_lf_gear3_ratio=			XPLMGetDataf(m_f_gear3_ratio);
	m_lf_gear4_ratio=			XPLMGetDataf(m_f_gear4_ratio);
	m_lf_gear5_ratio=			XPLMGetDataf(m_f_gear5_ratio);

	m_li_gear_handle =			XPLMGetDatai(m_i_gear_handle);
	m_lf_flap_ratio =			XPLMGetDataf(m_f_flap_ratio);
 
	m_lf_ground_speed =			XPLMGetDataf(m_f_ground_speed) * 3600 / 1852;	// is m/s && we want nm/h;
	m_lf_ias= 					XPLMGetDataf(m_f_ias);
	m_lf_sun_pitch= 			XPLMGetDataf(m_f_sun_pitch);
	m_lf_airport_light= 		XPLMGetDataf(m_f_airport_light);
	m_lf_world_light_precent= 	XPLMGetDataf(m_f_world_light_precent);
	m_li_has_skid= 				XPLMGetDatai(m_i_has_skid);
	m_li_transponder_mode= 		XPLMGetDatai(m_i_transponder_mode);
	m_li_sim_ground_speed=		XPLMGetDatai(m_i_sim_ground_speed);
 
	m_li_temp_sl= 				XPLMGetDatai(m_i_temp_sl);
	m_li_dew_sl= 				XPLMGetDatai(m_i_dew_sl);
 
	m_li_landing_lights= 		XPLMGetDatai(m_i_landing_lights);
	m_li_beacon_lights= 		XPLMGetDatai(m_i_beacon_lights);
	m_li_nav_lights= 			XPLMGetDatai(m_i_nav_lights);
	m_li_strobe_lights= 		XPLMGetDatai(m_i_strobe_lights);
	m_li_taxi_lights= 			XPLMGetDatai(m_i_taxi_lights);
	m_li_cockpit_lights= 		XPLMGetDatai(m_i_cockpit_lights);
	m_lf_radio_alt= 			XPLMGetDataf(m_f_radio_alt);
	m_lf_decision_height=		XPLMGetDataf(m_f_decision_height);
	m_lf8_batter_charge= 		XPLMGetDataf(m_f8_batter_charge);
 
	m_li_battery_on = 			XPLMGetDatai(m_i_battery_on);
	m_li_gpu_on = 				XPLMGetDatai(m_i_gpu_on);
 
	m_li_flaps_overspeed = 	    XPLMGetDatai(m_i_flaps_overspeed);
	m_li_gear_overspeed = 	    XPLMGetDatai(m_i_gear_overspeed);
	m_li_aircraft_overspeed =   XPLMGetDatai(m_i_aircraft_overspeed);
	m_lf_aircraft_vne = 		XPLMGetDataf(m_f_aircraft_vne);
	m_li_stall = 				XPLMGetDatai(m_i_stall);
 
	m_li_cloud_0 = 				XPLMGetDatai(m_i_cloud_0);
	m_li_cloud_1 = 				XPLMGetDatai(m_i_cloud_1);
	m_li_cloud_2 = 				XPLMGetDatai(m_i_cloud_2);
	m_lf_visibility = 			XPLMGetDataf(m_f_visibility);
	m_li_rain = 				XPLMGetDatai(m_i_rain);
	m_li_thunder = 				XPLMGetDatai(m_i_thunder);
	m_li_turbulence = 			XPLMGetDatai(m_i_turbulence);
 
 
	m_li_batt1= 				XPLMGetDatai(m_i_batt1);
	m_li_batt2= 				XPLMGetDatai(m_i_batt2);
 
	m_li_tire1= 				XPLMGetDatai(m_i_tire1);
	m_li_tire2= 				XPLMGetDatai(m_i_tire2);
	m_li_tire3= 				XPLMGetDatai(m_i_tire3);
	m_li_tire4= 				XPLMGetDatai(m_i_tire4);
	m_li_tire5= 				XPLMGetDatai(m_i_tire5);
 
	m_li_fire1= 				XPLMGetDatai(m_i_fire1);
	m_li_fire2= 				XPLMGetDatai(m_i_fire2);
	m_li_fire3= 				XPLMGetDatai(m_i_fire3);
	m_li_fire4= 				XPLMGetDatai(m_i_fire4);
	m_li_fire5= 				XPLMGetDatai(m_i_fire5);
	m_li_fire6= 				XPLMGetDatai(m_i_fire6);
	m_li_fire7= 				XPLMGetDatai(m_i_fire7);
	m_li_fire8= 				XPLMGetDatai(m_i_fire8);
 
	m_li_flameout1= 			XPLMGetDatai(m_i_flameout1);
	m_li_flameout2= 			XPLMGetDatai(m_i_flameout2);
	m_li_flameout3=				XPLMGetDatai(m_i_flameout3);
	m_li_flameout4= 			XPLMGetDatai(m_i_flameout4);
	m_li_flameout5= 			XPLMGetDatai(m_i_flameout5);
	m_li_flameout6= 			XPLMGetDatai(m_i_flameout6);
	m_li_flameout7= 			XPLMGetDatai(m_i_flameout7);
	m_li_flameout8= 			XPLMGetDatai(m_i_flameout8);
 
 
 
	m_li_engine_failure1= 		XPLMGetDatai(m_i_engine_failure1);
	m_li_engine_failure2= 		XPLMGetDatai(m_i_engine_failure2);
	m_li_engine_failure3= 		XPLMGetDatai(m_i_engine_failure3);
	m_li_engine_failure4= 		XPLMGetDatai(m_i_engine_failure4);
	m_li_engine_failure5= 		XPLMGetDatai(m_i_engine_failure5);
	m_li_engine_failure6= 		XPLMGetDatai(m_i_engine_failure6);
	m_li_engine_failure7= 		XPLMGetDatai(m_i_engine_failure7);
	m_li_engine_failure8= 		XPLMGetDatai(m_i_engine_failure8);
 
	m_li_hot1= 					XPLMGetDatai(m_i_hot1);
	m_li_hot2= 					XPLMGetDatai(m_i_hot2);
	m_li_hot3= 					XPLMGetDatai(m_i_hot3);
	m_li_hot4= 					XPLMGetDatai(m_i_hot4);
	m_li_hot5= 					XPLMGetDatai(m_i_hot5);
	m_li_hot6= 					XPLMGetDatai(m_i_hot6);
	m_li_hot7= 					XPLMGetDatai(m_i_hot7);
	m_li_hot8= 					XPLMGetDatai(m_i_hot8);
 
	m_lf_ice_frame = 			XPLMGetDataf(m_f_ice_frame);
	m_lf_ice_pitot = 			XPLMGetDataf(m_f_ice_pitot);
	m_lf_ice_propeller = 		XPLMGetDataf(m_f_ice_propeller);
	m_lf_ice_window = 			XPLMGetDataf(m_f_ice_window);
 
	m_li_bird = 				XPLMGetDatai(m_i_bird);
 
	m_lf_g_normal= 				XPLMGetDataf(m_f_g_normal);
	m_lf_g_forward= 			XPLMGetDataf(m_f_g_forward);
	m_lf_g_side= 				XPLMGetDataf(m_f_g_side);
 
 
	m_lf_pitch= 				XPLMGetDataf(m_f_pitch);
	m_lf_roll= 					XPLMGetDataf(m_f_roll);
	m_lf_yaw= 					XPLMGetDataf(m_f_yaw);
 
 
	m_lf_cab_press= 			XPLMGetDataf(m_f_cab_press);
	m_lf_cab_rate= 				XPLMGetDataf(m_f_cab_rate);
	//cab humidity ?;
	//cab temp ?;
 
	m_lf_outside_temp1= 		XPLMGetDataf(m_f_outside_temp1);
	m_lf_outside_temp2= 		XPLMGetDataf(m_f_outside_temp2);
	m_lf_outside_temp3= 		XPLMGetDataf(m_f_outside_temp3);
 
	m_lf_baro_set = 			XPLMGetDataf(m_f_baro_set);
	m_li_baro_set = 			int(m_lf_baro_set * 100);
	m_lf_baro_sea_level = 		XPLMGetDataf(m_f_baro_sea_level);
	m_li_baro_sea_level = 		int(m_lf_baro_sea_level / 33.8639f);
	m_lf_baro_alt = 			XPLMGetDataf(m_f_baro_alt);

	float current_alt = m_lf_baro_alt + ((m_lf_baro_sea_level - m_lf_baro_set) * 1000.0f);
	float pressure_alt = current_alt + ((29.92f - m_lf_baro_sea_level) * 1000.0f);
	m_lf_density_alt = pressure_alt + 120.0f * (m_lf_outside_temp1 - (15 - (current_alt * 0.3048f * 6.5f / 1000.0f)));
 
	m_lf_wind_direction = 		XPLMGetDataf(m_f_wind_direction);
	m_lf_wind_speed_kt = 		XPLMGetDataf(m_f_wind_speed_kt) * 1.94f;
 
	m_lf_slats_1 = 				XPLMGetDataf(m_f_slats_1);
	m_lf_flaps_1 = 				XPLMGetDataf(m_f_flaps_1);
 
	m_ld_latitude = 			XPLMGetDatad(m_d_latitude);
	m_ld_longitude = 			XPLMGetDatad(m_d_longitude);
 
	m_li_nonsmoking =           XPLMGetDatai(m_i_nonsmoking);
	m_li_fastenseatbelts =      XPLMGetDatai(m_i_fastenseatbelt);

	m_lf_hsi_hdef_dot = XPLMGetDataf(m_f_hsi_hdef_dot);
	m_lf_hsi_vdef_dot = XPLMGetDataf(m_f_hsi_vdef_dot);
	m_li_hsi_source_select = XPLMGetDatai(m_i_hsi_source_select);

	m_li_hsi_display_h = XPLMGetDatai(m_i_hsi_display_h);
	m_li_hsi_display_v = XPLMGetDatai(m_i_hsi_display_v);

	m_lf_acf_heading = XPLMGetDataf(m_f_acf_heading);
	

	m_lf_speed_brake = XPLMGetDataf(m_f_speed_brake);

	XPLMGetDatavi(m_ia_engine_start, m_lia_engine_start, 0, 8);
	XPLMGetDatavi(m_ia_engine_running, m_lia_engine_running, 0, 8);
	XPLMGetDatavi(m_ia_generator_running, m_lia_generator_running, 0, 8);

	
	m_lf_vertical_speed = XPLMGetDataf(m_f_vertical_speed);


	m_li_apu_starter_switch = XPLMGetDatai(m_i_apu_starter_switch); 
	m_li_avionics = XPLMGetDatai(m_i_avionics);


	

	m_lf_throttle_ratio = XPLMGetDataf(m_f_throttle_ratio);
	
	XPLMGetDatavf(m_f_n1_percent, m_lf_n1_percent, 0, 8);

	XPLMGetDatavf(m_f_n2_percent, m_lf_n2_percent, 0, 8);
	XPLMGetDatavf(m_f_egt_c, m_lf_egt_c, 0, 8);
	XPLMGetDatavf(m_f_fuel_flow, m_lf_fuel_flow, 0, 8);
	
	//m_lf_n1_percent = XPLMGetDataf(m_f_n1_percent);
	//m_lf_n2_percent = XPLMGetDataf(m_f_n2_percent);
	
 
	m_ivyAircraft->UpdateData();
 
	m_data_read_valid = true;

	// Store Graph Data

	std::memmove(m_graph_altitude, m_graph_altitude + 1, sizeof(float) * (MAX_GRAPH_DATA - 1));
	std::memmove(m_graph_climb, m_graph_climb + 1, sizeof(float) * (MAX_GRAPH_DATA - 1));
	std::memmove(m_graph_g_vert, m_graph_g_vert + 1, sizeof(float) * (MAX_GRAPH_DATA - 1));
	std::memmove(m_graph_g_horiz, m_graph_g_horiz + 1, sizeof(float) * (MAX_GRAPH_DATA - 1));
	std::memmove(m_graph_g_side, m_graph_g_side + 1, sizeof(float) * (MAX_GRAPH_DATA - 1));



	m_graph_altitude[MAX_GRAPH_DATA-1] = m_lf_baro_alt;
	m_graph_climb[MAX_GRAPH_DATA-1] = m_lf_climb_rate;
	m_graph_g_vert[MAX_GRAPH_DATA-1] = m_lf_g_normal;
	m_graph_g_horiz[MAX_GRAPH_DATA-1] = m_lf_g_forward;
	m_graph_g_side[MAX_GRAPH_DATA-1] = m_lf_g_side;

	XPLMGetDatavf(m_f_cyclic_ail, m_lf_cyclic_ail, 0, 1);
	XPLMGetDatavf(m_f_cyclic_elev, m_lf_cyclic_elev, 0, 1);
	XPLMGetDatavf(m_fa_torque, m_lfa_torque, 0, 8);
	XPLMGetDatavf(m_fa_itt, m_lfa_itt, 0, 8);

	m_li_pitot_heat = XPLMGetDatai(m_i_pitot_heat);
	XPLMGetDatavi(m_ia_eng_anti_ice, m_lia_eng_anti_ice, 0, 8);

	if ((m_ivyAircraft->m_is_206B3 == true) && (m_i_206_br_adf != NULL))
	{
		m_li_206_br_adf = XPLMGetDatai(m_i_206_br_adf);
		m_li_206_br_avionics = XPLMGetDatai(m_i_206_br_avionics);
		m_li_206_br_cable = XPLMGetDatai(m_i_206_br_cable);
		m_li_206_br_cargo_hook = XPLMGetDatai(m_i_206_br_cargo_hook);
		m_li_206_br_caution_light = XPLMGetDatai(m_i_206_br_caution_light);
		m_li_206_br_cockpit_light = XPLMGetDatai(m_i_206_br_cockpit_light);
		m_li_206_br_anitice = XPLMGetDatai(m_i_206_br_anitice);
		m_li_206_br_xmsn = XPLMGetDatai(m_i_206_br_xmsn);
		m_li_206_br_floats = XPLMGetDatai(m_i_206_br_floats);
		m_li_206_br_boost_f = XPLMGetDatai(m_i_206_br_boost_f);
		m_li_206_br_boost_a = XPLMGetDatai(m_i_206_br_boost_a);
		m_li_206_br_fuel_press = XPLMGetDatai(m_i_206_br_fuel_press);
		m_li_206_br_fuel_valve = XPLMGetDatai(m_i_206_br_fuel_valve);
		m_li_206_br_gen_field = XPLMGetDatai(m_i_206_br_gen_field);
		m_li_206_br_gen_reset = XPLMGetDatai(m_i_206_br_gen_reset);
		m_li_206_br_gov = XPLMGetDatai(m_i_206_br_gov);
		m_li_206_br_gps = XPLMGetDatai(m_i_206_br_gps);
		m_li_206_br_hmike = XPLMGetDatai(m_i_206_br_hmike);
		m_li_206_br_heat = XPLMGetDatai(m_i_206_br_heat);
		m_li_206_br_hoist = XPLMGetDatai(m_i_206_br_hoist);
		m_li_206_br_htr_cont = XPLMGetDatai(m_i_206_br_htr_cont);
		m_li_206_br_htr_pwr = XPLMGetDatai(m_i_206_br_htr_pwr);
		m_li_206_br_htr_hyd_boost = XPLMGetDatai(m_i_206_br_htr_hyd_boost);
		m_li_206_br_htr_ics = XPLMGetDatai(m_i_206_br_htr_ics);
		m_li_206_br_htr_ign = XPLMGetDatai(m_i_206_br_htr_ign);
		m_li_206_br_htr_inst_light = XPLMGetDatai(m_i_206_br_htr_inst_light);
		m_li_206_br_htr_ldg_light = XPLMGetDatai(m_i_206_br_htr_ldg_light);
		m_li_206_br_htr_sas = XPLMGetDatai(m_i_206_br_htr_sas);
		m_li_206_br_htr_starter = XPLMGetDatai(m_i_206_br_htr_starter);
		m_li_206_br_htr_tot_ind = XPLMGetDatai(m_i_206_br_htr_tot_ind);
		m_li_206_br_htr_turn = XPLMGetDatai(m_i_206_br_htr_turn);
		m_li_206_br_htr_vhf = XPLMGetDatai(m_i_206_br_htr_vhf);
		m_li_206_br_htr_xpdf = XPLMGetDatai(m_i_206_br_htr_xpdf);

		m_lf_206_doors = XPLMGetDataf(m_f_206_doors);
		m_lf_206_rotor_rpm = XPLMGetDataf(m_f_206_rotor_rpm);
		m_lf_206_rotor_rpm = m_lf_206_rotor_rpm / 4.0f;
		m_li_206_hydraulics = XPLMGetDatai(m_i_206_hydraulics);
		m_lf_206_throttle = XPLMGetDataf(m_f_206_throttle);
		m_li_206_caut_test = XPLMGetDatai(m_i_206_caut_test);
		m_li_206_tot_test = XPLMGetDatai(m_i_206_tot_test);

		m_lf_206_oil_press = XPLMGetDataf(m_f_206_oil_press);
		m_lf_206_oil_temp = XPLMGetDataf(m_f_206_oil_temp);
		m_li_206_floats_active = XPLMGetDatai(m_i_206_floats_active);
		m_li_206_ldg_lights_switch = XPLMGetDatai(m_i_206_ldg_lights_switch);

	}

	if ((m_ivyAircraft->m_is_206L3 == true) && (m_i_206L3_br_adf != NULL))
	{
		m_li_206L3_br_adf = XPLMGetDatai(m_i_206L3_br_adf);
		m_li_206L3_br_avionics = XPLMGetDatai(m_i_206L3_br_avionics);
		m_li_206L3_br_cable = XPLMGetDatai(m_i_206L3_br_cable);
		m_li_206L3_br_cargo_hook = XPLMGetDatai(m_i_206L3_br_cargo_hook);
		m_li_206L3_br_caution_light = XPLMGetDatai(m_i_206L3_br_caution_light);
		m_li_206L3_br_cockpit_light = XPLMGetDatai(m_i_206L3_br_cockpit_light);
		m_li_206L3_br_anitice = XPLMGetDatai(m_i_206L3_br_anitice);
		m_li_206L3_br_xmsn = XPLMGetDatai(m_i_206L3_br_xmsn);
		m_li_206L3_br_floats = XPLMGetDatai(m_i_206L3_br_floats);
		m_li_206L3_br_boost_f = XPLMGetDatai(m_i_206L3_br_boost_f);
		m_li_206L3_br_boost_a = XPLMGetDatai(m_i_206L3_br_boost_a);
		m_li_206L3_br_fuel_press = XPLMGetDatai(m_i_206L3_br_fuel_press);
		m_li_206L3_br_fuel_valve = XPLMGetDatai(m_i_206L3_br_fuel_valve);
		m_li_206L3_br_gen_field = XPLMGetDatai(m_i_206L3_br_gen_field);
		m_li_206L3_br_gen_reset = XPLMGetDatai(m_i_206L3_br_gen_reset);
		m_li_206L3_br_gov = XPLMGetDatai(m_i_206L3_br_gov);
		m_li_206L3_br_gps = XPLMGetDatai(m_i_206L3_br_gps);
		m_li_206L3_br_hmike = XPLMGetDatai(m_i_206L3_br_hmike);
		m_li_206L3_br_heat = XPLMGetDatai(m_i_206L3_br_heat);
		m_li_206L3_br_hoist = XPLMGetDatai(m_i_206L3_br_hoist);
		m_li_206L3_br_htr_cont = XPLMGetDatai(m_i_206L3_br_htr_cont);
		m_li_206L3_br_htr_pwr = XPLMGetDatai(m_i_206L3_br_htr_pwr);
		m_li_206L3_br_htr_hyd_boost = XPLMGetDatai(m_i_206L3_br_htr_hyd_boost);
		m_li_206L3_br_htr_ics = XPLMGetDatai(m_i_206L3_br_htr_ics);
		m_li_206L3_br_htr_ign = XPLMGetDatai(m_i_206L3_br_htr_ign);
		m_li_206L3_br_htr_inst_light = XPLMGetDatai(m_i_206L3_br_htr_inst_light);
		m_li_206L3_br_htr_ldg_light = XPLMGetDatai(m_i_206L3_br_htr_ldg_light);
		m_li_206L3_br_htr_sas = XPLMGetDatai(m_i_206L3_br_htr_sas);
		m_li_206L3_br_htr_starter = XPLMGetDatai(m_i_206L3_br_htr_starter);
		m_li_206L3_br_htr_tot_ind = XPLMGetDatai(m_i_206L3_br_htr_tot_ind);
		m_li_206L3_br_htr_turn = XPLMGetDatai(m_i_206L3_br_htr_turn);
		m_li_206L3_br_htr_vhf = XPLMGetDatai(m_i_206L3_br_htr_vhf);
		m_li_206L3_br_htr_xpdf = XPLMGetDatai(m_i_206L3_br_htr_xpdf);

		m_lf_206L3_doors = XPLMGetDataf(m_f_206L3_doors);
		m_lf_206L3_rotor_rpm = XPLMGetDataf(m_f_206L3_rotor_rpm);
		m_lf_206L3_rotor_rpm = m_lf_206L3_rotor_rpm / 4.0f;
		m_li_206L3_hydraulics = XPLMGetDatai(m_i_206L3_hydraulics);
		m_lf_206L3_throttle = XPLMGetDataf(m_f_206L3_throttle);
		m_li_206L3_caut_test = XPLMGetDatai(m_i_206L3_caut_test);
		m_li_206L3_tot_test = XPLMGetDatai(m_i_206L3_tot_test);

		m_lf_206L3_oil_press = XPLMGetDataf(m_f_206L3_oil_press);
		m_lf_206L3_oil_temp = XPLMGetDataf(m_f_206L3_oil_temp);
		m_li_206L3_floats_active = XPLMGetDatai(m_i_206L3_floats_active);
		m_li_206L3_ldg_lights_switch = XPLMGetDatai(m_i_206L3_ldg_lights_switch);

	}
}

void MyIvy::WriteErrorLog()
{
	boost::property_tree::ptree pt;

	for (int index = 0; index < m_ivy_flighttime_history.size(); index++)
	{
		std::string flight = "flight_" + std::to_string(index);

		std::string node = flight + "." + "flight_time";
		pt.put(node, m_ivy_flighttime_history[index]);

		for (auto ivy_object : (*m_ivy_object_list))
		{
			node = flight + "." + ivy_object->m_event_name;
			if (index < ivy_object->m_error_history.size()) 
				pt.put(node, ivy_object->m_error_history[index]);
		}
	}

	boost::property_tree::xml_writer_settings<char> settings(' ', 4);
	boost::property_tree::write_xml(m_ivyConfig->m_errorlog_path, pt, std::locale(), settings);

	CalculateStatistics();

}

void MyIvy::ReadErrorLog()
{
	boost::property_tree::ptree pt;
	try
	{
		boost::property_tree::read_xml(m_ivyConfig->m_errorlog_path, pt);
	}
	catch (...)
	{
		ivy_output_file << "Could not read error log file" << std::endl;
		ivy_output_file.flush();
		return;
	}

	bool log_eof = false;
	int index = 0;

	while (!log_eof)
	{
		std::string flight = "flight_" + std::to_string(index);
		std::string node = flight + "." + "flight_time";

		int value = 0;

		try { value = pt.get<int>(node); }
		catch (...) { log_eof = true; }

		if (!log_eof)
		{
			m_ivy_flighttime_history.push_back(value);

			for (auto ivy_object : (*m_ivy_object_list))
			{
				node = flight + "." + ivy_object->m_event_name;

				try { value = pt.get<int>(node); }
				catch (...) { value = 0; }

				ivy_object->m_error_history.push_back(value);
			}
		}
		index++;

	}

	CalculateStatistics();

}

void MyIvy::CalculateStatistics()
{
	m_aoc_list_low.clear();
	m_aoc_list_med.clear();
	m_aoc_list_high.clear();

	for (auto ivy_response : *m_ivy_object_list)
	{
		ivy_response->CalcAOCCount(m_ivyConfig->m_aoc_count_total);

		if (ivy_response->m_aoc_count >= m_ivyConfig->m_aoc_level_high)			m_aoc_list_high.push_back(ivy_response);
		else if (ivy_response->m_aoc_count >= m_ivyConfig->m_aoc_level_med)		m_aoc_list_med.push_back(ivy_response);
		else if (ivy_response->m_aoc_count >= m_ivyConfig->m_aoc_level_low)		m_aoc_list_low.push_back(ivy_response);
	}


	//for (auto item : m_ivy_error_count_history)
	//	item = 0;
	for (int index = 0; index < MAX_ERROR_HISTORY; index++)
		m_ivy_error_count_history[index] = 0;
	

	m_ivy_error_count_history_max = 0;

	for (int index = 0; index < m_ivy_flighttime_history.size(); index++)
	{
		int flight_errors = 0;

		for (auto ivy_object : (*m_ivy_object_list))
		{
			if (ivy_object->m_is_error > 0)
				if (index < ivy_object->m_error_history.size())
					flight_errors += ivy_object->m_error_history[index];
		}

		if (flight_errors >= MAX_ERROR_HISTORY)
			flight_errors = MAX_ERROR_HISTORY - 1;

		m_ivy_error_count_history[flight_errors]++;

		if (m_ivy_error_count_history_max < m_ivy_error_count_history[flight_errors])
			m_ivy_error_count_history_max = m_ivy_error_count_history[flight_errors];
	}


}

// ToDo: Load Datarefs First!
void MyIvy::StartupSwitches()
{
	//LoadAircraftDatarefs();

	if ((m_ivyAircraft->m_is_206B3 == true) && (m_lia_engine_running[0] == 0))
	{
		std::vector<XPLMDataRef> switches;
		std::vector<XPLMDataRef> breakers;

		breakers.push_back(m_i_206_br_adf);
		//breakers.push_back(m_i_206_br_avionics);
		breakers.push_back(m_i_206_br_cable);
		breakers.push_back(m_i_206_br_cargo_hook);
		breakers.push_back(m_i_206_br_caution_light);
		breakers.push_back(m_i_206_br_cockpit_light);
		breakers.push_back(m_i_206_br_anitice);
		breakers.push_back(m_i_206_br_xmsn);
		breakers.push_back(m_i_206_br_floats);
		//breakers.push_back(m_i_206_br_boost_f);
		//breakers.push_back(m_i_206_br_boost_a);
		breakers.push_back(m_i_206_br_fuel_press);
		breakers.push_back(m_i_206_br_fuel_valve);
		breakers.push_back(m_i_206_br_gen_field);
		breakers.push_back(m_i_206_br_gen_reset);
		breakers.push_back(m_i_206_br_gov);
		breakers.push_back(m_i_206_br_gps);
		breakers.push_back(m_i_206_br_hmike);
		breakers.push_back(m_i_206_br_hoist);
		breakers.push_back(m_i_206_br_htr_cont);
		breakers.push_back(m_i_206_br_htr_pwr);
		breakers.push_back(m_i_206_br_htr_hyd_boost);
		breakers.push_back(m_i_206_br_htr_ics);
		breakers.push_back(m_i_206_br_htr_ign);
		breakers.push_back(m_i_206_br_htr_inst_light);
		breakers.push_back(m_i_206_br_htr_ldg_light);
		breakers.push_back(m_i_206_br_htr_sas);
		breakers.push_back(m_i_206_br_htr_starter);
		breakers.push_back(m_i_206_br_htr_tot_ind);
		breakers.push_back(m_i_206_br_htr_turn);
		breakers.push_back(m_i_206_br_htr_vhf);
		breakers.push_back(m_i_206_br_htr_xpdf);
		
		
		switches.push_back(m_i_beacon_lights);
		switches.push_back(m_i_nav_lights);
		switches.push_back(m_i_206_br_heat);

		switches.push_back(m_ia_generator_running);
		switches.push_back(m_i_pitot_heat);
		switches.push_back(m_ia_eng_anti_ice);
		switches.push_back(m_i_avionics);
		switches.push_back(m_i_206_hydraulics);
		switches.push_back(m_i_206_floats_active);
		switches.push_back(m_i_206_ldg_lights_switch);

		int number_switches = 0;

		number_switches = rand() % 3;

		for (int count = 0; count <= number_switches; count++)
		{
			int type = rand() % 2;

			if (type == 0)
			{
				int breaker_index = rand() % breakers.size();
				XPLMSetDatai(breakers[breaker_index], 1);
			}
			else
			{
				int value = 1;
				int switch_index = rand() % switches.size();
				if      (switches[switch_index] == m_ia_generator_running) 
					XPLMSetDatavi(switches[switch_index], &value, 0, 1);
				else if (switches[switch_index] == m_ia_eng_anti_ice)      
					XPLMSetDatavi(switches[switch_index], &value, 0, 1);
				else													   
					XPLMSetDatai(switches[switch_index], 1);
			}
		}


	}

	if ((m_ivyAircraft->m_is_206L3 == true) && (m_lia_engine_running[0] == 0))
	{
		std::vector<XPLMDataRef> switches;
		std::vector<XPLMDataRef> breakers;

		breakers.push_back(m_i_206L3_br_adf);
		//breakers.push_back(m_i_206L3_br_avionics);
		breakers.push_back(m_i_206L3_br_cable);
		breakers.push_back(m_i_206L3_br_cargo_hook);
		breakers.push_back(m_i_206L3_br_caution_light);
		breakers.push_back(m_i_206L3_br_cockpit_light);
		breakers.push_back(m_i_206L3_br_anitice);
		breakers.push_back(m_i_206L3_br_xmsn);
		breakers.push_back(m_i_206L3_br_floats);
		//breakers.push_back(m_i_206L3_br_boost_f);
		//breakers.push_back(m_i_206L3_br_boost_a);
		breakers.push_back(m_i_206L3_br_fuel_press);
		breakers.push_back(m_i_206L3_br_fuel_valve);
		breakers.push_back(m_i_206L3_br_gen_field);
		breakers.push_back(m_i_206L3_br_gen_reset);
		breakers.push_back(m_i_206L3_br_gov);
		breakers.push_back(m_i_206L3_br_gps);
		breakers.push_back(m_i_206L3_br_hmike);
		breakers.push_back(m_i_206L3_br_hoist);
		breakers.push_back(m_i_206L3_br_htr_cont);
		breakers.push_back(m_i_206L3_br_htr_pwr);
		breakers.push_back(m_i_206L3_br_htr_hyd_boost);
		breakers.push_back(m_i_206L3_br_htr_ics);
		breakers.push_back(m_i_206L3_br_htr_ign);
		breakers.push_back(m_i_206L3_br_htr_inst_light);
		breakers.push_back(m_i_206L3_br_htr_ldg_light);
		breakers.push_back(m_i_206L3_br_htr_sas);
		breakers.push_back(m_i_206L3_br_htr_starter);
		breakers.push_back(m_i_206L3_br_htr_tot_ind);
		breakers.push_back(m_i_206L3_br_htr_turn);
		breakers.push_back(m_i_206L3_br_htr_vhf);
		breakers.push_back(m_i_206L3_br_htr_xpdf);


		switches.push_back(m_i_beacon_lights);
		switches.push_back(m_i_nav_lights);
		switches.push_back(m_i_206L3_br_heat);

		switches.push_back(m_ia_generator_running);
		switches.push_back(m_i_pitot_heat);
		switches.push_back(m_ia_eng_anti_ice);
		switches.push_back(m_i_avionics);
		switches.push_back(m_i_206L3_hydraulics);
		switches.push_back(m_i_206L3_floats_active);
		switches.push_back(m_i_206L3_ldg_lights_switch);

		int number_switches = 0;

		number_switches = rand() % 3;

		for (int count = 0; count <= number_switches; count++)
		{
			int type = rand() % 2;

			if (type == 0)
			{
				int breaker_index = rand() % breakers.size();
				XPLMSetDatai(breakers[breaker_index], 1);
			}
			else
			{
				int value = 1;
				int switch_index = rand() % switches.size();
				if (switches[switch_index] == m_ia_generator_running)
					XPLMSetDatavi(switches[switch_index], &value, 0, 1);
				else if (switches[switch_index] == m_ia_eng_anti_ice)
					XPLMSetDatavi(switches[switch_index], &value, 0, 1);
				else
					XPLMSetDatai(switches[switch_index], 1);
			}
		}


	}
}

void MyIvy::WriteLogMessage(std::string message)
{
	/*m_ivyLogWriter->m_error_string = message;
	m_ivyLogWriter->Activate(m_time);
	m_ivyLogWriter->Deactivate(m_time);*/

	std::string text = "";

	std::string s_value = std::to_string((int)(m_time / 3600));
	if (s_value.size() < 2) s_value = "0" + s_value;
	text += s_value + ":";

	s_value = std::to_string((int)((((int)m_time) % 3600) / 60));
	if (s_value.size() < 2) s_value = "0" + s_value;
	text += s_value + ":";

	s_value = std::to_string(((int)m_time) % 60);
	if (s_value.size() < 2) s_value = "0" + s_value;
	text += s_value + " - Error: " + message;

	m_error_list.push_back(text);
}

void MyIvy::LoadAircraftDatarefs()
{
	if ((m_ivyAircraft->m_is_206B3 == true) && (m_i_206_br_adf == NULL))
	{
		m_i_206_br_adf = XPLMFindDataRef("206B3/adf/br");
		m_i_206_br_avionics = XPLMFindDataRef("206B3/avionics/br");
		m_i_206_br_cable = XPLMFindDataRef("206B3/cable/br");
		m_i_206_br_cargo_hook = XPLMFindDataRef("206B3/cargo/hook/br");
		m_i_206_br_caution_light = XPLMFindDataRef("206B3/caution/lt/br");
		m_i_206_br_cockpit_light = XPLMFindDataRef("206B3/cockpit/lt/br");
		m_i_206_br_anitice = XPLMFindDataRef("206B3/eng/antiice/br");
		m_i_206_br_xmsn = XPLMFindDataRef("206B3/eng/xmsn/br");
		m_i_206_br_floats = XPLMFindDataRef("206B3/floats/br");
		m_i_206_br_boost_f = XPLMFindDataRef("206B3/fuel/boost/fwd/br");
		m_i_206_br_boost_a = XPLMFindDataRef("206B3/fuel/boost/aft/br");
		m_i_206_br_fuel_press = XPLMFindDataRef("206B3/fuel/press/br");
		m_i_206_br_fuel_valve = XPLMFindDataRef("206B3/fuel/valve/br");
		m_i_206_br_gen_field = XPLMFindDataRef("206B3/gen_field/br");
		m_i_206_br_gen_reset = XPLMFindDataRef("206B3/gen_reset/br");
		m_i_206_br_gov = XPLMFindDataRef("206B3/gov/cont/br");
		m_i_206_br_gps = XPLMFindDataRef("206B3/gps/br");
		m_i_206_br_hmike = XPLMFindDataRef("206B3/hand_mike/br");
		m_i_206_br_heat = XPLMFindDataRef("206B3/heat/br");
		m_i_206_br_hoist = XPLMFindDataRef("206B3/hoist/br");
		m_i_206_br_htr_cont = XPLMFindDataRef("206B3/htr_cont/br");
		m_i_206_br_htr_pwr = XPLMFindDataRef("206B3/htr_pwr/br");
		m_i_206_br_htr_hyd_boost = XPLMFindDataRef("206B3/hyd/boost/br");
		m_i_206_br_htr_ics = XPLMFindDataRef("206B3/ics/br");
		m_i_206_br_htr_ign = XPLMFindDataRef("206B3/ign/eng/br");
		m_i_206_br_htr_inst_light = XPLMFindDataRef("206B3/inst/lt/br");
		m_i_206_br_htr_ldg_light = XPLMFindDataRef("206B3/ldg/lt/br");
		m_i_206_br_htr_sas = XPLMFindDataRef("206B3/sas_cont/br");
		m_i_206_br_htr_starter = XPLMFindDataRef("206B3/start/eng/br");
		m_i_206_br_htr_tot_ind = XPLMFindDataRef("206B3/tot/ind/br");
		m_i_206_br_htr_turn = XPLMFindDataRef("206B3/turn/br");
		m_i_206_br_htr_vhf = XPLMFindDataRef("206B3/vhf_1/br");
		m_i_206_br_htr_xpdf = XPLMFindDataRef("206B3/xpdr/br");

		m_f_206_doors = XPLMFindDataRef("206B3/doors_fmod");
		m_f_206_rotor_rpm = XPLMFindDataRef("206B3/rotor_rpm_0");
		m_i_206_hydraulics = XPLMFindDataRef("206B3/hydraulics/onoff");
		m_f_206_throttle = XPLMFindDataRef("206B3/throttle");

		m_f_206_oil_press = XPLMFindDataRef("206B3/xmsn/press");
		m_f_206_oil_temp = XPLMFindDataRef("206B3/xmsn/temp");
		m_i_206_caut_test = XPLMFindDataRef("206B3/caut/test_annun");
		m_i_206_tot_test = XPLMFindDataRef("206B3/tot/test");

		m_i_206_floats_active = XPLMFindDataRef("206B3/acc/floats_active");
		m_i_206_ldg_lights_switch = XPLMFindDataRef("206B3/ldglts/sw");
		m_f_206_fmod = XPLMFindDataRef("206/fmod_timer");

		StartupSwitches();
	}

	float fmod = 0;

	if (m_i_206_br_adf != NULL)
	{
		fmod = XPLMGetDataf(m_f_206_fmod);
		if ((fmod > 5) && m_206_switches_set == false)
		{
			StartupSwitches();
			m_206_switches_set = true;
		}
	}

	if ((m_ivyAircraft->m_is_206L3 == true) && (m_i_206L3_br_adf == NULL))
	{
		m_i_206L3_br_adf = XPLMFindDataRef("206L3/adf/br");
		m_i_206L3_br_avionics = XPLMFindDataRef("206L3/avionics/br");
		m_i_206L3_br_cable = XPLMFindDataRef("206L3/cable/br");
		m_i_206L3_br_cargo_hook = XPLMFindDataRef("206L3/cargo/hook/br");
		m_i_206L3_br_caution_light = XPLMFindDataRef("206L3/caution/lt/br");
		m_i_206L3_br_cockpit_light = XPLMFindDataRef("206L3/cockpit/lt/br");
		m_i_206L3_br_anitice = XPLMFindDataRef("206L3/eng/antiice/br");
		m_i_206L3_br_xmsn = XPLMFindDataRef("206L3/eng/xmsn/br");
		m_i_206L3_br_floats = XPLMFindDataRef("206L3/floats/br");
		m_i_206L3_br_boost_f = XPLMFindDataRef("206L3/fuel/boost/fwd/br");
		m_i_206L3_br_boost_a = XPLMFindDataRef("206L3/fuel/boost/aft/br");
		m_i_206L3_br_fuel_press = XPLMFindDataRef("206L3/fuel/press/br");
		m_i_206L3_br_fuel_valve = XPLMFindDataRef("206L3/fuel/valve/br");
		m_i_206L3_br_gen_field = XPLMFindDataRef("206L3/gen_field/br");
		m_i_206L3_br_gen_reset = XPLMFindDataRef("206L3/gen_reset/br");
		m_i_206L3_br_gov = XPLMFindDataRef("206L3/gov/cont/br");
		m_i_206L3_br_gps = XPLMFindDataRef("206L3/gps/br");
		m_i_206L3_br_hmike = XPLMFindDataRef("206L3/hand_mike/br");
		m_i_206L3_br_heat = XPLMFindDataRef("206L3/heat/br");
		m_i_206L3_br_hoist = XPLMFindDataRef("206L3/hoist/br");
		m_i_206L3_br_htr_cont = XPLMFindDataRef("206L3/htr_cont/br");
		m_i_206L3_br_htr_pwr = XPLMFindDataRef("206L3/htr_pwr/br");
		m_i_206L3_br_htr_hyd_boost = XPLMFindDataRef("206L3/hyd/boost/br");
		m_i_206L3_br_htr_ics = XPLMFindDataRef("206L3/ics/br");
		m_i_206L3_br_htr_ign = XPLMFindDataRef("206L3/ign/eng/br");
		m_i_206L3_br_htr_inst_light = XPLMFindDataRef("206L3/inst/lt/br");
		m_i_206L3_br_htr_ldg_light = XPLMFindDataRef("206L3/ldg/lt/br");
		m_i_206L3_br_htr_sas = XPLMFindDataRef("206L3/sas_cont/br");
		m_i_206L3_br_htr_starter = XPLMFindDataRef("206L3/start/eng/br");
		m_i_206L3_br_htr_tot_ind = XPLMFindDataRef("206L3/tot/ind/br");
		m_i_206L3_br_htr_turn = XPLMFindDataRef("206L3/turn/br");
		m_i_206L3_br_htr_vhf = XPLMFindDataRef("206L3/vhf_1/br");
		m_i_206L3_br_htr_xpdf = XPLMFindDataRef("206L3/xpdr/br");

		m_f_206L3_doors = XPLMFindDataRef("206L3/doors_fmod");
		m_f_206L3_rotor_rpm = XPLMFindDataRef("206L3/rotor_rpm_0");
		m_i_206L3_hydraulics = XPLMFindDataRef("206L3/hydraulics/onoff");
		m_f_206L3_throttle = XPLMFindDataRef("206L3/throttle");

		m_f_206L3_oil_press = XPLMFindDataRef("206L3/xmsn/press");
		m_f_206L3_oil_temp = XPLMFindDataRef("206L3/xmsn/temp");
		m_i_206L3_caut_test = XPLMFindDataRef("206L3/caut/test_annun");
		m_i_206L3_tot_test = XPLMFindDataRef("206L3/tot/test");

		m_i_206L3_floats_active = XPLMFindDataRef("206L3/acc/floats_active");
		m_i_206L3_ldg_lights_switch = XPLMFindDataRef("206L3/ldglts/sw");
		m_f_206L3_fmod = XPLMFindDataRef("206L3/fmod_timer");

		StartupSwitches();
	}

	fmod = 0;

	if (m_i_206L3_br_adf != NULL)
	{
		fmod = XPLMGetDataf(m_f_206L3_fmod);
		if ((fmod > 5) && m_206L3_switches_set == false)
		{
			StartupSwitches();
			m_206L3_switches_set = true;
		}
	}
}
