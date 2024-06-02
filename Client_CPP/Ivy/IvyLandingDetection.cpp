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
#include "IvyLandingDetection.h"



IvyLandingDetection::IvyLandingDetection(float time, float sink_rate, float g_normal, float g_side, float g_forward)
{
	m_time = time;
	m_sink_rate = sink_rate;
	m_g_normal = g_normal;
	m_g_side = g_side;
	m_g_forward = g_forward;
	m_rating = 0;
	RateLanding();
}


IvyLandingDetection::~IvyLandingDetection()
{
}

void IvyLandingDetection::RateLanding(void)
{
	if ((m_sink_rate > -100) && (m_g_normal < 1.5)) 
		m_rating = 1;
	else if ((m_sink_rate > -250) && (m_g_normal < 2)) 
		m_rating = 2;
	else if ((m_sink_rate > -400) && (m_g_normal < 3))
		m_rating = 3;
	else if ((m_sink_rate > -500) && (m_g_normal < 4))
		m_rating = 4;
	else 
		m_rating = 5;
}

int IvyLandingDetection::GetCurrentRate(float last_landing_time, float max_time)
{
	if ((last_landing_time - m_time) < max_time)
		return m_rating;
	else
		return 0;
}
