/*
 * This file is part of the Ivy distribution (https://github.com/kristian80/Ivy).
 * Copyright (c) 2019 Kristian80.
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

#define SWAP_32(value)                 \
        (((((unsigned short)value)<<8) & 0xFF00)   | \
         ((((unsigned short)value)>>8) & 0x00FF))

#define SWAP_16(value)                     \
        (((((unsigned int)value)<<24) & 0xFF000000)  | \
         ((((unsigned int)value)<< 8) & 0x00FF0000)  | \
         ((((unsigned int)value)>> 8) & 0x0000FF00)  | \
         ((((unsigned int)value)>>24) & 0x000000FF))

#define FAIL(X) { IvyDebugString(X); free(mem); return 0; }

#define RIFF_ID 0x46464952			// 'RIFF'
#define FMT_ID  0x20746D66			// 'FMT '
#define DATA_ID 0x61746164			// 'DATA'

#define XPSOUND_INVALID_SOUND 0xFFFFFFFF

/*// Wave files are RIFF files, which are "chunky" - each section has an ID and a length.  This lets us skip
// things we can't understand to find the parts we want.  This header is common to all RIFF chunks.
struct chunk_header {
	int			id;
	int			size;
};

// WAVE file format info.  We pass this through to OpenAL so we can support mono/stereo, 8/16/bit, etc.
struct format_info {
	short		format;				// PCM = 1, not sure what other values are legal.
	short		num_channels;
	int			sample_rate;
	int			byte_rate;
	short		block_align;
	short		bits_per_sample;
};*/


class XPSound
{
	/*ALuint			m_snd_src = 0;				// Sample source and buffer - this is one "sound" we play.
	ALuint			m_snd_buffer = 0;
	float			m_pitch = 1.0f;			// Start with 1.0 pitch - no pitch shift.

	std::list<ALuint> m_sound_list;
	std::list<ALuint> m_buffer_list;

	ALCdevice *		mpSoundDevice = NULL;			// We make our own device and context to play sound through.
	ALCcontext *	mpSoundCtx = NULL;

	//char			mp_mp3_path[2048] = "";*/

	char *					WaveFindChunk(char * file_begin, char * file_end, int desired_id, int swapped);
	char *					WaveChunkEnd(char * chunk_start, int swapped);
	void					CheckOpenALError(void);
	ALuint					LoadWave(const char * file_name);
public:
							XPSound();
							~XPSound();
	// Initialize OpenAL
	int						InitSound();
	// Load a Wave file OpenAL
	FMOD_SOUND*				CreateBuffer(std::string &file_name);
	//void					RemoveBuffer(ALuint ali_buffer_source);
	FMOD_CHANNELGROUP*      CreateSound(XPLMAudioBus audioType);
	bool					PlaySingleSound(FMOD_CHANNELGROUP* channelgroup, FMOD_SOUND* sound); // Gain could be set right here
	bool					IsPlayingSound(FMOD_CHANNELGROUP* playing_channel_group)
	//void					SetSoundGain(ALuint ali_sound_source, float gain);
	//void					RemoveSound(ALuint ali_sound_source);
	//void					RemoveAllBuffers();
};

