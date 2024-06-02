 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////;
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////;
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////;
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////;
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////;
// PythonInterface;
//;
// Main class where we do our stuff;
 
class PythonInterface{

 
	
 
	
 
 
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////;
	// FlightLoopCallback;
	//;
	// This is where all the error detection is performed;
 
	def FlightLoopCallback(self, elapsedMe, elapsedSim, counter, refcon){
 
		m_time = m_time + m_ivyConfig->m_data_rate;
 
		m_passengersScreaming = false;
		m_passengerVolume = 0.3;
 
		// We reset the aircraft loaded situation after 60 seconds;
		if (m_time > (60 + m_ivyConfig->m_disable_after_loading))
			m_aircraft_loaded = 0;
 
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
		
		else if (len(m_play_mp3_queu-> > 0)
		{
			if (m_ivyChannel.get_busy() == false)
			{
				actsound = pygame.mixer.Sound(m_play_mp3_queu->0]);
				m_ivyChannel.play(actsound);
				del m_play_mp3_queu->0];
				m_deact_queue = m_ivyConfig->m_deact_after_queue;
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
			m_f_g_front_delta = (m_lf_g_forward - m_f_g_forward_old) / m_ivyConfig->m_data_rate;
 
			m_f_g_normal_old = m_lf_g_normal;
			m_f_g_side_old = m_lf_g_side;
			m_f_g_forward_old = m_lf_g_forward;
 
			if (m_cab_press_old != 0){ 	m_cab_press_rate = 60 * (m_lf_cab_press - m_cab_press_old) / (m_li_sim_ground_speed * m_ivyConfig->m_data_rate);
			m_cab_press_old = m_lf_cab_press;
 
 
 
			DetectLanding();
 
 
			// Ouch when bumping on ground;
			if ((m_li_on_ground == 1) && ((m_lf_g_normal) > m_ivyConfig->m_ivy_ouch_g)) //play ouch;
			{
				actsound = pygame.mixer.Sound(m_ivyConfig->m_mp3_path + "ouch_1.wav");
				m_ivyChannel.play(actsound);
			}
			// Check for announcemnt to make;
			CheckAnnouncement();
 
 
 
			// Announcement deactivation only. Either activated by command || by CheckAnnouncement;
			if (m_li_on_ground == 1)																												m_ivyAnnounceLanding->Deactivate(m_time);
			if (m_li_on_ground == 0)																												m_ivyAnnounceTakeOff->Deactivate(m_time);
 
			if ((m_li_on_ground == 0) && (m_lf_climb_rate > m_ivyConfig->m_pos_rate_climb))															m_ivyPosRateClimb->Activate(m_time);
			else if (m_li_on_ground == 1)																											m_ivyPosRateClimb->Deactivate(m_time);
 
			// Decision Height Arm;
			if ((m_li_on_ground == 0) && (m_lf_radio_alt > (m_lf_decision_height + m_ivyConfig->m_decition_height_arm)))							m_ivyArmMinimums->Activate(m_time);
			else if (m_li_on_ground == 1)																											m_ivyArmMinimums->Deactivate(m_time);
 
			if ((m_li_on_ground == 0) &&
			   (m_ivyArmMinimums.played == 1) &&
			   (m_lf_decision_height > 0) &&
			   (m_lf_radio_alt < (m_lf_decision_height + m_ivyConfig->m_decition_height_plus)))														m_ivyMinimums->Activate(m_time);
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
			else{																																	m_ivyGearUp->Deactivate(m_time);
 
			// Tire blown;
			if ((m_li_tire1 + m_li_tire2 + m_li_tire3 + m_li_tire4 + m_li_tire5) > 0)																m_ivyTyre->Activate(m_time);
			else																																	m_ivyTyre->Deactivate(m_time);
 
			// Hard braking;
			if ((m_li_on_ground == 1) && (m_lf_g_forward > m_ivyConfig->m_brake_max_forward_g))		 												m_ivyBrake->Activate(m_time);
			else																																	m_ivyBrake->Deactivate(m_time);
 
			// Transponder not active when airborne;
			if ((m_li_on_ground == 0) && (m_li_transponder_mode < 2))																				m_ivyTransponder->Activate(m_time) // Dataref{ Mode2=ON, B407 4=ON;
			else if (m_li_on_ground == 1)																											m_ivyTransponder->Deactivate(m_time);
 
			// Landing lights not on when landing in the night;
			if ((m_li_on_ground == 0) && (m_lf_radio_alt < m_ivyConfig->m_alt_landing_lights_low) &&
			    (m_lf_world_light_precent > m_ivyConfig->m_night_world_light_precent) && (m_li_landing_lights == 0))								m_ivyLandingLights->Activate(m_time);
			else																																	m_ivyLandingLights->Deactivate(m_time);
 
			// Landing lights not off on high altitude;
			if ((m_li_on_ground == 0) && (m_lf_radio_alt > 3000) && (m_lf_baro_alt > m_ivyConfig->m_alt_landing_lights_high) &&
			   (m_li_landing_lights == 1))																											m_ivyLandingLightsHigh->Activate(m_time);
			else																																	m_ivyLandingLightsHigh->Deactivate(m_time);
 
			// Beacon lights not when taxiing;
			if ((m_li_on_ground == 1) && (m_lf_ground_speed > m_ivyConfig->m_taxi_ground_speed_min) && (m_li_beacon_lights == 0))					m_ivyBeaconLights->Activate(m_time);
			else if (m_li_beacon_lights != 0)																										m_ivyBeaconLights->Deactivate(m_time);
 
			// Nav lights lights not when airborne;
			if ((m_li_on_ground == 0) && (m_li_nav_lights == 0))																					m_ivyNavLights->Activate(m_time);
			else																																	m_ivyNavLights->Deactivate(m_time);
 
			// Strobes not on when airborne;
			if ((m_li_on_ground == 0) && (m_li_strobe_lights == 0))																					m_ivyStrobes->Activate(m_time);
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
			if ((m_li_flameout1 + m_li_flameout2 + m_li_flameout3 + m_li_flameout4 +;
				 m_li_flameout5 + m_li_flameout6 + m_li_flameout7 + m_li_flameout8) > 0)															m_ivyEngineFlameout->Activate(m_time);
			else																																	m_ivyEngineFlameout->Deactivate(m_time);
 
			// Engine ground failure;
			if ((m_li_on_ground == 1) &&
			   ((m_li_engine_failure1 + m_li_engine_failure2 + m_li_engine_failure3 + m_li_engine_failure4 +;
				 m_li_engine_failure5 + m_li_engine_failure6 + m_li_engine_failure7 + m_li_engine_failure8) > 0))									m_ivyEngineFailureGround->Activate(m_time);
			else																																	m_ivyEngineFailureGround->Deactivate(m_time);
 
			// Engine airborne failure;
			if ((m_li_on_ground == 0) &&
			   ((m_li_engine_failure1 + m_li_engine_failure2 + m_li_engine_failure3 + m_li_engine_failure4 +;
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
			if (m_li_on_ground == 0) && (m_li_stall > 0)																							m_ivyStall->Activate(m_time);
			else																																	m_ivyStall->Deactivate(m_time);
 
			// Aircraft Overspeed;
			if (m_lf_ias > m_lf_aircraft_vne) && (m_lf_aircraft_vne > 1)																			m_ivyOverspeedAircraft->Activate(m_time);
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
			if ((m_li_on_ground == 0) && (max(m_lf_cab_rate, m_lf_climb_rate) < m_ivyConfig->m_cab_rate_low))										m_ivyCabinDownNormal->Activate(m_time);
			else if (max(m_lf_cab_rate, m_lf_climb_rate) > (m_ivyConfig->m_cab_rate_low + m_ivyConfig->m_cab_rate_reset_hysteresis))				m_ivyCabinDownNormal->Deactivate(m_time);
 
			// Cabin pressure falling rapidely;
			if ((m_li_on_ground == 0) && (max(m_lf_cab_rate, m_lf_climb_rate) < m_ivyConfig->m_cab_rate_high))
			{
																																					m_ivyCabinDownFast->Activate(m_time);
																																					m_ivyCabinDownNormal.SetAsPlayed(m_time);
			}
			else if (max(m_lf_cab_rate, m_lf_climb_rate) > (m_ivyConfig->m_cab_rate_high + m_ivyConfig->m_cab_rate_reset_hysteresis))				m_ivyCabinDownFast->Deactivate(m_time);
 
			// Bank angle pre-warning;
			if ((m_li_on_ground == 0) && (abs(m_lf_roll) > m_ivyConfig->m_bank_low))																m_ivyBankNormal->Activate(m_time);
			else if (abs(m_lf_roll) < m_ivyConfig->m_bank_reset_low)																				m_ivyBankNormal->Deactivate(m_time);
 
			// Bank angle too high;
			if ((m_li_on_ground == 0) && (abs(m_lf_roll) > m_ivyConfig->m_bank_high))
			{
																																					m_ivyBankHigh->Activate(m_time);
																																					m_ivyBankNormal.SetAsPlayed(m_time);
			}
			else if (abs(m_lf_roll) < m_ivyConfig->m_bank_low)																						m_ivyBankHigh->Deactivate(m_time);
 
			// Bank angle extremely high;
			if ((m_li_on_ground == 0) && (abs(m_lf_roll) > m_ivyConfig->m_bank_xhigh))
			{
																																					m_passengersScreaming = true;
																																					m_ivyBankXHigh->Activate(m_time);
																																					m_ivyBankHigh.SetAsPlayed(m_time);
																																					m_ivyBankNormal.SetAsPlayed(m_time);
			}
			else if (abs(m_lf_roll) < m_ivyConfig->m_bank_high)																						m_ivyBankXHigh->Deactivate(m_time);
 
 
			// Pitch down pre-warning;
			if ((m_li_on_ground == 0) && (m_lf_pitch < m_ivyConfig->m_pitch_low))																	m_ivyPitchDownNormal->Activate(m_time);
			else if (m_lf_pitch > m_ivyConfig->m_pitch_reset_low)																					m_ivyPitchDownNormal->Deactivate(m_time);
 
			// Pitch too low;
			if ((m_li_on_ground == 0) && (m_lf_pitch <= m_ivyConfig->m_pitch_high))
			{
																																					m_passengersScreaming = true;
																																					m_ivyPitchDownHigh->Activate(m_time);
																																					m_ivyPitchDownNormal.SetAsPlayed(m_time);
			}
			else if (m_lf_pitch > m_ivyConfig->m_pitch_low){																							m_ivyPitchDownHigh->Deactivate(m_time);
 
 
			// Normal G Force high;
			if ((m_li_on_ground == 0) && (m_lf_g_normal >= m_ivyConfig->m_max_g_down_low))
			{
																																					m_passengersScreaming = true;
																																					m_passengerVolume = max (m_passengerVolume, abs(m_lf_g_normal) / 6);
																																					m_ivyGNormalFlightNormal->Activate(m_time);
			}
			else if (m_lf_g_normal <= m_ivyConfig->m_max_g_down_low_reset)																			m_ivyGNormalFlightNormal->Deactivate(m_time);
 
			// Normal G Force very high;
			if ((m_li_on_ground == 0) && (m_lf_g_normal >= m_ivyConfig->m_max_g_down_high))
			{
																																					m_ivyGNormalFlightHigh->Activate(m_time);
																																					m_ivyGNormalFlightNormal.SetAsPlayed(m_time);
			}
			else if (m_lf_g_normal <= m_ivyConfig->m_max_g_down_low_reset)																			m_ivyGNormalFlightHigh->Deactivate(m_time);
 
			// Normal G Force very, very high;
			if ((m_li_on_ground == 0) && (m_lf_g_normal >= m_ivyConfig->m_max_g_down_xhigh))
			{
																																					m_ivyGNormalFlightXHigh->Activate(m_time);
																																					m_ivyGNormalFlightHigh.SetAsPlayed(m_time);
																																					m_ivyGNormalFlightNormal.SetAsPlayed(m_time);
			}
			else if (m_lf_g_normal <= m_ivyConfig->m_max_g_down_low_reset)																			m_ivyGNormalFlightXHigh->Deactivate(m_time);
 
 
			// Normal G Force too low;
			if ((m_li_on_ground == 0) && (m_lf_g_normal <= 0.5))
			{
																																					m_passengersScreaming = true;
																																					m_passengerVolume = max (m_passengerVolume, abs(m_lf_g_normal - 0.5) / 2);
																																					m_ivyGNormalNegativeLow->Activate(m_time);
			}
			else if (m_lf_g_normal > 0.8)																											m_ivyGNormalNegativeLow->Deactivate(m_time);
 
			// Normal G Force negative;
			if ((m_li_on_ground == 0) && (m_lf_g_normal <= 0))
			{
																																					m_ivyGNormalNegativeHigh->Activate(m_time);
																																					m_ivyGNormalNegativeLow.SetAsPlayed(m_time);
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
																																					if ((m_ivyBaroGround.played == 1) && (m_pressure_said == 0))
																																					{
																																						m_pressure_said = 1;
																																						m_SayBaro();
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
 
			// 60 knots callout;
			if ((m_li_on_ground == 1) && (m_ivyConfig->m_kt60_enabled == true) && (m_lf_ias > 58) && (m_lf_ias < 70))								m_ivy60kt->Activate(m_time);
			else																																	m_ivy60kt->Deactivate(m_time);
 
			// 80 knots callout;
			if ((m_li_on_ground == 1) && (m_ivyConfig->m_kt80_enabled == true) && (m_lf_ias > 78) && (m_lf_ias < 90))								m_ivy80kt->Activate(m_time);
			else																																	m_ivy80kt->Deactivate(m_time);
 
			// 100 knots callout;
			if ((m_li_on_ground == 1) && (m_ivyConfig->m_kt100_enabled == true) && (m_lf_ias > 98) && (m_lf_ias < 110))								m_ivy100kt->Activate(m_time);
			else																																	m_ivy100kt->Deactivate(m_time);
 
			// Not rotated;
			if ((m_li_on_ground == 1) && (m_lf_ias > 180) && (m_landing_detected == 0))																m_ivyRotate->Activate(m_time);
			else																																	m_ivyRotate->Deactivate(m_time);
 
			// Ice airframe low;
			if (m_lf_ice_frame > m_ivyConfig->m_ice_low)																							m_ivyIceAirframeLow->Activate(m_time);
			else																																	m_ivyIceAirframeLow->Deactivate(m_time);
 
			// Ice airframe high;
			if (m_lf_ice_frame > m_ivyConfig->m_ice_high)
			{
																																					m_ivyIceAirframeHigh->Activate(m_time);
																																					m_ivyIceAirframeLow.SetAsPlayed(m_time);
			}
			else																																	m_ivyIceAirframeHigh->Deactivate(m_time);
 
			// Ice pitot low;
			if (m_lf_ice_pitot > m_ivyConfig->m_ice_low)																							m_ivyIcePitotLow->Activate(m_time);
			else																																	m_ivyIcePitotLow->Deactivate(m_time);
 
			// Ice pitot high;
			if (m_lf_ice_pitot > m_ivyConfig->m_ice_high)
			{
																																					m_ivyIcePitotHigh->Activate(m_time);
																																					m_ivyIcePitotLow.SetAsPlayed(m_time);
			}
			else																																	m_ivyIcePitotHigh->Deactivate(m_time);
 
			// Ice propeller low;
			if (m_lf_ice_propeller > m_ivyConfig->m_ice_low)																						m_ivyIcePropellerLow->Activate(m_time);
			else																																	m_ivyIcePropellerLow->Deactivate(m_time);
 
			// Ice propeller high;
			if (m_lf_ice_propeller > m_ivyConfig->m_ice_high)
			{
																																					m_ivyIcePropellerHigh->Activate(m_time);
																																					m_ivyIcePropellerLow.SetAsPlayed(m_time);
			}
			else																																	m_ivyIcePropellerHigh->Deactivate(m_time);
 
			// Ice cockpit window low;
			if (m_lf_ice_window > m_ivyConfig->m_ice_low)																							m_ivyIceWindowLow->Activate(m_time);
			else																																	m_ivyIceWindowLow->Deactivate(m_time);
 
			// Ice cockpit window high;
			if (m_lf_ice_window > m_ivyConfig->m_ice_high)
			{
																																					m_ivyIceWindowHigh->Activate(m_time);
																																					m_ivyIceWindowLow.SetAsPlayed(m_time);
			}
			else																																	m_ivyIceWindowHigh->Deactivate(m_time);
 
			// Cabin pressure low;
			if (m_lf_cab_press > m_ivyConfig->m_cab_press_low)																						m_ivyPressureLow->Activate(m_time);
			else																																	m_ivyPressureLow->Deactivate(m_time);
 
			// Cabin pressure too low to breath;
			if (m_lf_cab_press > m_ivyConfig->m_cab_press_high)
			{
																																					m_ivyPressureXLow->Activate(m_time);
																																					m_ivyPressureLow.SetAsPlayed(m_time);
			}
			else																																	m_ivyPressureXLow->Deactivate(m_time);
 
			// Birdstrike;
			if (m_li_bird != 0)																														m_ivyBirdStrike->Activate(m_time);
			else																																	m_ivyBirdStrike->Deactivate(m_time);
 
			////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////;
			// V-Speed callouts if configured in IvyAircraft ini file;
			// V1;
			// VR;
			// V2;
			// V2 not achieved within 5 seconds after take off;
 
			if ((m_li_on_ground == 1) && (m_ivyAircraft->m_li_v1 > 0) &&
			    (m_lf_ias >= m_ivyAircraft->m_li_v1) && (m_lf_ground_speed > m_ivyConfig->m_taxi_ground_speed_min))									m_ivyV1->Activate(m_time);
			else if ((m_li_on_ground == 1) && (m_lf_ias < 10))																						m_ivyV1->Deactivate(m_time);
 
			if ((m_li_on_ground == 1) && (m_ivyAircraft->m_li_vr > 0) &&
			    (m_lf_ias >= m_ivyAircraft->m_li_vr) && (m_lf_ground_speed > m_ivyConfig->m_taxi_ground_speed_min))									m_ivyVR->Activate(m_time);
			else if ((m_li_on_ground == 1) && (m_lf_ias < 10))																						m_ivyVR->Deactivate(m_time);
 
			if ((m_li_on_ground == 0) && (m_ivyAircraft->m_li_v2 > 0) &&
			    (m_lf_ias >= m_ivyAircraft->m_li_v2))																								m_ivyAboveV2->Activate(m_time);
			else if (m_li_on_ground == 1)																											m_ivyAboveV2->Deactivate(m_time);
 
			if ((m_li_on_ground == 0) && (m_ivyAircraft->m_li_v2 > 0) &&
			    (m_lf_ias < m_ivyAircraft->m_li_v2) && (m_ivyAboveV2.played == 0))
			{
																																					m_ivyBelowV2->Activate(m_time);
																																					m_ivyAboveV2->Deactivate(m_time);
			}
			else if (m_li_on_ground == 1)																											m_ivyBelowV2->Deactivate(m_time);
 
 
			////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////;
			// Slats callout if configured in IvyAircraft ini file;
			if ((m_ivyAircraft->m_slats_enabled == true) && (m_ivyAircraft->m_lf_slats == 0))														m_ivySlatsRetracted->Activate(m_time);
			else																																	m_ivySlatsRetracted->Deactivate(m_time);
 
			if (m_ivyAircraft->m_slats_enabled == true)
			{
				slats_activated = false;
				for index in range(0,len(m_ivyAircraft->m_slats_deploy_value))
				{
					if (abs(m_ivyAircraft->m_lf_slats - m_ivyAircraft->m_slats_deploy_value[index]) < m_ivyAircraft->m_slats_tolerance)
					{
						slats_activated = true;
						if (m_ivySlatsPosition.active == 0) && (m_ivyChannel.get_busy() == false)
						{
																																					m_ivySlatsPosition->Activate(m_time);
																																					m_SpellOutNumber(m_ivyAircraft->m_slats_deploy_pos[index]);
						}
					}
				}
				if (slats_activated == false)																										m_ivySlatsPosition->Deactivate(m_time);
			}
 
			////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////;
			// Flaps callout if configured in IvyAircraft ini file;
			if ((m_ivyAircraft->m_flaps_enabled == true) && (m_ivyAircraft->m_lf_flaps < m_ivyAircraft->m_flaps_tolerance))							m_ivyFlapsRetracted->Activate(m_time);
			else																																	m_ivyFlapsRetracted->Deactivate(m_time);
 
			if (m_ivyAircraft->m_flaps_enabled == true)
			{
				flaps_activated = false;
				for index in range(0,len(m_ivyAircraft->m_flaps_deploy_value))
				{
					if (abs(m_ivyAircraft->m_lf_flaps - m_ivyAircraft->m_flaps_deploy_value[index]) < m_ivyAircraft->m_flaps_tolerance)
					{
						flaps_activated = true;
						if (m_ivyFlapsPosition.active == 0) && (m_ivyChannel.get_busy() == false)
						{
																																					m_ivyFlapsPosition->Activate(m_time);
																																					m_SpellOutNumber(m_ivyAircraft->m_flaps_deploy_pos[index]);
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
			if ((m_li_on_ground == 0) && (m_lf_radio_alt > 100) && (m_lf_climb_rate > 100))															m_ivyArmLanding->Activate(m_time);
 
			if ((m_landing_detected == 1) && (m_landing_rated > 0) &&
			    (m_ivyArmLanding.played == 1) && (m_ivyConfig->m_passengers_enabled == true))														m_ivyApplause->Activate(m_time);
			else if (m_li_on_ground == 0)																											m_ivyApplause->Deactivate(m_time);
 
 
			if ((m_landing_detected == 1) && (m_landing_rated == 1) && (m_lf_ground_speed < m_ivyConfig->m_taxi_ground_speed_min) && (m_ivyArmLanding.played == 1))
			{
																																					m_ivyLandingXGood->Activate(m_time);
																																					if (m_ivyLandingXGood.played == 1)
																																					{
																																						m_EndOfFlightEvaluation();
																																						m_ivyArmLanding->Deactivate(m_time);
																																					}
			}
			else																																		m_ivyLandingXGood->Deactivate(m_time);
 
			if ((m_landing_detected == 1) && (m_landing_rated == 2) && (m_lf_ground_speed < m_ivyConfig->m_taxi_ground_speed_min) && (m_ivyArmLanding.played == 1))
			{
																																					m_ivyLandingGood->Activate(m_time);
																																					if (m_ivyLandingGood.played == 1)
																																					{
																																						m_EndOfFlightEvaluation();
																																						m_ivyArmLanding->Deactivate(m_time);
																																					}
			}
			else																																	m_ivyLandingGood->Deactivate(m_time);
 
			if ((m_landing_detected == 1) && (m_landing_rated == 3) && (m_lf_ground_speed < m_ivyConfig->m_taxi_ground_speed_min) && (m_ivyArmLanding.played == 1))
			{
																																					m_ivyLandingNormal->Activate(m_time);
																																					if (m_ivyLandingNormal.played == 1)
																																					{
																																						m_EndOfFlightEvaluation();
																																						m_ivyArmLanding->Deactivate(m_time);
			}
			else																																	m_ivyLandingNormal->Deactivate(m_time);
 
			if ((m_landing_detected == 1) && (m_landing_rated == 4) && (m_lf_ground_speed < m_ivyConfig->m_taxi_ground_speed_min) && (m_ivyArmLanding.played == 1))
			{
																																					m_ivyLandingBad->Activate(m_time);
																																					if (m_ivyLandingBad.played == 1)
																																					{
																																						m_EndOfFlightEvaluation();
																																						m_ivyArmLanding->Deactivate(m_time);
																																					}
			}
			else																																	m_ivyLandingBad->Deactivate(m_time);
 
 
			if ((m_landing_detected == 1) && (m_landing_rated == 5) && (m_lf_ground_speed < m_ivyConfig->m_taxi_ground_speed_min) && (m_ivyArmLanding.played == 1))
			{
																																					m_ivyLandingXBad->Activate(m_time);
																																					if (m_ivyLandingXBad.played == 1)
																																					{
																																						m_EndOfFlightEvaluation();
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
		m_passengerVolume = max (m_passengerVolume, abs(m_lf_roll) / 120);
		m_passengerVolume = max (m_passengerVolume, abs(m_lf_pitch) / 60);
 
		if (m_ivyConfig->m_passengers_enabled == true)		m_ivyPassengers.MakeScream(m_passengersScreaming, m_passengerVolume);
 
		return m_ivyConfig->m_data_rate;
 
	
 