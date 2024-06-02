'''
PI_Ivy.py 

This is Ivy, your new co-pilot. 

Gone are the days, when you could fly without transponder, enlighten the stratosphere with your landing lights and transform the passenger's meal in pottery to pottery in passenger meal upon landing, without anyone complaining.

Now that your airline has assigned your new co-pilot no mistake will go unnoticed. So make sure that you fly properly!

Installation:

You need the following to run Ivy:
- Python 2.7
- Sandy Barbour's Python Interface
- Pygame (for audio support)
- Copy the contents of this folder to X-Plane 11\Resources\plugins\PythonScripts


Python 2.7:
https://www.python.org/downloads/

Sandy Barbour's Python Interface
http://www.xpluginsdk.org/python_interface.htm

Pygame can be installed via Python:
https://www.pygame.org/wiki/GettingStarted

Personally, I use the following commands to install pygame. Remember to set your environment variables, so that python can be started from anywhere:
python -m ensurepip
python -m ensurepip --upgrade
pip install wheel
pip install wheel --upgrade
python -m pip install --upgrade pip
python -m pip install -U pygame --user

If you need support installing pygame or python interface, please refer to the pygame or x-plane community. 

Implemented Failure Detections:


1.	Bump on the ground
2.	Tire blown
3.	Hard braking
4.	Transponder not active when airborne
5.	Landing lights not on when close to the ground in the night
6.	Landing lights not off on high altitude
7.	Beacon lights not on when taxiing
8.	Nav lights not on when airborne
9.	Strobes not on when airborne
10.	Battery low
11.	Engine fire
12.	Engine flameout
13.	Engine ground failure
14.	Engine airborne failure
15.	Engine hot start
16.	Battery not on
17.	Cabin pressure raising too fast
18.	Cabin pressure raising extremely rapidly
19.	Bank angle pre-warning
20.	Bank angle too high
21.	Bank angle extremely high
22.	Pitch down pre-warning
23.	Pitch too low
24.	Vertical G Force high
25.	Vertical G Force very high
26.	Vertical G Force very, very high
27.	Vertical G Force too low
28.	Vertical G Force negative
29.	Barometric pressure not set accordingly while close to ground or taxiing (within tolerance)
30.	Barometric pressure not set to standard above transition altitude
31.	Ice airframe low
32.	Ice airframe high
33.	Ice pitot low
34.	Ice pitot high
35.	Ice propeller low
36.	Ice propeller high
37.	Ice cockpit window low
38.	Ice cockpit window high
39.	Cabin pressure low
40.	Cabin pressure too low to breath
41.	Birdstrike

Most variables needed to configure the tolerances of failure detection are editable in the Ivy.ini file.

Landing evaluation:

Most people only think about sink rate upon landing,
however, your passengers will not fly with you again,
if the g-forces upon landing are too high. 
No matter what your vertical speed was.

Or to put it in other words: 
A friend of my father was once happy like a little kid, because upon short runway on a greek island (I think it was Mykonos), he put his MD-80 with force to the ground (to ensure a no-flare situation), which resulted in serious pain in his back, broken ceramics of the passenger meals they had for their return flight, but the technician said after checking his data: "No, this was not a hard landing."

Well, the passengers might have other constraints than your technicians. Hence, the rating is the following:

Rating A:
	Sink rate < 100 ft/min
	Vertical forces < 1.5g
Rating B:
	Sink rate < 250 ft/min
	Vertical forces < 2g
Rating C:
	Sink rate < 400 ft/min
	Vertical forces < 3g
Rating D:
	Sink rate < 500 ft/min
	Vertical forces < 4g

Rating F:
	Everything else that did not trigger the X-Plane crash detection

A proper landing requires you to touch down more than 5 seconds. 
The rating includes all bounces within a 10 seconds window before your final touchdown.

Rating of your flight, depending on the errors you made:
0 		Errors	: Excellent
<5 		Errors	: Good (nice)
<10 		Errors	: Bad
>=10	  	Errors	: Horrible

Details of your highest sink rate and vertical g-forces are spoken upon landing (including all bounces in the evaluation)

Every landing is stored in your IvyLogbook.


Implemented callouts for all aircraft:
1.	Gear down callout (default: 100ft/min)
2.	Gear up callout
3.	60 knots callout (need to be compatible with smaller aircraft)
4.	Positive rate of climb
5.	Approaching Minimums (default: DH+100, DH must not be zero)
6.	Fasten Seatbelts
7.	Take Off Announcement on Non-Smoking Toogle or Commmand
8.	Landing Announcement on Non-Smoking Toogle or Commmand

Remember that Ivy is a Union member and will only perform one take off and one landing announcement per flight. However, she might consider doing it on multi-leg flights.

Implemented callouts for specific aircrafts:
- V-Speeds:
- V1
- VR
- V2
- V2 not achieved within 5 seconds after take off
- Flaps settings
- Slats settings

I supply multiple aircraft configuration files, but I can only implement and test them for aircraft I own. V-Speeds are currently available for CL 300 and Rotate MD-80. Unfortunately, CRJ-200 does not provide V-Speeds as datarefs.
- Standard MD-80
- Baron B58
- Cessna 172 SP Skyhawk
- Cirrus personal jet
- King Air C90
- Stinson L5 Sentinel
- Bombardier Challenger 300 for XP 11
- Rotate MD-80
- Jetstream 32
- CRJ-200
- Twin Otter Version 2
- Douglas C-47
- VSKYLABS DC-3
- Standard B747-400
- ERJ-140

You can open the data for slats and flaps positions via menu or command and create your own configuration file if you like
Commands
The following commands can be bound to your keyboard:
- Ivy/cabin_announcement: Ivy will make a Take-Off or Landing announcement.
- Ivy/say_baro: Say the current barometric pressure
- Ivy/say_wind: Say wind direction and speed 
- Ivy/show_output: Show the flaps/slats position for creating IvyAircraft_X.ini
- Ivy/reset_ivy: Resets Ivy. Recommended for multi leg flights.

Loogbook
Ivy remembers everything! She keeps precise tracking of all your mistakes and landings, noting every detail in your logbook. At least, most of it. You can open your loogbook in the plugins menu.

What else is there to say?

Remember that fun is subjective. If you don't like certain call outs, you can simply remove the individual mp3 file. No need to renumber the sound files, Ivy is not that picky. If you don't like the plug-in at all, go write your own.

All speech was generated using the Amazon Polly Text-to-Speech synthesis engine. You may generate your own sound files, if you want more proper call outs. You just need an AWS account, which is currently free of charge or any other speech synthesis software. However, I hereby deny the use of any speech that contains sexism, racism or fascism (there are always some idiots out there). 

Amazon offers a variety of voices and it is definitely on my ToDo list to generate different voice packs. Ivy is just the most funny voice that I decided to start with.

Any other sounds were taken from freesound.org, where all chosen sounds were using the creative commons 0 license. One sound was taken from GNU GPL licensed software (WeakAuras).

This software is published under the GNU General Public License v3. Remember that this gives you no warranty for functionality and by using this software, you yourself take the full responsibility for any fatalities caused by any bugs.

This software was not written by a professional pilot. It does not follow any real life procedures and is not safe for flight training. If you cause a fatal crash, because you followed Ivy's suggestions, we might consider your nomination for the Darwin Award.

Many animals were hurt during the creation of this product. Deere were hit on the runway, birds were soaked into the engine. Most of them are better now. Even though the turkey was too well done. 

Not all virtual pilots yet recovered from the injuries of countless crashes that were used to train Ivy's supervision talents. However, as freeware does not produce any income, we cannot afford to pay them a doctor. Yes, you should feel bad about that! 


'''


from XPLMDefs import *
from XPLMProcessing import *
from XPLMDisplay import *
from XPLMGraphics import *
from XPLMDataAccess import *
from XPLMUtilities import *
from XPLMNavigation import *
from XPLMPlugin import *
from XPLMMenus import *
from XPWidgetDefs import *
from XPWidgets import *
from XPStandardWidgets import *
from XPWidgetUtils import *
from math import *

import os.path
import random
import time
import ConfigParser
import pygame
import datetime

'''
##########################################################################################################################################################################################################
# MyIvyAircraft
#
# Legacy: Originally intended to use different classes for different aircrafts specific settings. 
# In the end, choose the way to use ini files, which allows users to do custom settings for their aircrafts
'''
class MyIvyAircraft(object):
		
		def __init__(self):
			self.vspeeds_enabled = False
			self.li_v1 = 0
			self.li_vr = 0
			self.li_v2 = 0
			self.name = "Standard"
			pass
		
		
		def UpdateData(self):
			pass

'''
##########################################################################################################################################################################################################
# MyIvyConfigAircraft
#
# Flap/Slats and V-Speeds are aircraft specific and defined by the author of the aircraft.
# This class reads the aircraft specific part from the IvyAircraft ini file and provides the necessary datarefs
'''
class MyIvyConfigAircraft(MyIvyAircraft):
		
		##########################################################################################################################################################################################################
		# __init__
		def __init__(self, aircraft_config_path):
		
			self.vspeeds_enabled = False
			self.li_v1 = 0
			self.li_vr = 0
			self.li_v2 = 0
			
			self.li_v1_data_ref = ""
			self.li_vr_data_ref = ""
			self.li_v2_data_ref = ""
			
			self.v_array = False
			self.v_array_size = 0 
			self.v1_pos = -1
			self.v2_pos = -1
			self.vr_pos = -1
			
			self.slats_enabled = False
			self.flaps_enabled = False
			self.lf_flaps = 0
			self.lf_slats = 0
			self.flaps_tolerance = 0
			self.slats_tolerance = 0
			self.lf_flaps_data_ref = ""
			self.lf_slats_data_ref = ""
			
			self.flaps_deploy_value = []
			self.flaps_deploy_pos = []
			self.flaps_count_max = 10
			self.slats_count_max = 10
			
			self.slats_deploy_value = []
			self.slats_deploy_pos = []
			
			self.dataref_init = False

			
			self.name = "UnconfiguredIvyAircraft"
			
			if (aircraft_config_path != ""): 
				self.ReadConfigFile(aircraft_config_path)
			pass
		
		##########################################################################################################################################################################################################
		# InitDataRefs
		#
		# The script is loaded before the aircraft specific settings are available
		# Hence, we load the datarefs outside the constructor
		def InitDataRefs(self):	
			
			self.dataref_init = True
			
			if (self.vspeeds_enabled == True):
				if (self.v_array == False):
					self.i_v1 = 				XPLMFindDataRef(self.li_v1_data_ref)
					self.i_vr = 				XPLMFindDataRef(self.li_vr_data_ref)
					self.i_v2 = 				XPLMFindDataRef(self.li_v2_data_ref)
				else:
					self.i_v1 = 				XPLMFindDataRef(self.li_v1_data_ref)
				
			if (self.slats_enabled == True):
				self.f_slats = 				XPLMFindDataRef(self.lf_slats_data_ref)
				
			if (self.flaps_enabled == True):
				self.f_flaps = 				XPLMFindDataRef(self.lf_flaps_data_ref)
			
			
			
			pass
		
		##########################################################################################################################################################################################################
		# UpdateData
		#
		# Get the aircraft specific data
		# Checks if dataref init was performed, if not InitDataRefs	is used
		def UpdateData(self):
		
			if (self.dataref_init == False):
				self.InitDataRefs()
		
			if (self.vspeeds_enabled == True):
				# Handling via direct variables, e.g., CL300
				if (self.v_array == False):
					if (self.li_v1_data_ref != ""):		self.li_v1 = 				XPLMGetDatai(self.i_v1)
					if (self.li_vr_data_ref != ""):		self.li_vr = 				XPLMGetDatai(self.i_vr)
					if (self.li_v2_data_ref != ""):		self.li_v2 = 				XPLMGetDatai(self.i_v2)
				# Handling for the Rotate MD80
				elif (self.li_v1_data_ref != ""):
					v_array = []
					XPLMGetDatavf(self.i_v1, v_array, 0, self.v_array_size)
					if (self.v1_pos != -1):	
						self.li_v1 = v_array[self.v1_pos]
					if (self.vr_pos != -1):	
						self.li_vr = v_array[self.vr_pos]
					if (self.v2_pos != -1):	
						self.li_v2 = v_array[self.v2_pos]
					
				
			if (self.slats_enabled == True):
				if (self.lf_slats_data_ref != ""):	self.lf_slats = XPLMGetDataf(self.f_slats)
			
			if (self.flaps_enabled == True):
				if (self.lf_flaps_data_ref != ""):	self.lf_flaps = XPLMGetDataf(self.f_flaps)			
				
			pass	
		
		
		##########################################################################################################################################################################################################
		# ReadConfigFile
		#
		# Read the configuration from the ini file
		def ReadConfigFile(self, aircraft_config_path):
			
			config = ConfigParser.SafeConfigParser()
			config.read(aircraft_config_path)
			
			try:	self.name 						= config.get("IVY_AIRCRAFT","aircraft_name")
			except:	pass
		
			try:	self.vspeeds_enabled 			= config.getboolean("IVY_AIRCRAFT","vspeeds_enabled")
			except:	pass
			
			try:	self.li_v1_data_ref 			= config.get("IVY_AIRCRAFT","v1_data_ref")
			except:	pass
			try:	self.li_vr_data_ref 			= config.get("IVY_AIRCRAFT","vr_data_ref")
			except:	pass
			try:	self.li_v2_data_ref 			= config.get("IVY_AIRCRAFT","v2_data_ref")
			except:	pass
			
			try:	self.v_array 					= config.getboolean("IVY_AIRCRAFT","v_array")
			except:	pass
			try:	self.v_array_size 				= config.getint("IVY_AIRCRAFT","v_array_size")
			except:	pass
			try:	self.v1_pos 					= config.getint("IVY_AIRCRAFT","v1_pos")
			except:	pass
			try:	self.v2_pos 					= config.getint("IVY_AIRCRAFT","v2_pos")
			except:	pass
			try:	self.vr_pos 					= config.getint("IVY_AIRCRAFT","vr_pos")
			except:	pass
			
			try:	self.li_v1 						= config.getint("IVY_AIRCRAFT","v1_static")
			except:	pass
			
			try:	self.li_v2 						= config.getint("IVY_AIRCRAFT","v2_static")
			except:	pass
			
			try:	self.li_vr						= config.getint("IVY_AIRCRAFT","vr_static")
			except:	pass
			
			try:	self.slats_enabled 				= config.getboolean("IVY_AIRCRAFT","slats_enabled")
			except:	pass
			try:	self.lf_slats_data_ref 			= config.get("IVY_AIRCRAFT","slats_data_ref")
			except:	pass
			try:	self.slats_tolerance 			= config.getfloat("IVY_AIRCRAFT","slats_tolerance")
			except:	pass
			
			try:	self.flaps_enabled 				= config.getboolean("IVY_AIRCRAFT","flaps_enabled")
			except:	pass
			try:	self.lf_flaps_data_ref 			= config.get("IVY_AIRCRAFT","flaps_data_ref")
			except:	pass
			try:	self.flaps_tolerance 			= config.getfloat("IVY_AIRCRAFT","flaps_tolerance")
			except:	pass
			
			for index_number in range(1,self.slats_count_max):
				try:	
						act_value					= config.getfloat("IVY_AIRCRAFT","slats_value_" + str(index_number))			
						act_position 				= config.getint("IVY_AIRCRAFT","slats_position_" + str(index_number))
						
						self.slats_deploy_value.append(act_value)
						self.slats_deploy_pos.append(act_position)
						
				except:	pass	
				
			for index_number in range(1,self.flaps_count_max):
				try:	
						act_value					= config.getfloat("IVY_AIRCRAFT","flaps_value_" + str(index_number))			
						act_position 				= config.getint("IVY_AIRCRAFT","flaps_position_" + str(index_number))
						
						self.flaps_deploy_value.append(act_value)
						self.flaps_deploy_pos.append(act_position)
						
				except:	pass
			
			
			#try:	self. 			= config.getfloat("IVY_AIRCRAFT","")
			#except:	pass
			
			
			pass
'''
##########################################################################################################################################################################################################
# MyIvyConfiguration
#
# Class to read, store and write configuration data from/to ini file
'''
class MyIvyConfiguration(object):
		
		def __init__(self):
		
			self.mp3_dir                    = "IvyMP3s"
			self.mp3_path 					= XPLMGetSystemPath() + "\\Resources\\plugins\\PythonScripts\\" + self.mp3_dir + "\\"
			self.number_path 				= self.mp3_path + "numbers\\"
			self.ini_path 					= XPLMGetSystemPath() + "\\Resources\\plugins\\PythonScripts\\Ivy.ini"
			self.config_path				= XPLMGetSystemPath() + "\\Resources\\plugins\\PythonScripts\\IvyConfig\\"
			self.logbook_path 				= XPLMGetSystemPath() + "\\Resources\\plugins\\PythonScripts\\IvyConfig\\IvyLogbook.txt"
		
			self.data_rate 					= 0.1
			self.disable_after_loading 		= 10 #debug, 20 = normal
			self.deact_after_queue 			= 0
			
			self.passengers_enabled         = True
		
			self.pos_rate_climb 			= 100
			self.ivy_ouch_g 				= 1.5
			self.brake_max_forward_g 		= 0.5
			self.alt_landing_lights_low 	= 1000
			self.alt_landing_lights_high 	= 10000
			self.night_world_light_precent 	= 0.5
			self.taxi_ground_speed_min 		= 5
			self.vis_is_fog 				= 5000
			self.cab_rate_low 				= -1500
			self.cab_rate_high 				= -2500
			self.cab_rate_reset_hysteresis 	= 500
			
			self.kt60_enabled  				= False
			self.kt80_enabled  				= True
			self.kt100_enabled 				= False
			
			self.bank_reset_low 			= 15
			self.bank_low 					= 28
			self.bank_high 					= 35
			self.bank_xhigh 				= 45
			
			self.pitch_reset_low 			= -5
			self.pitch_low 					= -10
			self.pitch_high 				= -20
			
			self.max_g_down_low_reset 		= 1.5
			self.max_g_down_low 			= 2
			self.max_g_down_high 			= 3
			self.max_g_down_xhigh 			= 5
			
			
			self.trans_alt 					= 18000
			self.trans_hysteresis 			= 1000
			self.baro_tolerance 			= 3
			self.baro_alt_low 				= 3000
			
			self.ice_low 					= 0.05
			self.ice_high 					= 0.2
			
			#self.carb_ice_low 				= 0.02
			#self.carb_ice_high 				= 0.10
			
			self.cab_press_low 				= 13000
			self.cab_press_high 			= 20000
			
			self.non_smoking_annoucetime    = 3
			self.decition_height_arm		= 500
			self.decition_height_plus		= 100
			
			self.log_window_pos_x			= 300
			self.log_window_pos_y			= 550
			self.log_window_height			= 350
			self.log_window_width			= 1300
			self.log_window_entries			= 15
			self.log_afc_name_length		= 40
			
			
		pass
		
		def WriteConfig(self):
			pass
			config = ConfigParser.SafeConfigParser()
			
			config.add_section("IVY_SETTINGS")
			
			config.set("IVY_SETTINGS","mp3_dir",str(self.mp3_dir))
			config.set("IVY_SETTINGS","passengers_enabled",str(self.passengers_enabled))
			
			config.set("IVY_SETTINGS","pos_rate_climb",str(self.pos_rate_climb))
			config.set("IVY_SETTINGS","ivy_ouch_g",str(self.ivy_ouch_g))
			config.set("IVY_SETTINGS","brake_max_forward_g",str(self.brake_max_forward_g))
			config.set("IVY_SETTINGS","alt_landing_lights_low",str(self.alt_landing_lights_low))
			config.set("IVY_SETTINGS","alt_landing_lights_high",str(self.alt_landing_lights_high))
			config.set("IVY_SETTINGS","night_world_light_precent",str(self.night_world_light_precent))
			config.set("IVY_SETTINGS","taxi_ground_speed_min",str(self.taxi_ground_speed_min))
			config.set("IVY_SETTINGS","vis_is_fog",str(self.vis_is_fog))
			config.set("IVY_SETTINGS","cab_rate_low",str(self.cab_rate_low))
			config.set("IVY_SETTINGS","cab_rate_high",str(self.cab_rate_high))
			config.set("IVY_SETTINGS","cab_rate_reset_hysteresis",str(self.cab_rate_reset_hysteresis))
			config.set("IVY_SETTINGS","bank_reset_low",str(self.bank_reset_low))
			config.set("IVY_SETTINGS","bank_low",str(self.bank_low))
			config.set("IVY_SETTINGS","bank_high",str(self.bank_high))
			config.set("IVY_SETTINGS","bank_xhigh",str(self.bank_xhigh))
			config.set("IVY_SETTINGS","pitch_reset_low",str(self.pitch_reset_low))
			config.set("IVY_SETTINGS","pitch_low",str(self.pitch_low))
			config.set("IVY_SETTINGS","pitch_high",str(self.pitch_high))
			config.set("IVY_SETTINGS","max_g_down_low_reset",str(self.max_g_down_low_reset))
			config.set("IVY_SETTINGS","max_g_down_low",str(self.max_g_down_low))
			config.set("IVY_SETTINGS","max_g_down_high",str(self.max_g_down_high))
			config.set("IVY_SETTINGS","max_g_down_xhigh",str(self.max_g_down_xhigh))
			config.set("IVY_SETTINGS","trans_alt",str(self.trans_alt))
			config.set("IVY_SETTINGS","trans_hysteresis",str(self.trans_hysteresis))
			config.set("IVY_SETTINGS","baro_tolerance",str(self.baro_tolerance))
			config.set("IVY_SETTINGS","baro_alt_low",str(self.baro_alt_low))
			config.set("IVY_SETTINGS","ice_low",str(self.ice_low))
			config.set("IVY_SETTINGS","ice_high",str(self.ice_high))
			config.set("IVY_SETTINGS","cab_press_low",str(self.cab_press_low))
			config.set("IVY_SETTINGS","cab_press_high",str(self.cab_press_high))
			config.set("IVY_SETTINGS","non_smoking_annoucetime",str(self.non_smoking_annoucetime))
			config.set("IVY_SETTINGS","decition_height_arm",str(self.decition_height_arm))
			config.set("IVY_SETTINGS","decition_height_plus",str(self.decition_height_plus))
			
			config.set("IVY_SETTINGS","kt60_enabled",str(self.kt60_enabled))
			config.set("IVY_SETTINGS","kt80_enabled",str(self.kt80_enabled))
			config.set("IVY_SETTINGS","kt100_enabled",str(self.kt100_enabled))
			
			config.set("IVY_SETTINGS","log_window_pos_x",str(self.log_window_pos_x))
			config.set("IVY_SETTINGS","log_window_pos_y",str(self.log_window_pos_y))
			config.set("IVY_SETTINGS","log_window_height",str(self.log_window_height))
			config.set("IVY_SETTINGS","log_window_width",str(self.log_window_width))
			config.set("IVY_SETTINGS","log_window_entries",str(self.log_window_entries))
			config.set("IVY_SETTINGS","log_afc_name_length",str(self.log_afc_name_length))
			
			with open(self.ini_path , 'wb') as configfile: config.write(configfile)
				

		
		def ReadConfig(self):
			pass
			config = ConfigParser.SafeConfigParser()
			config.read(self.ini_path)
				
			
			try:	self.mp3_dir 					= config.get("IVY_SETTINGS","mp3_dir")
			except:	pass
			
			try:	self.passengers_enabled			= config.getboolean("IVY_SETTINGS","passengers_enabled")
			except:	pass
			
			try:	self.pos_rate_climb 			= config.getfloat("IVY_SETTINGS","pos_rate_climb")
			except:	pass
			try:	self.ivy_ouch_g 				= config.getfloat("IVY_SETTINGS","ivy_ouch_g")
			except:	pass
			try:	self.brake_max_forward_g 		= config.getfloat("IVY_SETTINGS","brake_max_forward_g")
			except:	pass
			try:	self.alt_landing_lights_low 	= config.getfloat("IVY_SETTINGS","alt_landing_lights_low")
			except:	pass
			try:	self.alt_landing_lights_high 	= config.getfloat("IVY_SETTINGS","alt_landing_lights_high")
			except:	pass
			try:	self.night_world_light_precent 	= config.getfloat("IVY_SETTINGS","night_world_light_precent")
			except:	pass
			try:	self.taxi_ground_speed_min 		= config.getfloat("IVY_SETTINGS","taxi_ground_speed_min")
			except:	pass
			try:	self.vis_is_fog 				= config.getfloat("IVY_SETTINGS","vis_is_fog")
			except:	pass
			try:	self.cab_rate_low 				= config.getfloat("IVY_SETTINGS","cab_rate_low")
			except:	pass
			try:	self.cab_rate_high 				= config.getfloat("IVY_SETTINGS","cab_rate_high")
			except:	pass
			try:	self.cab_rate_reset_hysteresis 	= config.getfloat("IVY_SETTINGS","cab_rate_reset_hysteresis")
			except:	pass
			
			try:	self.bank_reset_low 			= config.getfloat("IVY_SETTINGS","bank_reset_low")
			except:	pass
			try:	self.bank_low 					= config.getfloat("IVY_SETTINGS","bank_low")
			except:	pass
			try:	self.bank_high 					= config.getfloat("IVY_SETTINGS","bank_high")
			except:	pass
			try:	self.bank_xhigh 				= config.getfloat("IVY_SETTINGS","bank_xhigh")
			except:	pass
			
			try:	self.pitch_reset_low 			= config.getfloat("IVY_SETTINGS","pitch_reset_low")
			except:	pass
			try:	self.pitch_low 					= config.getfloat("IVY_SETTINGS","pitch_low")
			except:	pass
			try:	self.pitch_high 				= config.getfloat("IVY_SETTINGS","pitch_high")
			except:	pass
			
			try:	self.max_g_down_low_reset 		= config.getfloat("IVY_SETTINGS","max_g_down_low_reset")
			except:	pass
			try:	self.max_g_down_low 			= config.getfloat("IVY_SETTINGS","max_g_down_low")
			except:	pass
			try:	self.max_g_down_high 			= config.getfloat("IVY_SETTINGS","max_g_down_high")
			except:	pass
			try:	self.max_g_down_xhigh 			= config.getfloat("IVY_SETTINGS","max_g_down_xhigh")
			except:	pass
			
			
			try:	self.trans_alt 					= config.getfloat("IVY_SETTINGS","trans_alt")
			except:	pass
			try:	self.trans_hysteresis 			= config.getfloat("IVY_SETTINGS","trans_hysteresis")
			except:	pass
			try:	self.baro_tolerance 			= config.getfloat("IVY_SETTINGS","baro_tolerance")
			except:	pass
			try:	self.baro_alt_low 				= config.getfloat("IVY_SETTINGS","baro_alt_low")
			except:	pass
			
			try:	self.ice_low 					= config.getfloat("IVY_SETTINGS","ice_low")
			except:	pass
			try:	self.ice_high 					= config.getfloat("IVY_SETTINGS","ice_high")
			except:	pass
			
			try:	self.cab_press_low 				= config.getfloat("IVY_SETTINGS","cab_press_low")
			except:	pass
			try:	self.cab_press_high 			= config.getfloat("IVY_SETTINGS","cab_press_high")
			except:	pass
			
			try:	self.non_smoking_annoucetime 	= config.getfloat("IVY_SETTINGS","non_smoking_annoucetime")
			except:	pass
			
			try:	self.decition_height_arm 		= config.getfloat("IVY_SETTINGS","decition_height_arm")
			except:	pass
			try:	self.decition_height_plus 		= config.getfloat("IVY_SETTINGS","decition_height_plus")
			except:	pass
			
			try:	self.kt60_enabled 				= config.getboolean("IVY_SETTINGS","kt60_enabled")
			except:	pass
			try:	self.kt80_enabled 				= config.getboolean("IVY_SETTINGS","kt80_enabled")
			except:	pass
			try:	self.kt100_enabled 				= config.getboolean("IVY_SETTINGS","kt100_enabled")
			except:	pass
			
			
			
			try:	self.log_window_pos_x 			= config.getfloat("IVY_SETTINGS","log_window_pos_x")
			except:	pass
			try:	self.log_window_pos_y 			= config.getfloat("IVY_SETTINGS","log_window_pos_y")
			except:	pass
			try:	self.log_window_height 			= config.getfloat("IVY_SETTINGS","log_window_height")
			except:	pass
			try:	self.log_window_width 			= config.getfloat("IVY_SETTINGS","log_window_width")
			except:	pass
			try:	self.log_window_entries 		= config.getfloat("IVY_SETTINGS","log_window_entries")
			except:	pass
			try:	self.log_afc_name_length 		= config.getfloat("IVY_SETTINGS","log_afc_name_length")
			except:	pass
			
			self.mp3_path 					= XPLMGetSystemPath() + "\\Resources\\plugins\\PythonScripts\\" + self.mp3_dir + "\\"
			self.number_path 				= self.mp3_path + "numbers\\"
			
			
			
'''		
##########################################################################################################################################################################################################
# MyIvyResponse
#
# This class stores the corresponding response mp3 files, selects a random file upon play and checks for activation, deactivation time		
'''			

class MyIvyResponse(object):
	def __init__(self, event_name,mp3_path, active_on_load, minimum_occ,deactivate_time, is_error, ivy_object_list, sound_channel):
	
		self.queue_output = 0
		self.event_name = event_name
		self.is_error = is_error
		self.minimum_occ = minimum_occ
		self.deactivating = 0
		self.deactivate_time = deactivate_time
		self.time_activated = 0
		self.time_deactivated = 0
		self.time_deactivating = 0
		self.time_active = 0
		self.active = active_on_load
		self.played = active_on_load
		self.active_on_load = active_on_load
		self.mute = 0
		self.error_count = 0
		self.channel_number = sound_channel
		
		self.ivyChannel = pygame.mixer.Channel(sound_channel)
		self.sounds = []
		
		self.play_files = []
		
		ivy_object_list.append(self)
		
		#Append all file names that match our event
		for file_number in range(1,21):
			if (os.path.isfile(mp3_path + event_name + "_" + str(file_number) + ".ogg")):
				self.play_files.append(mp3_path + event_name + "_" + str(file_number) + ".ogg")
				actsound = pygame.mixer.Sound(mp3_path + event_name + "_" + str(file_number) + ".ogg")
				self.sounds.append(actsound)
		pass

	def Activate(self, time):
		if (self.active == 0): 
			self.time_activated = time
		
		self.active = 1
		
		self.time_active = time - self.time_activated
		
		if ((self.time_active >= self.minimum_occ) and (self.played == 0)):
			self.Play()
		pass
		
	def SetAsPlayed(self, time):
		self.active = 1
		self.played = 1
		self.time_activated = time
		pass
		
	def Deactivate(self, time):
		if (self.active == 0): 
			return
		
		if (self.deactivating == 0):
			self.time_deactivated = time
		
		self.deactivating = 1
		self.time_deactivating = time - self.time_deactivated
		
		if (self.time_deactivating >= self.deactivate_time):
			self.deactivating = 0
			self.time_deactivated = 0
			self.active = 0
			self.time_activated = 0
			self.time_active = 0
			self.played = 0
		pass
		
		
	def Play(self):
		if (len(self.play_files) == 0):
			self.played = 1
			return 1

		if ((self.queue_output == 0) and (self.ivyChannel.get_busy() == True)):
			return 0
		
		if (self.played == 1):
			return 0 
		
		if (self.is_error > 0): 
			self.error_count = self.error_count + 1
		
		if (len(self.play_files) > 1):	
			random_number = random.randint(0,len(self.play_files)-1)
		else:
			random_number = 0
		
		self.played = 1
		if (len(self.play_files) > 0):
			if (self.queue_output == 1):
				self.ivyChannel.queue(self.play_files[random_number])
			else:
				self.ivyChannel.play(self.sounds[random_number])
			return 1
'''
##########################################################################################################################################################################################################
# IvyLandingDetection
#
# Small class to store our landing data, rate the landing and check if the touchdown was within the given time frame
'''			
class IvyLandingDetection(object):
	def __init__(self, time, sink_rate, g_normal, g_side, g_forward):
		self.time = time
		self.sink_rate = sink_rate
		self.g_normal = g_normal
		self.g_side = g_side
		self.g_forward = g_forward
		self.rating = 0
		self.RateLanding()
		pass
		
	def RateLanding(self):
		if ((self.sink_rate > -100) and (self.g_normal < 1.5)):		self.rating = 1
		elif (self.sink_rate > -250) and (self.g_normal < 2):		self.rating = 2
		elif (self.sink_rate > -400) and (self.g_normal < 3):		self.rating = 3
		elif (self.sink_rate > -500) and (self.g_normal < 4):		self.rating = 4
		else:														self.rating = 5
		pass
		
	def GetCurrentRate(self, last_landing_time, max_time):
		if ((last_landing_time - self.time) < max_time):
			return self.rating
		else:
			return 0
'''			
##########################################################################################################################################################################################################
# IvyPassengers
#
# Small class to make our passenger noise
'''			
class IvyPassengers(object):
	def __init__(self, ivyConfig, channel):
		self.ivyConfig = ivyConfig
		self.is_screaming = False
		#self.fading = False
		
		self.passengerChannel 	= pygame.mixer.Channel(channel)
		self.passengerChannel.stop()
		self.screamSound = pygame.mixer.Sound(self.ivyConfig.mp3_path + "passenger_screams.ogg")
		
		pass
		
	def MakeScream(self, screaming, volume):
		volume = min(1 , volume)
		volume = max(0.3 , volume)
		
		self.passengerChannel.set_volume(volume)
		
		if ((screaming == True) and (self.is_screaming == False)):
			#self.fading = False
			self.is_screaming = True
			self.passengerChannel.play(self.screamSound, -1, 0, 1000)
		
		elif ((screaming == False) and (self.is_screaming == True)):
		#elif (screaming == False):
			self.is_screaming = False
			self.passengerChannel.fadeout(500)
		pass
		

			
'''
#######################################################################################################################################################################################################################################################################################
#######################################################################################################################################################################################################################################################################################
#######################################################################################################################################################################################################################################################################################
#######################################################################################################################################################################################################################################################################################
#######################################################################################################################################################################################################################################################################################
# PythonInterface
#
# Main class where we do our stuff		
'''		
class PythonInterface:

	##########################################################################################################################################################################################################
	# ResetIvy
	#
	# Resets all datasets and reloads the IvyAircraft depending upon the aircraft name given in the acf file

	def ResetIvy(self):
		self.time = 0
		self.landing_detected = 0
		self.landing_rated = 0
		self.landing_bounces = 0
		self.landing_g_normal = 0
		self.landing_sink_rate = 0
		self.aircraft_crashed = 0
		self.pressure_said = 0
		self.non_smoking_old = 0
		self.non_smoking_event = -100 # Needed for Startup with Non-Smoking enabled.
		
		self.airport_departure = "NONE"
		self.airport_departure_temp = "NONE"
		self.airport_arrival   = "NONE"
		self.time_departure = 0
		
		
		self.play_mp3_queue = []
		self.ivy_landing_list = []
		

		
		for obj_number in range(0,len(self.ivy_object_list)):
			self.ivy_object_list[obj_number].error_count = 0
			self.ivy_object_list[obj_number].active = self.ivy_object_list[obj_number].active_on_load
			self.ivy_object_list[obj_number].played = self.ivy_object_list[obj_number].active_on_load
		
		lba_acf_descrip= []
		lba_acf_tailnumber = [] 

		XPLMGetDatab(self.s_acf_descrip,lba_acf_descrip,0,240) 	
		XPLMGetDatab(self.s_acf_tailnumber,lba_acf_tailnumber,0,40)
		
		self.ls_acf_descrip = str(lba_acf_descrip) + str(lba_acf_tailnumber)
		
		self.ivyAircraft = self.ivy_aircraft_list[0]
		
		for index in range(0,len(self.ivy_aircraft_list)):
			if (self.ivy_aircraft_list[index].name in self.ls_acf_descrip):
				self.ivyAircraft = self.ivy_aircraft_list[index]

				
		
		pass
		
	##########################################################################################################################################################################################################
	# XPluginStart
	#
	# Startup function for loading error responses and loading datarefs
	
	def XPluginStart(self):

		self.Name = "Ivy"
		self.Sig =  "ka.Python.Ivy"
		self.Desc = "The nagging Co-Pilot"
		
		self.play_mp3_queue = []
		self.ivy_object_list = []
		self.ivy_landing_list = []
		self.ivy_aircraft_list = []
		
		#self.ivyAircraft = MyIvyAircraft()
		
		self.aircraft_loaded = 0
		self.plugin_enabled = 0
		self.deact_queue = 0
		

		
		self.no_aircraft = True 
		self.draw_window = 0
		self.MenuVSpeedsShow = 0
		self.logbook_index = 0
		

		
		self.ivyConfig = MyIvyConfiguration()
		self.ivyConfig.ReadConfig()
		
		
		
		self.ivy_aircraft_list.append(MyIvyConfigAircraft(""))
		self.ivyAircraft = self.ivy_aircraft_list[0]
		
		for index in range(1,100):
			aircraft_ini_path = self.ivyConfig.config_path + "IvyAircraft_" + str(index) + ".ini"
			if (os.path.isfile(aircraft_ini_path)):
				self.ivy_aircraft_list.append(MyIvyConfigAircraft(aircraft_ini_path))

		
		
		
		#self.startup = 1
		
		self.s_acf_descrip = 			XPLMFindDataRef("sim/aircraft/view/acf_descrip") 
		self.s_acf_tailnumber = 		XPLMFindDataRef("sim/aircraft/view/acf_tailnum") 
		self.ResetIvy()
		#self.Clicked = 0
		
		self.outputPath = self.ivyConfig.config_path + "IvyLog.txt"
		self.OutputFile = open(self.outputPath, 'w')
		
		# Init the pygame mixer
		pygame.mixer.init()
		self.ivyChannel 		= pygame.mixer.Channel(0)
		
		self.ivyPassengers = IvyPassengers(self.ivyConfig, 1)
		
		
		random.seed()
		
		
		
		#self.END_MUSIC_EVENT = pygame.USEREVENT + 0    # ID for music Event
		#pygame.mixer.music.set_endevent(END_MUSIC_EVENT)
		
		#												#Name						PATH						DEACT_ON_LOAD		MINIMUM_OCC_TIME		DEACT_TIME				IS_ERROR			IVY_OBJECT_LIST		CHANNEL
		self.ivyOuch = 					MyIvyResponse(	"ouch", 					self.ivyConfig.mp3_path,	0,				0, 						0, 						0,					self.ivy_object_list, 	0)
		self.ivyPosRateClimb = 			MyIvyResponse(	"pos_climb", 				self.ivyConfig.mp3_path,	0,				1, 						20, 					0,					self.ivy_object_list, 	0)
		self.ivyTyre = 					MyIvyResponse(	"tyre", 					self.ivyConfig.mp3_path,	0,				0, 						0, 						1,					self.ivy_object_list, 	0)
		self.ivyBrake = 				MyIvyResponse(	"brake", 					self.ivyConfig.mp3_path,	0,				2, 						10,						1,					self.ivy_object_list, 	0)
		self.ivyTransponder = 			MyIvyResponse(	"transponder", 				self.ivyConfig.mp3_path,	0,				120, 					0, 						1,					self.ivy_object_list, 	0)
		self.ivyCockpitLights = 		MyIvyResponse(	"cockpit_lights", 			self.ivyConfig.mp3_path,	0,				5, 						0, 						1,					self.ivy_object_list, 	0) # TBD
		self.ivyLandingLights = 		MyIvyResponse(	"landing_lights", 			self.ivyConfig.mp3_path,	0,				0, 						0, 						1,					self.ivy_object_list, 	0)
		
		self.ivyGearUp = 				MyIvyResponse(	"gear_up", 					self.ivyConfig.mp3_path,	1,				1, 						0, 						0,					self.ivy_object_list, 	0)
		self.ivyGearDown = 				MyIvyResponse(	"gear_down", 				self.ivyConfig.mp3_path,	1,				1, 						0, 						0,					self.ivy_object_list, 	0)
		
		self.ivyBeaconLights = 			MyIvyResponse(	"beacon", 					self.ivyConfig.mp3_path,	0,				20, 					0, 						1,					self.ivy_object_list, 	0)
		self.ivyNavLights = 			MyIvyResponse(	"nav_lights", 				self.ivyConfig.mp3_path,	0,				50, 					0, 						1,					self.ivy_object_list, 	0)
		self.ivyStrobes = 				MyIvyResponse(	"strobes", 					self.ivyConfig.mp3_path,	0,				200, 					0, 						1,					self.ivy_object_list, 	0)
		self.ivySkidTyres = 			MyIvyResponse(	"skid_tyres", 				self.ivyConfig.mp3_path,	0,				5, 						0, 						0,					self.ivy_object_list, 	0)
		self.ivyBatteryOut = 			MyIvyResponse(	"battery_out", 				self.ivyConfig.mp3_path,	0,				0, 						0, 						1,					self.ivy_object_list, 	0)
		self.ivyEngineFire = 			MyIvyResponse(	"engine_fire", 				self.ivyConfig.mp3_path,	0,				0, 						0, 						1,					self.ivy_object_list, 	0)
		self.ivyEngineFlameout = 		MyIvyResponse(	"engine_flameout", 			self.ivyConfig.mp3_path,	0,				0, 						0, 						1,					self.ivy_object_list, 	0)
		self.ivyEngineFailureGround = 	MyIvyResponse(	"engine_failure_ground", 	self.ivyConfig.mp3_path,	1,				0, 						0, 						1,					self.ivy_object_list, 	0)
		self.ivyEngineFailureAir = 		MyIvyResponse(	"engine_failure_air", 		self.ivyConfig.mp3_path,	0,				0, 						0, 						1,					self.ivy_object_list, 	0)
		self.ivyEngineHotStart = 		MyIvyResponse(	"engine_hot", 				self.ivyConfig.mp3_path,	0,				0, 						0, 						1,					self.ivy_object_list, 	0)
		self.ivyBirdStrike = 			MyIvyResponse(	"bird", 					self.ivyConfig.mp3_path,	0,				0, 						0, 						1,					self.ivy_object_list, 	0)
		
		self.ivyOverspeedFlaps = 		MyIvyResponse(	"overspeed_flaps", 			self.ivyConfig.mp3_path,	1,				0, 						1, 						1,					self.ivy_object_list, 	0)
		self.ivyOverspeedGear = 		MyIvyResponse(	"overspeed_gear", 			self.ivyConfig.mp3_path,	1,				0, 						1, 						1,					self.ivy_object_list, 	0)
		self.ivyOverspeedAircraft = 	MyIvyResponse(	"overspeed_aircraft", 		self.ivyConfig.mp3_path,	1,				1, 						1, 						1,					self.ivy_object_list, 	0)
		self.ivyStall = 				MyIvyResponse(	"stall", 					self.ivyConfig.mp3_path,	1,				0, 						5, 						1,					self.ivy_object_list, 	0)
		
		self.ivyNoBatt = 				MyIvyResponse(	"no_batt", 					self.ivyConfig.mp3_path,	0,				180, 					0, 						0,					self.ivy_object_list, 	0)
		self.ivyHelloSun = 				MyIvyResponse(	"hello_sun", 				self.ivyConfig.mp3_path,	0,				15, 					0, 						0,					self.ivy_object_list, 	0)
		self.ivyHelloRain = 			MyIvyResponse(	"hello_rain", 				self.ivyConfig.mp3_path,	0,				15, 					0, 						0,					self.ivy_object_list, 	0)
		self.ivyHelloFog = 				MyIvyResponse(	"hello_fog", 				self.ivyConfig.mp3_path,	0,				15, 					0, 						0,					self.ivy_object_list, 	0)
		self.ivyHelloNormal = 			MyIvyResponse(	"hello_normal", 			self.ivyConfig.mp3_path,	0,				15, 					0, 						0,					self.ivy_object_list, 	0)
		self.ivyCabinDownNormal = 		MyIvyResponse(	"cabin_down_normal", 		self.ivyConfig.mp3_path,	0,				2, 						0, 						1,					self.ivy_object_list, 	0)
		self.ivyCabinDownFast = 		MyIvyResponse(	"cabin_down_fast", 			self.ivyConfig.mp3_path,	0,				2, 						0, 						1,					self.ivy_object_list, 	0)
		self.ivyBankNormal = 			MyIvyResponse(	"bank_normal", 				self.ivyConfig.mp3_path,	0,				0, 						0, 						0,					self.ivy_object_list, 	0)
		self.ivyBankHigh = 				MyIvyResponse(	"bank_high", 				self.ivyConfig.mp3_path,	0,				2, 						0, 						1,					self.ivy_object_list, 	0)
		self.ivyBankXHigh = 			MyIvyResponse(	"bank_xhigh", 				self.ivyConfig.mp3_path,	0,				2, 						0, 						1,					self.ivy_object_list, 	0)
		self.ivyPitchDownNormal = 		MyIvyResponse(	"pitch_down_normal", 		self.ivyConfig.mp3_path,	0,				0, 						0, 						0,					self.ivy_object_list, 	0)
		self.ivyPitchDownHigh = 		MyIvyResponse(	"pitch_down_high", 			self.ivyConfig.mp3_path,	0,				2, 						0, 						1,					self.ivy_object_list, 	0)
		self.ivyGNormalFlightNormal = 	MyIvyResponse(	"g_normal_flight_normal", 	self.ivyConfig.mp3_path,	0,				2, 						0, 						0,					self.ivy_object_list, 	0)
		self.ivyGNormalFlightHigh = 	MyIvyResponse(	"g_normal_flight_high", 	self.ivyConfig.mp3_path,	0,				2, 						0, 						1,					self.ivy_object_list, 	0)
		self.ivyGNormalFlightXHigh = 	MyIvyResponse(	"g_normal_flight_xhigh", 	self.ivyConfig.mp3_path,	0,				2, 						0, 						1,					self.ivy_object_list, 	0)
		self.ivyGNormalNegativeLow = 	MyIvyResponse(	"g_normal_negative_low", 	self.ivyConfig.mp3_path,	0,				0.5, 					0, 						1,					self.ivy_object_list, 	0)
		self.ivyGNormalNegativeHigh = 	MyIvyResponse(	"g_normal_negative_high", 	self.ivyConfig.mp3_path,	0,				0.5, 					0, 						1,					self.ivy_object_list, 	0)
		self.ivyTurbulenceNormal = 		MyIvyResponse(	"turbulence_normal", 		self.ivyConfig.mp3_path,	0,				20, 					0, 						0,					self.ivy_object_list, 	0)
		self.ivyTurbolenceHigh = 		MyIvyResponse(	"turbulence_high", 			self.ivyConfig.mp3_path,	0,				20, 					0, 						0,					self.ivy_object_list, 	0)
		
		self.ivyLandingXGood = 			MyIvyResponse(	"landing_xgood", 			self.ivyConfig.mp3_path,	0,				5, 						0, 						0,					self.ivy_object_list, 	0)
		self.ivyLandingGood = 			MyIvyResponse(	"landing_good", 			self.ivyConfig.mp3_path,	0,				5, 						0, 						0,					self.ivy_object_list, 	0)
		self.ivyLandingNormal = 		MyIvyResponse(	"landing_normal", 			self.ivyConfig.mp3_path,	0,				5, 						0, 						0,					self.ivy_object_list, 	0) 
		self.ivyLandingBad = 			MyIvyResponse(	"landing_bad", 				self.ivyConfig.mp3_path,	0,				5, 						0, 						0,					self.ivy_object_list, 	0)
		self.ivyLandingXBad = 			MyIvyResponse(	"landing_xbad", 			self.ivyConfig.mp3_path,	0,				5, 						0, 						1,					self.ivy_object_list, 	0)
		
		self.ivyBaroLow = 				MyIvyResponse(	"baro_low", 				self.ivyConfig.mp3_path,	0,				5, 						0, 						1,					self.ivy_object_list, 	0)
		self.ivyBaroGround = 			MyIvyResponse(	"baro_low", 				self.ivyConfig.mp3_path,	0,				60,						0, 						0,					self.ivy_object_list, 	0)
		self.ivyBaroHigh = 				MyIvyResponse(	"baro_high", 				self.ivyConfig.mp3_path,	0,				120,					0, 						1,					self.ivy_object_list, 	0)
		self.ivyLandingLightsHigh = 	MyIvyResponse(	"landing_lights_high", 		self.ivyConfig.mp3_path,	0,				30, 					0, 						1,					self.ivy_object_list, 	0)
		self.ivyRotate = 				MyIvyResponse(	"rotate", 					self.ivyConfig.mp3_path,	0,				0, 						0, 						0,					self.ivy_object_list, 	0)
		self.ivy60kt = 					MyIvyResponse(	"60kt", 					self.ivyConfig.mp3_path,	0,				0, 						5, 						0,					self.ivy_object_list, 	0)
		self.ivy80kt = 					MyIvyResponse(	"80kt", 					self.ivyConfig.mp3_path,	0,				0, 						5, 						0,					self.ivy_object_list, 	0)
		self.ivy100kt = 				MyIvyResponse(	"100kt", 					self.ivyConfig.mp3_path,	0,				0, 						5, 						0,					self.ivy_object_list, 	0)
		self.ivyV1 = 					MyIvyResponse(	"v1", 						self.ivyConfig.mp3_path,	0,				0, 						5, 						0,					self.ivy_object_list, 	0)
		self.ivyVR = 					MyIvyResponse(	"vr", 						self.ivyConfig.mp3_path,	0,				0, 						5, 						0,					self.ivy_object_list, 	0)
		self.ivyBelowV2 = 				MyIvyResponse(	"below_v2", 				self.ivyConfig.mp3_path,	0,				5, 						5, 						1,					self.ivy_object_list, 	0)
		self.ivyAboveV2 = 				MyIvyResponse(	"above_v2", 				self.ivyConfig.mp3_path,	0,				0, 						5, 						0,					self.ivy_object_list, 	0)
		self.ivyFlapsRetracted = 		MyIvyResponse(	"flaps_retracted", 			self.ivyConfig.mp3_path,	1,				0, 						0, 						0,					self.ivy_object_list, 	0)
		self.ivySlatsRetracted = 		MyIvyResponse(	"slats_retracted", 			self.ivyConfig.mp3_path,	1,				0, 						0, 						0,					self.ivy_object_list, 	0)
		self.ivyFlapsPosition = 		MyIvyResponse(	"flaps", 					self.ivyConfig.mp3_path,	1,				0, 						0, 						0,					self.ivy_object_list, 	0)
		self.ivySlatsPosition = 		MyIvyResponse(	"slats", 					self.ivyConfig.mp3_path,	1,				0, 						0, 						0,					self.ivy_object_list, 	0)
		
		self.ivyCrash = 				MyIvyResponse(	"crash", 					self.ivyConfig.mp3_path,	0,				3, 						0, 						1,					self.ivy_object_list, 	0)
		# TODO
		
		self.ivyPressureLow = 			MyIvyResponse(	"pressure_low", 			self.ivyConfig.mp3_path,	0,				5, 						0, 						1,					self.ivy_object_list, 	0)
		self.ivyPressureXLow = 			MyIvyResponse(	"pressure_xlow", 			self.ivyConfig.mp3_path,	0,				1, 						0, 						1,					self.ivy_object_list, 	0)
		
		self.ivyIceWindowLow = 			MyIvyResponse(	"ice_window_low", 			self.ivyConfig.mp3_path,	0,				20, 					0, 						1,					self.ivy_object_list, 	0)
		self.ivyIceWindowHigh = 		MyIvyResponse(	"ice_window_high", 			self.ivyConfig.mp3_path,	0,				20, 					0, 						1,					self.ivy_object_list, 	0)
		self.ivyIcePropellerLow = 		MyIvyResponse(	"ice_propeller_low", 		self.ivyConfig.mp3_path,	0,				40, 					0, 						1,					self.ivy_object_list, 	0) # Inlet ice
		self.ivyIcePropellerHigh = 		MyIvyResponse(	"ice_propeller_high", 		self.ivyConfig.mp3_path,	0,				40, 					0, 						1,					self.ivy_object_list, 	0)
		self.ivyIcePitotLow = 			MyIvyResponse(	"ice_pitot_low", 			self.ivyConfig.mp3_path,	0,				60, 					0, 						1,					self.ivy_object_list, 	0) # Ice low bei 0.05
		self.ivyIcePitotHigh = 			MyIvyResponse(	"ice_pitot_high", 			self.ivyConfig.mp3_path,	0,				60, 					0, 						1,					self.ivy_object_list, 	0)
		self.ivyIceAirframeLow = 		MyIvyResponse(	"ice_airframe_low", 		self.ivyConfig.mp3_path,	0,				100, 					0, 						1,					self.ivy_object_list, 	0) # Ice high bei 0.15
		self.ivyIceAirframeHigh = 		MyIvyResponse(	"ice_airframe_high", 		self.ivyConfig.mp3_path,	0,				100, 					0, 						1,					self.ivy_object_list, 	0)
		
		self.ivyAnnounceTakeOff = 		MyIvyResponse(	"takeoff", 					self.ivyConfig.mp3_path,	0,				0, 						0, 						0,					self.ivy_object_list, 	0)
		self.ivyAnnounceLanding = 		MyIvyResponse(	"landing", 					self.ivyConfig.mp3_path,	0,				0, 						0, 						0,					self.ivy_object_list, 	0)
		
		self.ivySeatBelts = 			MyIvyResponse(	"seatbelts", 				self.ivyConfig.mp3_path,	0,				1, 						0, 						0,					self.ivy_object_list, 	0)
		
		self.ivyMinimums = 				MyIvyResponse(	"minimums", 				self.ivyConfig.mp3_path,	0,				0, 						10, 					0,					self.ivy_object_list, 	0)
		self.ivyApplause = 				MyIvyResponse(	"passenger_applause", 		self.ivyConfig.mp3_path,	1,				5, 						30, 					0,					self.ivy_object_list, 	1)
		
		# No Callout Events
		self.ivyArmLanding = 			MyIvyResponse(	"arm_landing", 				self.ivyConfig.mp3_path,	0,				1, 						0, 						0,					self.ivy_object_list, 	0)
		self.ivyArmMinimums = 			MyIvyResponse(	"arm_descent", 				self.ivyConfig.mp3_path,	0,				1, 						10, 					0,					self.ivy_object_list, 	0)
			
	
		#self.ivy = 		MyIvyResponse(	"landing_lights", 		self.ivyConfig.mp3_path,	0,			0, 						0, 						0,					self.ivy_object_list)


		self.f_g_normal_max = 1
		self.f_g_side_max = 0
		self.f_g_forward_max = 0
		
		self.f_g_normal_min = 1
		self.f_g_side_min = 0
		self.f_g_forward_min = 0
		
		self.f_g_normal_delta = 0
		self.f_g_side_delta = 0
		self.f_g_forward_delta = 0
		
		self.f_g_normal_old = 1
		self.f_g_side_old = 0
		self.f_g_forward_old = 0

		self.li_on_ground_old = 1
		#self.Clicked = 0
		
		self.cab_press_rate = 0.0
		self.cab_press_old = 0.0
		
		self.show_output = 1
		
		
		
		
		##################################### DEBUG
		for obj_number in range(0,len(self.ivy_object_list)):
			buf = "IvyObject: " + self.ivy_object_list[obj_number].event_name + " Number of Soundfiles: " + str(len(self.ivy_object_list[obj_number].play_files)) + "\n\r"
			self.OutputFile.write(buf)
		self.OutputFile.flush()
		##################################### END DEBUG
		
		##########################################################################################
		# Register Flight Callback
		
		self.FlightLoopCB = self.FlightLoopCallback
		
		XPLMRegisterFlightLoopCallback(self, self.FlightLoopCB, self.ivyConfig.data_rate, 0)                
		
		##########################################################################################
		# Register Command Callbacks
		
		self.SayBaroCB 				= self.SayBaroCallback
		self.ResetIvyCB 			= self.ResetIvyCallback
		self.SayWindCB				= self.SayWindCallback
		self.ToogleWindowCB			= self.ToogleWindowCallback
		self.AnnouncementCB			= self.AnnouncementCallback
		
		self.CmdSayBaro 			= XPLMCreateCommand ( "Ivy/say_baro" 	, "Ask Ivy to tell you the barometric pressure" )
		self.CmdSayWind 			= XPLMCreateCommand ( "Ivy/say_wind" 	, "Ivy holds out a finger and tells you the current wind direction plus speed" )
		self.CmdToogleWindow		= XPLMCreateCommand ( "Ivy/show_output" , "Shows the loaded aircraft name, IvyAircraft name, and slats/flaps datarefs from the IvyAircraft ini for custom aircraft config" )
		self.CmdResetIvy 			= XPLMCreateCommand ( "Ivy/reset_ivy" 	, "Reset Ivy" )
		self.CmdAnnouncement		= XPLMCreateCommand ( "Ivy/cabin_announcement" 	, "Tell Ivy to make a cabin announcement" )
		
		XPLMRegisterCommandHandler 	( self , 	self.CmdSayBaro , 		self.SayBaroCB 		, 0 , 0 )            
		XPLMRegisterCommandHandler 	( self , 	self.CmdResetIvy , 		self.ResetIvyCB 	, 0 , 0 )   
		XPLMRegisterCommandHandler 	( self , 	self.CmdSayWind , 		self.SayWindCB 		, 0 , 0 )  
		XPLMRegisterCommandHandler 	( self , 	self.CmdToogleWindow , 	self.ToogleWindowCB , 0 , 0 ) 
		XPLMRegisterCommandHandler 	( self , 	self.CmdAnnouncement , 	self.AnnouncementCB , 0 , 0 ) 

		# Menu
		self.IvyMenu = XPLMAppendMenuItem(XPLMFindPluginsMenu(), "Ivy", 0, 1)
		self.ShowLogMenuHandlerCB = self.IvyMenuHandler
		self.MenuId = XPLMCreateMenu(self, "Ivy", XPLMFindPluginsMenu(), self.IvyMenu, self.ShowLogMenuHandlerCB, 0)
		XPLMAppendMenuItem(self.MenuId, "Show Logbook", 1, 1)
		XPLMAppendMenuItem(self.MenuId, "Set V-Speeds", 2, 1)
		XPLMAppendMenuItem(self.MenuId, "Make Announcement", 3, 1)
		XPLMAppendMenuItem(self.MenuId, "Barometric Pressure", 4, 1)
		XPLMAppendMenuItem(self.MenuId, "Wind Situtation", 5, 1)
		XPLMAppendMenuItem(self.MenuId, "Show Output", 6, 1)
		XPLMAppendMenuItem(self.MenuId, "Reset Ivy", 7, 1)
		
		# Flag to tell us if theLogbook is shown
		self.MenuLogbookShow = 0
		

		#########################################################################################
		#
		#						DATAREF Find
		#
		#########################################################################################
		
		
		
		self.i_on_ground = 				XPLMFindDataRef("sim/flightmodel/failures/onground_any")
		self.f_climb_rate = 			XPLMFindDataRef("sim/flightmodel/position/vh_ind_fpm")
		
		self.f_gear1_ratio = 			XPLMFindDataRef("sim/flightmodel/movingparts/gear1def")
		self.f_gear2_ratio = 			XPLMFindDataRef("sim/flightmodel/movingparts/gear2def")
		self.f_gear3_ratio = 			XPLMFindDataRef("sim/flightmodel/movingparts/gear3def")
		self.f_gear4_ratio = 			XPLMFindDataRef("sim/flightmodel/movingparts/gear4def")
		self.f_gear5_ratio = 			XPLMFindDataRef("sim/flightmodel/movingparts/gear5def")
		
		self.f_ground_speed = 			XPLMFindDataRef("sim/flightmodel/position/groundspeed")
		self.f_ias = 					XPLMFindDataRef("sim/flightmodel/position/indicated_airspeed")
		self.f_sun_pitch = 				XPLMFindDataRef("sim/graphics/scenery/sun_pitch_degrees")
		self.f_airport_light = 			XPLMFindDataRef("sim/graphics/scenery/airport_light_level") #0-1
		self.f_world_light_precent = 	XPLMFindDataRef("sim/graphics/scenery/percent_lights_on")
		self.i_has_skid = 				XPLMFindDataRef("sim/aircraft/gear/acf_gear_is_skid")
		self.i_transponder_mode = 		XPLMFindDataRef("sim/cockpit/radios/transponder_mode") # on = 3
		self.i_sim_ground_speed = 		XPLMFindDataRef("sim/time/ground_speed")
		
		self.i_temp_sl = 				XPLMFindDataRef("sim/weather/temperature_sealevel_c") # Td ~ T - (100-RH)/5 where Td is the dew point, T is temperature, RH is relative humidity
		self.i_dew_sl = 				XPLMFindDataRef("sim/weather/dewpoi_sealevel_c")
		
		self.i_landing_lights = 		XPLMFindDataRef("sim/cockpit/electrical/landing_lights_on")
		self.i_beacon_lights = 			XPLMFindDataRef("sim/cockpit/electrical/beacon_lights_on")
		self.i_nav_lights = 			XPLMFindDataRef("sim/cockpit/electrical/nav_lights_on")
		self.i_strobe_lights = 			XPLMFindDataRef("sim/cockpit/electrical/strobe_lights_on")
		self.i_taxi_lights = 			XPLMFindDataRef("sim/cockpit/electrical/taxi_light_on")
		self.i_cockpit_lights = 		XPLMFindDataRef("sim/cockpit/electrical/cockpit_lights_on")
		self.f_radio_alt = 				XPLMFindDataRef("sim/cockpit2/gauges/indicators/radio_altimeter_height_ft_pilot")
		self.f_decision_height = 		XPLMFindDataRef("sim/cockpit/misc/radio_altimeter_minimum")
		self.f8_batter_charge = 		XPLMFindDataRef("sim/cockpit/electrical/battery_charge_watt_hr")
		self.i_battery_on = 			XPLMFindDataRef("sim/cockpit/electrical/battery_on")
		self.i_gpu_on = 				XPLMFindDataRef("sim/cockpit/electrical/gpu_on")
		
		self.i_flaps_overspeed = 		XPLMFindDataRef("sim/flightmodel/failures/over_vfe")
		self.i_gear_overspeed = 		XPLMFindDataRef("sim/flightmodel/failures/over_vle")
		self.f_aircraft_vne 		= 	XPLMFindDataRef("sim/aircraft/view/acf_Vne")
		self.i_aircraft_overspeed = 	XPLMFindDataRef("sim/flightmodel/failures/over_vne")
		self.i_stall = 					XPLMFindDataRef("sim/flightmodel/failures/stallwarning")
		
		self.i_cloud_0 = 				XPLMFindDataRef("sim/weather/cloud_type[0]")
		self.i_cloud_1 = 				XPLMFindDataRef("sim/weather/cloud_type[0]")
		self.i_cloud_2 = 				XPLMFindDataRef("sim/weather/cloud_type[0]")
		self.f_visibility = 			XPLMFindDataRef("sim/weather/visibility_reported_m")
		self.i_rain = 					XPLMFindDataRef("sim/weather/rain_percent")
		self.i_thunder = 				XPLMFindDataRef("sim/weather/thunderstorm_percent")
		self.i_turbulence = 			XPLMFindDataRef("sim/weather/wind_turbulence_percent")
		
		self.i_batt1 = 					XPLMFindDataRef("sim/operation/failures/rel_bat0_lo")
		self.i_batt2 = 					XPLMFindDataRef("sim/operation/failures/rel_bat1_lo")
		
		self.i_tire1 = 					XPLMFindDataRef("sim/operation/failures/rel_tire1") #eight
		self.i_tire2 = 					XPLMFindDataRef("sim/operation/failures/rel_tire2")
		self.i_tire3 = 					XPLMFindDataRef("sim/operation/failures/rel_tire3")
		self.i_tire4 = 					XPLMFindDataRef("sim/operation/failures/rel_tire4")
		self.i_tire5 = 					XPLMFindDataRef("sim/operation/failures/rel_tire5")
		
		self.i_fire1 = 					XPLMFindDataRef("sim/operation/failures/rel_engfir0") #multiple
		self.i_fire2 = 					XPLMFindDataRef("sim/operation/failures/rel_engfir1")
		self.i_fire3 = 					XPLMFindDataRef("sim/operation/failures/rel_engfir2")
		self.i_fire4 = 					XPLMFindDataRef("sim/operation/failures/rel_engfir3")
		self.i_fire5 =					XPLMFindDataRef("sim/operation/failures/rel_engfir4")
		self.i_fire6 = 					XPLMFindDataRef("sim/operation/failures/rel_engfir5")
		self.i_fire7 = 					XPLMFindDataRef("sim/operation/failures/rel_engfir6")
		self.i_fire8 = 					XPLMFindDataRef("sim/operation/failures/rel_engfir7")
		
		self.i_flameout1 = 				XPLMFindDataRef("sim/operation/failures/rel_engfla0") #multiple
		self.i_flameout2 = 				XPLMFindDataRef("sim/operation/failures/rel_engfla1")
		self.i_flameout3 = 				XPLMFindDataRef("sim/operation/failures/rel_engfla2")
		self.i_flameout4 =				XPLMFindDataRef("sim/operation/failures/rel_engfla3")
		self.i_flameout5 = 				XPLMFindDataRef("sim/operation/failures/rel_engfla4")
		self.i_flameout6 = 				XPLMFindDataRef("sim/operation/failures/rel_engfla5")
		self.i_flameout7 = 				XPLMFindDataRef("sim/operation/failures/rel_engfla6")
		self.i_flameout8 = 				XPLMFindDataRef("sim/operation/failures/rel_engfla7")
		
		
		
		self.i_engine_failure1 = 		XPLMFindDataRef("sim/operation/failures/rel_engfai0") #multiple
		self.i_engine_failure2 = 		XPLMFindDataRef("sim/operation/failures/rel_engfai1")
		self.i_engine_failure3 = 		XPLMFindDataRef("sim/operation/failures/rel_engfai2")
		self.i_engine_failure4 = 		XPLMFindDataRef("sim/operation/failures/rel_engfai3")
		self.i_engine_failure5 = 		XPLMFindDataRef("sim/operation/failures/rel_engfai4")
		self.i_engine_failure6 = 		XPLMFindDataRef("sim/operation/failures/rel_engfai5")
		self.i_engine_failure7 = 		XPLMFindDataRef("sim/operation/failures/rel_engfai6")
		self.i_engine_failure8 = 		XPLMFindDataRef("sim/operation/failures/rel_engfai7")
		
		self.i_hot1 = 					XPLMFindDataRef("sim/operation/failures/rel_hotsta0") #multiple
		self.i_hot2 = 					XPLMFindDataRef("sim/operation/failures/rel_hotsta1")
		self.i_hot3 = 					XPLMFindDataRef("sim/operation/failures/rel_hotsta2")
		self.i_hot4 = 					XPLMFindDataRef("sim/operation/failures/rel_hotsta3")
		self.i_hot5 = 					XPLMFindDataRef("sim/operation/failures/rel_hotsta4")
		self.i_hot6 = 					XPLMFindDataRef("sim/operation/failures/rel_hotsta5")
		self.i_hot7 = 					XPLMFindDataRef("sim/operation/failures/rel_hotsta6")
		self.i_hot8 = 					XPLMFindDataRef("sim/operation/failures/rel_hotsta7")
		
		self.f_ice_frame = 				XPLMFindDataRef("sim/flightmodel/failures/frm_ice")
		self.f_ice_pitot = 				XPLMFindDataRef("sim/flightmodel/failures/pitot_ice")
		self.f_ice_propeller = 			XPLMFindDataRef("sim/flightmodel/failures/prop_ice")
		self.f_ice_window = 			XPLMFindDataRef("sim/flightmodel/failures/window_ice")
		  
		self.f_g_normal = 				XPLMFindDataRef("sim/flightmodel/forces/g_nrml") 
		self.f_g_forward = 				XPLMFindDataRef("sim/flightmodel/forces/g_axil")
		self.f_g_side = 				XPLMFindDataRef("sim/flightmodel/forces/g_side")
		
		self.i_bird = 					XPLMFindDataRef("sim/operation/failures/rel_bird_strike")
		
		
		self.f_pitch = 					XPLMFindDataRef("sim/flightmodel/position/theta")
		self.f_roll = 					XPLMFindDataRef("sim/flightmodel/position/phi")
		self.f_yaw = 					XPLMFindDataRef("sim/flightmodel/position/beta")
		
		#cab press
		self.f_cab_press = 				XPLMFindDataRef("sim/cockpit/pressure/cabin_altitude_actual_m_msl") # meter?
		#cab climb rate
		self.f_cab_rate = 				XPLMFindDataRef("sim/cockpit/pressure/cabin_vvi_actual_m_msec") # meter per second? 1000ft/min = 5m/s
		#cab humidity
		#cab temp
		
		self.f_outside_temp1 = 			XPLMFindDataRef("sim/weather/temperature_ambient_c")
		self.f_outside_temp2 = 			XPLMFindDataRef("sim/cockpit2/temperature/outside_air_temp_degc")
		self.f_outside_temp3 = 			XPLMFindDataRef("sim/cockpit2/temperature/outside_air_LE_temp_degc")
		
		self.f_baro_set =	 			XPLMFindDataRef("sim/cockpit/misc/barometer_setting")
		self.f_baro_sea_level =			XPLMFindDataRef("sim/weather/barometer_sealevel_inhg")
		self.f_baro_alt =				XPLMFindDataRef("sim/flightmodel/misc/h_ind")
		
		self.f_wind_direction = 		XPLMFindDataRef("sim/weather/wind_direction_degt")
		self.f_wind_speed_kt = 			XPLMFindDataRef("sim/weather/wind_speed_kt")
		
		self.f_slats_1 =		    	XPLMFindDataRef("sim/flightmodel2/controls/slat1_deploy_ratio")
		self.f_flaps_1 =		    	XPLMFindDataRef("sim/flightmodel2/controls/flap1_deploy_ratio")
		
		self.d_latitude	=				XPLMFindDataRef("sim/flightmodel/position/latitude")
		self.d_longitude =				XPLMFindDataRef("sim/flightmodel/position/longitude")
		
		self.i_nonsmoking = 			XPLMFindDataRef("sim/cockpit/switches/no_smoking")
		self.i_fastenseatbelt = 		XPLMFindDataRef("sim/cockpit/switches/fasten_seat_belts")
		
		self.i_replay = 				XPLMFindDataRef("sim/operation/prefs/replay_mode")
		
		######################################################################################### End of DATAREF Find
		
		#self.ReadData() # Read data for the first time to ensure window handler can process valid data from the start
	
		self.data_read_valid = False

		self.DrawWindowCB = self.DrawWindowCallback
		self.KeyCB = self.KeyCallback
		self.MouseClickCB = self.MouseClickCallback
		self.WindowId = XPLMCreateWindow(self, 10, 200, 500, 500, 1, self.DrawWindowCB, self.KeyCB, self.MouseClickCB, 0)
		
		# Make sure file exists
		logbook_file 	= open(self.ivyConfig.logbook_path, 'a+')
		logbook_file.close()
		
		
		return self.Name, self.Sig, self.Desc
	
	##########################################################################################################################################################################################################
	# VSpeeds Functions following
	def CreateVSpeedsWidget(self):
		x=300
		y=300
		x2=x+200
		y2=y-200
		
		self.VSpeedsWidget = XPCreateWidget(x, y, x2, y2, 1, "Ivy VSpeeds", 1,	0, xpWidgetClass_MainWindow)
		
		
		# Add Close Box decorations to the Main Widget
		XPSetWidgetProperty(self.VSpeedsWidget, xpProperty_MainWindowHasCloseBoxes, 1)
		
		self.v1_label = XPCreateWidget(x+10, y-25, x+50, y-40, 1,	"V1:",  0, self.VSpeedsWidget, xpWidgetClass_Caption)
		self.vr_label = XPCreateWidget(x+10, y-45, x+50, y-60, 1,	"VR:",  0, self.VSpeedsWidget, xpWidgetClass_Caption)
		self.v2_label = XPCreateWidget(x+10, y-65, x+50, y-80, 1,	"V2:",  0, self.VSpeedsWidget, xpWidgetClass_Caption)
		self.dh_label = XPCreateWidget(x+10, y-85, x+50, y-100, 1,	"DH:",  0, self.VSpeedsWidget, xpWidgetClass_Caption)
		
		self.kt60_label  = XPCreateWidget(x+10, y-105, x+50, y-120, 1,	"60kt:",  0, self.VSpeedsWidget, xpWidgetClass_Caption)
		self.kt80_label  = XPCreateWidget(x+10, y-125, x+50, y-140, 1,	"80kt:",  0, self.VSpeedsWidget, xpWidgetClass_Caption)
		self.kt100_label = XPCreateWidget(x+10, y-145, x+50, y-160, 1,	"100kt:",  0, self.VSpeedsWidget, xpWidgetClass_Caption)
		
		self.v1_label_val = XPCreateWidget(x+50, y-25, x+80, y-40, 1,	str(self.ivyAircraft.li_v1),  		0, self.VSpeedsWidget, xpWidgetClass_Caption)
		self.vr_label_val = XPCreateWidget(x+50, y-45, x+80, y-60, 1,	str(self.ivyAircraft.li_vr),  		0, self.VSpeedsWidget, xpWidgetClass_Caption)
		self.v2_label_val = XPCreateWidget(x+50, y-65, x+80, y-80, 1,	str(self.ivyAircraft.li_v2),  		0, self.VSpeedsWidget, xpWidgetClass_Caption)
		self.dh_label_val = XPCreateWidget(x+50, y-85, x+80, y-100, 1,	str(int(self.lf_decision_height)),  0, self.VSpeedsWidget, xpWidgetClass_Caption)

		self.v1_textbox = XPCreateWidget(x+80, y-30, x+120, y-40, 1,	str(self.ivyAircraft.li_v1),  		0, self.VSpeedsWidget, xpWidgetClass_TextField)
		self.vr_textbox = XPCreateWidget(x+80, y-50, x+120, y-60, 1,	str(self.ivyAircraft.li_vr),  		0, self.VSpeedsWidget, xpWidgetClass_TextField)
		self.v2_textbox = XPCreateWidget(x+80, y-70, x+120, y-80, 1,	str(self.ivyAircraft.li_v2),  		0, self.VSpeedsWidget, xpWidgetClass_TextField)		
		self.dh_textbox = XPCreateWidget(x+80, y-90, x+120, y-100, 1,	str(int(self.lf_decision_height)), 	0, self.VSpeedsWidget, xpWidgetClass_TextField)		
		
		XPSetWidgetProperty(self.v1_textbox, xpProperty_TextFieldType, xpTextEntryField)
		XPSetWidgetProperty(self.vr_textbox, xpProperty_TextFieldType, xpTextEntryField)
		XPSetWidgetProperty(self.v2_textbox, xpProperty_TextFieldType, xpTextEntryField)
		XPSetWidgetProperty(self.dh_textbox, xpProperty_TextFieldType, xpTextEntryField)
		
		XPSetWidgetProperty (self.v1_textbox, xpProperty_Enabled , 1 )
		XPSetWidgetProperty (self.vr_textbox, xpProperty_Enabled , 1 )
		XPSetWidgetProperty (self.v2_textbox, xpProperty_Enabled , 1 )
		XPSetWidgetProperty (self.dh_textbox, xpProperty_Enabled , 1 )
		
		self.v1_button = XPCreateWidget(x+140, y-30, x+180, y-40, 1,	"Set",  0, self.VSpeedsWidget, xpWidgetClass_Button)
		self.vr_button = XPCreateWidget(x+140, y-50, x+180, y-60, 1,	"Set",  0, self.VSpeedsWidget, xpWidgetClass_Button)
		self.v2_button = XPCreateWidget(x+140, y-70, x+180, y-80, 1,	"Set",  0, self.VSpeedsWidget, xpWidgetClass_Button)	
		self.dh_button = XPCreateWidget(x+140, y-90, x+180, y-100, 1,	"Set",  0, self.VSpeedsWidget, xpWidgetClass_Button)	
		
		self.kt60_button = XPCreateWidget(x+55, y-110, x+65, y-120, 1,	"",  0, self.VSpeedsWidget, xpWidgetClass_Button)	
		XPSetWidgetProperty(self.kt60_button, xpProperty_ButtonType, xpRadioButton)
		XPSetWidgetProperty(self.kt60_button, xpProperty_ButtonBehavior, xpButtonBehaviorCheckBox)
		if (self.ivyConfig.kt60_enabled == True): 	XPSetWidgetProperty(self.kt60_button, xpProperty_ButtonState, 1)
		else:										XPSetWidgetProperty(self.kt60_button, xpProperty_ButtonState, 0)
		
		self.kt80_button = XPCreateWidget(x+55, y-130, x+65, y-140, 1,	"",  0, self.VSpeedsWidget, xpWidgetClass_Button)	
		XPSetWidgetProperty(self.kt80_button, xpProperty_ButtonType, xpRadioButton)
		XPSetWidgetProperty(self.kt80_button, xpProperty_ButtonBehavior, xpButtonBehaviorCheckBox)
		if (self.ivyConfig.kt80_enabled == True): 	XPSetWidgetProperty(self.kt80_button, xpProperty_ButtonState, 1)
		else:										XPSetWidgetProperty(self.kt80_button, xpProperty_ButtonState, 0)
		
		self.kt100_button = XPCreateWidget(x+55, y-150, x+65, y-160, 1,	"",  0, self.VSpeedsWidget, xpWidgetClass_Button)	
		XPSetWidgetProperty(self.kt100_button, xpProperty_ButtonType, xpRadioButton)
		XPSetWidgetProperty(self.kt100_button, xpProperty_ButtonBehavior, xpButtonBehaviorCheckBox)
		if (self.ivyConfig.kt100_enabled == True): 	XPSetWidgetProperty(self.kt100_button, xpProperty_ButtonState, 1)
		else:										XPSetWidgetProperty(self.kt100_button, xpProperty_ButtonState, 0)
		
		
		
		self.IvyVSpeedHandlerCB = self.IvyVSpeedHandler
		XPAddWidgetCallback(self, self.VSpeedsWidget, self.IvyVSpeedHandlerCB)
		
		pass
		
		

	
	def IvyVSpeedHandler(self, inMessage, inWidget,	inParam1, inParam2):
		if (inMessage == xpMessage_CloseButtonPushed):
			if (self.MenuVSpeedsShow == 1):
				XPDestroyWidget(self, self.VSpeedsWidget, 1)
				self.MenuVSpeedsShow = 0
			return 1
			
		if (inMessage == xpMsg_PushButtonPressed):
			if (inParam1 == self.v1_button):
				buffer = []
				XPGetWidgetDescriptor(self.v1_textbox, buffer, 256)
				text = buffer[0]
				if (text.isdigit() == True): self.ivyAircraft.li_v1 = int(text)
				XPSetWidgetDescriptor(self.v1_textbox,   str(self.ivyAircraft.li_v1))
				XPSetWidgetDescriptor(self.v1_label_val, str(self.ivyAircraft.li_v1))
				
			if (inParam1 == self.vr_button):
				buffer = []
				XPGetWidgetDescriptor(self.vr_textbox, buffer, 256)
				text = buffer[0]
				if (text.isdigit() == True): self.ivyAircraft.li_vr = int(text)
				XPSetWidgetDescriptor(self.vr_textbox,   str(self.ivyAircraft.li_vr))
				XPSetWidgetDescriptor(self.vr_label_val, str(self.ivyAircraft.li_vr))
				
			if (inParam1 == self.v2_button):
				buffer = []
				XPGetWidgetDescriptor(self.v2_textbox, buffer, 256)
				text = buffer[0]
				if (text.isdigit() == True): self.ivyAircraft.li_v2 = int(text)
				XPSetWidgetDescriptor(self.v2_textbox,   str(self.ivyAircraft.li_v2))
				XPSetWidgetDescriptor(self.v2_label_val, str(self.ivyAircraft.li_v2))
				
			if (inParam1 == self.dh_button):
				buffer = []
				XPGetWidgetDescriptor(self.dh_textbox, buffer, 256)
				text = buffer[0]
				if (text.isdigit() == True): 
					dh_new = float(int(text))
					XPLMSetDataf(self.f_decision_height, dh_new)
					self.lf_decision_height = XPLMGetDataf(self.f_decision_height)
				XPSetWidgetDescriptor(self.dh_textbox,   str(int(self.lf_decision_height)))
				XPSetWidgetDescriptor(self.dh_label_val, str(int(self.lf_decision_height)))
			
		if (inMessage == xpMsg_ButtonStateChanged):	
			if (inParam1 == self.kt60_button):
				if (XPGetWidgetProperty(self.kt60_button, xpProperty_ButtonState, None) == 1):			self.ivyConfig.kt60_enabled = True
				else:																					self.ivyConfig.kt60_enabled = False
				
			if (inParam1 == self.kt80_button):
				if (XPGetWidgetProperty(self.kt80_button, xpProperty_ButtonState, None) == 1):			self.ivyConfig.kt80_enabled = True
				else:																					self.ivyConfig.kt80_enabled = False	
			
			if (inParam1 == self.kt100_button):
				if (XPGetWidgetProperty(self.kt100_button, xpProperty_ButtonState, None) == 1):			self.ivyConfig.kt100_enabled = True
				else:																					self.ivyConfig.kt100_enabled = False
			

		return 0
		pass	
		
	
	##########################################################################################################################################################################################################
	# Logbook Widget Functions following
		
	def CreateLogbookWidget(self, x, y, w, h):
		x2 = x + w
		y2 = y - h

		# Create the Main Widget window
		self.LogbookWidget = XPCreateWidget(x, y, x2, y2, 1, "Ivy Loogbook", 1,	0, xpWidgetClass_MainWindow)
		self.IvyLogbookHandlerCB = self.IvyLogbookHandler
		XPAddWidgetCallback(self, self.LogbookWidget, self.IvyLogbookHandlerCB)

		# Add Close Box decorations to the Main Widget
		XPSetWidgetProperty(self.LogbookWidget, xpProperty_MainWindowHasCloseBoxes, 1)
		logbookstring = "test1; test2\n\r test3"
		
		self.logbook_lines = int(self.ivyConfig.log_window_entries)
		self.logbook_index = 0
		self.text_field_array = []
		self.logbook_entries = []
		
		logbook_file = open(self.ivyConfig.logbook_path, 'a+')
		self.logbook_entries = logbook_file.readlines()
		logbook_file.close()
		
		self.LogbookScrollBar = XPCreateWidget(x2-10, y-20, x2-5, y2, 1,	"",	0, self.LogbookWidget, xpWidgetClass_ScrollBar)
		XPSetWidgetProperty(self.LogbookScrollBar, xpProperty_ScrollBarMin, 0)
		XPSetWidgetProperty(self.LogbookScrollBar, xpProperty_ScrollBarMax, max(len(self.logbook_entries)+2, 0))
		XPSetWidgetProperty(self.LogbookScrollBar, xpProperty_ScrollBarPageAmount, self.logbook_lines)
		XPSetWidgetProperty(self.LogbookScrollBar, xpProperty_ScrollBarSliderPosition, min(self.logbook_lines,len(self.logbook_entries))) # Set page to show last flight max(len(self.logbook_entries), self.logbook_lines)
		
		self.IvyScrollbarHandlerCB = self.IvyLogbookScrollHandler
		XPAddWidgetCallback(self, self.LogbookScrollBar, self.IvyScrollbarHandlerCB)
		
		for index in range (0,self.logbook_lines):
			self.text_field_array.append(XPCreateWidget(x+5, y-(30 + (index*20)), x2-15, y-(40 + (index*20)), 1,	"test" + str(index),  0, self.LogbookWidget, xpWidgetClass_TextField))
			#subwindow =                  XPCreateWidget(x+5, y-(50 + (index*20)), x2-15, y-(60 + (index*20)),1, "",	0, self.LogbookWidget, xpWidgetClass_SubWindow)
			#XPSetWidgetProperty(subwindow, xpProperty_SubWindowType, xpSubWindowStyle_SubWindow)
			

		
		# Text Draw 
		self.IvyFillLogbook()
		

		pass

	def IvyLogbookHandler(self, inMessage, inWidget,	inParam1, inParam2):
		if (inMessage == xpMessage_CloseButtonPushed):
			if (self.MenuLogbookShow == 1):
				XPDestroyWidget(self, self.LogbookWidget, 1)
				self.MenuLogbookShow = 0
			return 1

		return 0
		pass
		
	def IvyLogbookScrollHandler(self, inMessage, inWidget,	inParam1, inParam2):
		if (inMessage == xpMsg_ScrollBarSliderPositionChanged):
			self.IvyFillLogbook()
			return 1
		return 0
		pass
		

		
	def IvyFillLogbook(self):
		self.logbook_index = max(len(self.logbook_entries) - XPGetWidgetProperty(self.LogbookScrollBar, xpProperty_ScrollBarSliderPosition, None),0)
		for index in range(0,len(self.text_field_array)):
			text_index = index + self.logbook_index
			if (text_index < len(self.logbook_entries)):
				XPSetWidgetDescriptor(self.text_field_array[index], self.logbook_entries[text_index])
			else:
				XPSetWidgetDescriptor(self.text_field_array[index], "")
	
		pass

	
	##########################################################################################################################################################################################################
	# Utility Functions following
	
	def XPluginStop(self):
		XPLMUnregisterFlightLoopCallback	(self, 		self.FlightLoopCB, 		0)
		XPLMUnregisterCommandHandler 		( self , 	self.CmdSayBaro , 		self.SayBaroCB , 		0 , 0 )
		XPLMUnregisterCommandHandler 		( self , 	self.CmdResetIvy , 		self.ResetIvyCB , 		0 , 0 )
		XPLMUnregisterCommandHandler 		( self , 	self.CmdSayWind , 		self.SayWindCB 		, 	0 , 0 )  
		XPLMUnregisterCommandHandler 		( self , 	self.CmdToogleWindow , 	self.ToogleWindowCB , 	0 , 0 ) 
		XPLMUnregisterCommandHandler 		( self , 	self.CmdAnnouncement , 	self.AnnouncementCB , 	0 , 0 ) 
		XPLMDestroyWindow(self, self.WindowId)
		XPLMDestroyMenu(self, self.MenuId)
		#XPLMDestroyMenu(self, self.IvyMenu)
		
		if (self.MenuLogbookShow == 1): XPDestroyWidget(self, self.LogbookWidget, 1)
		if (self.MenuVSpeedsShow == 1): XPDestroyWidget(self, self.VSpeedsWidget, 1)
		self.OutputFile.close()
		self.ivyConfig.WriteConfig()
		pass
			
	def XPluginEnable(self):
		self.plugin_enabled = 1
		return 1


	def XPluginDisable(self):
		self.plugin_enabled = 0
		pass


	def XPluginReceiveMessage(self, inFromWho, inMessage, inParam):
	
		if (inFromWho == XPLM_PLUGIN_XPLANE):
			if (inMessage == XPLM_MSG_PLANE_LOADED):
				self.aircraft_loaded = 1
				self.no_aircraft = False
				self.ResetIvy()
			if (inMessage == XPLM_MSG_AIRPORT_LOADED):
				self.aircraft_loaded = 1
				self.no_aircraft = False
				self.ResetIvy()
			if (inMessage == XPLM_MSG_PLANE_CRASHED):
				self.aircraft_crashed = 1
		pass



	def DrawWindowCallback(self, inWindowID, inRefcon):
		
		lLeft = [];	lTop = []; lRight = [];	lBottom = []
		XPLMGetWindowGeometry(inWindowID, lLeft, lTop, lRight, lBottom)
		left = int(lLeft[0]); top = int(lTop[0]); right = int(lRight[0]); bottom = int(lBottom[0])

		color = 1.0, 1.0, 1.0
		
		if (self.data_read_valid == True) and (self.draw_window == 1):
		
			XPLMDrawTranslucentDarkBox(left, top, right, bottom)
			XPLMDrawString(color, left + 5, top - 10, "Aircraft Name:    " + str(self.ls_acf_descrip), 0, xplmFont_Basic)
			XPLMDrawString(color, left + 5, top - 20, "IvyAircraft Name: " + str(self.ivyAircraft.name), 0, xplmFont_Basic)
			XPLMDrawString(color, left + 5, top - 30, "Slats position: 	 " + str(self.ivyAircraft.lf_slats), 0, xplmFont_Basic)
			XPLMDrawString(color, left + 5, top - 40, "Flaps position:   " + str(self.ivyAircraft.lf_flaps), 0, xplmFont_Basic)
			XPLMDrawString(color, left + 5, top - 50, "V1:               " + str(self.ivyAircraft.li_v1), 0, xplmFont_Basic)
			XPLMDrawString(color, left + 5, top - 60, "VR:               " + str(self.ivyAircraft.li_vr), 0, xplmFont_Basic)
			XPLMDrawString(color, left + 5, top - 70, "V2:               " + str(self.ivyAircraft.li_v2), 0, xplmFont_Basic)
			XPLMDrawString(color, left + 5, top - 80, "Decision Height:  " + str(int(self.lf_decision_height)), 0, xplmFont_Basic)
			#XPLMDrawString(color, left + 5, top - 90, "Debug1:           " + str(self.lf_cab_rate), 0, xplmFont_Basic)
			#XPLMDrawString(color, left + 5, top -100, "Debug2:           " + str(self.lf_cab_press), 0, xplmFont_Basic)
			#XPLMDrawString(color, left + 5, top -110, "Debug3:           " + str(self.lf_climb_rate), 0, xplmFont_Basic)

# For Debug			
#		if ((self.li_on_ground == 0) and (self.lf_radio_alt > (self.lf_decision_height + self.ivyConfig.decition_height_arm))):
#			XPLMDrawString(color, left + 5, top - 100, "Statement TRUE", 0, xplmFont_Basic)
#		else: 
#			XPLMDrawString(color, left + 5, top - 100, "Statement FALSE", 0, xplmFont_Basic)
		
		
		return 0


	def KeyCallback(self, inWindowID, inKey, inFlags, inVirtualKey, inRefcon, losingFocus):
		return 0


	def MouseClickCallback(self, inWindowID, x, y, inMouse, inRefcon):
		return 0
		
	##########################################################################################################################################################################################################
	# Callback Functions	
	
	def IvyMenuHandler(self, inMenuRef, inItemRef):
		# If menu selected show our logbook
		if (inItemRef == 1):
			if (self.MenuLogbookShow == 0):
				self.CreateLogbookWidget(int(self.ivyConfig.log_window_pos_x), int(self.ivyConfig.log_window_pos_y), int(self.ivyConfig.log_window_width), int(self.ivyConfig.log_window_height))
				self.MenuLogbookShow = 1
			else:
				self.MenuLogbookShow = 0
				XPDestroyWidget(self, self.LogbookWidget, 1)
		elif (inItemRef == 2):
			if (self.MenuLogbookShow == 0):
				self.CreateVSpeedsWidget()
				self.MenuVSpeedsShow = 1
			else:
				self.MenuVSpeedsShow = 0
				XPDestroyWidget(self, self.VSpeedsWidget, 1)
		elif (inItemRef == 3):
			self.AnnouncementCallback(0,0,0)
		elif (inItemRef == 4):
			self.SayBaroCallback(0,0,0)
		elif (inItemRef == 5):
			self.SayWindCallback(0,0,0)
		elif (inItemRef == 6):
			self.ToogleWindowCallback(0,0,0)
		elif (inItemRef == 7):
			self.ResetIvyCallback(0,0,0)
		pass	

	def SayBaroCallback( self , cmd , phase , refcon ) :
		if ( phase == 0 ) :	
			self.SayBaro()
		return 0
		
	def SayWindCallback( self , cmd , phase , refcon ) :
		if ( phase == 0 ) :	
			self.SayWind()
		return 0
		
	def AnnouncementCallback( self , cmd , phase , refcon ) :
		if ( phase == 0 ) :	
			if (self.li_on_ground == 1):
				self.ivyAnnounceTakeOff.Activate(self.time)
			else:
				self.ivyAnnounceLanding.Activate(self.time)
		return 0
		
	def ResetIvyCallback( self , cmd , phase , refcon ) :
		if ( phase == 0 ) :	
			self.ResetIvy()
		return 0
	
	def ToogleWindowCallback( self , cmd , phase , refcon ) :
		if ( phase == 0 ) :	
			self.draw_window = 1 - self.draw_window
		return 0
	
	##########################################################################################################################################################################################################
	# SayBaro
	#
	# Ivy tells you the current barometric pressure
	
	def SayBaro(self):

		self.play_mp3_queue.append(self.ivyConfig.mp3_path + "baro_press_1.ogg")	
		self.SpellOutDigits(self.li_baro_sea_level)
		pass
	
	##########################################################################################################################################################################################################
	# SayWind
	#
	# Ivy tells you the current wind direction and speed
	
	def SayWind(self):
		self.play_mp3_queue.append(self.ivyConfig.mp3_path + "wind1.ogg")
		self.SpellOutNumber(int(self.lf_wind_direction))
		self.play_mp3_queue.append(self.ivyConfig.mp3_path + "wind2.ogg")
		self.SpellOutNumber(int(self.lf_wind_speed_kt))
		self.play_mp3_queue.append(self.ivyConfig.mp3_path + "knots.ogg")
	
	
	##########################################################################################################################################################################################################
	# SpellOutDigits
	#
	# Ivy spells the single digits of the number given		
		
	def SpellOutDigits(self, spell_number):
		# 1000	
		digit = int((spell_number % 10000) / 1000 )
		self.OutputFile.write(str(digit))
		self.play_mp3_queue.append(self.ivyConfig.number_path + str(digit) + ".ogg")

		# 100
		digit = int((spell_number % 1000) / 100 )
		self.OutputFile.write(str(digit))
		self.play_mp3_queue.append(self.ivyConfig.number_path + str(digit) + ".ogg")
		
		# 10
		digit = int((spell_number % 100) / 10 )
		self.OutputFile.write(str(digit))
		self.play_mp3_queue.append(self.ivyConfig.number_path + str(digit) + ".ogg")
		
		# 1
		digit = int(spell_number % 10)
		self.OutputFile.write(str(digit))
		self.play_mp3_queue.append(self.ivyConfig.number_path + str(digit) + ".ogg")
		
		self.OutputFile.write("Digits \n\r")
		self.OutputFile.flush()
		self.OutputFile.flush()		
		pass

	##########################################################################################################################################################################################################
	# SpellOutNumber
	#
	# Ivy says the number given	
	
	def SpellOutNumber(self, spell_number):
		self.OutputFile.write("SpellOutNumber: ")
		# 1000
		digit = int((spell_number % 10000) / 1000 )
		self.OutputFile.write(str(digit) + " ")
		if (digit > 0):
			self.play_mp3_queue.append(self.ivyConfig.number_path + str(digit) + ".ogg")
			self.play_mp3_queue.append(self.ivyConfig.number_path + "1000" + ".ogg")
		# 100
		digit = int((spell_number % 1000) / 100)
		self.OutputFile.write(str(digit) + " ")
		if (digit > 0):
			self.play_mp3_queue.append(self.ivyConfig.number_path + str(digit) + ".ogg")
			self.play_mp3_queue.append(self.ivyConfig.number_path + "100" + ".ogg")
			
		# 10
		digit = int((spell_number % 100) / 10)
		self.OutputFile.write(str(digit) + " ")
		if (digit > 1):
			self.play_mp3_queue.append(self.ivyConfig.number_path + str(digit*10) + ".ogg")
			digit = int(spell_number % 10)
		else:
			digit = int(spell_number % 100)
		
		# Single digit or <20
		self.OutputFile.write(str(digit) + " ")
		if (digit > 0): 
			self.play_mp3_queue.append(self.ivyConfig.number_path + str(digit) + ".ogg")
		# if total value is zero, say zero
		elif (int(spell_number) == 0):
			self.play_mp3_queue.append(self.ivyConfig.number_path + "0" + ".ogg")

		
		
		self.OutputFile.write("Number End \n\r")
		self.OutputFile.flush()
		self.OutputFile.flush()
		pass
	
	##########################################################################################################################################################################################################
	# CheckAnnouncement
	# 
	# Checks toogling of non smoking sign to make announcement
	
	def CheckAnnouncement(self):
		if (self.non_smoking_old != self.li_nonsmoking):
			if ((self.time - self.non_smoking_event) < self.ivyConfig.non_smoking_annoucetime):
				if (self.li_on_ground == 1):
					self.ivyAnnounceTakeOff.Activate(self.time)
				else:
					self.ivyAnnounceLanding.Activate(self.time)
				
			self.non_smoking_event = self.time
			
		self.non_smoking_old = self.li_nonsmoking
		pass

	##########################################################################################################################################################################################################
	# DetectLanding
	#
	# Detect each single touchdown, create a IvyLandingDetection object with the corresponding values and store it in the ivy_landing_list	
		
	def DetectLanding(self):	
		# Detect potential depature airport first:
		# Need to store temporarily, because helicopters on bouncing might trigger this
		if ((self.li_on_ground == 1) and (self.lf_ground_speed < self.ivyConfig.taxi_ground_speed_min)):
			self.airport_departure_ref = XPLMFindNavAid(None, None, self.ld_latitude, self.ld_longitude, None, xplm_Nav_Airport)	
			self.airport_name = []
			XPLMGetNavAidInfo(self.airport_departure_ref, None, None, None, None, None, None, self.airport_name, None, None)								   
			self.airport_departure_temp = self.airport_name[0]
			
		
			
		if (self.li_on_ground == 0):
			self.landing_detected = 0
			self.landing_rated = 0
			
			# Store the potential departure airport within a 10s window after positive climb callout 
			# Pos climb has a 20s cooldown, which means that we only take the new departure after 20s on ground
			flight_time = self.time - self.ivyPosRateClimb.time_activated
			if ((flight_time > 0) and (flight_time < 10)):	self.airport_departure = self.airport_departure_temp
			
			
		elif ((self.li_on_ground_old == 0) and (self.li_on_ground == 1)):
			self.landing_detected = 1
			
			buf = "Landing detected: " + str(self.time) + " Sinkrate: " + str(self.lf_climb_rate) + " G-Force: " + str(self.lf_g_normal) + "\n\r"
			self.OutputFile.write(buf)
			self.OutputFile.flush()
			
			
			landing_object = IvyLandingDetection(self.time, self.lf_climb_rate, self.lf_g_normal, self.lf_g_side, self.lf_g_forward)
			self.ivy_landing_list.append(landing_object)
			
			self.landing_rated = 0
			self.landing_sink_rate = 0
			self.landing_g_normal = 0
			self.landing_bounces = 0
			
			# Check all touch downs. Rating is 0 if not in window
			for obj_number in range(0,len(self.ivy_landing_list)):
				act_rating = self.ivy_landing_list[obj_number].GetCurrentRate(self.time, 10)
				
				if (act_rating > 0 ) :
					self.landing_sink_rate = max(self.landing_sink_rate, abs(self.ivy_landing_list[obj_number].sink_rate))
					self.landing_g_normal = max(self.landing_g_normal, abs(self.ivy_landing_list[obj_number].g_normal))
					self.landing_bounces = self.landing_bounces + 1
					
				
				
				if (act_rating > self.landing_rated) : self.landing_rated = act_rating
				buf = "Landing T=" + str(self.ivy_landing_list[obj_number].time) + " Sink Rate " + str(abs(self.ivy_landing_list[obj_number].sink_rate)) + " g: " + str(abs(self.ivy_landing_list[obj_number].g_normal)) + " Grade: " + str(act_rating) + " | "  
				self.OutputFile.write(buf)
			self.OutputFile.write("\n\r")
			self.OutputFile.flush()
			
		self.li_on_ground_old = self.li_on_ground
		pass

	##########################################################################################################################################################################################################
	# EndOfFlightEvaluation
	#
	# Here, all the touchdowns (if bouncing, it would have been more than one) are evaluated
		
	def EndOfFlightEvaluation(self):
		error_rate = 0
		self.landing_detected = 0
		
		# Get the proper values to speak (before and after decimal point for g forces)
		sink_rate = int(self.landing_sink_rate)
		g_force_int = int(self.landing_g_normal)
		g_force_dec_2 = int(((self.landing_g_normal - g_force_int) * 100))
		
		# Count all the errors that occurred
		for obj_number in range(0,len(self.ivy_object_list)):
			if (self.ivy_object_list[obj_number].is_error != 0):
				error_rate = error_rate + self.ivy_object_list[obj_number].error_count
		
		# Evaluate Flight		
		if (error_rate == 0): 
			self.play_mp3_queue.append(self.ivyConfig.mp3_path + "error_zero" + ".ogg")
		elif (error_rate < 5):
			self.play_mp3_queue.append(self.ivyConfig.mp3_path + "error_good" + ".ogg")
		elif (error_rate < 10):
			self.play_mp3_queue.append(self.ivyConfig.mp3_path + "error_bad" + ".ogg")
		else:
			self.play_mp3_queue.append(self.ivyConfig.mp3_path + "error_xbad" + ".ogg")
	
		
		if (error_rate > 0): self.SpellOutNumber(error_rate)
	
		# Singular - Plural
		if (error_rate == 1):
			self.play_mp3_queue.append(self.ivyConfig.mp3_path + "error_a" + ".ogg")
		elif (error_rate > 1):
			self.play_mp3_queue.append(self.ivyConfig.mp3_path + "error_b" + ".ogg")
		
		# Tell landing sinkrate and g forces
		self.play_mp3_queue.append(self.ivyConfig.mp3_path + "landing_rate" + ".ogg")
		self.SpellOutNumber(sink_rate)
		self.play_mp3_queue.append(self.ivyConfig.mp3_path + "landing_feet" + ".ogg")
		
		self.play_mp3_queue.append(self.ivyConfig.mp3_path + "landing_g" + ".ogg")
		self.SpellOutNumber(g_force_int)
		self.play_mp3_queue.append(self.ivyConfig.mp3_path + "dot" + ".ogg")
		self.SpellOutNumber(g_force_dec_2)
		
		# Tell bounces
		if (self.landing_bounces <= 1):
			self.play_mp3_queue.append(self.ivyConfig.mp3_path + "no_bounce" + ".ogg")
		else:

			self.play_mp3_queue.append(self.ivyConfig.mp3_path + "bounce1" + ".ogg")
			self.SpellOutNumber(self.landing_bounces-1)
			if (self.landing_bounces == 2): 
				self.play_mp3_queue.append(self.ivyConfig.mp3_path + "bounce2s" + ".ogg") # singular
			else:
				self.play_mp3_queue.append(self.ivyConfig.mp3_path + "bounce2" + ".ogg")
			
		flight_time 		= int(self.time - self.ivyPosRateClimb.time_activated)
		flight_hours 		= str(int(flight_time/3600))
		flight_minutes 		= str(int((flight_time % 3600)/60))
		flight_seconds  	= str(flight_time % 60)
		
		if (len(flight_hours) <= 1): 	flight_hours 	= "0" + flight_hours
		if (len(flight_minutes) <= 1): 	flight_minutes 	= "0" + flight_minutes
		if (len(flight_seconds) <= 1): 	flight_seconds 	= "0" + flight_seconds
		
		# Logbook
		
		lba_acf_descrip= []
		XPLMGetDatab(self.s_acf_descrip,lba_acf_descrip,0,240) 	
		aircraft_name = str(lba_acf_descrip) 
  
		airport_arrival_ref = XPLMFindNavAid(None, None, self.ld_latitude, self.ld_longitude, None, xplm_Nav_Airport)	
		airport_name = []
		XPLMGetNavAidInfo(airport_arrival_ref, None, None, None, None, None, None, airport_name, None, None)								   
		self.airport_arrival = airport_name[0]
		acf_len = int(self.ivyConfig.log_afc_name_length)
		aircraft_short = aircraft_name + (" " * acf_len)
		aircraft_short = aircraft_short[:acf_len]
		
		now = datetime.datetime.now()
		year = str(now.year)
	
		if (self.landing_rated == 1):	grade="A"
		elif (self.landing_rated == 2):	grade="B"
		elif (self.landing_rated == 3):	grade="C"
		elif (self.landing_rated == 4):	grade="D"
		else:							grade="F"
		
		if (self.landing_bounces >= 1):   self.landing_bounces = self.landing_bounces - 1
		
		# Format strings caused random errors, using manual alignment instead
		sink_rate_str 		= (max(5-len(str(sink_rate)),0) * " ") 				+ str(sink_rate)
		g_force_int_str 	= (max(2-len(str(g_force_int)),0) * " ") 			+ str(g_force_int)
		g_force_dec_2_str 	= str(g_force_dec_2) 								+ (max(2-len(str(g_force_dec_2)),0) * " ") # decimal part needs spaces afterwards
		bounces_str 		= (max(3-len(str(self.landing_bounces)),0) * " ")	+ str(self.landing_bounces)
		error_rate_str		= (max(3-len(str(error_rate)),0) * " ") 			+ str(error_rate)
		
		dep_str 			= (max(6-len(str(self.airport_departure)),0) * " ") + str(self.airport_departure)
		app_str				= (max(6-len(str(self.airport_arrival)),0) * " ") 	+ str(self.airport_arrival)
		month				= (max(2-len(str(now.month)),0) * " ") 				+ str(now.month)
		day					= (max(2-len(str(now.day)),0) * " ") 				+ str(now.day)
		
		logbook_entry   = ""
		logbook_entry 	= logbook_entry + year + "/" + month + "/" + day + " "
		logbook_entry 	= logbook_entry + "Aircraft: " + aircraft_short + ", "
		logbook_entry   = logbook_entry + "Dep: " + dep_str + ", "		
		logbook_entry   = logbook_entry + "Arr: " + app_str + ", "
		logbook_entry   = logbook_entry + "Flight Time: " + flight_hours + ":" + flight_minutes + ":" + flight_seconds + ", "
		logbook_entry   = logbook_entry + "Errors: " + error_rate_str + ", " 
		
		logbook_entry   = logbook_entry + "Landing: " + grade + ", " + sink_rate_str+ " ft/min, " + g_force_int_str + "." + g_force_dec_2_str + "g, " + bounces_str + " bounce(s)\n\r"
		
		# Do not write in replay mode
		if (self.li_replay == 0):
			logbook_file 	= open(self.ivyConfig.logbook_path, 'a+')
			logbook_file.write(logbook_entry)
			logbook_file.close()
			
			# Reset error counters
			for obj_number in range(0,len(self.ivy_object_list)):
				self.ivy_object_list[obj_number].error_count = 0
		
		pass
	

	##########################################################################################################################################################################################################
	# FlightLoopCallback
	#
	# This is where all the error detection is performed
	
	def FlightLoopCallback(self, elapsedMe, elapsedSim, counter, refcon):
	
		self.time = self.time + self.ivyConfig.data_rate
		
		self.passengersScreaming = False
		self.passengerVolume = 0.3
		
		# We reset the aircraft loaded situation after 60 seconds
		if (self.time > (60 + self.ivyConfig.disable_after_loading)): 
			self.aircraft_loaded = 0
		
		# Get all the fresh data from the datarefs
		if (self.plugin_enabled == 1):
			self.ReadData()
		
		# If started to play queue file, we deactivate for X cycles
		# Currently deactivated, as it seems we do not need this
		#if (self.deact_queue > 0):
		#	self.deact_queue = self.deact_queue - 1
		
		# Playlist. Here we can queue text that is longer than a single mp3
		# If we still have to say something, error detection is disabled. We would not have time to say it anyways.
		if (self.plugin_enabled == 0):
			pass
		elif (len(self.play_mp3_queue) > 0):
			if (self.ivyChannel.get_busy() == False):
				actsound = pygame.mixer.Sound(self.play_mp3_queue[0])
				self.ivyChannel.play(actsound)
				del self.play_mp3_queue[0]
				self.deact_queue = self.ivyConfig.deact_after_queue
		
		##########################################################################################################################################################################################################
		# NOT after Load and NOT after Crash

		elif ((self.time > self.ivyConfig.disable_after_loading) and (self.aircraft_crashed == 0) and (self.li_replay == 0)):
	
			if (self.lf_g_normal > self.f_g_normal_max): self.f_g_normal_max = self.lf_g_normal
			if (self.lf_g_side > self.f_g_side_max): self.f_g_side_max = self.lf_g_side
			if (self.lf_g_forward > self.f_g_forward_max): self.f_g_forward_max = self.lf_g_forward
			
			if (self.lf_g_normal < self.f_g_normal_min): self.f_g_normal_min = self.lf_g_normal
			if (self.lf_g_side < self.f_g_side_min): self.f_g_side_min = self.lf_g_side
			if (self.lf_g_forward < self.f_g_forward_min): self.f_g_forward_min = self.lf_g_forward
			

			
			self.f_g_normal_delta = (self.lf_g_normal - self.f_g_normal_old) / self.ivyConfig.data_rate
			self.f_g_side_delta = (self.lf_g_side - self.f_g_side_old) / self.ivyConfig.data_rate
			self.f_g_front_delta = (self.lf_g_forward - self.f_g_forward_old) / self.ivyConfig.data_rate
		
			self.f_g_normal_old = self.lf_g_normal
			self.f_g_side_old = self.lf_g_side
			self.f_g_forward_old = self.lf_g_forward
			
			if (self.cab_press_old != 0): 	self.cab_press_rate = 60 * (self.lf_cab_press - self.cab_press_old) / (self.li_sim_ground_speed * self.ivyConfig.data_rate)
			self.cab_press_old = self.lf_cab_press
			
			
			
			self.DetectLanding()
			
			
			# Ouch when bumping on ground
			if ((self.li_on_ground == 1) and ((self.lf_g_normal) > self.ivyConfig.ivy_ouch_g)): #play ouch
				actsound = pygame.mixer.Sound(self.ivyConfig.mp3_path + "ouch_1.ogg")
				self.ivyChannel.play(actsound)
			
			# Check for announcemnt to make
			self.CheckAnnouncement()
			
			
			
			# Announcement deactivation only. Either activated by command or by CheckAnnouncement
			if (self.li_on_ground == 1):																												self.ivyAnnounceLanding.Deactivate(self.time)
			if (self.li_on_ground == 0):																												self.ivyAnnounceTakeOff.Deactivate(self.time)
			
			if ((self.li_on_ground == 0) and (self.lf_climb_rate > self.ivyConfig.pos_rate_climb)):														self.ivyPosRateClimb.Activate(self.time)
			elif (self.li_on_ground == 1):																												self.ivyPosRateClimb.Deactivate(self.time)
			
			# Decision Height Arm
			if ((self.li_on_ground == 0) and (self.lf_radio_alt > (self.lf_decision_height + self.ivyConfig.decition_height_arm))):						self.ivyArmMinimums.Activate(self.time)
			elif (self.li_on_ground == 1):																												self.ivyArmMinimums.Deactivate(self.time)
			
			if ((self.li_on_ground == 0) and 
			   (self.ivyArmMinimums.played == 1) and
			   (self.lf_decision_height > 0) and 
			   (self.lf_radio_alt < (self.lf_decision_height + self.ivyConfig.decition_height_plus))):													self.ivyMinimums.Activate(self.time)
			elif (self.lf_radio_alt > (self.lf_decision_height + self.ivyConfig.decition_height_arm)):													self.ivyMinimums.Deactivate(self.time)

			
			# Fasten Seatbelts
			if (self.li_fastenseatbelts > 0):																											
																																						self.ivySeatBelts.Activate(self.time)
			else:																																		self.ivySeatBelts.Deactivate(self.time)

			# Gear down callout
			#if (self.lf_gear1_ratio > 0.999):																											self.ivyGearDown.Activate(self.time)
			#else:																																		self.ivyGearDown.Deactivate(self.time)
			
			if ((self.lf_gear1_ratio == 1) and 
			   (self.lf_gear2_ratio in range(0,2)) and 
			   (self.lf_gear3_ratio in range(0,2)) and 
			   (self.lf_gear4_ratio in range(0,2)) and 
			   (self.lf_gear5_ratio in range(0,2))):																									self.ivyGearDown.Activate(self.time)
			else:																																		self.ivyGearDown.Deactivate(self.time)
			
			# Gear up callout
			#if (self.lf_gear1_ratio < 0.001):																											self.ivyGearUp.Activate(self.time)
			#else:																																		self.ivyGearUp.Deactivate(self.time)
			
			if ((self.lf_gear1_ratio == 0) and 
			   (self.lf_gear2_ratio in range(0,2)) and 
			   (self.lf_gear3_ratio in range(0,2)) and 
			   (self.lf_gear4_ratio in range(0,2)) and 
			   (self.lf_gear5_ratio in range(0,2))):																									self.ivyGearUp.Activate(self.time)
			else:																																		self.ivyGearUp.Deactivate(self.time)
			
			# Tire blown
			if ((self.li_tire1 + self.li_tire2 + self.li_tire3 + self.li_tire4 + self.li_tire5) > 0):													self.ivyTyre.Activate(self.time)
			else:																																		self.ivyTyre.Deactivate(self.time)
			
			# Hard braking
			if ((self.li_on_ground == 1) and (self.lf_g_forward > self.ivyConfig.brake_max_forward_g)): 												self.ivyBrake.Activate(self.time)
			else:																																		self.ivyBrake.Deactivate(self.time)		
			
			# Transponder not active when airborne
			if ((self.li_on_ground == 0) and (self.li_transponder_mode < 2)):																			self.ivyTransponder.Activate(self.time) # Dataref: Mode2=ON, B407 4=ON
			elif (self.li_on_ground == 1):																												self.ivyTransponder.Deactivate(self.time)
			
			# Landing lights not on when landing in the night
			if ((self.li_on_ground == 0) and (self.lf_radio_alt < self.ivyConfig.alt_landing_lights_low) and 
			    (self.lf_world_light_precent > self.ivyConfig.night_world_light_precent) and (self.li_landing_lights == 0)):							self.ivyLandingLights.Activate(self.time)
			else:																																		self.ivyLandingLights.Deactivate(self.time)
			
			# Landing lights not off on high altitude
			if ((self.li_on_ground == 0) and (self.lf_radio_alt > 3000) and (self.lf_baro_alt > self.ivyConfig.alt_landing_lights_high) and 
			   (self.li_landing_lights == 1)):																											self.ivyLandingLightsHigh.Activate(self.time)
			else:																																		self.ivyLandingLightsHigh.Deactivate(self.time)
			
			# Beacon lights not when taxiing
			if ((self.li_on_ground == 1) and (self.lf_ground_speed > self.ivyConfig.taxi_ground_speed_min) and (self.li_beacon_lights == 0)):			self.ivyBeaconLights.Activate(self.time) 
			elif (self.li_beacon_lights != 0):																											self.ivyBeaconLights.Deactivate(self.time)
			
			# Nav lights lights not when airborne			
			if ((self.li_on_ground == 0) and (self.li_nav_lights == 0)):																				self.ivyNavLights.Activate(self.time) 
			else:																																		self.ivyNavLights.Deactivate(self.time) 
			
			# Strobes not on when airborne
			if ((self.li_on_ground == 0) and (self.li_strobe_lights == 0)):																				self.ivyStrobes.Activate(self.time) 				
			else:																																		self.ivyStrobes.Deactivate(self.time)
			
			# Comment on X-Plane tire blown message when aircraft has skid
			if (((self.li_tire1 + self.li_tire2 + self.li_tire3 + self.li_tire4 + self.li_tire5) > 0) and (self.li_has_skid == 1)):						self.ivySkidTyres.Activate(self.time) 			
			else:																																		self.ivySkidTyres.Deactivate(self.time)
			
			# Battery low
			if ((self.li_batt1 + self.li_batt2) > 0):																									self.ivyBatteryOut.Activate(self.time) 			
			else:																																		self.ivyBatteryOut.Deactivate(self.time) 
			
			# Engine fire
			if ((self.li_fire1 + self.li_fire2 + self.li_fire3 + self.li_fire4 + self.li_fire5 + self.li_fire6 + self.li_fire7 + self.li_fire8) > 0):	self.ivyEngineFire.Activate(self.time) 			
			else:																																		self.ivyEngineFire.Deactivate(self.time)
			
			# Engine flameout
			if ((self.li_flameout1 + self.li_flameout2 + self.li_flameout3 + self.li_flameout4 + 
				 self.li_flameout5 + self.li_flameout6 + self.li_flameout7 + self.li_flameout8) > 0):													self.ivyEngineFlameout.Activate(self.time) 	
			else:																																		self.ivyEngineFlameout.Deactivate(self.time)
			
			# Engine ground failure
			if ((self.li_on_ground == 1) and 
			   ((self.li_engine_failure1 + self.li_engine_failure2 + self.li_engine_failure3 + self.li_engine_failure4 + 
				 self.li_engine_failure5 + self.li_engine_failure6 + self.li_engine_failure7 + self.li_engine_failure8) > 0)):							self.ivyEngineFailureGround.Activate(self.time) 	
			else:																																		self.ivyEngineFailureGround.Deactivate(self.time)
			
			# Engine airborne failure
			if ((self.li_on_ground == 0) and 
			   ((self.li_engine_failure1 + self.li_engine_failure2 + self.li_engine_failure3 + self.li_engine_failure4 + 
				 self.li_engine_failure5 + self.li_engine_failure6 + self.li_engine_failure7 + self.li_engine_failure8) > 0)):							self.ivyEngineFailureAir.Activate(self.time) 		
			else:																																		self.ivyEngineFailureAir.Deactivate(self.time)
			
			# Engine hot start
			if ((self.li_hot1 + self.li_hot2 + self.li_hot3 + self.li_hot4 + self.li_hot5 + self.li_hot6 + self.li_hot7 + self.li_hot8) > 0):			self.ivyEngineHotStart.Activate(self.time) 		
			else:																																		self.ivyEngineHotStart.Deactivate(self.time)
			
			# Battery not on
			if ((self.li_battery_on == 0) and (self.li_gpu_on == 0)):																					self.ivyNoBatt.Activate(self.time) 				
			else:																																		self.ivyNoBatt.Deactivate(self.time)
			
			# Flaps Overspeed
			if (self.li_flaps_overspeed > 0):																											self.ivyOverspeedFlaps.Activate(self.time) 				
			else:																																		self.ivyOverspeedFlaps.Deactivate(self.time)
			
			# Gear Overspeed
			if (self.li_gear_overspeed > 0):																											self.ivyOverspeedGear.Activate(self.time) 				
			else:																																		self.ivyOverspeedGear.Deactivate(self.time)
			
			# Stall
			if (self.li_stall > 0):																														self.ivyStall.Activate(self.time) 				
			else:																																		self.ivyStall.Deactivate(self.time)
			
			# Aircraft Overspeed
			if (self.lf_ias > self.lf_aircraft_vne) and (self.lf_aircraft_vne > 1):																		self.ivyOverspeedAircraft.Activate(self.time) 				
			else:																																		self.ivyOverspeedAircraft.Deactivate(self.time)
			
			# Hello - Depending on weather
			if ((self.aircraft_loaded == 1) and 
				(self.li_cloud_0 < 1) and (self.li_cloud_1 < 1) and (self.li_cloud_2 < 1) and 
				(self.lf_visibility > self.ivyConfig.vis_is_fog) and (self.li_rain == 0) and (self.li_thunder == 0)):									self.ivyHelloSun.Activate(self.time) 				
			else:																																		self.ivyHelloSun.Deactivate(self.time)
			
			if ((self.aircraft_loaded == 1) and 
				(self.lf_visibility > self.ivyConfig.vis_is_fog) and (self.li_rain > 0) and (self.li_thunder == 0)):									self.ivyHelloRain.Activate(self.time) 			
			else:																																		self.ivyHelloRain.Deactivate(self.time)
			
			#ToDo:
			#if ((self.aircraft_loaded == 1) and 
			#	(self.lf_visibility > self.ivyConfig.vis_is_fog) and (self.li_thunder > 0)):															self.ivyHelloThunder.Activate(self.time) 			
			#else:																																		self.ivyHelloThunder.Deactivate(self.time)
			
			if ((self.aircraft_loaded == 1) and 
				(self.lf_visibility <= self.ivyConfig.vis_is_fog)):																						self.ivyHelloFog.Activate(self.time) 				
			else:																																		self.ivyHelloFog.Deactivate(self.time)
			
			if ((self.aircraft_loaded == 1) and 
				((self.li_cloud_0 >= 1) or (self.li_cloud_1 >= 1) or (self.li_cloud_2 >= 1)) and 
				(self.lf_visibility > self.ivyConfig.vis_is_fog) and (self.li_rain == 0) and (self.li_thunder == 0)):									self.ivyHelloNormal.Activate(self.time) 			
			else:																																		self.ivyHelloNormal.Deactivate(self.time)
			
			# Cabin pressure falling too fast
			# Some aircraft do not get it right, when you increase the ground speed. Hence, I use both, my own and the Aircraft computation
			if ((self.li_on_ground == 0) and (max(self.lf_cab_rate, self.lf_climb_rate) < self.ivyConfig.cab_rate_low)):								self.ivyCabinDownNormal.Activate(self.time) 		
			elif (max(self.lf_cab_rate, self.lf_climb_rate) > (self.ivyConfig.cab_rate_low + self.ivyConfig.cab_rate_reset_hysteresis)):				self.ivyCabinDownNormal.Deactivate(self.time)
			
			# Cabin pressure falling rapidely
			if ((self.li_on_ground == 0) and (max(self.lf_cab_rate, self.lf_climb_rate) < self.ivyConfig.cab_rate_high)):																				
																																						self.ivyCabinDownFast.Activate(self.time) 		
																																						self.ivyCabinDownNormal.SetAsPlayed(self.time)
			elif (max(self.lf_cab_rate, self.lf_climb_rate) > (self.ivyConfig.cab_rate_high + self.ivyConfig.cab_rate_reset_hysteresis)):				self.ivyCabinDownFast.Deactivate(self.time)
			
			# Bank angle pre-warning
			if ((self.li_on_ground == 0) and (abs(self.lf_roll) > self.ivyConfig.bank_low)):															self.ivyBankNormal.Activate(self.time) 			
			elif (abs(self.lf_roll) < self.ivyConfig.bank_reset_low):																					self.ivyBankNormal.Deactivate(self.time)
			
			# Bank angle too high
			if ((self.li_on_ground == 0) and (abs(self.lf_roll) > self.ivyConfig.bank_high)):		
																																						self.ivyBankHigh.Activate(self.time) 
																																						self.ivyBankNormal.SetAsPlayed(self.time)
			elif (abs(self.lf_roll) < self.ivyConfig.bank_low):																							self.ivyBankHigh.Deactivate(self.time)
			
			# Bank angle extremely high
			if ((self.li_on_ground == 0) and (abs(self.lf_roll) > self.ivyConfig.bank_xhigh)):															
																																						self.passengersScreaming = True
																																						self.ivyBankXHigh.Activate(self.time) 
																																						self.ivyBankHigh.SetAsPlayed(self.time)
																																						self.ivyBankNormal.SetAsPlayed(self.time)		
			elif (abs(self.lf_roll) < self.ivyConfig.bank_high):																						self.ivyBankXHigh.Deactivate(self.time)
			
			
			# Pitch down pre-warning
			if ((self.li_on_ground == 0) and (self.lf_pitch < self.ivyConfig.pitch_low)):																self.ivyPitchDownNormal.Activate(self.time) 		
			elif (self.lf_pitch > self.ivyConfig.pitch_reset_low):																						self.ivyPitchDownNormal.Deactivate(self.time)
			
			# Pitch too low
			if ((self.li_on_ground == 0) and (self.lf_pitch <= self.ivyConfig.pitch_high)):																					
																																						self.passengersScreaming = True
																																						self.ivyPitchDownHigh.Activate(self.time) 
																																						self.ivyPitchDownNormal.SetAsPlayed(self.time)
			elif (self.lf_pitch > self.ivyConfig.pitch_low):																							self.ivyPitchDownHigh.Deactivate(self.time)
			
			
			# Normal G Force high
			if ((self.li_on_ground == 0) and (self.lf_g_normal >= self.ivyConfig.max_g_down_low)):														
																																						self.passengersScreaming = True
																																						self.passengerVolume = max (self.passengerVolume, abs(self.lf_g_normal) / 6)
																																						self.ivyGNormalFlightNormal.Activate(self.time) 	
			elif (self.lf_g_normal <= self.ivyConfig.max_g_down_low_reset):																				self.ivyGNormalFlightNormal.Deactivate(self.time)
			
			# Normal G Force very high
			if ((self.li_on_ground == 0) and (self.lf_g_normal >= self.ivyConfig.max_g_down_high)):														
																																						self.ivyGNormalFlightHigh.Activate(self.time) 
																																						self.ivyGNormalFlightNormal.SetAsPlayed(self.time)
			elif (self.lf_g_normal <= self.ivyConfig.max_g_down_low_reset):																				self.ivyGNormalFlightHigh.Deactivate(self.time)
			
			# Normal G Force very, very high
			if ((self.li_on_ground == 0) and (self.lf_g_normal >= self.ivyConfig.max_g_down_xhigh)):																					
																																						self.ivyGNormalFlightXHigh.Activate(self.time)
																																						self.ivyGNormalFlightHigh.SetAsPlayed(self.time)
																																						self.ivyGNormalFlightNormal.SetAsPlayed(self.time)
			elif (self.lf_g_normal <= self.ivyConfig.max_g_down_low_reset):																				self.ivyGNormalFlightXHigh.Deactivate(self.time)
			
			
			# Normal G Force too low
			if ((self.li_on_ground == 0) and (self.lf_g_normal <= 0.5)):																				
																																						self.passengersScreaming = True
																																						self.passengerVolume = max (self.passengerVolume, abs(self.lf_g_normal - 0.5) / 2)
																																						self.ivyGNormalNegativeLow.Activate(self.time) 	
			elif (self.lf_g_normal > 0.8):																												self.ivyGNormalNegativeLow.Deactivate(self.time)

			# Normal G Force negative
			if ((self.li_on_ground == 0) and (self.lf_g_normal <= 0)):																					
																																						self.ivyGNormalNegativeHigh.Activate(self.time) 
																																						self.ivyGNormalNegativeLow.SetAsPlayed(self.time)
			elif (self.lf_g_normal > 0.5):																												self.ivyGNormalNegativeHigh.Deactivate(self.time)			
					
			# TBD
			#self.li_turbulence = XPLMGetDatai(self.i_turbulence)
			#if ((self.li_on_ground == 0) and (self.li_turbulence > 10)):																				self.ivyTurbulenceNormal.Activate(self.time) 		
			#elif (self.li_turbulence < 2):																												self.ivyTurbulenceNormal.Deactivate(self.time)
			
			#if ((self.li_on_ground == 0) and (self.li_turbulence > 30)):																				self.ivyTurbolenceHigh.Activate(self.time) 		
			#elif (self.li_turbulence < 5):																												self.ivyTurbolenceHigh.Deactivate(self.time)
			
			# Barometric pressure not set accordingly while close to ground or taxiing (within tolerance)
			if ((self.lf_radio_alt < self.ivyConfig.baro_alt_low) and 
			    (abs(self.li_baro_set - self.li_baro_sea_level) > self.ivyConfig.baro_tolerance) and 
				(self.lf_ground_speed > self.ivyConfig.taxi_ground_speed_min)):																
																																						self.ivyBaroGround.Activate(self.time)
																																						if ((self.ivyBaroGround.played == 1) and (self.pressure_said == 0)): 
																																							self.pressure_said = 1
																																							self.SayBaro()
			elif ((abs(self.li_baro_set - self.li_baro_sea_level) <= self.ivyConfig.baro_tolerance) or (self.lf_baro_alt > self.ivyConfig.trans_alt)) :
																																						self.ivyBaroGround.Deactivate(self.time)
																																						self.pressure_said = 0
			
			# Barometric pressure not set to standard above transition altitude
			if ((self.lf_baro_alt > (self.ivyConfig.trans_alt + self.ivyConfig.trans_hysteresis)) and 
			    (abs(2992 - self.li_baro_set) > self.ivyConfig.baro_tolerance)):																		self.ivyBaroHigh.Activate(self.time)
			else:																																		self.ivyBaroHigh.Deactivate(self.time)
			
			# 60 knots callout
			if ((self.li_on_ground == 1) and (self.ivyConfig.kt60_enabled == True) and (self.lf_ias > 58) and (self.lf_ias < 70)):						self.ivy60kt.Activate(self.time) 
			else:																																		self.ivy60kt.Deactivate(self.time)
			
			# 80 knots callout
			if ((self.li_on_ground == 1) and (self.ivyConfig.kt80_enabled == True) and (self.lf_ias > 78) and (self.lf_ias < 90)):						self.ivy80kt.Activate(self.time) 
			else:																																		self.ivy80kt.Deactivate(self.time)
			
			# 100 knots callout
			if ((self.li_on_ground == 1) and (self.ivyConfig.kt100_enabled == True) and (self.lf_ias > 98) and (self.lf_ias < 110)):					self.ivy100kt.Activate(self.time) 
			else:																																		self.ivy100kt.Deactivate(self.time)
			
			# Not rotated
			if ((self.li_on_ground == 1) and (self.lf_ias > 180) and (self.landing_detected == 0)):														self.ivyRotate.Activate(self.time) 
			else:																																		self.ivyRotate.Deactivate(self.time)
			
			# Ice airframe low
			if (self.lf_ice_frame > self.ivyConfig.ice_low):																							self.ivyIceAirframeLow.Activate(self.time)
			else:																																		self.ivyIceAirframeLow.Deactivate(self.time)
			
			# Ice airframe high
			if (self.lf_ice_frame > self.ivyConfig.ice_high):																												
																																						self.ivyIceAirframeHigh.Activate(self.time)
																																						self.ivyIceAirframeLow.SetAsPlayed(self.time)
			else:																																		self.ivyIceAirframeHigh.Deactivate(self.time)
			
			# Ice pitot low
			if (self.lf_ice_pitot > self.ivyConfig.ice_low):																							self.ivyIcePitotLow.Activate(self.time)
			else:																																		self.ivyIcePitotLow.Deactivate(self.time)
			
			# Ice pitot high
			if (self.lf_ice_pitot > self.ivyConfig.ice_high):																												
																																						self.ivyIcePitotHigh.Activate(self.time)
																																						self.ivyIcePitotLow.SetAsPlayed(self.time)
			else:																																		self.ivyIcePitotHigh.Deactivate(self.time)
			
			# Ice propeller low
			if (self.lf_ice_propeller > self.ivyConfig.ice_low):																						self.ivyIcePropellerLow.Activate(self.time)
			else:																																		self.ivyIcePropellerLow.Deactivate(self.time)
			
			# Ice propeller high
			if (self.lf_ice_propeller > self.ivyConfig.ice_high):																												
																																						self.ivyIcePropellerHigh.Activate(self.time)
																																						self.ivyIcePropellerLow.SetAsPlayed(self.time)
			else:																																		self.ivyIcePropellerHigh.Deactivate(self.time)
			
			# Ice cockpit window low
			if (self.lf_ice_window > self.ivyConfig.ice_low):																							self.ivyIceWindowLow.Activate(self.time)
			else:																																		self.ivyIceWindowLow.Deactivate(self.time)
			
			# Ice cockpit window high
			if (self.lf_ice_window > self.ivyConfig.ice_high):																												
																																						self.ivyIceWindowHigh.Activate(self.time)
																																						self.ivyIceWindowLow.SetAsPlayed(self.time)
			else:																																		self.ivyIceWindowHigh.Deactivate(self.time)
			
			# Cabin pressure low
			if (self.lf_cab_press > self.ivyConfig.cab_press_low):																						self.ivyPressureLow.Activate(self.time) 
			else:																																		self.ivyPressureLow.Deactivate(self.time)
			
			# Cabin pressure too low to breath
			if (self.lf_cab_press > self.ivyConfig.cab_press_high):																												
																																						self.ivyPressureXLow.Activate(self.time) 
																																						self.ivyPressureLow.SetAsPlayed(self.time)
			else:																																		self.ivyPressureXLow.Deactivate(self.time)
			
			# Birdstrike
			if (self.li_bird != 0):																														self.ivyBirdStrike.Activate(self.time)
			else:																																		self.ivyBirdStrike.Deactivate(self.time)
			
			##########################################################################################################################################################################################################
			# V-Speed callouts if configured in IvyAircraft ini file
			# V1
			# VR
			# V2
			# V2 not achieved within 5 seconds after take off
			
			if ((self.li_on_ground == 1) and (self.ivyAircraft.li_v1 > 0) and 
			    (self.lf_ias >= self.ivyAircraft.li_v1) and (self.lf_ground_speed > self.ivyConfig.taxi_ground_speed_min)):								self.ivyV1.Activate(self.time)
			elif ((self.li_on_ground == 1) and (self.lf_ias < 10)):																						self.ivyV1.Deactivate(self.time)
			
			if ((self.li_on_ground == 1) and (self.ivyAircraft.li_vr > 0) and 
			    (self.lf_ias >= self.ivyAircraft.li_vr) and (self.lf_ground_speed > self.ivyConfig.taxi_ground_speed_min)):								self.ivyVR.Activate(self.time)
			elif ((self.li_on_ground == 1) and (self.lf_ias < 10)):																						self.ivyVR.Deactivate(self.time)
			
			if ((self.li_on_ground == 0) and (self.ivyAircraft.li_v2 > 0) and 
			    (self.lf_ias >= self.ivyAircraft.li_v2)):																								self.ivyAboveV2.Activate(self.time)
			elif (self.li_on_ground == 1):																												self.ivyAboveV2.Deactivate(self.time)
			
			if ((self.li_on_ground == 0) and (self.ivyAircraft.li_v2 > 0) and 
			    (self.lf_ias < self.ivyAircraft.li_v2) and (self.ivyAboveV2.played == 0)):																
																																						self.ivyBelowV2.Activate(self.time)
																																						self.ivyAboveV2.Deactivate(self.time)
			elif (self.li_on_ground == 1):																												self.ivyBelowV2.Deactivate(self.time)
			
			
			##########################################################################################################################################################################################################
			# Slats callout if configured in IvyAircraft ini file
			if ((self.ivyAircraft.slats_enabled == True) and (self.ivyAircraft.lf_slats == 0)):															self.ivySlatsRetracted.Activate(self.time)
			else: 																																		self.ivySlatsRetracted.Deactivate(self.time)
			
			if (self.ivyAircraft.slats_enabled == True):
				slats_activated = False
				for index in range(0,len(self.ivyAircraft.slats_deploy_value)):
					if (abs(self.ivyAircraft.lf_slats - self.ivyAircraft.slats_deploy_value[index]) < self.ivyAircraft.slats_tolerance):
						slats_activated = True
						if (self.ivySlatsPosition.active == 0) and (self.ivyChannel.get_busy() == False):
																																						self.ivySlatsPosition.Activate(self.time)
																																						self.SpellOutNumber(self.ivyAircraft.slats_deploy_pos[index])																										
				if (slats_activated == False):																											self.ivySlatsPosition.Deactivate(self.time)


			##########################################################################################################################################################################################################
			# Flaps callout if configured in IvyAircraft ini file
			if ((self.ivyAircraft.flaps_enabled == True) and (self.ivyAircraft.lf_flaps < self.ivyAircraft.flaps_tolerance)):							self.ivyFlapsRetracted.Activate(self.time)
			else: 																																		self.ivyFlapsRetracted.Deactivate(self.time)
			
			if (self.ivyAircraft.flaps_enabled == True):
				flaps_activated = False
				for index in range(0,len(self.ivyAircraft.flaps_deploy_value)):
					if (abs(self.ivyAircraft.lf_flaps - self.ivyAircraft.flaps_deploy_value[index]) < self.ivyAircraft.flaps_tolerance):
						flaps_activated = True
						if (self.ivyFlapsPosition.active == 0) and (self.ivyChannel.get_busy() == False):
																																						self.ivyFlapsPosition.Activate(self.time)
																																						self.SpellOutNumber(self.ivyAircraft.flaps_deploy_pos[index])																										
				if (flaps_activated == False):																											self.ivyFlapsPosition.Deactivate(self.time)				
			
			##########################################################################################################################################################################################################
			# After landing:
			# Rating of the Landing
			# End of flight evaluation
			
			# We want to get airborne before landing evaluation - too many false alarms on load
			if ((self.li_on_ground == 0) and (self.lf_radio_alt > 100) and (self.lf_climb_rate > 100)):													self.ivyArmLanding.Activate(self.time)
			
			if ((self.landing_detected == 1) and (self.landing_rated > 0) and 
			    (self.ivyArmLanding.played == 1) and (self.ivyConfig.passengers_enabled == True)):														self.ivyApplause.Activate(self.time)
			elif (self.li_on_ground == 0):																												self.ivyApplause.Deactivate(self.time)
			
			
			if ((self.landing_detected == 1) and (self.landing_rated == 1) and (self.lf_ground_speed < self.ivyConfig.taxi_ground_speed_min) and (self.ivyArmLanding.played == 1)):																			
																																						self.ivyLandingXGood.Activate(self.time) 	
																																						if (self.ivyLandingXGood.played == 1):	
																																							self.EndOfFlightEvaluation()
																																							self.ivyArmLanding.Deactivate(self.time)
			else:																																		self.ivyLandingXGood.Deactivate(self.time)
			
			if ((self.landing_detected == 1) and (self.landing_rated == 2) and (self.lf_ground_speed < self.ivyConfig.taxi_ground_speed_min) and (self.ivyArmLanding.played == 1)):																			
																																						self.ivyLandingGood.Activate(self.time) 	
																																						if (self.ivyLandingGood.played == 1):	
																																							self.EndOfFlightEvaluation()
																																							self.ivyArmLanding.Deactivate(self.time)
			else:																																		self.ivyLandingGood.Deactivate(self.time)
			
			if ((self.landing_detected == 1) and (self.landing_rated == 3) and (self.lf_ground_speed < self.ivyConfig.taxi_ground_speed_min) and (self.ivyArmLanding.played == 1)):																			
																																						self.ivyLandingNormal.Activate(self.time) 	
																																						if (self.ivyLandingNormal.played == 1):	
																																							self.EndOfFlightEvaluation()
																																							self.ivyArmLanding.Deactivate(self.time)
			else:																																		self.ivyLandingNormal.Deactivate(self.time)
			
			if ((self.landing_detected == 1) and (self.landing_rated == 4) and (self.lf_ground_speed < self.ivyConfig.taxi_ground_speed_min) and (self.ivyArmLanding.played == 1)):																			
																																						self.ivyLandingBad.Activate(self.time) 	
																																						if (self.ivyLandingBad.played == 1):	
																																							self.EndOfFlightEvaluation()
																																							self.ivyArmLanding.Deactivate(self.time)
			else:																																		self.ivyLandingBad.Deactivate(self.time)
			
			
			if ((self.landing_detected == 1) and (self.landing_rated == 5) and (self.lf_ground_speed < self.ivyConfig.taxi_ground_speed_min) and (self.ivyArmLanding.played == 1)):																			
																																						self.ivyLandingXBad.Activate(self.time) 
																																						if (self.ivyLandingXBad.played == 1):	
																																							self.EndOfFlightEvaluation()
																																							self.ivyArmLanding.Deactivate(self.time)
			else:																																		self.ivyLandingXBad.Deactivate(self.time)
		
		##########################################################################################################################################################################################################
		# After Load or after Crash

		if ((self.aircraft_crashed == 1) and (self.time > self.ivyConfig.disable_after_loading)):														self.ivyCrash.Activate(self.time) 			
		else:																																			self.ivyCrash.Deactivate(self.time)
		
		# Here comes the screaming
		self.passengerVolume = max (self.passengerVolume, abs(self.lf_roll) / 120)
		self.passengerVolume = max (self.passengerVolume, abs(self.lf_pitch) / 60)
		
		if (self.ivyConfig.passengers_enabled == True):		self.ivyPassengers.MakeScream(self.passengersScreaming, self.passengerVolume)
			
		#if ((self.lf_world_light_precent > 0.5) and (self.lf_climb_rate > 100)):  pass
		
		#buf = "Normal: " + "{:.2f}".format(self.f_g_normal_delta) + "Side: " + "{:.2f}".format(self.f_g_side_delta) + "Normal: " + "{:.2f}".format(self.f_g_forward_delta) + "\r\n"
		#self.OutputFile.write(buf)
		
		return self.ivyConfig.data_rate

	##########################################################################################################################################################################################################
	# ReadData
	#
	# This function get's all new values from the DataRefs
	# Calls IvyAircraft object for update too -> only call when aircraft is loaded

	def ReadData(self):
		#########################################################################################
		#
		#						DATAREF to Local Variables
		#
		#########################################################################################
				
		
		
		lba_acf_descrip= []
		lba_acf_tailnumber = [] 

		XPLMGetDatab(self.s_acf_descrip,lba_acf_descrip,0,240) 	
		XPLMGetDatab(self.s_acf_tailnumber,lba_acf_tailnumber,0,40)
		
		self.ls_acf_descrip = str(lba_acf_descrip) + str(lba_acf_tailnumber)
		
		
		self.li_on_ground= 				XPLMGetDatai(self.i_on_ground) 			
		self.lf_climb_rate= 			XPLMGetDataf(self.f_climb_rate) 
		
		self.lf_gear1_ratio=			XPLMGetDataf(self.f_gear1_ratio) 
		self.lf_gear2_ratio=			XPLMGetDataf(self.f_gear2_ratio) 
		self.lf_gear3_ratio=			XPLMGetDataf(self.f_gear3_ratio) 
		self.lf_gear4_ratio=			XPLMGetDataf(self.f_gear4_ratio) 
		self.lf_gear5_ratio=			XPLMGetDataf(self.f_gear5_ratio) 
		
		self.lf_ground_speed= 			XPLMGetDataf(self.f_ground_speed) 	* 3600/1852	# is m/s and we want nm/h
		self.lf_ias= 					XPLMGetDataf(self.f_ias)
		self.lf_sun_pitch= 				XPLMGetDataf(self.f_sun_pitch) 			
		self.lf_airport_light= 			XPLMGetDataf(self.f_airport_light) 		
		self.lf_world_light_precent= 	XPLMGetDataf(self.f_world_light_precent) 	
		self.li_has_skid= 				XPLMGetDatai(self.i_has_skid) 			
		self.li_transponder_mode= 		XPLMGetDatai(self.i_transponder_mode) 
		self.li_sim_ground_speed=		XPLMGetDatai(self.i_sim_ground_speed)
		
		self.li_temp_sl= 				XPLMGetDatai(self.i_temp_sl) 				
		self.li_dew_sl= 				XPLMGetDatai(self.i_dew_sl) 				
		
		self.li_landing_lights= 		XPLMGetDatai(self.i_landing_lights) 		
		self.li_beacon_lights= 			XPLMGetDatai(self.i_beacon_lights) 		
		self.li_nav_lights= 			XPLMGetDatai(self.i_nav_lights) 			
		self.li_strobe_lights= 			XPLMGetDatai(self.i_strobe_lights) 		
		self.li_taxi_lights= 			XPLMGetDatai(self.i_taxi_lights) 			
		self.li_cockpit_lights= 		XPLMGetDatai(self.i_cockpit_lights) 	
		self.lf_radio_alt= 				XPLMGetDataf(self.f_radio_alt)
		self.lf_decision_height=		XPLMGetDataf(self.f_decision_height)
		self.lf8_batter_charge= 		XPLMGetDataf(self.f8_batter_charge) 

		self.li_battery_on = 			XPLMGetDatai(self.i_battery_on)	
		self.li_gpu_on = 				XPLMGetDatai(self.i_gpu_on)	
		
		self.li_flaps_overspeed = 	    XPLMGetDatai(self.i_flaps_overspeed)	
		self.li_gear_overspeed = 	    XPLMGetDatai(self.i_gear_overspeed)
		self.li_aircraft_overspeed =    XPLMGetDatai(self.i_aircraft_overspeed)
		self.lf_aircraft_vne = 			XPLMGetDataf(self.f_aircraft_vne)
		self.li_stall = 				XPLMGetDatai(self.i_stall)

		self.li_cloud_0 = 				XPLMGetDatai(self.i_cloud_0) 				
		self.li_cloud_1 = 				XPLMGetDatai(self.i_cloud_1) 				
		self.li_cloud_2 = 				XPLMGetDatai(self.i_cloud_2) 				
		self.lf_visibility = 			XPLMGetDataf(self.f_visibility) 			
		self.li_rain = 					XPLMGetDatai(self.i_rain) 					
		self.li_thunder = 				XPLMGetDatai(self.i_thunder) 				
		self.li_turbulence = 			XPLMGetDatai(self.i_turbulence) 			
		
		
		self.li_batt1= 					XPLMGetDatai(self.i_batt1)			
		self.li_batt2= 					XPLMGetDatai(self.i_batt2)			
		
		self.li_tire1= 					XPLMGetDatai(self.i_tire1) 				
		self.li_tire2= 					XPLMGetDatai(self.i_tire2) 				
		self.li_tire3= 					XPLMGetDatai(self.i_tire3) 				
		self.li_tire4= 					XPLMGetDatai(self.i_tire4) 				
		self.li_tire5= 					XPLMGetDatai(self.i_tire5) 								
		
		self.li_fire1= 					XPLMGetDatai(self.i_fire1) 				
		self.li_fire2= 					XPLMGetDatai(self.i_fire2) 				
		self.li_fire3= 					XPLMGetDatai(self.i_fire3) 				
		self.li_fire4= 					XPLMGetDatai(self.i_fire4) 				
		self.li_fire5= 					XPLMGetDatai(self.i_fire5) 				
		self.li_fire6= 					XPLMGetDatai(self.i_fire6) 				
		self.li_fire7= 					XPLMGetDatai(self.i_fire7) 				
		self.li_fire8= 					XPLMGetDatai(self.i_fire8) 				
		
		self.li_flameout1= 				XPLMGetDatai(self.i_flameout1) 			
		self.li_flameout2= 				XPLMGetDatai(self.i_flameout2) 			
		self.li_flameout3=				XPLMGetDatai(self.i_flameout3) 			
		self.li_flameout4= 				XPLMGetDatai(self.i_flameout4) 			
		self.li_flameout5= 				XPLMGetDatai(self.i_flameout5) 			
		self.li_flameout6= 				XPLMGetDatai(self.i_flameout6) 			
		self.li_flameout7= 				XPLMGetDatai(self.i_flameout7) 			
		self.li_flameout8= 				XPLMGetDatai(self.i_flameout8) 			
		
		
		
		self.li_engine_failure1= 		XPLMGetDatai(self.i_engine_failure1) 		
		self.li_engine_failure2= 		XPLMGetDatai(self.i_engine_failure2) 		
		self.li_engine_failure3= 		XPLMGetDatai(self.i_engine_failure3) 		
		self.li_engine_failure4= 		XPLMGetDatai(self.i_engine_failure4) 		
		self.li_engine_failure5= 		XPLMGetDatai(self.i_engine_failure5) 		
		self.li_engine_failure6= 		XPLMGetDatai(self.i_engine_failure6) 		
		self.li_engine_failure7= 		XPLMGetDatai(self.i_engine_failure7) 		
		self.li_engine_failure8= 		XPLMGetDatai(self.i_engine_failure8) 		
		
		self.li_hot1= 					XPLMGetDatai(self.i_hot1) 					
		self.li_hot2= 					XPLMGetDatai(self.i_hot2) 					
		self.li_hot3= 					XPLMGetDatai(self.i_hot3) 					
		self.li_hot4= 					XPLMGetDatai(self.i_hot4) 					
		self.li_hot5= 					XPLMGetDatai(self.i_hot5) 					
		self.li_hot6= 					XPLMGetDatai(self.i_hot6) 					
		self.li_hot7= 					XPLMGetDatai(self.i_hot7) 					
		self.li_hot8= 					XPLMGetDatai(self.i_hot8) 	

		self.lf_ice_frame = 			XPLMGetDataf(self.f_ice_frame)	
		self.lf_ice_pitot = 			XPLMGetDataf(self.f_ice_pitot)	
		self.lf_ice_propeller = 		XPLMGetDataf(self.f_ice_propeller)
		self.lf_ice_window = 			XPLMGetDataf(self.f_ice_window)	
		
		self.li_bird = 					XPLMGetDatai(self.i_bird)
		  
		self.lf_g_normal= 				XPLMGetDataf(self.f_g_normal) 			
		self.lf_g_forward= 				XPLMGetDataf(self.f_g_forward) 			
		self.lf_g_side= 				XPLMGetDataf(self.f_g_side) 				
		
		
		self.lf_pitch= 					XPLMGetDataf(self.f_pitch) 				
		self.lf_roll= 					XPLMGetDataf(self.f_roll) 				
		self.lf_yaw= 					XPLMGetDataf(self.f_yaw) 					
		

		self.lf_cab_press= 				XPLMGetDataf(self.f_cab_press) 			
		self.lf_cab_rate= 				XPLMGetDataf(self.f_cab_rate) 			
		#cab humidity ?
		#cab temp ?
		
		self.lf_outside_temp1= 			XPLMGetDataf(self.f_outside_temp1) 		
		self.lf_outside_temp2= 			XPLMGetDataf(self.f_outside_temp2) 		
		self.lf_outside_temp3= 			XPLMGetDataf(self.f_outside_temp3) 	

		self.lf_baro_set = 				XPLMGetDataf(self.f_baro_set)
		self.li_baro_set = 				int(self.lf_baro_set * 100)
		self.lf_baro_sea_level = 		XPLMGetDataf(self.f_baro_sea_level)
		self.li_baro_sea_level = 		int(self.lf_baro_sea_level * 100)
		self.lf_baro_alt = 				XPLMGetDataf(self.f_baro_alt)
		
		self.lf_wind_direction = 		XPLMGetDataf(self.f_wind_direction)
		self.lf_wind_speed_kt = 		XPLMGetDataf(self.f_wind_speed_kt)
		
		self.lf_slats_1 = 				XPLMGetDataf(self.f_slats_1)
		self.lf_flaps_1 = 				XPLMGetDataf(self.f_flaps_1)
		
		self.ld_latitude = 				XPLMGetDatad(self.d_latitude)
		self.ld_longitude = 			XPLMGetDatad(self.d_longitude)
		
		self.li_nonsmoking =            XPLMGetDatai(self.i_nonsmoking)
		self.li_fastenseatbelts =       XPLMGetDatai(self.i_fastenseatbelt)
		self.li_replay = 				XPLMGetDatai(self.i_replay)
		
		self.ivyAircraft.UpdateData()
		
		self.data_read_valid = True
		
		######################################################################################### End of DATAREF Local Variables
		pass
		