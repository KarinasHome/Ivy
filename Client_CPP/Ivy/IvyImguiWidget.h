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
class IvyImguiWidget :
	public ImgWindow
{
public:
	IvyImguiWidget(MyIvy *pIvyNew, int left, int top, int right, int bot, int decoration);
	~IvyImguiWidget();

	void Visible(bool visible);

protected:
	void configureImguiContext() override;
	void buildInterface() override;
private:
	ImFont* font2;
	ImFont* font3;
	ImFont* font4;
	ImFont* font5;
	ImFont* font6;
	ImFont* font7;
	ImFont* font8;
	ImFont* font9;
	ImFont* font10;
	ImFont* font11;
	ImFont* font12;
	ImFont* font13;
	ImFont* font14;
	ImFont* font15;

	float win_width = 0;
	float win_height = 0;

	float output_dh = 0;

	int selected_tab = 1;
	int selected_graph = 1;

	int selected_radio = 0;

	MyIvy *pIvy = NULL;
};

