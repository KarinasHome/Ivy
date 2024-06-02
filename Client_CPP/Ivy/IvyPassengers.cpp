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

#include "IvyPassengers.h"



IvyPassengers::IvyPassengers(MyIvyConfiguration *pivyConfig, FMOD_CHANNELGROUP* channel, XPSound *xpSound)
{
	LoadSound(pivyConfig, channel, xpSound);
}


IvyPassengers::~IvyPassengers()
{
}

void IvyPassengers::LoadSound(MyIvyConfiguration *pivyConfig, FMOD_CHANNELGROUP* channel, XPSound *xpSound)
{
	mp_ivyConfig = pivyConfig;
	m_is_screaming = false;
	//m_fading = false;
	mXPSound = xpSound;
	m_passengerChannel = channel;
	m_gain = 0;


	m_screamSound = mXPSound->CreateBuffer(pivyConfig->m_mp3_path + "passenger_screams.mp3");
	mXPSound->SetSoundGain(m_passengerChannel, m_gain);
	mXPSound->PlaySingleSound(m_passengerChannel, m_screamSound);
}

void IvyPassengers::MakeScream(bool screaming, float gain)
{
	/*if (screaming == true)
	{
		if (mXPSound->IsPlayingSound(m_passengerChannel) == false)
		{
			mXPSound->PlaySingleSound(m_passengerChannel, m_screamSound);
		}
	}*/

	if (mXPSound->IsPlayingSound(m_passengerChannel) == false)
	{
		mXPSound->PlaySingleSound(m_passengerChannel, m_screamSound);
	}


	
	if ((screaming == true) && (m_gain < gain))
	{
		m_gain = min(m_gain + m_gain_step, 1); // Limit gain to 1
		m_is_screaming = true;
	}
	else if ((screaming == true) && (m_gain > gain))
	{
		m_gain = max(m_gain - m_gain_step, 0); // Limit gain to 1
		m_is_screaming = true;
	}
	else if ((screaming == false) && (m_gain > 0))
	{
		m_gain = max(m_gain - m_gain_step, 0);
		m_is_screaming = false;
	}
	mXPSound->SetSoundGain(m_passengerChannel, m_gain);
}
