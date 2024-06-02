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

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////;
// IvyLandingDetection
//
// Small class to store our landing data, rate the landing && check if the touchdown was within the given time frame

class IvyLandingDetection
{
public:
	float m_time;
	float m_sink_rate;
	float m_g_normal;
	float m_g_side;
	float m_g_forward;
	int m_rating;

	IvyLandingDetection(float time, float sink_rate, float g_normal, float g_side, float g_forward);
	~IvyLandingDetection();
	void RateLanding(void);
	int GetCurrentRate(float last_landing_time, float max_time);
};

