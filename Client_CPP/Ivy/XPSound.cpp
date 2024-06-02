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

#include "XPSound.h"


// Delete this define, if it is not part of Ivy (controls debug output only)
#define IVY

#ifdef IVY
	#include "Ivy.h"
	#define XPSoundDebugString IvyDebugString
#else
	#define XPSoundDebugString XPLMDebugString
#endif


XPSound::XPSound()
{


}


XPSound::~XPSound()
{

}

void XPSound::ErrorHandler(const std::string& file, int line, FMOD_RESULT result)
{
	if (result != FMOD_OK)
	{
		std::string error_message = "XPSound: Error in " + file + ", line " + std::to_string(line) + ": " + FMOD_ErrorString(result) + "\n";
		XPSoundDebugString(error_message);
	}
}



// Initialize FMOD
int XPSound::InitSound()
{
	XPSoundDebugString("XPSound: Init Sound");

	m_fmod_studio = XPLMGetFMODStudio();
	if (m_fmod_studio == nullptr)
	{
		XPSoundDebugString("XPSound: Unable to find FMOD Studio");
		return 1;
	}

	FMOD_RESULT result;

	// Get a pointer to the FMOD core system
	result = FMOD_Studio_System_GetCoreSystem(XPLMGetFMODStudio(), &m_fmod_system);
	ErrorHandler(__FILE__, __LINE__ - 1, result);

	return 0;
}

FMOD_CHANNELGROUP* XPSound::CreateSound(XPLMAudioBus audioType, std::string name)
//ALuint		XPSound::CreateSound(ALenum looping)
{
	FMOD_RESULT result;
	FMOD_CHANNELGROUP* channel_group;
	FMOD_CHANNELGROUP* xp_channel_group;

	static int counter = 0;

	counter++;

	name += std::to_string(counter);

	XPSoundDebugString("XPSound: Creating new channel group: " + name);

	// create a new channel group
	result = FMOD_System_CreateChannelGroup(m_fmod_system, name.c_str(), &channel_group);
	ErrorHandler(__FILE__, __LINE__ - 1, result);

	

	xp_channel_group = XPLMGetFMODChannelGroup(audioType);
	result = FMOD_ChannelGroup_AddGroup(xp_channel_group, channel_group, true, nullptr);
	ErrorHandler(__FILE__, __LINE__ - 1, result);

	//if (result != 0)
	//	return NULL;

	return channel_group;

}


FMOD_SOUND* XPSound::CreateBuffer(std::string &file_name)
{
	FMOD_RESULT result;
	FMOD_SOUND* new_sound;

#if APL
	ConvertPath(file_name, file_name, sizeof(file_name));
#endif

	result = FMOD_System_CreateSound(m_fmod_system, file_name.c_str(), FMOD_DEFAULT, nullptr, &new_sound);
	ErrorHandler(__FILE__, __LINE__ - 1, result);
	return new_sound;
}





bool XPSound::PlaySingleSound(FMOD_CHANNELGROUP* channelgroup, FMOD_SOUND* sound)
{
	/*FMOD_RESULT FMOD_System_PlaySound(
  FMOD_SYSTEM *system,
  FMOD_SOUND *sound,
  FMOD_CHANNELGROUP *channelgroup,
  FMOD_BOOL paused,
  FMOD_CHANNEL **channel
);*/
	//if (channelgroup == NULL) return false;
	FMOD_CHANNEL* sound_channel;
	FMOD_RESULT result = FMOD_System_PlaySound(m_fmod_system, sound, channelgroup, false, &sound_channel);
	ErrorHandler(__FILE__, __LINE__ - 1, result);
	if (result != 0) return false;
	return true;
}

bool XPSound::IsPlayingSound(FMOD_CHANNELGROUP* playing_channel_group)
{
	bool playing = true;

	//if (playing_channel_group == NULL) return false;

	/*
	FMOD_RESULT FMOD_ChannelGroup_IsPlaying(
  FMOD_CHANNELGROUP *channelgroup,
  FMOD_BOOL *isplaying
);
	*/

	FMOD_BOOL isplaying;
	FMOD_RESULT result = FMOD_ChannelGroup_IsPlaying(playing_channel_group, &isplaying);
	ErrorHandler(__FILE__, __LINE__ - 1, result);
	
	if (isplaying > 0)
		return true;

	return false;
}

void XPSound::SetSoundGain(FMOD_CHANNELGROUP* playing_channel_group, float gain)
{
	//if (playing_channel_group == NULL) return;
	FMOD_RESULT result = FMOD_ChannelGroup_SetVolume(playing_channel_group, gain);
	ErrorHandler(__FILE__, __LINE__ - 1, result);
}


