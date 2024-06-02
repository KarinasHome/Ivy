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

#include "MyIvyResponse.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <list>
#include <sys/stat.h>




MyIvyResponse::MyIvyResponse(std::string event_name, std::string mp3_path, int active_on_load, float minimum_occ, float deactivate_time, int is_error, std::vector<MyIvyResponse*> *ivy_object_list, FMOD_CHANNELGROUP* sound_channel, XPSound *xpSound, std::vector<std::string> &error_list, bool *p_enable, std::string error_string):
	m_error_list(error_list),
	m_error_string(error_string),
	m_error_history()
{
	m_event_name = event_name;
	m_is_error = is_error;
	m_minimum_occ = minimum_occ;
	m_deactivate_time = deactivate_time;
	m_active = active_on_load;
	m_played = active_on_load;
	m_active_on_load = active_on_load;
	m_channel_number = sound_channel;
	m_ivyChannel = sound_channel;
	mXPSound = xpSound;

	m_p_enable = p_enable;
	
	//std::list<std::string> m_play_files;
	
	// Append this response to the ivy_object_list
	ivy_object_list->push_back(this);

	LoadSound(mp3_path, xpSound);
	
}


MyIvyResponse::~MyIvyResponse()
{
}

void MyIvyResponse::LoadSound(std::string mp3_path, XPSound *xpSound)
{

	m_sounds.clear();

	for (int file_number = 1; file_number <= IVY_RESPONSE_MAX_FILE; ++file_number)
	{
		std::string sound_file_path = mp3_path + m_event_name + "_" + std::to_string(file_number) + ".mp3";
		if (file_exists(sound_file_path))
		{
			//m_play_files.push_back(sound_file_path);
			FMOD_SOUND* act_sound = xpSound->CreateBuffer(sound_file_path);

			m_sounds.push_back(act_sound);
		}
	}

	ivy_output_file << "IvyResponse " << m_event_name << " loaded sound files : " << m_sounds.size() << std::endl;
	ivy_output_file.flush();

}

void MyIvyResponse::Activate(float time)
{
	if (m_active == 0)
		m_time_activated = time;

	m_active = 1;
	m_deactivating = 0;

	m_time_active = time - m_time_activated;

	if ((m_time_active >= m_minimum_occ) && (m_played == 0))
		Play();
}

void MyIvyResponse::Deactivate(float time)
{
	if (m_active == 0)
		return;

	if (m_deactivating == 0)
		m_time_deactivated = time;

	m_deactivating = 1;
	m_time_deactivating = time - m_time_deactivated;

	if (m_time_deactivating >= m_deactivate_time)
	{
		m_deactivating = 0;
		m_time_deactivated = 0;
		m_active = 0;
		m_time_activated = 0;
		m_time_active = 0;
		m_played = 0;
		m_internal_note = 0;
	}
}

void MyIvyResponse::SetAsPlayed(float time)
{
	m_active = 1;
	m_played = 1;
	m_time_activated = time;
}

int MyIvyResponse::Play(void)
{
	int random_number = 0;
	if ((m_sounds.size() == 0) || (*m_p_enable == false))
	{
		m_played = 1;
		m_error_count = m_error_count + 1;
		if (m_is_error > 0) m_error_list.push_back(GetErrorString());
		return 1;
	}

	if ((m_queue_output == 0) && (mXPSound->IsPlayingSound(m_ivyChannel) == true)) // No Modulo 
		return 0;

	if (m_played == 1)
		return 0;

		

	if (m_sounds.size() > 1)
		random_number = rand() % m_sounds.size();
	else
		random_number = 0;

	if (mXPSound->PlaySingleSound(m_ivyChannel, m_sounds.at(random_number)) == true)
	{
		if (m_is_error > 0) m_error_list.push_back(GetErrorString());

		ivy_output_file << "IvyResponse played: " << m_event_name << std::endl;
		ivy_output_file.flush();

		m_error_count = m_error_count + 1;
		m_played = 1;
	}

	return m_played;
}

std::string MyIvyResponse::GetErrorString(void)
{
	std::string text = "";

	std::string s_value = std::to_string((int)(m_time_activated / 3600));
	if (s_value.size() < 2) s_value = "0" + s_value;
	text += s_value + ":";

	s_value = std::to_string((int)((((int) m_time_activated) % 3600) / 60));
	if (s_value.size() < 2) s_value = "0" + s_value;
	text += s_value + ":";

	s_value = std::to_string(((int)m_time_activated) % 60);
	if (s_value.size() < 2) s_value = "0" + s_value;
	text += s_value + " - Error: " + m_error_string;

	return text;
}

void MyIvyResponse::Archive() 
{ 
	m_error_history.push_back(m_error_count); 
	m_error_count = 0; 
}

int MyIvyResponse::CalcAOCCount(int m_aoc_flight_count)
{
	m_aoc_count = 0;

	if (m_is_error > 0)
	{
		int index = 0;
		if (m_aoc_flight_count < m_error_history.size())
			index = m_error_history.size() - m_aoc_flight_count;

		for (; index < m_error_history.size(); index++)
			m_aoc_count += m_error_history[index];
	}

	return m_aoc_count;
}
