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
#include "MyIvyResponse.h"
#include "MyIvyConfiguration.h"
#include "MyIvyConfigAircraft.h"
#include "IvyPassengers.h"
#include "IvyLandingDetection.h"
#include "IvyImguiWidget.h"
#include "IvyHVCheck.h"

/*
ToDo

Boarding is completed
Fuel value
cargo

Flight cancelled

Numbers better

Imgui

Graphics Landing:

Sinkrate
Forward G-Forces
Sideways G-Forces



*/



class MyIvy
{
public:

	std::queue<FMOD_SOUND*> *m_play_mp3_queue = NULL;
	std::vector<MyIvyResponse*> *m_ivy_object_list = NULL;
	std::vector<IvyLandingDetection*> *m_ivy_landing_list = NULL;
	std::vector<MyIvyConfigAircraft*> *m_ivy_aircraft_list = NULL;

	std::vector<int> m_ivy_flighttime_history;
	std::vector<MyIvyResponse*> m_aoc_list_low;
	std::vector<MyIvyResponse*> m_aoc_list_med;
	std::vector<MyIvyResponse*> m_aoc_list_high;

	float m_ivy_error_count_history[MAX_ERROR_HISTORY];
	float m_ivy_error_count_history_max = 0;

	float m_graph_altitude[MAX_GRAPH_DATA];
	float m_graph_climb[MAX_GRAPH_DATA];
	float m_graph_g_vert[MAX_GRAPH_DATA];
	float m_graph_g_horiz[MAX_GRAPH_DATA];
	float m_graph_g_side[MAX_GRAPH_DATA];

	std::shared_ptr<IvyImguiWidget> imguiPtr;

	MyIvyConfiguration *m_ivyConfig = NULL;
	MyIvyConfigAircraft *m_ivyAircraft = NULL;
	IvyPassengers *m_ivyPassengers = NULL;
	IvyHVCheck m_ivyHVCheck;

	//char *m_error_record[1024];
	//int m_error_record_count = 0;

	std::vector<std::string> m_error_list;

	std::string m_ds = "\\";

	//FILE *m_logbook_file = 0;

	XPSound * m_IvySound = NULL;
	FMOD_CHANNELGROUP* m_ivyChannel = 0;
	FMOD_CHANNELGROUP* m_ivyChannelPassengersScreaming = 0;
	FMOD_CHANNELGROUP* m_ivyChannelPassengersGeneral = 0;

	bool m_ivy_enabled = true;

	bool m_data_read_valid = false;

	bool m_initialized = false;

	int m_aircraft_loaded = 0;
	int m_plugin_enabled = 0;
	int m_deact_queue = 0;

	bool m_no_aircraft = true;
	int m_draw_window = 0;
	int m_MenuVSpeedsShow = 0;

	int m_menuindex_ivy_enable = 0;
	int m_menuindex_log_enable = 0;

	float m_time = 0;
	int m_landing_detected = 0;
	int m_landing_rated = 0;
	int m_landing_bounces = 0;
	float m_landing_g_normal = 0;
	float m_landing_sink_rate = 0;
	int m_aircraft_crashed = 0;
	int m_pressure_said = 0;
	int m_non_smoking_old = 0;
	int m_non_smoking_event = -100; // Needed for Startup with Non-Smoking enabled.

	std::string	m_airport_departure = "NONE";
	std::string	m_airport_departure_temp = "NONE";
	std::string	m_airport_arrival = "NONE";
	std::string m_ls_acf_descrip = "";
	std::string m_ls_acf_name = "";
	
	float m_time_departure = 0;



	float m_f_g_normal_max = 1;
	float m_f_g_side_max = 0;
	float m_f_g_forward_max = 0;

	float m_f_g_normal_min = 1;
	float m_f_g_side_min = 0;
	float m_f_g_forward_min = 0;

	float m_f_g_normal_delta = 0;
	float m_f_g_side_delta = 0;
	float m_f_g_forward_delta = 0;

	float m_f_g_normal_old = 1;
	float m_f_g_side_old = 0;
	float m_f_g_forward_old = 0;

	int m_li_on_ground_old = 1;
	//m_Clicked = 0;

	float m_cab_press_rate = 0.0;
	float m_cab_press_old = 0.0;

	int m_show_output = 1;

	bool m_passengersScreaming = false;
	float m_passengerVolume = 0.3;
	

	std::list<float> n1_values;
	float m_n1_deviation = 0;

	// Logbook:

	int m_logbook_lines = 0;
	int m_logbook_index = 0;

	std::vector<XPWidgetID> m_text_field_array;
	std::vector<std::string> m_logbook_entries;

	//////////////////////////////////////////////////////////////////////////////////////////////////
	// Sound Files

	std::vector<FMOD_SOUND*> m_s_low;
	std::vector<FMOD_SOUND*> m_s_hundreds;
	std::vector<FMOD_SOUND*> m_s_hrm;

	FMOD_SOUND* m_s_million = 0;
	FMOD_SOUND* m_s_thousand = 0;

	FMOD_SOUND* m_s_error_zero = 0;
	FMOD_SOUND* m_s_error_good = 0;
	FMOD_SOUND* m_s_error_bad = 0;
	FMOD_SOUND* m_s_error_xbad = 0;
	FMOD_SOUND* m_s_error_a = 0;
	FMOD_SOUND* m_s_error_b = 0;
	FMOD_SOUND* m_s_landing_rate = 0;
	FMOD_SOUND* m_s_landing_feet = 0;
	FMOD_SOUND* m_s_landing_g = 0;
	FMOD_SOUND* m_s_dot = 0;
	FMOD_SOUND* m_s_wind1 = 0;
	FMOD_SOUND* m_s_wind2 = 0;
	FMOD_SOUND* m_s_knots = 0;
	FMOD_SOUND* m_s_bounce1 = 0;
	FMOD_SOUND* m_s_bounce2s = 0;
	FMOD_SOUND* m_s_bounce2 = 0;
	FMOD_SOUND* m_s_no_bounce = 0;
	FMOD_SOUND* m_s_baro_press_1 = 0;
	FMOD_SOUND* m_s_ouch = 0;

	FMOD_SOUND* m_s_fse_start1 = 0;
	FMOD_SOUND* m_s_fse_start2 = 0;
	FMOD_SOUND* m_s_fse_start3 = 0;
	FMOD_SOUND* m_s_fse_start4 = 0;

	//////////////////////////////////////////////////////////////////////////////////////////////////
	// Callback Functions

	XPLMFlightLoop_f m_FlightLoopCB;
	
	XPLMCommandCallback_f m_SayBaroCB;
	XPLMCommandCallback_f m_ResetIvyCB;
	XPLMCommandCallback_f m_SayWindCB;
	XPLMCommandCallback_f m_ToogleWindowCB;
	XPLMCommandCallback_f m_AnnouncementCB;

	
	XPLMCommandRef m_CmdSayBaro;
	XPLMCommandRef m_CmdSayWind;
	XPLMCommandRef m_CmdToogleWindow;
	XPLMCommandRef m_CmdResetIvy;
	XPLMCommandRef m_CmdAnnouncement;

	int m_IvyMenu;
	XPLMMenuHandler_f m_ShowLogMenuHandlerCB;
	XPLMMenuID m_MenuId;

	int m_MenuLogbookShow = 0;

	XPLMWindowID m_WindowId;
	XPLMDrawWindow_f m_DrawWindowCB;
	XPLMHandleKey_f m_KeyCB;
	XPLMHandleMouseClick_f m_MouseClickCB;

	//////////////////////////////////////////////////////////////////////////////////////////////////
	// Widgets

	XPWidgetID m_VSpeedsWidget;
	XPWidgetID m_v1_label;
	XPWidgetID m_vr_label;
	XPWidgetID m_v2_label;
	XPWidgetID m_dh_label;

	XPWidgetID m_kt60_label;
	XPWidgetID m_kt80_label;
	XPWidgetID m_kt100_label;

	XPWidgetID m_v1_label_val;
	XPWidgetID m_vr_label_val;
	XPWidgetID m_v2_label_val;
	XPWidgetID m_dh_label_val;

	XPWidgetID m_v1_textbox;
	XPWidgetID m_vr_textbox;
	XPWidgetID m_v2_textbox;
	XPWidgetID m_dh_textbox;

	XPWidgetID m_v1_button;
	XPWidgetID m_vr_button;
	XPWidgetID m_v2_button;
	XPWidgetID m_dh_button;

	XPWidgetID m_kt60_button;
	XPWidgetID m_kt80_button;
	XPWidgetID m_kt100_button;

	XPWidgetFunc_t m_IvyVSpeedHandlerCB;

	XPWidgetID m_LogbookWidget;
	XPWidgetID m_LogbookScrollBar;

	//////////////////////////////////////////////////////////////////////////////////////////////////
	// IVY RESPONSE

	MyIvyResponse *m_ivyOuch;
	MyIvyResponse *m_ivyPosRateClimb;
	MyIvyResponse *m_ivyTyre;
	MyIvyResponse *m_ivyBrake;
	MyIvyResponse *m_ivyTransponder;
	MyIvyResponse *m_ivyCockpitLights;
	MyIvyResponse *m_ivyLandingLights;

	MyIvyResponse *m_ivyGearUp;
	MyIvyResponse *m_ivyGearDown;

	MyIvyResponse* m_ivyGearSelectedUp;
	MyIvyResponse* m_ivyGearSelectedDown;

	MyIvyResponse *m_ivyBeaconLights;
	MyIvyResponse *m_ivyNavLights;
	MyIvyResponse *m_ivyStrobes;
	MyIvyResponse *m_ivySkidTyres;
	MyIvyResponse *m_ivyBatteryOut;
	MyIvyResponse *m_ivyEngineFire;
	MyIvyResponse *m_ivyEngineFlameout;
	MyIvyResponse *m_ivyEngineFailureGround;
	MyIvyResponse *m_ivyEngineFailureAir;
	MyIvyResponse *m_ivyEngineHotStart;
	MyIvyResponse *m_ivyBirdStrike;

	MyIvyResponse *m_ivyOverspeedFlaps;
	MyIvyResponse *m_ivyOverspeedGear;
	MyIvyResponse *m_ivyOverspeedAircraft;
	MyIvyResponse *m_ivyStall;

	MyIvyResponse *m_ivyNoBatt;
	MyIvyResponse *m_ivyHelloSun;
	MyIvyResponse *m_ivyHellorain;
	MyIvyResponse *m_ivyHelloFog;
	MyIvyResponse *m_ivyHelloNormal;
	MyIvyResponse *m_ivyCabinDownNormal;
	MyIvyResponse *m_ivyCabinDownFast;
	MyIvyResponse *m_ivyBankNormal;
	MyIvyResponse *m_ivyBankHigh;
	MyIvyResponse *m_ivyBankXHigh;
	MyIvyResponse *m_ivyPitchDownNormal;
	MyIvyResponse *m_ivyPitchDownHigh;
	MyIvyResponse *m_ivyGNormalFlightNormal;
	MyIvyResponse *m_ivyGNormalFlightHigh;
	MyIvyResponse *m_ivyGNormalFlightXHigh;
	MyIvyResponse *m_ivyGNormalNegativeLow;
	MyIvyResponse *m_ivyGNormalNegativeHigh;
	MyIvyResponse *m_ivyTurbulenceNormal;
	MyIvyResponse *m_ivyTurbolenceHigh;

	MyIvyResponse *m_ivyLandingXGood;
	MyIvyResponse *m_ivyLandingGood;
	MyIvyResponse *m_ivyLandingNormal;
	MyIvyResponse *m_ivyLandingBad;
	MyIvyResponse *m_ivyLandingXBad;

	MyIvyResponse *m_ivyBaroLow;
	MyIvyResponse *m_ivyBaroGround;
	MyIvyResponse *m_ivyBaroHigh;
	MyIvyResponse *m_ivyLandingLightsHigh;
	MyIvyResponse *m_ivyRotate;
	MyIvyResponse *m_ivy60kt;
	MyIvyResponse* m_ivy60ktLanding;
	MyIvyResponse *m_ivy80kt;
	MyIvyResponse *m_ivy100kt;
	MyIvyResponse *m_ivyV1;
	MyIvyResponse *m_ivyVR;
	MyIvyResponse *m_ivyBelowV2;
	MyIvyResponse *m_ivyAboveV2;
	MyIvyResponse *m_ivyFlapsRetracted;
	MyIvyResponse *m_ivySlatsRetracted;
	MyIvyResponse *m_ivyFlapsPosition;
	MyIvyResponse *m_ivySlatsPosition;

	MyIvyResponse *m_ivyCrash;

	MyIvyResponse *m_ivyPressureLow;
	MyIvyResponse *m_ivyPressureXLow;

	MyIvyResponse *m_ivyIceWindowLow;
	MyIvyResponse *m_ivyIceWindowHigh;
	MyIvyResponse *m_ivyIcePropellerLow;
	MyIvyResponse *m_ivyIcePropellerHigh;
	MyIvyResponse *m_ivyIcePitotLow;
	MyIvyResponse *m_ivyIcePitotHigh;
	MyIvyResponse *m_ivyIceAirframeLow;
	MyIvyResponse *m_ivyIceAirframeHigh;

	MyIvyResponse *m_ivyAnnounceTakeOff;
	MyIvyResponse *m_ivyAnnounceLanding;

	MyIvyResponse *m_ivySeatBelts;

	MyIvyResponse *m_ivyOverweight;

	MyIvyResponse *m_ivyMinimums;
	MyIvyResponse *m_ivyApplause;

	MyIvyResponse *m_ivyArmLanding;
	MyIvyResponse *m_ivyArmMinimums;

	MyIvyResponse *m_ivyLocalizerAlive;
	MyIvyResponse * m_ivyGlideSlopeAlive;
	MyIvyResponse * m_ivySpoilersArmed;
	MyIvyResponse * m_ivyStartEngine1;
	MyIvyResponse * m_ivyStartEngine2;
	MyIvyResponse * m_ivyStartEngine3;
	MyIvyResponse * m_ivyStartEngine4;
	MyIvyResponse * m_ivyStartAPU;

	MyIvyResponse* m_ivyLightUp1;
	MyIvyResponse* m_ivyLightUp2;
	MyIvyResponse* m_ivyLightUp3;
	MyIvyResponse* m_ivyLightUp4;
	MyIvyResponse* m_ivyFuelOn1;
	MyIvyResponse* m_ivyFuelOn2;
	MyIvyResponse* m_ivyFuelOn3;
	MyIvyResponse* m_ivyFuelOn4;
	MyIvyResponse* m_ivyN21;
	MyIvyResponse* m_ivyN22;
	MyIvyResponse* m_ivyN23;
	MyIvyResponse* m_ivyN24;
	MyIvyResponse* m_ivyEngineStab1;
	MyIvyResponse* m_ivyEngineStab2;
	MyIvyResponse* m_ivyEngineStab3;
	MyIvyResponse* m_ivyEngineStab4;

	MyIvyResponse * m_ivySpoilersDisarmed;
	MyIvyResponse * m_ivySpeedBrakeRetracted;
	MyIvyResponse * m_ivySpeedBrakeExtended;

	MyIvyResponse* m_ivyAirSpeedAlive;
	MyIvyResponse* m_ivyPowerSet;
	MyIvyResponse* m_ivyNOE;

	MyIvyResponse* m_ivyHeightVelocity;

	MyIvyResponse * m_ivyRpmLow;
	MyIvyResponse * m_ivyRpmHigh;
	MyIvyResponse * m_ivyBattDrain;
	MyIvyResponse * m_ivyStarterPrecond;
	MyIvyResponse * m_ivyStarterOffEarly;
	MyIvyResponse * m_ivyStarterOffLate;
	MyIvyResponse * m_ivyCyclicNotCentered;
	MyIvyResponse * m_ivyCollectiveHigh;
	MyIvyResponse * m_ivyPitotHot;
	MyIvyResponse * m_ivyAntiIceHot;
	MyIvyResponse * m_ivyIdleTime;
	MyIvyResponse * m_ivyVRS;
	MyIvyResponse * m_ivyCircuitBreakers;
	MyIvyResponse * m_ivyEngineWear;
	MyIvyResponse* m_ivyServiceCeiling;
	MyIvyResponse* m_ivyWindAzimuth;
	MyIvyResponse* m_ivyGenLowRPM;
	MyIvyResponse* m_ivyLogWriter;






	//////////////////////////////////////////////////////////////////////////////////////////////////
	// IVY DATAREFS

	XPLMDataRef m_i_paused;

	XPLMDataRef m_s_acf_descrip;
	XPLMDataRef m_s_acf_tailnumber;

	XPLMDataRef m_i_fse_flying;
	XPLMDataRef m_f_fuel_tot;
	XPLMDataRef m_f_payload_tot;
	XPLMDataRef m_f_weight_max;
	XPLMDataRef m_f_weight_tot;

	XPLMDataRef m_i_on_ground;
	XPLMDataRef m_f_climb_rate;

	XPLMDataRef m_f_gear1_ratio;
	XPLMDataRef m_f_gear2_ratio;
	XPLMDataRef m_f_gear3_ratio;
	XPLMDataRef m_f_gear4_ratio;
	XPLMDataRef m_f_gear5_ratio;

	XPLMDataRef m_i_gear_handle;
	XPLMDataRef m_f_flap_ratio;

	XPLMDataRef m_f_ground_speed;
	XPLMDataRef m_f_ias;
	XPLMDataRef m_f_sun_pitch;
	XPLMDataRef m_f_airport_light;
	XPLMDataRef m_f_world_light_precent;
	XPLMDataRef m_i_has_skid;
	XPLMDataRef m_i_transponder_mode;
	XPLMDataRef m_i_sim_ground_speed;

	XPLMDataRef m_i_temp_sl;
	XPLMDataRef m_i_dew_sl;

	XPLMDataRef m_i_landing_lights;
	XPLMDataRef m_i_beacon_lights;
	XPLMDataRef m_i_nav_lights;
	XPLMDataRef m_i_strobe_lights;
	XPLMDataRef m_i_taxi_lights;
	XPLMDataRef m_i_cockpit_lights;
	XPLMDataRef m_f_radio_alt;
	XPLMDataRef m_f_decision_height;
	XPLMDataRef m_f8_batter_charge;
	XPLMDataRef m_i_battery_on;
	XPLMDataRef m_i_gpu_on;

	XPLMDataRef m_i_flaps_overspeed;
	XPLMDataRef m_i_gear_overspeed;
	XPLMDataRef m_f_aircraft_vne;
	XPLMDataRef m_i_aircraft_overspeed;
	XPLMDataRef m_i_stall;

	XPLMDataRef m_i_cloud_0;
	XPLMDataRef m_i_cloud_1;
	XPLMDataRef m_i_cloud_2;
	XPLMDataRef m_f_visibility;
	XPLMDataRef m_i_rain;
	XPLMDataRef m_i_thunder;
	XPLMDataRef m_i_turbulence;

	XPLMDataRef m_i_batt1;
	XPLMDataRef m_i_batt2;

	XPLMDataRef m_i_tire1;
	XPLMDataRef m_i_tire2;
	XPLMDataRef m_i_tire3;
	XPLMDataRef m_i_tire4;
	XPLMDataRef m_i_tire5;

	XPLMDataRef m_i_fire1;
	XPLMDataRef m_i_fire2;
	XPLMDataRef m_i_fire3;
	XPLMDataRef m_i_fire4;
	XPLMDataRef m_i_fire5;
	XPLMDataRef m_i_fire6;
	XPLMDataRef m_i_fire7;
	XPLMDataRef m_i_fire8;

	XPLMDataRef m_i_flameout1;
	XPLMDataRef m_i_flameout2;
	XPLMDataRef m_i_flameout3;
	XPLMDataRef m_i_flameout4;
	XPLMDataRef m_i_flameout5;
	XPLMDataRef m_i_flameout6;
	XPLMDataRef m_i_flameout7;
	XPLMDataRef m_i_flameout8;



	XPLMDataRef m_i_engine_failure1;
	XPLMDataRef m_i_engine_failure2;
	XPLMDataRef m_i_engine_failure3;
	XPLMDataRef m_i_engine_failure4;
	XPLMDataRef m_i_engine_failure5;
	XPLMDataRef m_i_engine_failure6;
	XPLMDataRef m_i_engine_failure7;
	XPLMDataRef m_i_engine_failure8;

	XPLMDataRef m_i_hot1;
	XPLMDataRef m_i_hot2;
	XPLMDataRef m_i_hot3;
	XPLMDataRef m_i_hot4;
	XPLMDataRef m_i_hot5;
	XPLMDataRef m_i_hot6;
	XPLMDataRef m_i_hot7;
	XPLMDataRef m_i_hot8;

	XPLMDataRef m_f_ice_frame;
	XPLMDataRef m_f_ice_pitot;
	XPLMDataRef m_f_ice_propeller;
	XPLMDataRef m_f_ice_window;

	XPLMDataRef m_f_g_normal;
	XPLMDataRef m_f_g_forward;
	XPLMDataRef m_f_g_side;

	XPLMDataRef m_i_bird;


	XPLMDataRef m_f_pitch;
	XPLMDataRef m_f_roll;
	XPLMDataRef m_f_yaw;

	//cab press;
	XPLMDataRef m_f_cab_press;
	//cab climb rate;
	XPLMDataRef m_f_cab_rate;
	//cab humidity;
	//cab temp;

	XPLMDataRef m_f_outside_temp1;
	XPLMDataRef m_f_outside_temp2;
	XPLMDataRef m_f_outside_temp3;

	XPLMDataRef m_f_baro_set;
	XPLMDataRef m_f_baro_sea_level;
	XPLMDataRef m_f_baro_alt;

	XPLMDataRef m_f_wind_direction;
	XPLMDataRef m_f_wind_speed_kt;

	XPLMDataRef m_f_slats_1;
	XPLMDataRef m_f_flaps_1;

	XPLMDataRef m_d_latitude;
	XPLMDataRef m_d_longitude;

	XPLMDataRef m_i_nonsmoking;
	XPLMDataRef m_i_fastenseatbelt;

	XPLMDataRef m_i_replay;

	XPLMDataRef m_f_hsi_hdef_dot;
	XPLMDataRef m_f_hsi_vdef_dot;
	XPLMDataRef m_i_hsi_source_select; // 0=NAV1, 1=NAV2

	XPLMDataRef m_i_hsi_display_h;
	XPLMDataRef m_i_hsi_display_v;
	XPLMDataRef m_f_acf_heading;

	XPLMDataRef m_f_speed_brake; //Speed Brake, -0.5 = armed, 0 = off, 1 = max deployment
	XPLMDataRef m_ia_engine_start;
	XPLMDataRef m_ia_engine_running;
	XPLMDataRef m_ia_generator_running;
	XPLMDataRef m_i_apu_starter_switch; // 2= Startup

	XPLMDataRef m_f_throttle_ratio;
	XPLMDataRef m_f_n1_percent;
	XPLMDataRef m_f_n2_percent;
	XPLMDataRef m_f_egt_c;
	XPLMDataRef m_f_fuel_flow;

	XPLMDataRef m_i_206_br_adf;
	XPLMDataRef m_i_206_br_avionics;
	XPLMDataRef m_i_206_br_cable;
	XPLMDataRef m_i_206_br_cargo_hook;
	XPLMDataRef m_i_206_br_caution_light;
	XPLMDataRef m_i_206_br_cockpit_light;
	XPLMDataRef m_i_206_br_anitice;
	XPLMDataRef m_i_206_br_xmsn;
	XPLMDataRef m_i_206_br_floats;
	XPLMDataRef m_i_206_br_boost_f;
	XPLMDataRef m_i_206_br_boost_a;
	XPLMDataRef m_i_206_br_fuel_press;
	XPLMDataRef m_i_206_br_fuel_valve;
	XPLMDataRef m_i_206_br_gen_field;
	XPLMDataRef m_i_206_br_gen_reset;
	XPLMDataRef m_i_206_br_gov;
	XPLMDataRef m_i_206_br_gps;
	XPLMDataRef m_i_206_br_hmike;
	XPLMDataRef m_i_206_br_heat;
	XPLMDataRef m_i_206_br_hoist;
	XPLMDataRef m_i_206_br_htr_cont;
	XPLMDataRef m_i_206_br_htr_pwr;
	XPLMDataRef m_i_206_br_htr_hyd_boost;
	XPLMDataRef m_i_206_br_htr_ics;
	XPLMDataRef m_i_206_br_htr_ign;
	XPLMDataRef m_i_206_br_htr_inst_light;
	XPLMDataRef m_i_206_br_htr_ldg_light;
	XPLMDataRef m_i_206_br_htr_sas;
	XPLMDataRef m_i_206_br_htr_starter;
	XPLMDataRef m_i_206_br_htr_tot_ind;
	XPLMDataRef m_i_206_br_htr_turn;
	XPLMDataRef m_i_206_br_htr_vhf;
	XPLMDataRef m_i_206_br_htr_xpdf;
	
	XPLMDataRef m_f_206_doors;
	XPLMDataRef m_f_206_rotor_rpm; 
	XPLMDataRef m_i_206_hydraulics;
	XPLMDataRef m_f_206_throttle;
	XPLMDataRef m_i_206_caut_test;
	XPLMDataRef m_i_206_tot_test;
	XPLMDataRef m_i_206_floats_active;
	XPLMDataRef m_i_206_ldg_lights_switch;
	XPLMDataRef m_f_206_fmod;

	XPLMDataRef m_f_206_oil_press;
	XPLMDataRef m_f_206_oil_temp;

	// L3:

	XPLMDataRef m_i_206L3_br_adf;
	XPLMDataRef m_i_206L3_br_avionics;
	XPLMDataRef m_i_206L3_br_cable;
	XPLMDataRef m_i_206L3_br_cargo_hook;
	XPLMDataRef m_i_206L3_br_caution_light;
	XPLMDataRef m_i_206L3_br_cockpit_light;
	XPLMDataRef m_i_206L3_br_anitice;
	XPLMDataRef m_i_206L3_br_xmsn;
	XPLMDataRef m_i_206L3_br_floats;
	XPLMDataRef m_i_206L3_br_boost_f;
	XPLMDataRef m_i_206L3_br_boost_a;
	XPLMDataRef m_i_206L3_br_fuel_press;
	XPLMDataRef m_i_206L3_br_fuel_valve;
	XPLMDataRef m_i_206L3_br_gen_field;
	XPLMDataRef m_i_206L3_br_gen_reset;
	XPLMDataRef m_i_206L3_br_gov;
	XPLMDataRef m_i_206L3_br_gps;
	XPLMDataRef m_i_206L3_br_hmike;
	XPLMDataRef m_i_206L3_br_heat;
	XPLMDataRef m_i_206L3_br_hoist;
	XPLMDataRef m_i_206L3_br_htr_cont;
	XPLMDataRef m_i_206L3_br_htr_pwr;
	XPLMDataRef m_i_206L3_br_htr_hyd_boost;
	XPLMDataRef m_i_206L3_br_htr_ics;
	XPLMDataRef m_i_206L3_br_htr_ign;
	XPLMDataRef m_i_206L3_br_htr_inst_light;
	XPLMDataRef m_i_206L3_br_htr_ldg_light;
	XPLMDataRef m_i_206L3_br_htr_sas;
	XPLMDataRef m_i_206L3_br_htr_starter;
	XPLMDataRef m_i_206L3_br_htr_tot_ind;
	XPLMDataRef m_i_206L3_br_htr_turn;
	XPLMDataRef m_i_206L3_br_htr_vhf;
	XPLMDataRef m_i_206L3_br_htr_xpdf;

	XPLMDataRef m_f_206L3_doors;
	XPLMDataRef m_f_206L3_rotor_rpm;
	XPLMDataRef m_i_206L3_hydraulics;
	XPLMDataRef m_f_206L3_throttle;
	XPLMDataRef m_i_206L3_caut_test;
	XPLMDataRef m_i_206L3_tot_test;
	XPLMDataRef m_i_206L3_floats_active;
	XPLMDataRef m_i_206L3_ldg_lights_switch;
	XPLMDataRef m_f_206L3_fmod;

	XPLMDataRef m_f_206L3_oil_press;
	XPLMDataRef m_f_206L3_oil_temp;



	XPLMDataRef m_f_cyclic_ail;
	XPLMDataRef m_f_cyclic_elev;
	XPLMDataRef m_fa_torque;
	XPLMDataRef m_fa_itt;
	XPLMDataRef m_i_pitot_heat;
	XPLMDataRef m_ia_eng_anti_ice;
	XPLMDataRef m_f_vertical_speed;

	XPLMDataRef m_i_avionics; //sim/cockpit/electrical/avionics_on


	


	/////////////////////////////////////////////////////////////////////////////////
	// Dataref Variables

	int m_li_paused;

	int m_li_on_ground ;
	float m_lf_climb_rate ;

	int m_li_fse_flying = 0;
	int m_li_fse_flying_old = 0;

	float m_lf_fuel_tot;
	float m_lf_payload_tot;
	float m_lf_weight_max;
	float m_lf_weight_tot;

	float m_lf_gear1_ratio ;
	float m_lf_gear2_ratio ;
	float m_lf_gear3_ratio ;
	float m_lf_gear4_ratio ;
	float m_lf_gear5_ratio ;
	int m_li_gear_handle;
	float m_lf_flap_ratio;

	float m_lf_ground_speed ;
	float m_lf_ias ;
	float m_lf_sun_pitch ;
	float m_lf_airport_light ;
	float m_lf_world_light_precent ;
	int m_li_has_skid ;
	int m_li_transponder_mode ;
	int m_li_sim_ground_speed ;

	int m_li_temp_sl ;
	int m_li_dew_sl ;

	int m_li_landing_lights ;
	int m_li_beacon_lights ;
	int m_li_nav_lights ;
	int m_li_strobe_lights ;
	int m_li_taxi_lights ;
	int m_li_cockpit_lights ;
	float m_lf_radio_alt ;
	float m_lf_decision_height ;
	float m_lf8_batter_charge ;

	int m_li_battery_on ;
	int m_li_gpu_on ;

	int m_li_flaps_overspeed ;
	int m_li_gear_overspeed ;
	int m_li_aircraft_overspeed ;
	float m_lf_aircraft_vne ;
	int m_li_stall ;

	int m_li_cloud_0 ;
	int m_li_cloud_1 ;
	int m_li_cloud_2 ;
	float m_lf_visibility ;
	int m_li_rain ;
	int m_li_thunder ;
	int m_li_turbulence ;


	int m_li_batt1 ;
	int m_li_batt2 ;

	int m_li_tire1 ;
	int m_li_tire2 ;
	int m_li_tire3 ;
	int m_li_tire4 ;
	int m_li_tire5 ;

	int m_li_fire1 ;
	int m_li_fire2 ;
	int m_li_fire3 ;
	int m_li_fire4 ;
	int m_li_fire5 ;
	int m_li_fire6 ;
	int m_li_fire7 ;
	int m_li_fire8 ;

	int m_li_flameout1 ;
	int m_li_flameout2 ;
	int m_li_flameout3 ;
	int m_li_flameout4 ;
	int m_li_flameout5 ;
	int m_li_flameout6 ;
	int m_li_flameout7 ;
	int m_li_flameout8 ;



	int m_li_engine_failure1 ;
	int m_li_engine_failure2 ;
	int m_li_engine_failure3 ;
	int m_li_engine_failure4 ;
	int m_li_engine_failure5 ;
	int m_li_engine_failure6 ;
	int m_li_engine_failure7 ;
	int m_li_engine_failure8 ;

	int m_li_hot1 ;
	int m_li_hot2 ;
	int m_li_hot3 ;
	int m_li_hot4 ;
	int m_li_hot5 ;
	int m_li_hot6 ;
	int m_li_hot7 ;
	int m_li_hot8 ;

	float m_lf_ice_frame ;
	float m_lf_ice_pitot ;
	float m_lf_ice_propeller ;
	float m_lf_ice_window ;

	int m_li_bird ;

	float m_lf_g_normal ;
	float m_lf_g_forward ;
	float m_lf_g_side ;


	float m_lf_pitch ;
	float m_lf_roll ;
	float m_lf_yaw ;


	float m_lf_cab_press ;
	float m_lf_cab_rate ;
	//cab humidity ?;
	//cab temp ?;

	float m_lf_outside_temp1 ;
	float m_lf_outside_temp2 ;
	float m_lf_outside_temp3 ;

	float m_lf_baro_set ;
	int m_li_baro_set ;
	float m_lf_baro_sea_level ;
	int m_li_baro_sea_level ;
	float m_lf_baro_alt ;
	float m_lf_density_alt;

	float m_lf_wind_direction ;
	float m_lf_wind_speed_kt ;

	float m_lf_slats_1 ;
	float m_lf_flaps_1 ;

	double m_ld_latitude ;
	double m_ld_longitude ;

	int m_li_nonsmoking ;
	int m_li_fastenseatbelts ;
	int m_li_replay ;

	float m_lf_hsi_hdef_dot;
	float m_lf_hsi_vdef_dot;
	int m_li_hsi_display_h;
	int m_li_hsi_display_v;
	float m_lf_acf_heading;

	int m_li_hsi_source_select; // 0=NAV1, 1=NAV2

	float m_lf_speed_brake = 0; //Speed Brake, -0.5 = armed, 0 = off, 1 = max deployment
	float m_lf_speed_brake_old = 1;
	
	int m_lia_engine_start[8];
	int m_lia_engine_running[8];
	int m_lia_generator_running[8];
	int m_lia_engine_start_old[8];
	int m_li_apu_starter_switch; // 2= Startup

	int m_li_avionics;

	float m_lf_throttle_ratio;
	float m_lf_n1_percent[8];
	float m_lf_n1_percent_old[8];
	float m_lf_n2_percent[8];
	float m_lf_egt_c[8];
	float m_lf_fuel_flow[8];


	// 206 B3

	int m_li_206_br_adf;
	int m_li_206_br_avionics;
	int m_li_206_br_cable;
	int m_li_206_br_cargo_hook;
	int m_li_206_br_caution_light;
	int m_li_206_br_cockpit_light;
	int m_li_206_br_anitice;
	int m_li_206_br_xmsn;
	int m_li_206_br_floats;
	int m_li_206_br_boost_f;
	int m_li_206_br_boost_a;
	int m_li_206_br_fuel_press;
	int m_li_206_br_fuel_valve;
	int m_li_206_br_gen_field;
	int m_li_206_br_gen_reset;
	int m_li_206_br_gov;
	int m_li_206_br_gps;
	int m_li_206_br_hmike;
	int m_li_206_br_heat;
	int m_li_206_br_hoist;
	int m_li_206_br_htr_cont;
	int m_li_206_br_htr_pwr;
	int m_li_206_br_htr_hyd_boost;
	int m_li_206_br_htr_ics;
	int m_li_206_br_htr_ign;
	int m_li_206_br_htr_inst_light;
	int m_li_206_br_htr_ldg_light;
	int m_li_206_br_htr_sas;
	int m_li_206_br_htr_starter;
	int m_li_206_br_htr_tot_ind;
	int m_li_206_br_htr_turn;
	int m_li_206_br_htr_vhf;
	int m_li_206_br_htr_xpdf;

	float m_lf_206_doors;
	float m_lf_206_rotor_rpm;
	int m_li_206_hydraulics;
	float m_lf_206_throttle;
	int m_li_206_caut_test;
	int m_li_206_tot_test;
	int m_li_206_floats_active;
	int m_li_206_ldg_lights_switch;
	float m_lf_206_fmod;

	float m_lf_206_oil_press;
	float m_lf_206_oil_temp;

	bool m_206_caution_test = false;
	bool m_206_tot_test = false;

	bool m_206_switches_set = false;

	// 206 L3

	int m_li_206L3_br_adf;
	int m_li_206L3_br_avionics;
	int m_li_206L3_br_cable;
	int m_li_206L3_br_cargo_hook;
	int m_li_206L3_br_caution_light;
	int m_li_206L3_br_cockpit_light;
	int m_li_206L3_br_anitice;
	int m_li_206L3_br_xmsn;
	int m_li_206L3_br_floats;
	int m_li_206L3_br_boost_f;
	int m_li_206L3_br_boost_a;
	int m_li_206L3_br_fuel_press;
	int m_li_206L3_br_fuel_valve;
	int m_li_206L3_br_gen_field;
	int m_li_206L3_br_gen_reset;
	int m_li_206L3_br_gov;
	int m_li_206L3_br_gps;
	int m_li_206L3_br_hmike;
	int m_li_206L3_br_heat;
	int m_li_206L3_br_hoist;
	int m_li_206L3_br_htr_cont;
	int m_li_206L3_br_htr_pwr;
	int m_li_206L3_br_htr_hyd_boost;
	int m_li_206L3_br_htr_ics;
	int m_li_206L3_br_htr_ign;
	int m_li_206L3_br_htr_inst_light;
	int m_li_206L3_br_htr_ldg_light;
	int m_li_206L3_br_htr_sas;
	int m_li_206L3_br_htr_starter;
	int m_li_206L3_br_htr_tot_ind;
	int m_li_206L3_br_htr_turn;
	int m_li_206L3_br_htr_vhf;
	int m_li_206L3_br_htr_xpdf;

	float m_lf_206L3_doors;
	float m_lf_206L3_rotor_rpm;
	int m_li_206L3_hydraulics;
	float m_lf_206L3_throttle;
	int m_li_206L3_caut_test;
	int m_li_206L3_tot_test;
	int m_li_206L3_floats_active;
	int m_li_206L3_ldg_lights_switch;
	float m_lf_206L3_fmod;

	float m_lf_206L3_oil_press;
	float m_lf_206L3_oil_temp;

	bool m_206L3_caution_test = false;
	bool m_206L3_tot_test = false;

	bool m_206L3_switches_set = false;

	float m_lfa_torque[8];
	float m_lfa_itt[8];
	int m_lia_eng_anti_ice[8];
	int m_li_pitot_heat;
	float m_lf_vertical_speed;

	float m_lf_cyclic_elev[8];
	float m_lf_cyclic_ail[8];

	

	
	int m_engine_on_old = 0;
	bool m_precond = true;
	int m_starter_switch_old = 0;
	bool m_starter_off_early = false;
	

	float m_rotor_th[IVY_ENGN_MAX];
	float m_rotor_sec[IVY_ENGN_MAX];
	bool m_rotor_failure[IVY_ENGN_MAX];
	float m_rotor_start[IVY_ENGN_MAX];

	bool m_n1_failure[IVY_ENGN_MAX];
	float m_n1_start[IVY_ENGN_MAX];
	bool m_torque_failure[IVY_ENGN_MAX];
	float m_torque_start[IVY_ENGN_MAX];
	bool m_itt_failure[IVY_ENGN_MAX];
	float m_itt_start[IVY_ENGN_MAX];
	bool m_oil_press_failure[IVY_ENGN_MAX];
	float m_oil_press_start[IVY_ENGN_MAX];
	bool m_oil_temp_failure[IVY_ENGN_MAX];
	float m_oil_temp_start[IVY_ENGN_MAX];

public:
	MyIvy();
	~MyIvy();
	void ResetIvy();
	void IvyStart();
	void IvyStop();
	void IvyEnable();
	void IvyDisable();

	void IvyLoadSoundFiles(bool reload = false);
	FMOD_SOUND* IvyLoadSingleSoundFile(std::string sound_file, bool is_number = false);
	void IvyAssignDataRefs();


	void CreateVSpeedsWidget();
	int IvyVSpeedHandler(XPWidgetMessage  inMessage, XPWidgetID  inWidget, intptr_t inParam1, intptr_t inParam2);
	void CreateLogbookWidget(int x, int y, int w, int h);
	int IvyLogbookHandler(XPWidgetMessage  inMessage, XPWidgetID  inWidget, intptr_t inParam1, intptr_t inParam2);
	int IvyLogbookScrollHandler(XPWidgetMessage  inMessage, XPWidgetID  inWidget, intptr_t inParam1, intptr_t inParam2);
	void IvyFillLogbook();

	void IvyReceiveMessage(XPLMPluginID inFrom, int inMsg, void * inParam);
	void IvyDrawOutputWindow(XPLMWindowID in_window_id, void * in_refcon);
	void IvyMenuHandler(void * in_menu_ref, void * in_item_ref);
	int SayBaroCallback(XPLMCommandRef cmd, XPLMCommandPhase phase, void *refcon);
	int SayWindCallback(XPLMCommandRef cmd, XPLMCommandPhase phase, void *refcon);
	int AnnouncementCallback(XPLMCommandRef cmd, XPLMCommandPhase phase, void *refcon);
	int ResetIvyCallback(XPLMCommandRef cmd, XPLMCommandPhase phase, void *refcon);
	int ToogleWindowCallback(XPLMCommandRef cmd, XPLMCommandPhase phase, void *refcon);
	void EngineLimitsCheck(float value, float* p_threshold, float* p_seconds, bool* p_failure, float* p_start, XPLMDataRef dr_error, MyIvyResponse *p_response, std::string log_entry);
	void WriteLogMessage(std::string message);

	void KeyCallback(XPLMWindowID inWindowID, char inKey, XPLMKeyFlags inFlags, char inVirtualKey, void * inRefcon, int losingFocus);
	int MouseClickCallback(XPLMWindowID inWindowID, int x, int y, XPLMMouseStatus inMouse, void * inRefcon);

	void SayBaro();
	void SayWind();
	void MenuCheck();
	void SpellOutDigits(int spell_number);
	void SayNumber(int spell_number);
	void CheckAnnouncement();
	void BoardingCompleted();
	void DetectLanding();
	void EndOfFlightEvaluation();
	void StartupSwitches();
	void LoadAircraftDatarefs();

	float IvyFlightLoopCallback(float elapsedMe, float elapsedSim, int counter, void * refcon);

	void ReadData();
	void RefreshLogbook();

	void WriteErrorLog();
	void ReadErrorLog();
	void CalculateStatistics();
};

