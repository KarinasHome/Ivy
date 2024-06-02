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

#define IVY_RESPONSE_MAX_FILE 20

class MyIvyResponse
{
	std::vector<std::string> &m_error_list;
	

	

public:
	bool *m_p_enable;
	std::string m_error_string;

	std::vector<int> m_error_history;

	int m_queue_output = 0;
	int m_deactivating = 0;
	float m_time_activated = 0;
	float m_time_deactivated = 0;
	float m_time_deactivating = 0;
	float m_time_active = 0;
	int m_mute = 0;
	int m_error_count = 0;
	int m_internal_note = 0;

	int m_aoc_count = 0;

	std::string m_event_name;
	int m_is_error;
	float m_minimum_occ;
	float m_deactivate_time;
	int m_active;
	int m_played;
	int m_active_on_load;
	int *mp_pitch;
	FMOD_CHANNELGROUP* m_channel_number;
	FMOD_CHANNELGROUP* m_ivyChannel;

	XPSound *mXPSound;
	
	std::vector<FMOD_SOUND*> m_sounds;

	MyIvyResponse(std::string event_name, std::string mp3_path, int active_on_load, float minimum_occ, float deactivate_time, int is_error, std::vector<MyIvyResponse*> *ivy_object_list, FMOD_CHANNELGROUP* sound_channel, XPSound *xpSound, std::vector<std::string> &error_list, bool *p_enable, std::string error_string);
	~MyIvyResponse();

	void Activate(float time);
	void Deactivate(float time);
	void SetAsPlayed(float time);
	int Play(void);
	std::string GetErrorString(void);
	void LoadSound(std::string mp3_path, XPSound *xpSound);

	void Archive();
	int CalcAOCCount(int m_aoc_flight_count);
};

