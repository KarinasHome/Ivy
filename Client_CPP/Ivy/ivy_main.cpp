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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "XPLMUtilities.h"
#include "XPLMProcessing.h"
#include "XPLMPlugin.h"
#include "XPSound.h"
#include "MyIvy.h"
#include "Ivy.h"

// OS X: we use this to convert our file path.
#if APL
#include <Carbon/Carbon.h>
#endif






/**************************************************************************************************************
 * Global Variables 
 **************************************************************************************************************/
MyIvy * pIvy;
std::ofstream ivy_output_file;



// Mac specific: this converts file paths from HFS (which we get from the SDK) to Unix (which the OS wants).
// See this for more info:
//
// http://www.xsquawkbox.net/xpsdk/mediawiki/FilePathsAndMacho

#if APL
int ConvertPath(const char * inPath, char * outPath, int outPathMaxLen) {

	CFStringRef inStr = CFStringCreateWithCString(kCFAllocatorDefault, inPath, kCFStringEncodingMacRoman);
	if (inStr == NULL)
		return -1;
	CFURLRef url = CFURLCreateWithFileSystemPath(kCFAllocatorDefault, inStr, kCFURLHFSPathStyle, 0);
	CFStringRef outStr = CFURLCopyFileSystemPath(url, kCFURLPOSIXPathStyle);
	if (!CFStringGetCString(outStr, outPath, outPathMaxLen, kCFURLPOSIXPathStyle))
		return -1;
	CFRelease(outStr);
	CFRelease(url);
	CFRelease(inStr);
	return 0;
}
#endif

// Initialization code.

static float InitPlugin(float elapsed, float elapsed_sim, int counter, void * ref)
{
	IvyDebugString("Ivy: Initializing.\n");
	pIvy->IvyStart();
	return 0.0f;
}

PLUGIN_API int XPluginStart(char * name, char * sig, char * desc)
{
	IvyDebugString("Ivy: Startup.\n");
	strcpy(name, "Ivy");
	strcpy(sig, "k80.Ivy");
	strcpy(desc, "The nagging co-pilot");

	ivy_output_file.open("IvyLog.txt");

	pIvy = new MyIvy();

	if (sizeof(unsigned int) != 4 ||
		sizeof(unsigned short) != 2)
	{
		IvyDebugString("Ivy: This plugin was compiled with a compiler with weird type sizes.\n");
		return 0;
	}

	// Do deferred sound initialization. See http://www.xsquawkbox.net/xpsdk/mediawiki/DeferredInitialization
	// for more info.
	XPLMRegisterFlightLoopCallback(InitPlugin, -1.0, NULL);

	

	return 1;
}

PLUGIN_API void XPluginStop(void)
{
	pIvy->IvyStop();
	delete pIvy;
	ivy_output_file.close();
}

PLUGIN_API int XPluginEnable(void)
{
	pIvy->IvyEnable();
	return 1;
}

PLUGIN_API void XPluginDisable(void)
{
	pIvy->IvyDisable();
}



PLUGIN_API void XPluginReceiveMessage(XPLMPluginID from, int msg, void * p)
{
	pIvy->IvyReceiveMessage(from, msg, p);
}

int WrapIvyVSpeedHandler(XPWidgetMessage  inMessage, XPWidgetID  inWidget, intptr_t inParam1, intptr_t inParam2)
{
	return pIvy->IvyVSpeedHandler(inMessage,inWidget,inParam1,inParam2);
}
int WrapIvyLogbookHandler(XPWidgetMessage  inMessage, XPWidgetID  inWidget, intptr_t inParam1, intptr_t inParam2)
{
	return pIvy->IvyLogbookHandler(inMessage,inWidget,inParam1,inParam2);
}
int WrapIvyLogbookScrollHandler(XPWidgetMessage  inMessage, XPWidgetID  inWidget, intptr_t inParam1, intptr_t inParam2)
{
	return pIvy->IvyLogbookScrollHandler(inMessage,inWidget,inParam1,inParam2);
}

void WrapIvyDrawOutputWindow(XPLMWindowID in_window_id, void * in_refcon)
{
	return pIvy->IvyDrawOutputWindow(in_window_id,in_refcon);
}
void WrapIvyMenuHandler(void * in_menu_ref, void * in_item_ref)
{
	return pIvy->IvyMenuHandler(in_menu_ref,in_item_ref);
}
int WrapSayBaroCallback(XPLMCommandRef cmd, XPLMCommandPhase phase, void *refcon)
{
	return pIvy->SayBaroCallback(cmd,phase,refcon);
}
int WrapSayWindCallback(XPLMCommandRef cmd, XPLMCommandPhase phase, void *refcon)
{
	return pIvy->SayWindCallback(cmd,phase,refcon);
}
int WrapAnnouncementCallback(XPLMCommandRef cmd, XPLMCommandPhase phase, void *refcon)
{
	return pIvy->AnnouncementCallback(cmd,phase,refcon);
}
int WrapResetIvyCallback(XPLMCommandRef cmd, XPLMCommandPhase phase, void *refcon)
{
	return pIvy->ResetIvyCallback(cmd,phase,refcon);
}
int WrapToogleWindowCallback(XPLMCommandRef cmd, XPLMCommandPhase phase, void *refcon)
{
	return pIvy->ToogleWindowCallback(cmd,phase,refcon);
}

void WrapKeyCallback(XPLMWindowID inWindowID, char inKey, XPLMKeyFlags inFlags, char inVirtualKey, void * inRefcon, int losingFocus)
{
	return pIvy->KeyCallback(inWindowID,inKey,inFlags,inVirtualKey,inRefcon,losingFocus);
}
int WrapMouseClickCallback(XPLMWindowID inWindowID, int x, int y, XPLMMouseStatus inMouse, void * inRefcon)
{
	return pIvy->MouseClickCallback(inWindowID,x,y,inMouse,inRefcon);
}

PLUGIN_API float WrapIvyFlightLoopCallback(float elapsedMe, float elapsedSim, int counter, void * refcon)
{
	return pIvy->IvyFlightLoopCallback(elapsedMe,elapsedSim,counter,refcon);
}