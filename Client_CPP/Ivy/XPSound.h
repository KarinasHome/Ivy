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

//#ifndef XP_SOUND_H
	#include <stdio.h>
	#include <string>
	#include <stdlib.h>
	#include <list>
	#include "fmod.h"
	#include "fmod_studio.h"
	#include "fmod_errors.h"
	#include "XPLMUtilities.h"
	#include "XPLMProcessing.h"
	#include "XPLMPlugin.h"
    #include "XPLMSound.h"
	

	#include "al.h"
	#include "alc.h"


	// OS X: we use this to convert our file path.
	#if APL
	#include <Carbon/Carbon.h>
	#endif

//#endif

#define XP_SOUND_H



//#define XPSOUND_INVALID_SOUND 0xFFFFFFFF




class XPSound
{
	FMOD_STUDIO_SYSTEM*		m_fmod_studio = NULL;
	FMOD_SYSTEM*			m_fmod_system = NULL;

	void XPSound::ErrorHandler(const std::string& file, int line, FMOD_RESULT result);

public:
							XPSound();
							~XPSound();
	// Initialize OpenAL
	int						InitSound();
	// Load a Wave file OpenAL
	FMOD_SOUND*				CreateBuffer(std::string &file_name);
	//void					RemoveBuffer(ALuint ali_buffer_source);
	FMOD_CHANNELGROUP*      CreateSound(XPLMAudioBus audioType, std::string name);
	bool					PlaySingleSound(FMOD_CHANNELGROUP* channelgroup, FMOD_SOUND* sound); // Gain could be set right here
	bool					IsPlayingSound(FMOD_CHANNELGROUP* playing_channel_group);
	void					SetSoundGain(FMOD_CHANNELGROUP* playing_channel_group, float gain);
	//void					RemoveSound(ALuint ali_sound_source);
	//void					RemoveAllBuffers();
};

