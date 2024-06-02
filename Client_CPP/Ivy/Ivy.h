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
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <list>
#include <sys/stat.h>
#include <iostream>
#include <sstream>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <algorithm>
#include <queue>
#include <fstream>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <set>

#include "XPLMDefs.h"
#include "XPLMProcessing.h"
#include "XPLMDisplay.h"
#include "XPLMGraphics.h"
#include "XPLMDataAccess.h"
#include "XPLMUtilities.h"
#include "XPLMNavigation.h"
#include "XPLMPlugin.h"
#include "XPLMMenus.h"
#include "XPWidgetDefs.h"
#include "XPWidgets.h"
#include "XPStandardWidgets.h"
#include "XPWidgetUtils.h"
#include "XPLMPlanes.h"


#include "ImgWindow.h"
#include "imgui.h"

// OS X: we use this to convert our file path.
#if APL
#include <Carbon/Carbon.h>
#endif

// Your include paths for OpenAL may vary by platform.
#include "al.h"
#include "alc.h"

#include "XPSound.h"

#define IVY_MAX_AIRCRAFT_CONFIG 100

#define MAX_ERROR_HISTORY 100

#define MAX_GRAPH_DATA 1000
#define MAX_GRAPH_TIME 100

#define HRM_MESSAGE_SOUND_1 0x85747400

#define HV_POLYGONS_MAX 5
#define HV_POINTS_MAX 100

#define HV_HEIGHT_MAX 500
#define HV_VELOCITY_MAX 150

struct HRM_Sound
{
	int sound_before = -1;
	int say_value = -1;
	int sound_after = -1;
};

class MyIvy;
extern MyIvy * pIvy;
extern std::ofstream ivy_output_file;

inline bool file_exists(const std::string& name) 
{
	struct stat buffer;
	return (stat(name.c_str(), &buffer) == 0);
}

inline bool is_int(float f)
{
	if (f == ((int)f)) return true;
	return false;
}

inline void IvyDebugString(std::string output)
{
	ivy_output_file << output << std::endl;
	ivy_output_file.flush();
}


int WrapIvyVSpeedHandler(XPWidgetMessage  inMessage, XPWidgetID  inWidget, intptr_t inParam1, intptr_t inParam2);
int WrapIvyLogbookHandler(XPWidgetMessage  inMessage, XPWidgetID  inWidget, intptr_t  inParam1, intptr_t  inParam2);
int WrapIvyLogbookScrollHandler(XPWidgetMessage  inMessage, XPWidgetID  inWidget, intptr_t inParam1, intptr_t inParam2);


void WrapIvyDrawOutputWindow(XPLMWindowID in_window_id, void * in_refcon);
void WrapIvyMenuHandler(void * in_menu_ref, void * in_item_ref);
int WrapSayBaroCallback(XPLMCommandRef cmd, XPLMCommandPhase phase, void *refcon);
int WrapSayWindCallback(XPLMCommandRef cmd, XPLMCommandPhase phase, void *refcon);
int WrapAnnouncementCallback(XPLMCommandRef cmd, XPLMCommandPhase phase, void *refcon);
int WrapResetIvyCallback(XPLMCommandRef cmd, XPLMCommandPhase phase, void *refcon);
int WrapToogleWindowCallback(XPLMCommandRef cmd, XPLMCommandPhase phase, void *refcon);

void WrapKeyCallback(XPLMWindowID inWindowID, char inKey, XPLMKeyFlags inFlags, char inVirtualKey, void * inRefcon, int losingFocus);
int WrapMouseClickCallback(XPLMWindowID inWindowID, int x, int y, XPLMMouseStatus inMouse, void * inRefcon);

PLUGIN_API float WrapIvyFlightLoopCallback(float elapsedMe, float elapsedSim, int counter, void * refcon);